#include <stdio.h> 
#include <unistd.h>
#include <time.h>
#include "../libansiscreen/libansiscreen.h"

char * values = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
#define MAX_ITER 69 * 2
#define HEIGHT 25 
#define WIDTH 80 

#define RE_START -2.0
#define RE_END 1.0
#define IM_START -1.2 
#define IM_END 1.3


float abs(x)
    float x;
    {
        if (x < 0) return -x;
        return x;
    }

int mandelbrot (cr, ci) 
    float cr;
    float ci;
        {
        int c = 0;
        float zr = 0;
        float zi = 0;
        float _zr = 0;
        float _zi = 0;

        while (c < MAX_ITER) {
            if (abs((zr * zr) + (zi * zi)) > 10) return c;

            _zr = (zr * zr) - (zi * zi) ;
            _zi = 2 * zr * zi;
            zr = _zr + cr;
            zi = _zi + ci;
            ++c;
        }
        return MAX_ITER / 2;
    }


int main (argc, argv) 
    int argc;
    char *argv;
    {
        int x, y;
        int screen_height, screen_width;
        int color;
        float r; 
        float i;
        
        settermraw();
        getscreensize(&screen_height, &screen_width);

        while(1) {

            putchar('\n');
            putchar('\r');
            putchar('\n');
            putchar('\r');

            for (y = 0; y < screen_height - 1; ++y) {
                for (x = 0; x < screen_width; ++x) {

                    r = RE_START + ((x * 1.0) / screen_width) * (RE_END - RE_START);
                    i = IM_START + ((y * 1.0) / screen_height) * (IM_END - IM_START);
                    color = mandelbrot(r, i);
                    
                    setfgcolor(7 - (color / 10));
                    putchar(values[color]);
                }
                putchar('\n');
                putchar('\r');
            }

            sleep(5);
            
            putchar('\n');
            putchar('\r');
            putchar('\n');
            putchar('\r');

        }
        
        settermcooked();
    } 
