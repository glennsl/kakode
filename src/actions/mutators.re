open Require.Option;
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

let repeatable action =>
  fun editor params state => {
    let rec loop = fun
    | 1 => action editor params state
    | i => (loop (i - 1)) |> Promise.andThen (action editor params);

    loop (params.Params.count |> Option.getOr 1);
  };