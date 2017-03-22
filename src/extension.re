open Require.Vscode;

let activate context => {

  let modeMachine = ModeMachine.make ();

  let disposables = [|
    (Vscode.Commands.registerCommand "type"
      (fun args => ModeMachine.handleKey modeMachine (String.get args##text 0))),

    (Vscode.Commands.registerCommand "kak.escape"
      (ModeMachine.escape modeMachine))
  |];

  let subs = context |> ExtensionContext.subscriptions;
  disposables |> Js.Array.forEach (fun d => ignore (subs |> Js.Array.push d));
}