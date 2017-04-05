open Require.Vscode;

let erase builder _ selection => {
  let selection =
    if (Selection.isEmpty selection) {
      let start = Selection.start selection;
      selection |> Selection.with_ end_::(start |> Position.translate char::1)
    } else {
      selection
    };

  builder |> TextEditorEdit.delete selection
};

let insertLineAbove builder _ selection => {
  let pos = selection
  |> Selection.start
  |> Position.with_ char::0;

  builder |> TextEditorEdit.insert pos "\n"
};

let insertLineBelow builder _ selection => {
  let pos = selection
  |> Selection.start
  |> Position.translate line::1
  |> Position.with_ char::0;

  builder |> TextEditorEdit.insert pos "\n"
};
