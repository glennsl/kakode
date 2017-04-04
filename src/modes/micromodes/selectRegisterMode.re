open Require.Promise;
open Require.Vscode;

let handleKey _ params state key =>
  Promise.resolve State.{ ...state, params: Params.{ ...params, register: Some (Register key) } };

let cursor = TextEditorCursorStyle.block;
let name = "Select Register Mode"