#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"

/* Uncomment the line below to enable DEBUG mode */
/* #define DEBUG */

/* ========== GAME 1: LUCKY NUMBER ========== */

int start_lucky_game(Difficulty difficulty) {
    int max_range;
    int secret;
    int guess;

    /* Determine range based on difficulty */
    switch (difficulty) {
        case DIFF_EASY:
            max_range = 10;
            break;
        case DIFF_MEDIUM:
            max_range = 50;
            break;
        case DIFF_HARD:
            max_range = 100;
            break;
        default:
            max_range = 50;
            break;
    }

    /* Generate secret number */
    secret = (rand() % max_range) + 1;

    clear_screen();
    printf("\n========== СЧАСТЛИВЧИК ==========\n");
    printf("Угадайте число от 1 до %d\n", max_range);

#ifdef DEBUG
    printf("[DEBUG] Загадано число: %d\n", secret);
#endif

    printf("Ваша догадка: ");
    guess = read_int_in_range(1, max_range);

    if (guess == secret) {
        printf("\nПоздравляем! Вы угадали число %d!\n", secret);
        return 1; /* Victory */
    } else {
        printf("\nНе угадали! Загаданное число было: %d\n", secret);
        return 0; /* Defeat */
    }
}

/* ========== GAME 2: FIND PAIRS ========== */

/* Helper function to allocate 2D character array */
char** allocate_char_field(int rows, int cols) {
    int i;
    char** field = (char**)malloc(rows * sizeof(char*));
    for (i = 0; i < rows; i++) {
        field[i] = (char*)malloc(cols * sizeof(char));
    }
    return field;
}

/* Helper function to free 2D character array */
void free_char_field(char** field, int rows) {
    int i;
    for (i = 0; i < rows; i++) {
        free(field[i]);
    }
    free(field);
}

/* Initialize pairs field with matching cards */
void init_pairs_field(char** field, char** visible, int rows, int cols) {
    int i, j;
    int total_cells = rows * cols;
    int pair_count = total_cells / 2;
    char* cards = (char*)malloc(total_cells * sizeof(char));
    int idx = 0;

    /* Create pairs */
    for (i = 0; i < pair_count; i++) {
        cards[idx++] = 'A' + (i % 26);
        cards[idx++] = 'A' + (i % 26);
    }

    /* Shuffle cards */
    for (i = total_cells - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }

    /* Copy to field */
    idx = 0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            field[i][j] = cards[idx++];
            visible[i][j] = '#'; /* Hidden */
        }
    }

    free(cards);
}

/* Print pairs field */
void print_pairs_field(char** visible, int rows, int cols, int show_all, char** field) {
    int i, j;

    printf("\n   ");
    for (j = 0; j < cols; j++) {
        printf(" %d ", j + 1);
    }
    printf("\n");

    for (i = 0; i < rows; i++) {
        printf("%d  ", i + 1);
        for (j = 0; j < cols; j++) {
            if (show_all) {
                printf(" %c ", field[i][j]);
            } else {
                printf(" %c ", visible[i][j]);
            }
        }
        printf("\n");
    }
}

/* Check if all pairs are opened */
int all_pairs_opened(char** visible, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (visible[i][j] == '#') {
                return 0;
            }
        }
    }
    return 1;
}

