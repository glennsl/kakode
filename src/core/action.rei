open Require.Promise;
open Require.Vscode;

type t = textEditor => Params.t => State.t => Promise.t State.t unit;

let then_ : t => t => t;