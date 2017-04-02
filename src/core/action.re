open Require.Promise;
open Require.Vscode;

type t = textEditor => Params.t => State.t => Promise.t State.t unit;

let then_ g f  =>
  fun editor params state => f editor params state |> Promise.andThen (g editor params);