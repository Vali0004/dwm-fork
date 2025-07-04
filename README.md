# dwm - dynamic window manager

**dwm** is an extremely fast, small, and dynamic window manager for X.

## Requirements

In order to build dwm, you need the Xlib header files.

## Installation

Edit `config.mk` to match your local setup (dwm is installed into the `/usr/local` namespace by default).

Afterwards, enter the following command to build and install dwm (if necessary as root):

```sh
make clean install
```

## Running dwm

Add the following line to your `.xinitrc` to start dwm using `startx`:

```sh
exec dwm
```

To connect dwm to a specific display, make sure that the `DISPLAY` environment variable is set correctly, e.g.:

```sh
DISPLAY=foo.bar:1 exec dwm
```

(This will start dwm on display `:1` of the host `foo.bar`.)

To display status info in the bar, you can add something like this in your `.xinitrc`:

```sh
while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
do
    sleep 1
done &
exec dwm
```

## Configuration

The configuration of dwm is done by creating a custom `config.h` and (re)compiling the source code.
