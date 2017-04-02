open Require.Promise;
open Require.Vscode;

let selectionCenter editor => {
  let selection = TextEditor.selection editor;
  let range = Range.make start::(Selection.start selection) end_::(Selection.end_ selection);
  editor |> TextEditor.revealRange range TextEditorRevealType.inCenter;
};

let selectionTop editor => {
  let lineNumber = editor |> TextEditor.selection |> Selection.start |> Position.line;
  Vscode.Commands.executeCommandWithArg "revealLine" { "lineNumber": lineNumber, "at": "top" };
};

let selectionBottom editor => {
  let lineNumber = editor |> TextEditor.selection |> Selection.end_ |> Position.line;
  Vscode.Commands.executeCommandWithArg "revealLine" { "lineNumber": lineNumber, "at": "bottom" };
};

let scrollDown _ =>
  Vscode.Commands.executeCommandWithArg "editorScroll" { "to": "down", "by": "line", "value": 1 };

let scrollUp _ =>
  Vscode.Commands.executeCommandWithArg "editorScroll" { "to": "up", "by": "line", "value": 1 };

let handleKey' editor key => {

  let action = 
    switch key {
    | 'v'
    | 'c' => selectionCenter;
    | 't' => selectionTop;
    | 'b' => selectionBottom;
    | 'j' => scrollDown;
    | 'k' => scrollUp;
    | _ => (fun _ => ());
    };
  action editor;

};

let handleKeyTransient editor state key => {
  handleKey' editor key;
  Promise.resolve State.{ ...state, mode: Mode.Normal }
};

let handleKeyLocked editor state key => {
  handleKey' editor key;
  Promise.resolve state
};

let cursor = TextEditorCursorStyle.block;
let name = "View Mode";