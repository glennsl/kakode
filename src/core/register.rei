type t;

let makeStatic : string => char => initial::(list string)? => unit => t;
let makeDynamic : string => char => get::(unit => list string) => set::(list string => unit) => t;
/*
let get : t => string;
let set : t => string => unit;
*/
module Collection: {
  type register = t;
  type t;

  let fromList : list register => t;
  let find : char => t => register;
  let get : char => t => list string;
  let set : char => list string => t => unit;
};