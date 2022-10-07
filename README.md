# slow

Simply, this program will run any command you give it, and simulate the specified baud.

By default, it runs `ssh 127.0.0.1` at 1200 baud, effectively giving you your standard shell, but with 1200 characters per second.

## Usage

* `--baud`/`-b`: Baud to simulate. This is 1200 by default.
* `--command`/`-c`: Command to run. By default this is `ssh 127.0.0.1`.


## Building

`gcc -o slow slow.c`
