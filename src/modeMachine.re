open Require.Promise;
open Require.Vscode;

external logError : 'a => unit = "console.error" [@@bs.val];

type t = {
  mutable state: State.t,
  modeIndicator: statusBarItem,
};

let sync self =>
  switch (Vscode.Window.activeTextEditor ()) {
  | None => ()
  | Some editor => {
    let editorOptions = editor |> TextEditor.options;

    let (cursor, name) =
      switch (self.state.mode) {
      | Insert => InsertMode.(cursor, name)
      | Normal => NormalMode.(cursor, name)
      | Replace => ReplaceMode.(cursor, name)
      | ViewTransient
      | ViewLocked => ViewMode.(cursor, name)
      };
    TextEditor.Options.setCursorStyle editorOptions cursor;

    StatusBarItem.setText self.modeIndicator ("-- " ^ (name |> Js.String.toUpperCase) ^ " --");
  }
  };

let make () => {
  let modeIndicator = Vscode.Window.createStatusBarItem StatusBarAlignment.left;
  StatusBarItem.show modeIndicator;

  let registers = Register.Collection.fromList [
    Register.makeStatic "a" (Register 'a') (),
    Register.makeStatic "b" (Register 'b') (),
    Register.makeDynamic "dot" (Register '.') get::(fun () => []) set::(fun _ => ()),
  ];

  let state = State.{
    registers,
    mode: Mode.Normal,
    params: Params.default
  };

  let self = { state, modeIndicator };
  sync self;
  self
};

let handleKey self key =>
  switch (Vscode.Window.activeTextEditor ()) {
  | None => ()
  | Some editor => {
    let modeHandleKey = switch (self.state.mode) {
    | Insert => InsertMode.handleKey
    | Normal => NormalMode.handleKey
    | Replace => ReplaceMode.handleKey
    | ViewTransient => ViewMode.handleKeyTransient
    | ViewLocked => ViewMode.handleKeyLocked
    };
  
    try {
      /* Params are transient, so we "pop" it off here, replace it with the
        default params and pass it separately to the key handler */
      let { State.params } = self.state;
      let state = { ...self.state, params: Params.default };

      modeHandleKey editor params state key
      |> Promise.then_
        (fun state' =>
          /*if (state' != self.state)*/ {
            self.state = state';
            sync self
          }
        )
      |> ignore;
    } {
      | e => logError e
    }
  }
};

let escape self () => {
  self.state = { ...self.state, mode: Mode.Normal };
  sync self
};