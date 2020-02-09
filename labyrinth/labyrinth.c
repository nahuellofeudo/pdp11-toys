#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include  "../libansiscreen/libansiscreen.h"

/* Screen stuff */
int width;
int height;

char *map;
int mapsize;

#define WALL 'X'
#define SPACE ' '
#define PATH '.'

/* Start and end positions along the x axis*/
int start_pos_x;
int start_pos_y;
int end_pos_x;
int end_pos_y;

char movements[24][4] = {
  "udrl", "udlr", "urdl", "urld", "ulrd", "uldr", 
  "dulr", "durl", "dlur", "dlru", "drul", "drlu", 
  "lrud", "lrdu", "ldru", "ldur", "ludr", "lurd", 
  "rldu", "rlud", "rdlu", "rdul", "ruld", "rudl"
}; 
 


char getpos(x, y) 
  int x;
  int y;
  {
    return map[(y * width) + x];
  }
  
void setpos(x, y, val) 
  int x;
  int y;
  char val;
  {
    map[(y * width) + x] = val;
    moveto(y, x);
    putchar(val); 
    flush();
    usleep(50000); 
  }


void generate(x, y) 
  int x;
  int y;
  {
    int try;
    int movement_order = rand() % 24;
    char * order;

    setpos(x, y, SPACE);
    
    order = movements[movement_order];
    for(try = 0; try < 4; try++) {
      char dir = order[try];
      switch (dir) {
        
        case 'u':
          if (y > 2 && getpos(x, y - 2) != SPACE) {
            setpos(x, y - 1, SPACE);
            generate(x, y - 2);
          }
          break;
        
        case 'd':
          if (y < (height - 3) && getpos(x, y + 2) != SPACE) {
            setpos(x, y + 1, SPACE);
            generate(x, y + 2);
          }
          break;
        
        case 'r':
          if (x < (width - 3) && getpos(x + 2, y) != SPACE) {
            setpos(x + 1, y, SPACE);
            generate(x + 2, y);
          }
          break;
        
        case 'l':
          if (x > 2 && getpos(x - 2, y) != SPACE) {
            setpos(x - 1, y, SPACE);
            generate(x - 2, y);
          }
          break;
      }
    }
  }  
  
  
  
int solve(last_pos_x, last_pos_y, pos_x, pos_y) 
  int last_pos_x;
  int last_pos_y;
  int pos_x;
  int pos_y;
  {
    int result = 0;
    
    /* Validate params */
    if (pos_x < 1 || pos_y < 1 || pos_x > width - 1 || pos_y > height - 1) return 0;
    
    /* See that the new space isn't blocked */
    if (getpos((last_pos_x + pos_x) / 2, (last_pos_y + pos_y) / 2) != SPACE) return 0;
      
    setpos((last_pos_x + pos_x) / 2, (last_pos_y + pos_y) / 2, PATH);
    setpos(pos_x, pos_y, PATH);
    if (pos_x == end_pos_x && pos_y == end_pos_y) return 1;
  
    
    /* Try down, left, right, up, then backtrack */
    if (solve(pos_x, pos_y, pos_x, pos_y + 2)) return 1;
    if (solve(pos_x, pos_y, pos_x + 2, pos_y)) return 1;
    if (solve(pos_x, pos_y, pos_x - 2, pos_y)) return 1;
    if (solve(pos_x, pos_y, pos_x, pos_y - 2)) return 1;
    
    /* No luck. Backtrack */
    setpos(pos_x, pos_y, SPACE);
    setpos((last_pos_x + pos_x) / 2, (last_pos_y + pos_y) / 2, SPACE);
    return 0;
  }
  

void die(param)
  int param;
  {
    clearscreen();
    settermcooked();
    putchar('\n');
    exit(0);
  }

  
  
void main(argc, argv)
  int argc;
  char *argv;
  {
    int x;
    int y;
          
    srand(time(0));
    signal(SIGINT, die);
    
    settermraw();
    clearscreen();
    
    getscreensize(&height, &width);
    
    /* round off the size of the screen to the nearest odd number */
    if (height % 2 == 0) height--;
    if (width % 2 == 0) width--;
    
    
    /* Define start and end positions */
    start_pos_x = 1;
    start_pos_y = 1;
    end_pos_x = width - 2;
    end_pos_y = height - 2;
    
    
    clearscreen();
    
    mapsize = height * width * sizeof(char);
    map = (char *)malloc(mapsize);
    while (1) {
      setfgcolor(COLOR_WHITE);
      setbgcolor(COLOR_WHITE);
      for(y = 0; y < height; y++) {
        moveto(y, 0);
        for (x = 0; x < width; x++) {
          map[(y * width) + x] = WALL;
          putchar(WALL);
        }
        flush();
        usleep(100000);
      }
      moveto(0, 0);
      setbgcolor(COLOR_BLACK);
      sleep(5);
      
      generate(start_pos_x, start_pos_y);    
      moveto(0, 0);

      sleep(10);
      solve(start_pos_x, start_pos_y, start_pos_x, start_pos_y);
      
      sleep(30);
      clearscreen();
    }
  }

  
