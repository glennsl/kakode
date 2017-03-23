open Require.Promise;
open Require.Vscode;

let handleKey _ state key => {
  Vscode.Commands.executeCommandWithArg "default:type" { "text": (key |> Char.code |> Js.String.fromCharCode) };
  Promise.resolve state
};

let cursor = TextEditorCursorStyle.line;
let name = "Insert Mode";