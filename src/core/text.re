open Require.Option;

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