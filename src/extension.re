open Require.Promise;
open Require.Vscode;

let handleKey args => {
  open Mutators;
  open SelectionActions;

  let text = args##text;
  let editor = Vscode.Window.activeTextEditor ();

  let action = 
    switch text {
    | "h" => selection moveLeft;
    | "j" => selection moveDown;
    | "k" => selection moveUp;
    | "l" => selection moveRight;
    | "w" => selection selectToNextWord;
    | "e" => selection selectToNextWordEnd;
    | "b" => selection selectToPreviousWord;
    | "x" => selection selectLine;
    | "%" => selection selectAll;
    | _ => (fun _ => Promise.resolve ());
    };
  
  ignore (action editor);
};

let escape _ =>
  ();

let activate context => {
  let disposables = [|
    (Vscode.Commands.registerCommand "type" handleKey),
    (Vscode.Commands.registerCommand "kak.escape" escape)
  |];

  let subs = context |> ExtensionContext.subscriptions;
  disposables |> Js.Array.forEach (fun d => ignore (subs |> Js.Array.push d));
}