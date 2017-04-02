open Require.Promise;
open Require.Vscode;

let edit action =>
  fun editor _ state => {
    editor
    |> TextEditor.edit (fun builder => action builder editor)
    |> Promise.then_ (fun _ => state)
  };

let mode mode =>
  fun _ _ state =>
    Promise.resolve State.{ ...state, mode };

let selection action =>
  fun editor _ state => {
    TextEditor.setSelection editor (action (editor |> TextEditor.selection) editor);
    Promise.resolve state
  };
