open Rebase;
open Vscode;

type t = {
  document: textDocument,
  mutable pos: position
};

let make document initalPosition =>
  { document, pos: initalPosition };

let pos iter =>
  iter.pos;

let line iter =>
  iter.document
  |> TextDocument.lineAtPosition iter.pos /* unsafe? */
  |> TextLine.text;

let get i iter => {
  let l = line iter;

  i < String.length l ? 
    Some (String.get l i) :
    None;
};

let current iter =>
  get (iter.pos |> Position.character) iter;

let prev iter => 
  get ((iter.pos |> Position.character) - 1) iter;

let next iter => 
  get ((iter.pos |> Position.character) + 1) iter;

let isEol iter =>
  (iter.pos |> Position.character) >= (String.length (line iter));

let isEof iter => {
  let lineIndex = Position.line iter.pos;
  let lineCount = TextDocument.lineCount iter.document;

  lineIndex > lineCount || (lineIndex == lineCount && isEol iter)
};

let isBol iter =>
  Position.character iter.pos <= 0;

let isBof iter =>
  Position.line iter.pos < 0 || (Position.line iter.pos == 0 && Position.character iter.pos <= 0);

let advance iter =>
  if (isEof iter) {
    None
  } else {
    if (isEol iter) {
      iter.pos = Position.make (Position.line iter.pos + 1) 0;
    } else {
      iter.pos = Position.translate lineDelta::0 charDelta::1 iter.pos;
    };
    Some iter.pos
  };

let advanceWhile predicate iter => {
  let break = ref false;
  while (not !break && not (isEof iter) && current iter |> Option.exists predicate) {
    if (advance iter |> Option.isNone) {
      break := true
    }
  };
  !break ? None : Some iter.pos
};

let retreat iter =>
  if (isBof iter) {
    None
  } else {
    if (isBol iter) {
      let lineIndex = Position.line iter.pos - 1;
      let charIndex = iter.document |> TextDocument.lineAt lineIndex |> TextLine.text |> String.length;
      iter.pos = Position.make lineIndex charIndex;
    } else {
      iter.pos = Position.translate lineDelta::0 charDelta::(-1) iter.pos;
    };
    Some iter.pos
  };

let retreatWhile predicate iter => {
  let break = ref false;
  while (not !break && not (isBof iter) && current iter |> Option.exists predicate) {
    if (retreat iter |> Option.isNone) {
      break := true
    }
  };
  !break ? None : Some iter.pos
};
