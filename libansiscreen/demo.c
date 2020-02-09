#include "libansiscreen.h"

#define MIN(a, b) (a>b?b:a)
#define MAX(a, b) (a<b?b:a)


int main() 
    {
        int lines;
        int columns;
        int loopmax;
        int loop;
        
        int x1, x2, y1, y2;
        
        getscreensize(&lines, &columns);
        
        loopmax = MIN(lines, columns) / 2;
        
        clearscreen();
        
        for (loop = 0; loop < loopmax; loop++) {
          drawbox(loop, loop, lines - loop - 1, columns - loop - 1, 0);          
        }
        
        sleep(10);
        
        for (loop = 0; loop < 100; loop++) {
          x1 = rand() % columns;
          x2 = rand() % columns;
          y1 = rand() % lines;
          y2 = rand() % lines;
          drawbox(MIN(y1, y2), MIN(x1, x2), MAX(y1, y2), MAX(x1, x2), 1);
          usleep(300000);
        }
        
        clearscreen();
        moveto(0,0);
        exit(0);
    }
