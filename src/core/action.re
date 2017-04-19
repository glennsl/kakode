open Require.Promise;
open Require.Vscode;

type t = textEditor => Params.t => State.t => Promise.t State.t;

let then_ g f  =>
  fun editor params state =>
    f editor params state |> Promise.then_ (g editor params);