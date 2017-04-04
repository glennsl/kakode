type symbol = | Register of char;

type t = {
  name: string,
  symbol: symbol,
  get: unit => array string,
  set: array string => unit
};

let makeStatic name symbol ::initial=[||] () => {
  let content = ref initial;
  {
    name,
    symbol,
    get: fun () => !content,
    set: fun values => content := values
  }
};

let makeDynamic name symbol ::get ::set => {
  name,
  symbol,
  get,
  set
};

let get register =>
  register.get ();

let set values register =>
  register.set values;

module Collection = {
  type register = t;
  type nonrec t = list register;

  let fromList registers =>
    registers;

  let rec find symbol => fun
    | [] => raise Not_found
    | [head, ...tail] =>
      head.symbol == symbol ?
        head :
        find symbol tail;

  let get symbol collection =>
    find symbol collection |> get;

  let set symbol values collection =>
    find symbol collection |> set values;
};
