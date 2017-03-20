
let activate context => {
  Js.log "Congratulations, your extension \"reasonable-vscode-starter\" is now active!";

  let disposable =
    Vscode.Commands.registerCommand "extension.sayHello"
      (fun () => Vscode.Window.showInformationMessage "Hello Reason!");

  Vscode.subscriptions context |> Js.Array.push disposable;
}