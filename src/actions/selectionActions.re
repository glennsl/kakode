open Require.Option;
open Require.Vscode;

let move v h =>
  fun selection (_: textEditor) => {
    let p =
      selection
      |> Selection.active
      |> Position.translate line::v char::h;
    Selection.make anchor::p active::p
  };

let moveLeft = move 0 (-1);
let moveDown = move 1 0;
let moveUp = move (-1) 0;
let moveRight = move 0 1;

let selectToNextWord selection editor => {
  open DocumentIterator;

  let iterator = make (TextEditor.document editor) (Selection.active selection);

  if (isEof iterator) {
    selection
  } else {
    if (Text.isOnBoundary iterator) {
      ignore (advance iterator);
    };

    while (isEol iterator) {
      ignore (advance iterator);
    };

    let start = pos iterator;
    ignore (advance iterator);

    if (iterator |> current |> Option.exists Text.isPunctuation) {
      ignore (advanceWhile Text.isPunctuation iterator);
    } else {
      ignore (advanceWhile Text.isWord iterator);
    };
    ignore (advanceWhile Text.isWhitespace iterator);

    Selection.make anchor::start active::(pos iterator)
  }
};

let selectToNextWordEnd selection editor => {
  open DocumentIterator;

  let iterator = make (TextEditor.document editor) (Selection.active selection);

  if (isEof iterator) {
    selection
  } else {
    if (Text.isOnBoundary iterator) {
      ignore (advance iterator);
    };

    while (isEol iterator) {
      ignore (advance iterator);
    };

    let start = pos iterator;
    ignore (advance iterator);

    ignore (advanceWhile Text.isWhitespace iterator);

    if (iterator |> current |> Option.exists Text.isPunctuation) {
      ignore (advanceWhile Text.isPunctuation iterator);
    } else {
      ignore (advanceWhile Text.isWord iterator);
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
    if (Text.isOnBoundary iterator) {
      ignore (advance iterator);
    };

    while (isEol iterator) {
      ignore (retreat iterator);
    };

    let end_ = pos iterator;

    ignore (retreatWhile Text.isWhitespace iterator);

    if (iterator |> current |> Option.exists Text.isPunctuation) {
      ignore (retreatWhile Text.isPunctuation iterator);
    } else {
      ignore (retreatWhile Text.isWord iterator);
    };

    Selection.make anchor::end_ active::(pos iterator)
  }
};

let selectLine selection editor => {
  let cursor = selection |> Selection.active;
  let line = editor
    |> TextEditor.document
    |> TextDocument.lineAtPosition cursor
    |> TextLine.text;

  let anchor = cursor |> Position.with_ char::0;
  let active = cursor |> Position.with_ char::(String.length line);
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