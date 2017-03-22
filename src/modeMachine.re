open Require.Promise;
open Require.Vscode;

type t = {
  mutable state: Types.state,
  modeIndicator: statusBarItem,
};

let sync self =>
  switch (Vscode.Window.activeTextEditor ()) {
  | None => ()
  | Some editor => {
    let editorOptions = editor |> TextEditor.options;
    TextEditor.Options.setCursorStyle editorOptions self.state.mode.cursor;

    StatusBarItem.setText self.modeIndicator ("-- " ^ (self.state.mode.name |> Js.String.toUpperCase) ^ " --");
  }
  };

let make () => {
  let modeIndicator = Vscode.Window.createStatusBarItem StatusBarAlignment.left;
  StatusBarItem.show modeIndicator;

  let state: Types.state = {
    mode: NormalMode.mode
  };

  let self = { state, modeIndicator };
  sync self;
  self
};

let handleKey self key =>
  switch (Vscode.Window.activeTextEditor ()) {
  | None => ()
  | Some editor => {
    self.state.mode.handleKey editor self.state key
    |> Promise.then_
      (fun state' =>
        /*if (state' != self.state)*/ {
          self.state = state';
          sync self
        }
      )
    |> ignore;
  }
};

let escape self () => {
  self.state = { mode: NormalMode.mode };
  sync self
};