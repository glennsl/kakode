open Require.Promise;
open Require.Vscode;

let exec = Vscode.Commands.executeCommand;

type command = Js.t {.
  label: string,
  description: string,
  action: unit => unit
};

let defineCommand commands description action =>
  commands |> List.map (fun label => { "label": label, "description": description, "action": action });

let commands = [
  defineCommand ["edit", "e"] "find and open file"
    (fun () => exec "workbench.action.quickOpen"),

  /* brokem, alias "edit" command?
  defineCommand ["buffer", "b"] "find and open tab"
    (fun () => ())
  */

  defineCommand ["buffernext", "bn"] "switch to next tab"
      (fun () => exec "workbench.action.nextEditor"),

  defineCommand ["bufferprev", "bp"] "switch to previous tab"
      (fun () => exec "workbench.action.previousEditor"),

  defineCommand ["delbuf", "db"] "close tab"
      (fun () => exec "workbench.action.closeActiveEditor"),

  defineCommand ["write", "w"] "save current file"
      (fun () => exec "workbench.action.files.save"),

  defineCommand ["writeall", "wa"] "save all files"
      (fun () => exec "workbench.action.files.saveAll"),

  defineCommand ["writeas"] "save current file as"
      (fun () => exec "workbench.action.files.saveAs"),

  defineCommand ["quit", "q"] "exit vscode"
      (fun () => exec "workbench.action.quit"),

  defineCommand ["wq"] "save current file and quit"
      (fun () => {
        exec "workbench.action.files.save";
        exec "workbench.action.quit"
      }),

  defineCommand ["waq"] "save all files and quit"
      (fun () => {
        exec "workbench.action.saveAll";
        exec "workbench.action.quit"
      }),
]
|> List.flatten
|> Array.of_list;

let command _ _ state =>
  Vscode.Window.showQuickPick commands { "placeHolder": ": command mode" }
  |> Promise.then_
      (fun (maybeItem: Js.undefined command) =>
        switch (Js.Undefined.to_opt maybeItem) {
        | Some item => let a = item##action; a ()
        | None => ()
        }
      )
  |> Promise.then_ (fun _ => state);