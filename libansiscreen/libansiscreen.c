#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include "libansiscreen.h"

/* Save state of tty flags to restore at exit */
int saved_flags = -1;
int saved_flags_set = 0;

void clearscreen() 
    {
        printf("%c[2J", ESC);
        flush();
    }

void moveto(line, column)
    int line;
    int column;
    {
        if (line < 0 || column < 0) {
            puts("libansiscreen:moveTo: Line and column must be >= 0");
            exit(1);
        }
        printf("%c[%d;%df", ESC, line + 1, column + 1);
        flush();
    }

void settermraw() 
    {
      struct sgttyb mode;
      ioctl(1, TIOCGETP, &mode);
      
      if(!saved_flags_set) {
        saved_flags = mode.sg_flags;
        saved_flags_set = 1;
      }
      
      mode.sg_flags |= RAW;
      mode.sg_flags |= LITOUT;
      mode.sg_flags |= PASS8;
      mode.sg_flags |= FLUSHO;
      mode.sg_flags &= ~ECHO;
      mode.sg_flags &= ~XTABS;
      mode.sg_flags &= ~CRMOD;
      ioctl(1, TIOCSETP, &mode);
      
      /* Set Telnet character mode */
      /*printf("%c%c%c", 255, 252, 34);*/
    }
    
void settermcooked() 
    {
      struct sgttyb mode;
      if(saved_flags_set) {
        ioctl(1, TIOCGETP, &mode);
        mode.sg_flags = saved_flags;
        ioctl(1, TIOCSETP, &mode);
      }
    }
    
    
void flush() {
  fflush(stdout);
}
    
void getcursorpos(line, column) 
    int *line;
    int *column;
    {        
        int read;
        *line = 0;
        *column = 0;
        
        settermraw();
        printf("%c[6n", ESC);
        if(getchar() != ESC || getchar() != '[') {
            printf ("Invalid response to getcursorpos\n");
            settermcooked();
            exit(1);
        }
        
        read = getchar();
        while (read >= '0' && read <= '9') {
            *line = *line * 10 + (read - '0');
            read = getchar();
        }
        
        if (read != ';') {
            printf ("Invalid formatted response to getcursorpos\n");
            settermcooked();
            exit(1);
        }
        
        read = getchar();
        while (read >= '0' && read <= '9') {
            *column = *column * 10 + (read - '0');
            read = getchar();
        }
        
        if (read != 'R') {
            printf ("Invalid terminator to getcursorpos\n");
            settermcooked();
            exit(1);
        }
        settermcooked();
    }
    
void getscreensize(lines, columns)
    int *lines;
    int *columns;
    {
        int curLine;
        int curColumn;
        
        getcursorpos(&curLine, &curColumn);
        moveto(999, 999);
        getcursorpos(lines, columns);
        moveto(curLine, curColumn);
    }
    
void setcolor (color)
    int color;
    {
        printf("%c[%dm", ESC, color);
        flush();
    }

void setfgcolor (color)
    int color;
    {
        setcolor (color + 30);
    }

void setbgcolor (color) 
    int color;
    {
        setcolor (color + 40);
    }

void cursorup(number)
    int number;
    {
        printf("%c[%dA", ESC, number);
        flush();
    }

void cursordown(number)
    int number;
    {
        printf("%c[%dB", ESC, number);
        flush();
    }

void cursorforward(number)
    int number;
    {
        printf("%c[%dC", ESC, number);
        flush();
    }

void cursorbackward(number)
    int number;
    {
        printf("%c[%dD", ESC, number);
        flush();
    }

/* Drawing functions */
void drawbox(top, left, bottom, right, clear) 
    int top;
    int left;
    int bottom;
    int right;
    int clear;
    {
        int column;
        int line;

        /* Switch to graphics mode */
        printf ("%c(0", ESC);
        
        /* Draw top */
        moveto(top, left);
        putchar(BOTTOM_RIGHT);
        for (column = left + 1; column < right; column++) {
            putchar (LEFT_RIGHT);
        }
        putchar(LEFT_BOTTOM);


        /* Draw bottom */
        moveto(bottom, left);
        putchar(TOP_RIGHT);
        for (column = left + 1; column < right; column++) {
            putchar (LEFT_RIGHT);
        }
        putchar(TOP_LEFT);
        
        
        /* Draw sides */
        for (line = top + 1; line < bottom; line ++) {
            moveto(line, left);
            putchar(TOP_BOTTOM);
            if (clear) {
                for (column = left + 1; column < right; column++) {
                    putchar (' ');
                }
            } else {
                moveto(line, right);
            }
            putchar(TOP_BOTTOM);
        }
        
        /* Switch back to text mode */
        printf ("%c(B", ESC);
        flush();
    }
