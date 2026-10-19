# clout

`clout` is a small Unix utility that colorizes the standard output and standard error streams of a command independently.

It executes a command, captures its standard output and standard error through separate pipes, and writes each stream using a configurable ANSI color. By default, stdout is green and stderr is red.

The project is written in C and uses fundamental Unix process and I/O interfaces, including `fork(2)`, `execvp(3)`, `pipe(2)`, `dup2(2)`, and `poll(2)`.

## Features

- Execute commands using `execvp(3)`.
- Capture stdout and stderr through separate pipes.
- Colorize stdout and stderr independently.
- Select the stdout color with `-o`.
- Select the stderr color with `-e`.
- Read both output streams using `poll(2)`.
- Handle nonblocking pipe I/O.
- Propagate the command's exit status.
- Report `128 + signal number` when the command terminates due to a signal.
- Provide a built-in usage message with `-h`.
- Include a manual page.

## Usage

```sh
clout [-h] [-e color] [-o color] command [argument ...]
```

The default colors are:

- stdout: green
- stderr: red

### Examples

Run a command with the default colors:

```sh
clout ls /
```

Set the stdout color:

```sh
clout -o cyan ls /
```

Set the stderr color:

```sh
clout -e yellow sh -c 'echo "error" >&2'
```

Configure both streams independently:

```sh
clout -o green -e red sh -c 'echo "output"; echo "error" >&2'
```

Display usage information:

```sh
clout -h
```

## Colors

The following colors are supported:

- black
- red
- green
- yellow
- blue
- purple
- cyan
- white
- gray
- bright-red
- bright-green
- bright-yellow
- bright-blue
- bright-purple
- bright-cyan
- bright-white

## Building

`clout` uses a standard Makefile and does not require an external build system.

Build:

```sh
make
```

Clean build artifacts:

```sh
make clean
```

## Documentation

The manual page is available as `clout(1)`.

Read it directly from the source tree:

```sh
man ./clout.1
```

Render it with `mandoc`:

```sh
mandoc clout.1
```

Check the manual page for errors:

```sh
mandoc -T lint clout.1
```

## License

Copyright (C) 2026 Mahdi Feyzolahy

License: BSD-2-Clause

See the `LICENSE` file for the full license text.