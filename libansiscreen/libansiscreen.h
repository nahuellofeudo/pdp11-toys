/* Some routines to manipulate screen output
   using ANSI codes
*/

/* Escape character */
#define ESC 27

/* Color constants */
#define COLOR_BLACK   0
#define COLOR_RED     1
#define COLOR_GREEN   2
#define COLOR_YELLOW  3
#define COLOR_BLUE    4
#define COLOR_MAGENTA 5
#define COLOR_CYAN    6
#define COLOR_WHITE   7

/* Graphical character definitions */
#define TOP_LEFT                0x6a  /* j ┘ */
#define LEFT_BOTTOM             0x6b  /* k ┐ */
#define BOTTOM_RIGHT            0x6c  /* l ┌ */
#define TOP_RIGHT               0x6d  /* m └ */
#define TOP_LEFT_BOTTOM_RIGHT   0x6e  /* n ┼ */
#define LEFT_RIGHT              0x71  /* q ─ */
#define TOP_BOTTOM_RIGHT        0x74  /* t ├ */
#define TOP_LEFT_BOTTOM         0x75  /* u ┤ */
#define TOP_LEFT_RIGHT          0x76  /* v ┴ */
#define LEFT_BOTTOM_RIGHT       0x77  /* w ┬ */
#define TOP_BOTTOM              0x78  /* x │ */

/* Cursor / screen manipulation */
void clearscreen();
void moveto();
void getcursorpos();
void getscreensize();
void settermcooked();
void settermraw();
void flush();

/* Color change */
void setcolor ();
void setfgcolor ();
void setbgcolor ();

/* Cursor control */
void cursorup();
void cursordown();
void cursorforward();
void cursorbackward();

/* Drawing functions */
void drawbox();
