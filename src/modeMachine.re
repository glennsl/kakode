open Require.Promise;
open Require.Vscode;

external logError : 'a => unit = "console.error" [@@bs.val];

type t = {
  mutable state: State.t,
  modeIndicator: statusBarItem,
  paramsIndicator: statusBarItem
};

let updateModeIndicator self => {
  switch (Vscode.Window.activeTextEditor ()) {
  | None =>
    StatusBarItem.setText self.modeIndicator ""
  | Some editor =>
    let editorOptions = editor |> TextEditor.options;

    let (cursor, name) =
      switch (self.state.mode) {
      | Insert => InsertMode.(cursor, name)
      | Normal => NormalMode.(cursor, name)
      | Replace => ReplaceMode.(cursor, name)
      | SelectRegisterMode => SelectRegisterMode.(cursor, name)
      | ViewTransient
      | ViewLocked => ViewMode.(cursor, name)
      };
    TextEditor.Options.setCursorStyle editorOptions cursor;

    StatusBarItem.setText self.modeIndicator ("-- " ^ (name |> Js.String.toUpperCase) ^ " --");
  }
};

let updateParamsIndicator self => {
  let formatCount count =>
    "param=" ^ (string_of_int count);
  let formatRegister (Register.Register symbol) =>
    "reg=" ^ (symbol |> Char.code |> Js.String.fromCharCode);
  let text =
    switch (self.state.params.count, self.state.params.register) {
    | (Some count, Some register) =>
       formatCount count ^ ", " ^ formatRegister register;
    | (Some count, None) =>
      formatCount count
    | (None, Some register) =>
      formatRegister register
    | (None, None) =>
      ""
    };
  StatusBarItem.setText self.paramsIndicator text;
};

let sync self => {
  updateModeIndicator self;
  updateParamsIndicator self;
};

let make () => {
  let modeIndicator = Vscode.Window.createStatusBarItem StatusBarAlignment.left;
  StatusBarItem.show modeIndicator;

  let paramsIndicator = Vscode.Window.createStatusBarItem StatusBarAlignment.right;
  StatusBarItem.show paramsIndicator;

  let registers = Register.Collection.fromList [
    Register.makeStatic "a" (Register 'a') (),
    Register.makeStatic "b" (Register 'b') (),
    Register.makeDynamic "dot" (Register '.') get::(fun () => [||]) set::(fun _ => ()),
    Register.makeStatic "dquote" (Register '"') (),
  ];

  let state = State.{
    registers,
    mode: Mode.Normal,
    params: Params.default
  };

  let self = { state, modeIndicator, paramsIndicator };
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
    | SelectRegisterMode => SelectRegisterMode.handleKey
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
  self.state = { ...self.state, mode: Mode.Normal, params: Params.default };
  sync self
};