int start_find_game(Difficulty difficulty) {
    int rows, cols;
    char** field;
    char** visible;
    int row1, col1, row2, col2;
    int pairs_found = 0;
    int total_pairs;

    /* Determine field size based on difficulty */
    switch (difficulty) {
        case DIFF_EASY:
            rows = 2;
            cols = 4;
            break;
        case DIFF_MEDIUM:
            rows = 4;
            cols = 4;
            break;
        case DIFF_HARD:
            rows = 4;
            cols = 6;
            break;
        default:
            rows = 4;
            cols = 4;
            break;
    }

    total_pairs = (rows * cols) / 2;

    /* Allocate fields */
    field = allocate_char_field(rows, cols);
    visible = allocate_char_field(rows, cols);

    /* Initialize field */
    init_pairs_field(field, visible, rows, cols);

    clear_screen();
    printf("\n========== НАЙДИ ПАРЫ ==========\n");
    printf("Найдите все пары карточек!\n");

#ifdef DEBUG
    printf("\n[DEBUG] Реальное поле:\n");
    print_pairs_field(visible, rows, cols, 1, field);
    printf("\nНажмите Enter для начала игры...");
    getchar();
#endif

    /* Game loop */
    while (!all_pairs_opened(visible, rows, cols)) {
        clear_screen();
        printf("\n========== НАЙДИ ПАРЫ ==========\n");
        printf("Найдено пар: %d/%d\n", pairs_found, total_pairs);
        print_pairs_field(visible, rows, cols, 0, field);

        /* Get first card */
        printf("\nВыберите первую карту (строка столбец): ");
        row1 = read_int_in_range(1, rows) - 1;
        col1 = read_int_in_range(1, cols) - 1;

        if (visible[row1][col1] != '#') {
            printf("Эта карта уже открыта! Нажмите Enter...");
            getchar();
            continue;
        }

        /* Get second card */
        printf("Выберите вторую карту (строка столбец): ");
        row2 = read_int_in_range(1, rows) - 1;
        col2 = read_int_in_range(1, cols) - 1;

        if (visible[row2][col2] != '#' || (row1 == row2 && col1 == col2)) {
            printf("Неверный выбор! Нажмите Enter...");
            getchar();
            continue;
        }

        /* Show both cards */
        clear_screen();
        printf("\n========== НАЙДИ ПАРЫ ==========\n");
        printf("Найдено пар: %d/%d\n", pairs_found, total_pairs);
        visible[row1][col1] = field[row1][col1];
        visible[row2][col2] = field[row2][col2];
        print_pairs_field(visible, rows, cols, 0, field);

        /* Check if match */
        if (field[row1][col1] == field[row2][col2]) {
            printf("\nПара найдена!\n");
            pairs_found++;
        } else {
            printf("\nНе совпадают!\n");
            visible[row1][col1] = '#';
            visible[row2][col2] = '#';
        }

        printf("Нажмите Enter для продолжения...");
        getchar();
    }

    /* Free memory */
    free_char_field(field, rows);
    free_char_field(visible, rows);

    printf("\nВы открыли все пары!\n");
    return 1; /* Victory */
}

/* ========== GAME 3: MINESWEEPER ========== */

/* Allocate minesweeper field */
char** allocate_field(int rows, int cols) {
    int i;
    char** field = (char**)malloc(rows * sizeof(char*));
    for (i = 0; i < rows; i++) {
        field[i] = (char*)malloc(cols * sizeof(char));
    }
    return field;
}

/* Free minesweeper field */
void free_field(char** field, int rows) {
    int i;
    for (i = 0; i < rows; i++) {
        free(field[i]);
    }
    free(field);
}

/* Initialize minesweeper field with bombs */
void init_minesweeper_field(char** field, int rows, int cols, int bombs_count) {
    int i, j;
    int bombs_placed = 0;

    /* Initialize empty field */
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            field[i][j] = '0';
        }
    }

    /* Place bombs randomly */
    while (bombs_placed < bombs_count) {
        int row = rand() % rows;
        int col = rand() % cols;

        if (field[row][col] != '*') {
            field[row][col] = '*';
            bombs_placed++;
        }
    }
}

/* Calculate numbers around bombs */
void calculate_numbers(char** field, int rows, int cols) {
    int i, j, di, dj;
    int count;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (field[i][j] == '*') {
                continue;
            }

            count = 0;
            /* Check all 8 neighbors */
            for (di = -1; di <= 1; di++) {
                for (dj = -1; dj <= 1; dj++) {
                    if (di == 0 && dj == 0) continue;

                    int ni = i + di;
                    int nj = j + dj;

                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                        if (field[ni][nj] == '*') {
                            count++;
                        }
                    }
                }
            }

            field[i][j] = '0' + count;
        }
    }
}

/* Print minesweeper field */
void print_minesweeper_visible(char** visible, int rows, int cols) {
    int i, j;

    printf("\n   ");
    for (j = 0; j < cols; j++) {
        printf("%2d ", j + 1);
    }
    printf("\n");

    for (i = 0; i < rows; i++) {
        printf("%2d ", i + 1);
        for (j = 0; j < cols; j++) {
            printf(" %c ", visible[i][j]);
        }
        printf("\n");
    }
}

