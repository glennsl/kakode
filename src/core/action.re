
let then_ g f  =>
  fun editor state => f editor state |> g editor;