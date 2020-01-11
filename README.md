# pdp11-toys
Small, simple programs written in K&amp;R C, to compile and run in P(i)DP11 machines

## Contents:

* labyrinth: draws random labyrinths on screen using ANSI/Vt100 escape sequences, and solves them
* mandel: draws a mandelbrot set on screen using ANSI/Vt100 escape sequences
* libansiscreen: library to manipulate cursor and screen. More limited than curses, but works on screen sizes other than 80x24. It's used by the other programs.

The folders that contain programs also have a build.sh script to compile the code. It's easier and more straightforward than using makefiles.