/* Open cell and recursive flood fill for zeros */
int open_cell(char** field, char** visible, int rows, int cols, int row, int col, int* safe_opened) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return 0;
    }

    if (visible[row][col] != '#' && visible[row][col] != 'F') {
        return 0; /* Already opened */
    }

    if (visible[row][col] == 'F') {
        return 0; /* Flagged */
    }

    visible[row][col] = field[row][col];

    if (field[row][col] == '*') {
        return 1; /* Hit bomb */
    }

    (*safe_opened)++;

    /* Flood fill for zeros */
    if (field[row][col] == '0') {
        int di, dj;
        for (di = -1; di <= 1; di++) {
            for (dj = -1; dj <= 1; dj++) {
                if (di == 0 && dj == 0) continue;
                open_cell(field, visible, rows, cols, row + di, col + dj, safe_opened);
            }
        }
    }

    return 0;
}

/* Toggle flag */
void toggle_flag(char** visible, int row, int col) {
    if (visible[row][col] == '#') {
        visible[row][col] = 'F';
    } else if (visible[row][col] == 'F') {
        visible[row][col] = '#';
    }
}

int start_minesweeper_game(Difficulty difficulty) {
    int rows, cols, bombs_count;
    char** field;
    char** visible;
    int i, j;
    char action;
    int row, col;
    int safe_cells;
    int safe_opened = 0;

    /* Determine field parameters based on difficulty */
    switch (difficulty) {
        case DIFF_EASY:
            rows = 8;
            cols = 8;
            bombs_count = 10;
            break;
        case DIFF_MEDIUM:
            rows = 10;
            cols = 10;
            bombs_count = 20;
            break;
        case DIFF_HARD:
            rows = 16;
            cols = 16;
            bombs_count = 40;
            break;
        default:
            rows = 10;
            cols = 10;
            bombs_count = 20;
            break;
    }

    safe_cells = rows * cols - bombs_count;

    /* Allocate fields */
    field = allocate_field(rows, cols);
    visible = allocate_field(rows, cols);

    /* Initialize fields */
    init_minesweeper_field(field, rows, cols, bombs_count);
    calculate_numbers(field, rows, cols);

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            visible[i][j] = '#';
        }
    }

    clear_screen();
    printf("\n========== САПЁР ==========\n");
    printf("Размер поля: %dx%d, бомб: %d\n", rows, cols, bombs_count);
    printf("Команды: O - открыть, F - флаг\n");

#ifdef DEBUG
    printf("\n[DEBUG] Реальное поле:\n");
    print_minesweeper_visible(field, rows, cols);
    printf("\nНажмите Enter для начала игры...");
    getchar();
#endif

    /* Game loop */
    while (safe_opened < safe_cells) {
        clear_screen();
        printf("\n========== САПЁР ==========\n");
        printf("Безопасных клеток открыто: %d/%d\n", safe_opened, safe_cells);
        printf("Команды: O - открыть, F - флаг, Q - выход\n");
        print_minesweeper_visible(visible, rows, cols);

        printf("\nВведите команду (O/F/Q): ");
        scanf(" %c", &action);
        while (getchar() != '\n');

        if (action == 'Q' || action == 'q') {
            free_field(field, rows);
            free_field(visible, rows);
            return 0; /* Early exit */
        }

        printf("Введите координаты (строка столбец): ");
        row = read_int_in_range(1, rows) - 1;
        col = read_int_in_range(1, cols) - 1;

        if (action == 'O' || action == 'o') {
            if (open_cell(field, visible, rows, cols, row, col, &safe_opened)) {
                /* Hit bomb */
                clear_screen();
                printf("\n========== САПЁР ==========\n");
                printf("ВЗРЫВ! Вы попали на бомбу!\n");
                print_minesweeper_visible(field, rows, cols);
                free_field(field, rows);
                free_field(visible, rows);
                return 0; /* Defeat */
            }
        } else if (action == 'F' || action == 'f') {
            toggle_flag(visible, row, col);
        }
    }

    /* Victory */
    clear_screen();
    printf("\n========== САПЁР ==========\n");
    printf("ПОБЕДА! Вы открыли все безопасные клетки!\n");
    print_minesweeper_visible(visible, rows, cols);

    free_field(field, rows);
    free_field(visible, rows);
    return 1;
}

/* ========== GAME 4: TIC-TAC-TOE ========== */

