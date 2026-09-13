# clout

`clout` is a small Unix utility for colorizing command standard error output.

It executes a command, captures its standard error, and writes the output using an ANSI color.

The project is written in C and is built around basic Unix process and file descriptor primitives such as `fork(2)`, `execvp(3)`, `pipe(2)`, and `dup2(2)`.

## Features

* Execute commands using `execvp(3)`
* Capture standard error using `pipe(2)`
* Redirect file descriptors using `dup2(2)`
* Create child processes using `fork(2)`
* Colorize standard error output with ANSI escape sequences
* Select the stderr color with `-e`
* Built-in manual page

## Usage

```sh
clout [-e color] command [argument ...]
```

The default stderr color is red.

For example:

```sh
clout ls /
```

Use a different color:

```sh
clout -e green ls /
```

Color the stderr output of a command:

```sh
clout -e yellow sh -c 'echo "error" >&2'
```

Display usage information:

```sh
clout -h
```

## Colors

The following colors are supported:

* black
* red
* green
* yellow
* blue
* purple
* cyan
* white


## Building

`clout` uses a standard POSIX-style Makefile and does not require an external build system.

Build:

```sh
make
```

Clean:

```sh
make clean
```

## Project Structure

```text
clout/
├── Makefile
├── README.md
├── LICENSE
├── clout.1
├── clout.c
├── color.c
└── color.h
```

## Documentation

The manual page is available as `clout(1)`.

Read it directly from the source tree:

```sh
man ./clout.1
```

Or render it with `mandoc`:

```sh
mandoc clout.1
```

Check the manual page for errors:

```sh
mandoc -T lint clout.1
```

## Goals

`clout` is also a learning project focused on Unix systems programming.

Future development will explore:

* stdout colorization
* simultaneous stdout and stderr handling
* `poll(2)`-based I/O
* command exit status propagation
* improved command-line handling
* additional color and formatting options

## License

Copyright (C) 2026 Mahdi Feyzolahy

License: BSD-2-Clause

See the `LICENSE` file for the full license text.

