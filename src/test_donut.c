#include <string.h>
#include <math.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h> 

//compile cc test_donut.c -o test_donut -lm -lmcurses && ./test_donut

void direction(float *A, float *B, int x, int y, int px, int py) {
    int dx = x - px;
    int dy = y - py;

    float sensitivity = 0.1;
    *A += dy * sensitivity;
    *B -= dx * sensitivity;
}


int main() {
    MEVENT event;
    initscr();
    noecho();  
    cbreak(); 
    keypad(stdscr, TRUE);
    timeout(0);
    curs_set(0);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    printf("\033[?1003h");
    fflush(stdout);

    int ch, currX = 0, currY = 0, prevX = 0, prevY = 0;
    int isHolding = 0;
    float A = 0, B = 0;

    while (1) {
        int rows, cols;
        getmaxyx(stdscr, rows, cols);

        int buffer_size = rows * cols;
        float *z = (float *)malloc(buffer_size * sizeof(float));
        char *b = (char *)malloc(buffer_size * sizeof(char));

        if (!z || !b) {
            endwin();
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        int CENTER_X = cols / 2;
        int CENTER_Y = rows / 2;

        memset(b, 32, buffer_size); // ' ' = 32 in ASCII
        memset(z, 0, buffer_size * sizeof(float)); // Fill with zeros

        for (float j = 0; j < 6.28; j += 0.07) {
            for (float i = 0; i < 6.28; i += 0.02) {
                float c = sin(i), d = cos(j), e = sin(A), f = sin(j);
                float g = cos(A), h = d + 2, D = 1 / (c * h * e + f * g + 5);
                float l = cos(i), m = cos(B), n = sin(B), t = c * h * g - f * e;

                // Compute screen coordinates
                int x = CENTER_X + 30 * D * (l * h * m - t * n);
                int y = CENTER_Y + 15 * D * (l * h * n + t * m);
                int o = x + cols * y; // Update `o` based on terminal width

                // Compute luminance
                int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);

                // Update buffers if within bounds
                if (y > 0 && y < rows && x > 0 && x < cols && D > z[o]) {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }

        clear();
        for (int k = 0; k < buffer_size; k++) {
            if (k % cols == 0) {
                printw("\n");
            } else {
                addch(b[k]);
            }
        }
        refresh();

        ch = getch();
        if (ch == 'q') break;

        if (ch == KEY_MOUSE && getmouse(&event) == OK) {
            if (event.bstate & BUTTON1_PRESSED) isHolding = 1;
            else if (event.bstate & BUTTON1_RELEASED) isHolding = 0;

            currX = event.x;
            currY = event.y;
            if (isHolding) direction(&A, &B, currX, currY, prevX, prevY);

            prevX = currX;
            prevY = currY;
        }
        free(z);
        free(b);
        usleep(8000); //下を使ったらこれをコメントしてください
        //ドーナツが自由に動かせたい場合これ使ってください
        // A += 0.1;
        // B += 0.1;
        //usleep(18000);
        
    }

    printf("\033[?1003l"); // Disable mouse tracking in xterm
    fflush(stdout);
    endwin();
    return 0;
}
