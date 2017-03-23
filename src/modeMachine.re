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
      | Normal => NormalMode.(cursor, name)
      | Insert => InsertMode.(cursor, name)
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

  let state = State.{
    mode: Mode.Normal
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
    | Normal => NormalMode.handleKey
    | Insert => InsertMode.handleKey
    | ViewTransient => ViewMode.handleKeyTransient
    | ViewLocked => ViewMode.handleKeyLocked
    };
  
    try {
      modeHandleKey editor self.state key
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
  self.state = { mode: Mode.Normal };
  sync self
};