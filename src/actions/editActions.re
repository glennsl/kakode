open Require.Vscode;

let erase builder editor => {
  let selection = TextEditor.selection editor;
  let selection =
    if (Selection.isEmpty selection) {
      let start = Selection.start selection;
      selection |> Selection.with_ end_::(start |> Position.translate char::1)
    } else {
      selection
    };

  builder |> TextEditorEdit.delete selection
};

let insertLineAbove builder editor => {
  let pos = editor
  |> TextEditor.selection
  |> Selection.start
  |> Position.with_ char::0;

  builder |> TextEditorEdit.insert pos "\n"
};

let insertLineBelow builder editor => {
  let pos = editor
  |> TextEditor.selection
  |> Selection.start
  |> Position.translate line::1
  |> Position.with_ char::0;

  builder |> TextEditorEdit.insert pos "\n"
};
