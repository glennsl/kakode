open Rebase;
open Vscode;
module Promise = Bs_promise;

let selection action =>
  fun editor => {
    TextEditor.setSelection editor (action (editor |> TextEditor.selection) editor);
    Promise.resolve ()
  };

let move v h =>
  fun selection _ => {
    let p =
      selection
      |> Selection.active
      |> Position.translate lineDelta::v charDelta::h;
    Selection.make anchor::p active::p
  };

let moveLeft = move 0 (-1);
let moveDown = move 1 0;
let moveUp = move (-1) 0;
let moveRight = move 0 1;

type charCategory =
| Word
| Punctuation
| Whitespace;

let isAlphaNum    c => [%re "/^[a-z0-9]+$/i"] |> Js.Re.test (c |> Char.code |> Js.String.fromCharCode);
let isWord        c => c == '_' || isAlphaNum c;
let isWhitespace  c => c == ' ' || c == '\t';
let isPunctuation c => not (isWord c || isWhitespace c);

let categorizeChar c =>
  isWord c       ? Word :
  isWhitespace c ? Whitespace :
                   Punctuation;

let isOnBoundary iterator => {
  let current =
    iterator
    |> DocumentIterator.current
    |> Option.map categorizeChar;
  let next =
    iterator
    |> DocumentIterator.current
    |> Option.map categorizeChar;
  
  current == next
};

let selectToNextWord selection editor => {
  open DocumentIterator;

  let iterator = make (TextEditor.document editor) (Selection.active selection);

  if (isEof iterator) {
    selection
  } else {
    if (isOnBoundary iterator) {
      ignore (advance iterator);
    };

    while (isEol iterator) {
      ignore (advance iterator);
    };

    let start = pos iterator;
    ignore (advance iterator);

    if (iterator |> current |> Option.exists isPunctuation) {
      ignore (advanceWhile isPunctuation iterator);
    } else {
      ignore (advanceWhile isWord iterator);
    };
    ignore (advanceWhile isWhitespace iterator);

    Selection.make anchor::start active::(pos iterator)
  }
};

let selectToNextWordEnd selection editor => {
  open DocumentIterator;

  let iterator = make (TextEditor.document editor) (Selection.active selection);

  if (isEof iterator) {
    selection
  } else {
    if (isOnBoundary iterator) {
      ignore (advance iterator);
    };

    while (isEol iterator) {
      ignore (advance iterator);
    };

    let start = pos iterator;
    ignore (advance iterator);

    ignore (advanceWhile isWhitespace iterator);

    if (iterator |> current |> Option.exists isPunctuation) {
      ignore (advanceWhile isPunctuation iterator);
    } else {
      ignore (advanceWhile isWord iterator);
    };

    Selection.make anchor::start active::(pos iterator)
  }
};

let selectToPreviousWord selection editor => {
  open DocumentIterator;

  let iterator = make (TextEditor.document editor) (Selection.active selection);

  if (isBof iterator) {
    selection
  } else {
    ignore (retreat iterator);
    if (isOnBoundary iterator) {
      ignore (advance iterator);
    };

    while (isEol iterator) {
      ignore (retreat iterator);
    };

    let end_ = pos iterator;

    ignore (retreatWhile isWhitespace iterator);

    if (iterator |> current |> Option.exists isPunctuation) {
      ignore (retreatWhile isPunctuation iterator);
    } else {
      ignore (retreatWhile isWord iterator);
    };

    Selection.make anchor::(pos iterator) active::end_
  }
};

let selectLine selection editor => {
  let cursor = selection |> Selection.active;
  let line = editor
    |> TextEditor.document
    |> TextDocument.lineAtPosition cursor
    |> TextLine.text;

  let anchor = cursor |> Position.with_ character::0 ();
  let active = cursor |> Position.with_ character::(String.length line) ();
  Selection.make ::anchor ::active
};

let selectAll _ editor => {
  let document = editor |> TextEditor.document;
  let lastLineIndex = (document |> TextDocument.lineCount) - 1;
  let lastCharIndex = (document |> TextDocument.lineAt lastLineIndex |> TextLine.text |> String.length);
  let anchor = Position.make 0 0;
  let active = Position.make lastLineIndex lastCharIndex;

  Selection.make ::anchor ::active
};

let handleKey args => {
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