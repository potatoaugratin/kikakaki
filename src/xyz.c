#include <ncurses.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//compile cc xyz.c -o xyz -lm -lncurses && ./xya

#define PI 3.14159265358979323846

//yaw rotate around z
//pitch rotate around y
//roll rotate around x
//vertically is y
//horizontally is x
//go in the screen is z

int main()
{
    initscr();
    curs_set(FALSE);
    noecho();
    timeout(0);
    int ch;
    float x=10, y=0, z=0;
    float x2 = 10, y2=0, z2=0;
    float x3 = 0, y3=10, z3=0;
    float radius = 10;
    float angle = 0;
    float increment = 0.1;
    int rows, cols;
    while (1) {
        clear();
        getmaxyx(stdscr, rows, cols);
        for (float t=-radius; t<radius; t+=0.5) {
            x = t;
            x2 = t;
            y3 = t;
            float x_rot = x*cos(angle)-y*sin(angle);
            float y_rot = x*sin(angle)+y*cos(angle);
            int screen_x = cols/4+(x_rot);
            int screen_y = rows/2+(y_rot)*0.4;
            mvprintw(screen_y, screen_x, "o");
            float x2_rot = x2*cos(angle)-z2*sin(angle);
            float z2_rot = x2*sin(angle)+z2*cos(angle);
            int screen_x2 = cols/2+(x2_rot);
            int screen_y2 = rows/2+y2;
            mvprintw(screen_y2, screen_x2, "x");

            float y3_rot = y3*cos(angle)-z3*sin(angle);
            float z3_rot = y3*sin(angle)+z3*cos(angle);
            int screen_x3 = (cols*2)/3+x3;
            int screen_y3 = rows/2+(y3_rot)*0.4;
            mvprintw(screen_y3, screen_x3, "+");
        }

        refresh();
        usleep(8000);
        angle += increment;
        if (angle>2*PI) angle = 0;
        ch = getch();
        if (ch == 'q') break;
    }
    endwin();
    return 0;
}