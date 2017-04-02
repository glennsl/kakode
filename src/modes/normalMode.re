open Require.Option;
open Require.Promise;
open Require.Vscode;

open Action;
open Mutators;
open CommandActions;
open EditActions;
open HistoryActions;
open SelectionActions;

let bindings = [

  /* move cursor */
  ('h', "", repeatable @@ selection moveLeft),
  ('j', "", repeatable @@ selection moveDown),
  ('k', "", repeatable @@ selection moveUp),
  ('l', "", repeatable @@ selection moveRight),
  /*
  ('g', "", goto (micromode)),
  ('&', "", align selection cursors),
  */

  /* select */
  ('w', "", repeatable @@ selection selectToNextWord),
  ('e', "", repeatable @@ selection selectToNextWordEnd),
  ('b', "", repeatable @@ selection selectToPreviousWord),
  ('x', "", repeatable @@ selection selectLine),
  ('%', "", selection selectAll),
  /*
  ('m', "", select to matching character (micromode)),

  ('[', "", select to object start),
  (']', "", select to object end),

  (';', "", reduce selections to cursor),
  (' ', "", reduce selections to main selection),

  ('\', "", rotate main selection),

  ('C', "", copy selection on next lines),

  ('z', "", restore selections from register),
  ('Z', "", save selections to register),
  */

  /* extend selection */
  /*
  | 'H' =>
  | 'J' =>
  | 'K' =>
  | 'L' =>
  | 'W' =>
  | 'E' =>
  | 'B' =>
  | 'X' =>
  | 'M' =>
  ('{', "", extend to object start),
  ('}', "", extend to object end),
  | 'G' =>
*/

  /* edit */
  ('d', "", edit erase),

  ('o', "", edit insertLineAbove |> then_ (selection moveDown) |> then_ (mode Mode.Insert)),
  ('O', "", edit insertLineBelow |> then_ (selection moveUp) |> then_ (mode Mode.Insert)),

  ('r', "", mode Mode.Replace),

  /*
  ('c', "", change),
  
  ('<', "", deindent),
  ('>', "", indent),

  ('`', "", convert selections to lower case),
  ('~', "", convert selections to upper case),

  ('@', "", convert tabs to spaces in selections),
  */

  /* history */
  ('u', "", undo),
  ('U', "", redo),
  /*
  ('.', "", repeat;),
  */

  /* micro modes */
  ('v', "", mode Mode.ViewTransient),
  ('V', "", mode Mode.ViewLocked),

  /* registers */
  /*
  ('"', "", select register),
  ('y', "", yank selection),
  ('p', "", paste after selection),
  ('P', "", paste before selection),
  ('R', "", replace selection with yanked),
  */

  /* search-ish */
  /*
  ('/', "", select next regex match (submode)),
  ('?', "", extend with next regex match (submode)),
  ('s', "", select regex matches in selection (submode)),
  ('S', "", split selected text on regex matches (submode)),
  | 
  ('n', "", select next match from current search pattern),
  ('N', "", extend next match from current search pattern),
  ('*', "", set current search pattern to main selection),
  ('|', "", pipe each selection through filter and replace with output (submode)),
  */

  /* macro */
  /*
  ('q', "", replay current macro),
  ('Q', "", start or end macro recording),
  */

  /* modes */
  ('i', "", mode Mode.Insert),
  /*
  ('I', "", insert at beginning of line),
  ('a', "", insert after selection),
  ('A', "", insert at end of line),
  
  // Submodes
  ('!', "", // insert output of command),
  ('$', "", // pipe each selection through shell command and keep the ones whose command succeed),
  */

  (':', "", command),
];

let actionByKey key => {
  let rec loop = fun
  | [] => None
  | [(k, _, action), ..._] when k == key => Some action
  | [_, ...rest] => loop rest;
  loop bindings;
};

let handleKeyDigit state params key => {
  let digit = int_of_char key - int_of_char '0';
  let count = params.Params.count
    |> Option.map (fun count => (count * 10) + digit)
    |> Option.or_ (digit > 0 ? Some digit : None);
  Promise.resolve State.{ ...state, params: { ...params, count }};
};

let handleKeyDefault editor state params key =>
  switch (actionByKey key) {
  | Some action =>
    action editor params state
  | None =>
    Promise.resolve state;
  };

let handleKey editor params state key =>
  switch key {
  | '0' .. '9' => handleKeyDigit state params key
  | _ =>          handleKeyDefault editor state params key
  };

let cursor = TextEditorCursorStyle.block;
let name = "Normal Mode";