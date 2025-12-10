#ifndef UTILITIES_H
#define UTILITIES_H

/* Enumeration for game difficulty levels */
typedef enum {
    DIFF_EASY = 1,
    DIFF_MEDIUM = 2,
    DIFF_HARD = 3
} Difficulty;

/* Console management functions */
void clear_screen(void);
void set_cursor(int row, int col);
void get_cursor(int* row, int* col);

/* Input functions */
int read_int_in_range(int min, int max);

/* Random number generator initialization */
void init_random(void);

#endif /* UTILITIES_H */
