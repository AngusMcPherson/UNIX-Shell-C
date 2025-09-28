# Simple UNIX Shell in C

## Overview
MiniShell is a simple UNIX shell implemented in C that allows users to execute commands, manage background and foreground processes, and handle process completion signals. It demonstrates fundamental concepts of process creation, signal handling, and command parsing in a POSIX environment.

---

## Features
- Executes commands in the **foreground** or **background**.
- Tracks background processes and notifies when they complete.
- Supports `cd` command to change directories.
- Handles `SIGCHLD` signals to detect child process termination.
- Parses commands with multiple arguments separated by spaces or tabs.
- Ignores comments (lines starting with `#`) and blank input.

---

## Usage

### Compile the program:
```bash
gcc minishell.c -o minishell