/* Initialize tic-tac-toe field */
void init_tictactoe_field(char* field) {
    int i;
    for (i = 0; i < 9; i++) {
        field[i] = '1' + i; /* Numbers 1-9 */
    }
}

/* Print tic-tac-toe field */
void print_tictactoe_field(char* field) {
    printf("\n");
    printf(" %c | %c | %c \n", field[0], field[1], field[2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", field[3], field[4], field[5]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", field[6], field[7], field[8]);
    printf("\n");
}

/* Check if someone won */
int check_tictactoe_win(char* field, char symbol) {
    /* Check rows */
    if (field[0] == symbol && field[1] == symbol && field[2] == symbol) return 1;
    if (field[3] == symbol && field[4] == symbol && field[5] == symbol) return 1;
    if (field[6] == symbol && field[7] == symbol && field[8] == symbol) return 1;

    /* Check columns */
    if (field[0] == symbol && field[3] == symbol && field[6] == symbol) return 1;
    if (field[1] == symbol && field[4] == symbol && field[7] == symbol) return 1;
    if (field[2] == symbol && field[5] == symbol && field[8] == symbol) return 1;

    /* Check diagonals */
    if (field[0] == symbol && field[4] == symbol && field[8] == symbol) return 1;
    if (field[2] == symbol && field[4] == symbol && field[6] == symbol) return 1;

    return 0;
}

/* Check if field is full */
int is_field_full(char* field) {
    int i;
    for (i = 0; i < 9; i++) {
        if (field[i] != 'X' && field[i] != 'O') {
            return 0;
        }
    }
    return 1;
}

/* Player move */
int player_move(char* field, char player_symbol) {
    int pos;

    while (1) {
        printf("Ваш ход (выберите позицию 1-9): ");
        pos = read_int_in_range(1, 9) - 1;

        if (field[pos] != 'X' && field[pos] != 'O') {
            field[pos] = player_symbol;
            return 1;
        }

        printf("Эта позиция уже занята!\n");
    }
}

/* Computer move with difficulty-based strategy */
int computer_move(char* field, char computer_symbol, char opponent_symbol, Difficulty difficulty) {
    int i;
    int move = -1;

    /* HARD: Check for winning move first */
    if (difficulty == DIFF_HARD) {
        for (i = 0; i < 9; i++) {
            if (field[i] != 'X' && field[i] != 'O') {
                char temp = field[i];
                field[i] = computer_symbol;
                if (check_tictactoe_win(field, computer_symbol)) {
                    return 1; /* Winning move made */
                }
                field[i] = temp;
            }
        }
    }

    /* HARD and MEDIUM: Check for blocking opponent's winning move */
    if (difficulty == DIFF_HARD || difficulty == DIFF_MEDIUM) {
        for (i = 0; i < 9; i++) {
            if (field[i] != 'X' && field[i] != 'O') {
                char temp = field[i];
                field[i] = opponent_symbol;
                if (check_tictactoe_win(field, opponent_symbol)) {
                    field[i] = computer_symbol; /* Block */
                    return 1;
                }
                field[i] = temp;
            }
        }
    }

    /* HARD: Try to take center */
    if (difficulty == DIFF_HARD && field[4] != 'X' && field[4] != 'O') {
        field[4] = computer_symbol;
        return 1;
    }

    /* MEDIUM: 50% chance to make random move */
    if (difficulty == DIFF_MEDIUM && (rand() % 2 == 0)) {
        /* Random move */
        while (1) {
            move = rand() % 9;
            if (field[move] != 'X' && field[move] != 'O') {
                field[move] = computer_symbol;
                return 1;
            }
        }
    }

    /* EASY or fallback: Random move */
    while (1) {
        move = rand() % 9;
        if (field[move] != 'X' && field[move] != 'O') {
            field[move] = computer_symbol;
            return 1;
        }
    }

    return 0;
}

/* Coin toss for first player */
int toss_coin_for_first_player(void) {
    return rand() % 2; /* 0 - player, 1 - computer */
}

int start_tictactoe_game(Difficulty difficulty) {
    char field[9];
    int first_player;
    char player_symbol;
    char computer_symbol;
    int current_player;

    init_tictactoe_field(field);

    /* Coin toss */
    first_player = toss_coin_for_first_player();

    if (first_player == 0) {
        player_symbol = 'X';
        computer_symbol = 'O';
        current_player = 0; /* Player starts */
        printf("\nБросок монеты: Вы ходите первым (X)!\n");
    } else {
        player_symbol = 'O';
        computer_symbol = 'X';
        current_player = 1; /* Computer starts */
        printf("\nБросок монеты: Компьютер ходит первым (X)!\n");
    }

    printf("Нажмите Enter для начала...");
    getchar();

    /* Game loop */
    while (1) {
        clear_screen();
        printf("\n========== КРЕСТИКИ-НОЛИКИ ==========\n");
        printf("Вы: %c, Компьютер: %c\n", player_symbol, computer_symbol);
        print_tictactoe_field(field);

        if (current_player == 0) {
            /* Player's turn */
            player_move(field, player_symbol);

            if (check_tictactoe_win(field, player_symbol)) {
                clear_screen();
                printf("\n========== КРЕСТИКИ-НОЛИКИ ==========\n");
                print_tictactoe_field(field);
                printf("Поздравляем! Вы выиграли!\n");
                return 1; /* Player victory */
            }

            current_player = 1;
        } else {
            /* Computer's turn */
            printf("Ход компьютера...\n");
            computer_move(field, computer_symbol, player_symbol, difficulty);

            if (check_tictactoe_win(field, computer_symbol)) {
                clear_screen();
                printf("\n========== КРЕСТИКИ-НОЛИКИ ==========\n");
                print_tictactoe_field(field);
                printf("Компьютер выиграл!\n");
                return 0; /* Computer victory */
            }

            current_player = 0;
        }

        /* Check for draw */
        if (is_field_full(field)) {
            clear_screen();
            printf("\n========== КРЕСТИКИ-НОЛИКИ ==========\n");
            print_tictactoe_field(field);
            printf("Ничья!\n");
            return 2; /* Draw */
        }
    }
}

/* ========== GAME 5: TETRIS ========== */

/* Tetromino shapes (4 rotations each) */
const int TETROMINOS[7][4][4][4] = {
    /* I */
    {
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}},
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}
    },
    /* O */
    {
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}
    },
    /* T */
    {
        {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}
    },
    /* S */
    {
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0}},
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0}}
    },
    /* Z */
    {
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}}
    },
    /* J */
    {
        {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0}}
    },
    /* L */
    {
        {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0}},
        {{1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}}
    }
};

