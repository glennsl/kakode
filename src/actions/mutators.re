open Require.Promise;
open Require.Vscode;

let selection action =>
  fun editor => {
    TextEditor.setSelection editor (action (editor |> TextEditor.selection) editor);
    Promise.resolve ()
  };
