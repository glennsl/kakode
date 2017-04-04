type t;
type symbol = | Register of char;

let makeStatic : string => symbol => initial::(array string)? => unit => t;
let makeDynamic : string => symbol => get::(unit => array string) => set::(array string => unit) => t;
/*
let get : t => string;
let set : t => string => unit;
*/
module Collection: {
  type register = t;
  type t;

  let fromList : list register => t;
  let find : symbol => t => register;
  let get : symbol => t => array string;
  let set : symbol => array string => t => unit;
};