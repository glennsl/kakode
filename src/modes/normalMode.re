open Require.Promise;
open Require.Vscode;

let handleKey editor state key => {
  open Mutators;
  open SelectionActions;

  let action = 
    switch key {
    | 'h' => selection moveLeft;
    | 'j' => selection moveDown;
    | 'k' => selection moveUp;
    | 'l' => selection moveRight;
    | 'w' => selection selectToNextWord;
    | 'e' => selection selectToNextWordEnd;
    | 'b' => selection selectToPreviousWord;
    | 'x' => selection selectLine;
    | '%' => selection selectAll;

    | 'i' => mode InsertMode.mode;

    | _ => (fun _ _ => Promise.resolve state);
    };
  
  action editor state
};

let mode: Types.mode = {
  handleKey,
  cursor: TextEditorCursorStyle.block,
  name: "Normal Mode"
}