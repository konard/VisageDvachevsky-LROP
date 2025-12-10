#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "utilities.h"

/* Clear the console screen */
void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* Set cursor position in console */
void set_cursor(int row, int col) {
#ifdef _WIN32
    COORD coord;
    coord.X = col;
    coord.Y = row;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    (void)row;
    (void)col;
    /* ANSI escape codes for non-Windows systems */
    printf("\033[%d;%dH", row + 1, col + 1);
#endif
}

/* Get current cursor position */
void get_cursor(int* row, int* col) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *row = csbi.dwCursorPosition.Y;
    *col = csbi.dwCursorPosition.X;
#else
    *row = 0;
    *col = 0;
#endif
}

/* Read integer with range validation */
int read_int_in_range(int min, int max) {
    int value;
    int result;

    while (1) {
        result = scanf("%d", &value);

        /* Clear input buffer */
        while (getchar() != '\n');

        if (result == 1 && value >= min && value <= max) {
            return value;
        }

        printf("Некорректный ввод. Введите число от %d до %d: ", min, max);
    }
}

/* Initialize random number generator */
void init_random(void) {
    srand((unsigned int)time(NULL));
}