/* Initialize tetris field */
void init_tetris_field(char** field, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            field[i][j] = '.';
        }
    }
}

/* Print tetris field */
void print_tetris_field(char** field, int rows, int cols) {
    int i, j;

    printf("\n+");
    for (j = 0; j < cols; j++) printf("-");
    printf("+\n");

    for (i = 0; i < rows; i++) {
        printf("|");
        for (j = 0; j < cols; j++) {
            printf("%c", field[i][j]);
        }
        printf("|\n");
    }

    printf("+");
    for (j = 0; j < cols; j++) printf("-");
    printf("+\n");
}

/* Check if tetromino can be placed */
int can_place_tetromino(char** field, int rows, int cols, int shape, int rotation, int pos_row, int pos_col) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (TETROMINOS[shape][rotation][i][j]) {
                int field_row = pos_row + i;
                int field_col = pos_col + j;

                if (field_row < 0 || field_row >= rows || field_col < 0 || field_col >= cols) {
                    return 0;
                }

                if (field[field_row][field_col] != '.') {
                    return 0;
                }
            }
        }
    }
    return 1;
}

/* Place tetromino on field */
void place_tetromino(char** field, int shape, int rotation, int pos_row, int pos_col, char symbol) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (TETROMINOS[shape][rotation][i][j]) {
                field[pos_row + i][pos_col + j] = symbol;
            }
        }
    }
}

/* Remove full lines and return count */
int remove_full_lines(char** field, int rows, int cols) {
    int i, j, k;
    int lines_cleared = 0;

    for (i = rows - 1; i >= 0; i--) {
        int full = 1;
        for (j = 0; j < cols; j++) {
            if (field[i][j] == '.') {
                full = 0;
                break;
            }
        }

        if (full) {
            lines_cleared++;
            /* Move all rows down */
            for (k = i; k > 0; k--) {
                for (j = 0; j < cols; j++) {
                    field[k][j] = field[k - 1][j];
                }
            }
            /* Clear top row */
            for (j = 0; j < cols; j++) {
                field[0][j] = '.';
            }
            i++; /* Check same row again */
        }
    }

    return lines_cleared;
}

