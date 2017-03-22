open Require.Vscode;

type mode = {
  handleKey: textEditor => state => char => Js.promise state unit,
  cursor: TextEditorCursorStyle.t,
  name: string
}
and state = {
  mode: mode
};