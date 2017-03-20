type disposable;
type extensionContext;

module Internal = {
  external vscode: Js.t {..} = "vscode" [@@bs.module];
};

module Commands = {
  let registerCommand : string => (unit => unit) => disposable =
    fun name callback => Internal.vscode##commands##registerCommand name callback;
};

module Window = {
  let showInformationMessage : string => unit =
    fun message => Internal.vscode##window##showInformationMessage message;
};

external subscriptions : extensionContext => array disposable = "" [@@bs.get];