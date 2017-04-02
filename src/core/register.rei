type t;
type symbol = | Register of char;

let makeStatic : string => symbol => initial::(list string)? => unit => t;
let makeDynamic : string => symbol => get::(unit => list string) => set::(list string => unit) => t;
/*
let get : t => string;
let set : t => string => unit;
*/
module Collection: {
  type register = t;
  type t;

  let fromList : list register => t;
  let find : symbol => t => register;
  let get : symbol => t => list string;
  let set : symbol => list string => t => unit;
};