type promise 'a 'e = Bs_promise.t 'a 'e;

type disposable;
type extensionContext;
type position;
type range;
type selection;
type textDocument;
type textEditor;
type textEditorEdit;
type textLine;

module Vscode = {
  /* internal */
  external vscode: Js.t {..} = "vscode" [@@bs.module];

  module Commands = {
    let executeCommand : string => unit =
      fun command => vscode##commands##executeCommand command;

    let executeCommandWithArg : string => 'a => unit =
      fun command arg => vscode##commands##executeCommand command arg;

    let registerCommand : string => ('a => unit) => disposable =
      fun name callback => vscode##commands##registerCommand name callback;
  };

  module Window = {
    let activeTextEditor : unit => textEditor =
      fun () => vscode##window##activeTextEditor;

    let onDidChangeActiveTextEditor : (textEditor => unit) => disposable =
      fun listener => vscode##window##onDidChangeActiveTextEditor listener;

    let showInformationMessage : string => unit =
      fun message => vscode##window##showInformationMessage message;

    let showQuickPick : array (Js.t {..} as 'a) => Js.t {..} => promise 'a unit =
      fun items options => vscode##window##showQUickPick items options;
  };
};


/* enums */

module TextEditorCursorStyle = {
  type t;

  external block : t = "TextEditorCursorStyle.Block" [@@bs.val] [@@bs.module "vscode"];
  external line : t = "TextEditorCursorStyle.Line" [@@bs.val] [@@bs.module "vscode"];
};

module TextEditorRevealType = {
  type t;

  external atTop : t = "TextEditorRevealType.AtTop" [@@bs.val] [@@bs.module "vscode"];
  external default : t = "TextEditorRevealType.Default" [@@bs.val] [@@bs.module "vscode"];
  external inCenter : t = "TextEditorRevealType.InCenter" [@@bs.val] [@@bs.module "vscode"];
  external inCenterIfOUtsideViewport : t = "TextEditorRevealType.InCenterIfOutsideViewport" [@@bs.val] [@@bs.module "vscode"];
};


/* objects */

module ExtensionContext = {
  external subscriptions : extensionContext => array disposable = "" [@@bs.get];
};

module Position = {
  type t = position;

  external make : int => int => t = "Position" [@@bs.new] [@@bs.module "vscode"];

  external character : t => int = "" [@@bs.get];
  external line : t => int = "" [@@bs.get];

  external translate : lineDelta::int => charDelta::int => position = "" [@@bs.send.pipe: t];
  external with_ : line::int? => character::int? => unit => position = "with" [@@bs.send.pipe: t];
};

module Range = {
  type t = range;

  external make : start::position => end_::position => t = "Range" [@@bs.new] [@@bs.module "vscode"];
};

module Selection = {
  type t = selection;

  external make : anchor::position => active::position => t = "Selection" [@@bs.new] [@@bs.module "vscode"];

  external active : t => position = "" [@@bs.get];
  external anchor : t => position = "" [@@bs.get];
  external end_ : t => position = "" [@@bs.get];
  external isEmpty : t => bool = "" [@@bs.get];
  external isReversed : t => bool = "" [@@bs.get];
  external isSingleLine : t => bool = "" [@@bs.get];
  external start : t => position = "" [@@bs.get];
};

module TextDocument = {
  type t = textDocument;

  external lineCount : t => int = "" [@@bs.get];

  external lineAt : int => textLine = "" [@@bs.send.pipe: t];
  external lineAtPosition : position => textLine = "lineAt" [@@bs.send.pipe: t];
};

module TextEditor = {
  type t = textEditor;

  external document : t => textDocument = "" [@@bs.get];
  external selection : t => selection = "" [@@bs.get];
  external setSelection : t => selection => unit = "selection" [@@bs.set];
  external selections : t => array selection = "" [@@bs.get];

  external edit : (textEditorEdit => unit) => promise Js.boolean unit = "" [@@bs.send.pipe: t];
  external revealRange : range => TextEditorRevealType.t => unit = "" [@@bs.send.pipe: t];
};


module TextEditorEdit = {
  type t = textEditorEdit;

  external delete : selection => unit = "" [@@bs.send.pipe: t];
  external insert : position => string => unit = "" [@@bs.send.pipe: t];
  external replace : range => string => unit = "" [@@bs.send.pipe: t];
};

module TextLine = {
  type t = textLine;

  external text : t => string = "" [@@bs.get];
};
