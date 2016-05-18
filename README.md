# ttyinject

A simple tool used to inject typed input from one virtual terminal to another.

## Build

```
make
```

## Usage

```
./ttyinject /dev/tty1
```

Now you can start typing. Look at the target terminal and see if it's working.

You can press twice the <kbd>ESC</kbd> key to exit the program.

## Why?

I couldn't find a USB keyboard to plug into a Raspberry Pi.

I launched this on the pi through SSH so I could write on the main console just like if my laptop's keyboard was attached to the board.
