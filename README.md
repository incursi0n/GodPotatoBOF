# GodPotatoBOF

A Beacon Object File port of [GodPotato](https://github.com/BeichenDream/GodPotato) for Cobalt Strike.

This BOF triggers the GodPotato privilege escalation flow and supports two behaviors:

- default mode: steal a SYSTEM token and spawn a process to run a command
- `token` mode: steal a SYSTEM token and apply it to the current Beacon with `BeaconUseToken()`

## To start

1. Git clone the repo
2. Run `make`

## Usage

1. Import `godpotato.cna` into Cobalt Strike
2. Execute the BOF with the CNA alias

```text
godpotato [token] [-cmd <command>] [-pipe <name>]
```

Argument summary:

```text
(none)             Run "cmd /c whoami" as SYSTEM.
token              Apply a SYSTEM token to the current Beacon with BeaconUseToken().
-cmd <cmd>         Run a command as SYSTEM in a spawned process.
-pipe <name>       Use a custom named pipe. Default is a random pipe name.
help,-h,--help,/?  Show this help.
```

Examples:

```text
godpotato
godpotato token
godpotato help
godpotato -cmd "cmd /c whoami /priv"
godpotato -cmd "cmd /c dir"
godpotato -cmd "cmd /c whoami" -pipe "mycustompipe"
godpotato -cmd "cmd /c whoami" -pipe "token"
```

Credits:

- https://github.com/BeichenDream/GodPotato
- https://github.com/MEhrn00/boflink
- https://github.com/trustedsec/CS-Situational-Awareness-BOF/tree/master/src/base_template
- https://github.com/CodeXTF2/bof_template
