open Require.Promise;
open Require.Vscode;

let undo _ state => {
  Vscode.Commands.executeCommand "undo";
  Promise.resolve state
};
  
let redo _ state => {
  Vscode.Commands.executeCommand "redo";
  Promise.resolve state
};