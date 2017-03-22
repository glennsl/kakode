open Require.Promise;
open Require.Vscode;

let selection action =>
  fun editor state => {
    TextEditor.setSelection editor (action (editor |> TextEditor.selection) editor);
    Promise.resolve state
  };

let mode mode =>
  fun _ _ =>
    Promise.resolve Types.{ mode: mode };