int start_tetris_game(Difficulty difficulty) {
    const int FIELD_ROWS = 20;
    const int FIELD_COLS = 10;
    int lines_to_clear;
    int lines_cleared = 0;
    char** field;
    int current_shape;
    int current_rotation;
    int current_row;
    int current_col;
    char input;
    int game_over = 0;

    /* Determine goal based on difficulty */
    switch (difficulty) {
        case DIFF_EASY:
            lines_to_clear = 5;
            break;
        case DIFF_MEDIUM:
            lines_to_clear = 10;
            break;
        case DIFF_HARD:
            lines_to_clear = 15;
            break;
        default:
            lines_to_clear = 10;
            break;
    }

    /* Allocate field */
    field = allocate_field(FIELD_ROWS, FIELD_COLS);
    init_tetris_field(field, FIELD_ROWS, FIELD_COLS);

    printf("\n========== ТЕТРИС ==========\n");
    printf("Цель: Собрать %d линий\n", lines_to_clear);
    printf("Управление: A - влево, D - вправо, W - поворот, S - падение, Q - выход\n");
    printf("Нажмите Enter для начала...");
    getchar();

    /* Game loop */
    while (lines_cleared < lines_to_clear && !game_over) {
        /* Spawn new tetromino */
        current_shape = rand() % 7;
        current_rotation = 0;
        current_row = 0;
        current_col = FIELD_COLS / 2 - 2;

        /* Check if can spawn */
        if (!can_place_tetromino(field, FIELD_ROWS, FIELD_COLS, current_shape, current_rotation, current_row, current_col)) {
            game_over = 1;
            break;
        }

        /* Tetromino fall loop */
        while (1) {
            clear_screen();
            printf("\n========== ТЕТРИС ==========\n");
            printf("Линий собрано: %d/%d\n", lines_cleared, lines_to_clear);

            /* Draw field with current tetromino */
            place_tetromino(field, current_shape, current_rotation, current_row, current_col, '#');
            print_tetris_field(field, FIELD_ROWS, FIELD_COLS);
            place_tetromino(field, current_shape, current_rotation, current_row, current_col, '.');

            printf("\nУправление: A/D - влево/вправо, W - поворот, S - вниз, Q - выход\n");
            printf("Команда: ");

            scanf(" %c", &input);
            while (getchar() != '\n');

            if (input == 'Q' || input == 'q') {
                free_field(field, FIELD_ROWS);
                return 0;
            }

            /* Handle input */
            if (input == 'A' || input == 'a') {
                if (can_place_tetromino(field, FIELD_ROWS, FIELD_COLS, current_shape, current_rotation, current_row, current_col - 1)) {
                    current_col--;
                }
            } else if (input == 'D' || input == 'd') {
                if (can_place_tetromino(field, FIELD_ROWS, FIELD_COLS, current_shape, current_rotation, current_row, current_col + 1)) {
                    current_col++;
                }
            } else if (input == 'W' || input == 'w') {
                int new_rotation = (current_rotation + 1) % 4;
                if (can_place_tetromino(field, FIELD_ROWS, FIELD_COLS, current_shape, new_rotation, current_row, current_col)) {
                    current_rotation = new_rotation;
                }
            }

            /* Try to move down */
            if (can_place_tetromino(field, FIELD_ROWS, FIELD_COLS, current_shape, current_rotation, current_row + 1, current_col)) {
                current_row++;
            } else {
                /* Lock tetromino */
                place_tetromino(field, current_shape, current_rotation, current_row, current_col, '#');

                /* Clear lines */
                int cleared = remove_full_lines(field, FIELD_ROWS, FIELD_COLS);
                lines_cleared += cleared;

                break; /* Spawn new tetromino */
            }
        }
    }

    clear_screen();
    printf("\n========== ТЕТРИС ==========\n");
    print_tetris_field(field, FIELD_ROWS, FIELD_COLS);

    free_field(field, FIELD_ROWS);

    if (lines_cleared >= lines_to_clear) {
        printf("\nПОБЕДА! Вы собрали %d линий!\n", lines_cleared);
        return 1;
    } else {
        printf("\nИГРА ОКОНЧЕНА! Собрано линий: %d/%d\n", lines_cleared, lines_to_clear);
        return 0;
    }
}
