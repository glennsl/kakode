type disposable;
type extensionContext;
type position;
type range;
type selection;
type statusBarItem;
type textDocument;
type textEditor;
type textEditorEdit;
type textLine;

/* internal */
external vscode: Js.t {..} = "vscode" [@@bs.module];


/* enums */

module StatusBarAlignment = {
  type t;

  let left : t = vscode##_StatusBarAlignment##_Left;
  let right : t = vscode##_StatusBarAlignment##_Right;
};

module TextEditorCursorStyle = {
  type t;

  let block : t = vscode##_TextEditorCursorStyle##_Block;
  let line : t = vscode##_TextEditorCursorStyle##_Line;
};

module TextEditorRevealType = {
  type t;

  let atTop : t = vscode##_TextEditorRevealType##_AtTop;
  let default : t = vscode##_TextEditorRevealType##_Default;
  let inCenter : t = vscode##_TextEditorRevealType##_InCenter;
  let inCenterIfOUtsideViewport : t = vscode##_TextEditorRevealType##_InCenterIfOutsideViewport;
};


module Vscode = {
  module Commands = {
    let executeCommand : string => unit =
      fun command => vscode##commands##executeCommand command;

    let executeCommandWithArg : string => 'a => unit =
      fun command arg => vscode##commands##executeCommand command arg;

    let registerCommand : string => ('a => unit) => disposable =
      fun name callback => vscode##commands##registerCommand name callback;
  };

  module Window = {
    let activeTextEditor : unit => option textEditor =
      fun () => Js.Undefined.to_opt vscode##window##activeTextEditor;

    let createStatusBarItem : StatusBarAlignment.t => statusBarItem =
      fun alignment => vscode##window##createStatusBarItem alignment;

    let onDidChangeActiveTextEditor : (textEditor => unit) => disposable =
      fun listener => vscode##window##onDidChangeActiveTextEditor listener;

    let showInformationMessage : string => unit =
      fun message => vscode##window##showInformationMessage message;

    let showQuickPick : array (Js.t {..} as 'a) => Js.t {..} => Js.promise 'a unit =
      fun items options => vscode##window##showQUickPick items options;
  };
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

module StatusBarItem = {
  type t = statusBarItem;

  external setText : t => string => unit = "text" [@@bs.set];

  external show : unit = "" [@@bs.send.pipe: t];
};

module TextDocument = {
  type t = textDocument;

  external lineCount : t => int = "" [@@bs.get];

  external lineAt : int => textLine = "" [@@bs.send.pipe: t];
  external lineAtPosition : position => textLine = "lineAt" [@@bs.send.pipe: t];
};

module TextEditor = {
  type t = textEditor;

  module Options = {
    type t;

    external setCursorStyle : t => TextEditorCursorStyle.t => unit = "cursorStyle" [@@bs.set];
  };

  external document : t => textDocument = "" [@@bs.get];
  external options : t => Options.t = "" [@@bs.get];
  external selection : t => selection = "" [@@bs.get];
  external setSelection : t => selection => unit = "selection" [@@bs.set];
  external selections : t => array selection = "" [@@bs.get];

  external edit : (textEditorEdit => unit) => Js.promise Js.boolean unit = "" [@@bs.send.pipe: t];
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