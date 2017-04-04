open Require.Option;
open Require.Promise;
open Require.Vscode;

let yank editor params state => {
  let document = TextEditor.document editor;
  let register = params.Params.register |> Option. getOr (Register.Register '"');
  let selections = TextEditor.selections editor
    |> Array.map Selection.asRange
    |> Array.map (fun range => TextDocument.getTextRange range document);
  Register.Collection.set register selections state.State.registers;

  Promise.resolve state
};

let pasteAfter editor params state => {
  let register = params.Params.register |> Option. getOr (Register.Register '"');
  let values = Register.Collection.get register state.State.registers;
  
  TextEditor.edit (fun builder =>
    TextEditor.selections editor
      |> Array.map Selection.end_
      |> Array.iteri
        (fun i pos => {
          let j = i >= Array.length values ? Array.length values - 1 : i;
          let text = Array.unsafe_get values j;
          TextEditorEdit.insert pos text builder;
        })
  ) editor
  |> Promise.then_ (fun _ => state);
};

let pasteBefore editor params state => {
  let register = params.Params.register |> Option. getOr (Register.Register '"');
  let values = Register.Collection.get register state.State.registers;
  
  TextEditor.edit (fun builder =>
    TextEditor.selections editor
      |> Array.map Selection.start
      |> Array.iteri
        (fun i pos => {
          let j = i >= Array.length values ? Array.length values - 1 : i;
          let text = Array.unsafe_get values j;
          TextEditorEdit.insert pos text builder;
        })
  ) editor
  |> Promise.then_ (fun _ => state);
};

let pasteReplace editor params state => {
  let register = params.Params.register |> Option. getOr (Register.Register '"');
  let values = Register.Collection.get register state.State.registers;
  
  TextEditor.edit (fun builder =>
    TextEditor.selections editor
      |> Array.map Selection.asRange
      |> Array.iteri
        (fun i range => {
          let j = i >= Array.length values ? Array.length values - 1 : i;
          let text = Array.unsafe_get values j;
          TextEditorEdit.replace range text builder;
        })
  ) editor
  |> Promise.then_ (fun _ => state);
};