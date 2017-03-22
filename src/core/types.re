open Require.Vscode;

type mode = {
  handleKey: textEditor => char => state
}
and state = {
  mode: mode
};