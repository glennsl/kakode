open Require.Option;
open Require.Promise;
open Require.Vscode;

let edit action =>
  fun editor _ state => {
    editor
    |> TextEditor.edit (fun builder => action builder editor)
    |> Promise.then_ (fun _ => Promise.resolve state)
  };

let mode mode =>
  fun _ _ state =>
    Promise.resolve State.{ ...state, mode };

let iterSelections action =>
  fun editor => {
    editor
    |> TextEditor.selections
    |> Array.iter (action editor)
  };

let eachSelection action =>
  fun editor _ state => {
    editor
    |> TextEditor.selections
    |> Array.map (action editor)
    |> TextEditor.setSelections editor;
    
    Promise.resolve state
  };

let selection action =>
  fun editor _ state => {
    TextEditor.setSelection editor (action editor (editor |> TextEditor.selection));
    Promise.resolve state
  };

let selections action =>
  fun editor _ state => {
    TextEditor.setSelections editor (action (editor |> TextEditor.selections) editor);
    Promise.resolve state
  };

let repeatable action =>
  fun editor params state => {
    let rec loop = fun
    | 1 => action editor params state
    | i => (loop (i - 1)) |> Promise.then_ (action editor params);

    loop (params.Params.count |> Option.getOr 1);
  };