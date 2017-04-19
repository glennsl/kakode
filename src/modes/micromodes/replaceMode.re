open Require.Promise;
open Require.Vscode;

let handleKey editor _ state key => {
  editor |> TextEditor.edit (fun builder => 
    editor
    |> TextEditor.selections
    |> Js.Array.forEach
      (fun selection => {
        let start = selection |> Selection.start |> Position.line;
        let end_ = selection |> Selection.end_ |> Position.line;

        for i in start to end_ {
          let start' = i == start ?
            selection |> Selection.start |> Position.character : 0;
          let end' = i == end_ ?
            selection |> Selection.end_ |> Position.character :
            editor |> TextEditor.document |> TextDocument.lineAt i |> TextLine.text |> String.length;
          let end' = start' == end' ? end' + 1 : end';
          let range = Range.make start::(Position.make i start') end_::(Position.make i end');
          let length = end' - start';

          builder |> TextEditorEdit.replace range (key |> Char.code |> Js.String.fromCharCode |> Js.String.repeat length)
        }
      })
  ) |> Promise.then_ (fun _ => Promise.resolve State.{...state,  mode: Mode.Normal })
};

let cursor = TextEditorCursorStyle.block;
let name = "Replace Mode"