#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "utilities.h"

/* Clear the console screen */
void clear_screen(void) {
    system("cls");
}

/* Set cursor position in console */
void set_cursor(int row, int col) {
    COORD coord;
    coord.X = col;
    coord.Y = row;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/* Get current cursor position */
void get_cursor(int* row, int* col) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *row = csbi.dwCursorPosition.Y;
    *col = csbi.dwCursorPosition.X;
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
