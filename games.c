#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "games.h"

/* Uncomment the line below to enable DEBUG mode */
/* #define DEBUG */

/* ========== GAME 1: СЧАСТЛИВЧИК (Угадай число) ========== */

int start_lucky_game(Difficulty difficulty) {
    int max_range, secret, guess;

    // выбираем диапазон по сложности
    max_range = (difficulty == DIFF_EASY) ? 10 : (difficulty == DIFF_MEDIUM) ? 50 : 100;
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
        return 1; // победа
    } else {
        printf("\nНе угадали! Загаданное число было: %d\n", secret);
        return 0; // поражение
    }
}

/* ========== GAME 2: НАЙДИ ПАРЫ ========== */

// создаём двумерный массив символов
char** create_field(int rows, int cols) {
    int i;
    char** field = (char**)malloc(rows * sizeof(char*));
    for (i = 0; i < rows; i++) {
        field[i] = (char*)malloc(cols * sizeof(char));
    }
    return field;
}

// освобождаем память двумерного массива
void free_field(char** field, int rows) {
    int i;
    for (i = 0; i < rows; i++) {
        free(field[i]);
    }
    free(field);
}

// заполняем поле парами и перемешиваем
void init_pairs(char** field, char** visible, int rows, int cols) {
    int i, j;
    int total = rows * cols;
    char* cards = (char*)malloc(total * sizeof(char));
    int idx = 0;

    // создаём пары карт (A A, B B, C C...)
    for (i = 0; i < total / 2; i++) {
        cards[idx++] = 'A' + (i % 26);
        cards[idx++] = 'A' + (i % 26);
    }

    // перемешиваем карты
    for (i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }

    // копируем на поле
    idx = 0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            field[i][j] = cards[idx++];
            visible[i][j] = '#'; // скрыта
        }
    }

    free(cards);
}

// выводим поле на экран
void print_pairs(char** visible, char** field, int rows, int cols, int show_all) {
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

// проверяем, все ли пары открыты
int all_opened(char** visible, int rows, int cols) {
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
    int rows, cols, row1, col1, row2, col2, pairs_found = 0, total_pairs;
    char** field;
    char** visible;

    // размер поля зависит от сложности
    if (difficulty == DIFF_EASY) { rows = 2; cols = 4; }
    else if (difficulty == DIFF_MEDIUM) { rows = 4; cols = 4; }
    else { rows = 4; cols = 6; }

    total_pairs = (rows * cols) / 2;

    // создаём поля
    field = create_field(rows, cols);
    visible = create_field(rows, cols);
    init_pairs(field, visible, rows, cols);

    clear_screen();
    printf("\n========== НАЙДИ ПАРЫ ==========\n");
    printf("Найдите все пары карточек!\n");

#ifdef DEBUG
    printf("\n[DEBUG] Реальное поле:\n");
    print_pairs(visible, field, rows, cols, 1);
    printf("\nНажмите Enter для начала игры...");
    getchar();
#endif

    // игровой цикл
    while (!all_opened(visible, rows, cols)) {
        clear_screen();
        printf("\n========== НАЙДИ ПАРЫ ==========\n");
        printf("Найдено пар: %d/%d\n", pairs_found, total_pairs);
        print_pairs(visible, field, rows, cols, 0);

        // выбираем первую карту
        printf("\nВыберите первую карту (строка столбец): ");
        row1 = read_int_in_range(1, rows) - 1;
        col1 = read_int_in_range(1, cols) - 1;

        if (visible[row1][col1] != '#') {
            printf("Эта карта уже открыта! Нажмите Enter...");
            getchar();
            continue;
        }

        // выбираем вторую карту
        printf("Выберите вторую карту (строка столбец): ");
        row2 = read_int_in_range(1, rows) - 1;
        col2 = read_int_in_range(1, cols) - 1;

        if (visible[row2][col2] != '#' || (row1 == row2 && col1 == col2)) {
            printf("Неверный выбор! Нажмите Enter...");
            getchar();
            continue;
        }

        // открываем обе карты
        visible[row1][col1] = field[row1][col1];
        visible[row2][col2] = field[row2][col2];

        clear_screen();
        printf("\n========== НАЙДИ ПАРЫ ==========\n");
        printf("Найдено пар: %d/%d\n", pairs_found, total_pairs);
        print_pairs(visible, field, rows, cols, 0);

        // проверяем совпадение
        if (field[row1][col1] == field[row2][col2]) {
            printf("\nПара найдена!\n");
            pairs_found++;
        } else {
            printf("\nНе совпадают!\n");
            visible[row1][col1] = '#'; // закрываем обратно
            visible[row2][col2] = '#';
        }

        printf("Нажмите Enter для продолжения...");
        getchar();
    }

    // освобождаем память
    free_field(field, rows);
    free_field(visible, rows);

    printf("\nВы открыли все пары!\n");
    return 1; // победа
}

/* ========== GAME 3: САПЁР ========== */

// инициализируем поле сапёра с бомбами и числами
void init_minesweeper(char** field, int rows, int cols, int bombs_count) {
    int i, j, placed = 0, di, dj;

    // заполняем нулями
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            field[i][j] = '0';
        }
    }

    // размещаем бомбы
    while (placed < bombs_count) {
        int row = rand() % rows;
        int col = rand() % cols;
        if (field[row][col] != '*') {
            field[row][col] = '*';
            placed++;
        }
    }

    // считаем числа вокруг бомб
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (field[i][j] == '*') continue;
            int count = 0;
            for (di = -1; di <= 1; di++) {
                for (dj = -1; dj <= 1; dj++) {
                    if (di == 0 && dj == 0) continue;
                    int ni = i + di, nj = j + dj;
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && field[ni][nj] == '*') {
                        count++;
                    }
                }
            }
            field[i][j] = '0' + count;
        }
    }
}

// выводим поле сапёра
void print_field(char** visible, int rows, int cols) {
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

// открываем клетку (с рекурсией для пустых)
int open_cell(char** field, char** visible, int rows, int cols, int row, int col, int* opened) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return 0;
    }

    if (visible[row][col] != '#' && visible[row][col] != 'F') {
        return 0; // уже открыта
    }

    if (visible[row][col] == 'F') {
        return 0; // помечена флагом
    }

    visible[row][col] = field[row][col];

    if (field[row][col] == '*') {
        return 1; // попали на бомбу - игра проиграна
    }

    (*opened)++;

    // если клетка пустая (0) - открываем соседей
    if (field[row][col] == '0') {
        int di, dj;
        for (di = -1; di <= 1; di++) {
            for (dj = -1; dj <= 1; dj++) {
                if (di == 0 && dj == 0) continue;
                open_cell(field, visible, rows, cols, row + di, col + dj, opened);
            }
        }
    }

    return 0;
}

// ставим или убираем флаг
void toggle_flag(char** visible, int row, int col) {
    if (visible[row][col] == '#') {
        visible[row][col] = 'F';
    } else if (visible[row][col] == 'F') {
        visible[row][col] = '#';
    }
}

int start_minesweeper_game(Difficulty difficulty) {
    int rows, cols, bombs, i, j, row, col, safe_cells, opened = 0;
    char** field;
    char** visible;
    char action;

    // параметры поля по сложности
    if (difficulty == DIFF_EASY) { rows = 8; cols = 8; bombs = 10; }
    else if (difficulty == DIFF_MEDIUM) { rows = 10; cols = 10; bombs = 20; }
    else { rows = 16; cols = 16; bombs = 40; }

    safe_cells = rows * cols - bombs;

    // создаём поля
    field = create_field(rows, cols);
    visible = create_field(rows, cols);

    // инициализируем поле с бомбами и числами
    init_minesweeper(field, rows, cols, bombs);

    // скрываем всё поле
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            visible[i][j] = '#';
        }
    }

    clear_screen();
    printf("\n========== САПЁР ==========\n");
    printf("Размер: %dx%d, бомб: %d\n", rows, cols, bombs);
    printf("Команды: O - открыть, F - флаг\n");

#ifdef DEBUG
    printf("\n[DEBUG] Реальное поле с бомбами:\n");
    print_field(field, rows, cols);
    printf("\nНажмите Enter для начала...");
    getchar();
#endif

    // игровой цикл
    while (opened < safe_cells) {
        clear_screen();
        printf("\n========== САПЁР ==========\n");
        printf("Безопасных открыто: %d/%d\n", opened, safe_cells);
        printf("Команды: O - открыть, F - флаг, Q - выход\n");
        print_field(visible, rows, cols);

        printf("\nВведите команду (O/F/Q): ");
        scanf(" %c", &action);
        while (getchar() != '\n');

        if (action == 'Q' || action == 'q') {
            free_field(field, rows);
            free_field(visible, rows);
            return 0; // досрочный выход
        }

        printf("Введите координаты (строка столбец): ");
        row = read_int_in_range(1, rows) - 1;
        col = read_int_in_range(1, cols) - 1;

        if (action == 'O' || action == 'o') {
            if (open_cell(field, visible, rows, cols, row, col, &opened)) {
                // попали на бомбу
                clear_screen();
                printf("\n========== САПЁР ==========\n");
                printf("ВЗРЫВ! Вы попали на бомбу!\n");
                print_field(field, rows, cols);
                free_field(field, rows);
                free_field(visible, rows);
                return 0; // поражение
            }
        } else if (action == 'F' || action == 'f') {
            toggle_flag(visible, row, col);
        }
    }

    // победа - все безопасные клетки открыты
    clear_screen();
    printf("\n========== САПЁР ==========\n");
    printf("ПОБЕДА! Все безопасные клетки открыты!\n");
    print_field(visible, rows, cols);

    free_field(field, rows);
    free_field(visible, rows);
    return 1; // победа
}

/* ========== GAME 4: КРЕСТИКИ-НОЛИКИ ========== */

// инициализация поля цифрами 1-9
void init_tictactoe(char* field) {
    int i;
    for (i = 0; i < 9; i++) {
        field[i] = '1' + i;
    }
}

// выводим поле 3x3
void print_tictactoe(char* field) {
    printf("\n");
    printf(" %c | %c | %c \n", field[0], field[1], field[2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", field[3], field[4], field[5]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", field[6], field[7], field[8]);
    printf("\n");
}

// проверка победы
int check_win(char* field, char symbol) {
    // проверяем строки
    if (field[0] == symbol && field[1] == symbol && field[2] == symbol) return 1;
    if (field[3] == symbol && field[4] == symbol && field[5] == symbol) return 1;
    if (field[6] == symbol && field[7] == symbol && field[8] == symbol) return 1;

    // проверяем столбцы
    if (field[0] == symbol && field[3] == symbol && field[6] == symbol) return 1;
    if (field[1] == symbol && field[4] == symbol && field[7] == symbol) return 1;
    if (field[2] == symbol && field[5] == symbol && field[8] == symbol) return 1;

    // проверяем диагонали
    if (field[0] == symbol && field[4] == symbol && field[8] == symbol) return 1;
    if (field[2] == symbol && field[4] == symbol && field[6] == symbol) return 1;

    return 0;
}

// проверка на ничью (поле заполнено)
int check_draw(char* field) {
    int i;
    for (i = 0; i < 9; i++) {
        if (field[i] != 'X' && field[i] != 'O') {
            return 0;
        }
    }
    return 1;
}

// ход игрока
void player_turn(char* field, char symbol) {
    int pos;

    while (1) {
        printf("Ваш ход (1-9): ");
        pos = read_int_in_range(1, 9) - 1;

        if (field[pos] != 'X' && field[pos] != 'O') {
            field[pos] = symbol;
            return;
        }

        printf("Место занято!\n");
    }
}

// ход компьютера с учётом сложности
void computer_turn(char* field, char comp, char opp, Difficulty difficulty) {
    int i;

    // HARD: пытаемся выиграть
    if (difficulty == DIFF_HARD) {
        for (i = 0; i < 9; i++) {
            if (field[i] != 'X' && field[i] != 'O') {
                char temp = field[i];
                field[i] = comp;
                if (check_win(field, comp)) {
                    return; // нашли победный ход
                }
                field[i] = temp;
            }
        }
    }

    // HARD и MEDIUM: блокируем противника
    if (difficulty == DIFF_HARD || difficulty == DIFF_MEDIUM) {
        for (i = 0; i < 9; i++) {
            if (field[i] != 'X' && field[i] != 'O') {
                char temp = field[i];
                field[i] = opp;
                if (check_win(field, opp)) {
                    field[i] = comp; // блокируем
                    return;
                }
                field[i] = temp;
            }
        }
    }

    // HARD: занимаем центр
    if (difficulty == DIFF_HARD && field[4] != 'X' && field[4] != 'O') {
        field[4] = comp;
        return;
    }

    // EASY или случайный ход
    while (1) {
        int pos = rand() % 9;
        if (field[pos] != 'X' && field[pos] != 'O') {
            field[pos] = comp;
            return;
        }
    }
}

// бросок монеты - кто ходит первым
int coin_toss(void) {
    return rand() % 2; // 0 - игрок, 1 - компьютер
}

int start_tictactoe_game(Difficulty difficulty) {
    char field[9], player, computer;
    int first, current;

    init_tictactoe(field);
    first = coin_toss();

    if (first == 0) {
        player = 'X'; computer = 'O'; current = 0;
        printf("\nБросок монеты: Вы ходите первым (X)!\n");
    } else {
        player = 'O'; computer = 'X'; current = 1;
        printf("\nБросок монеты: Компьютер ходит первым (X)!\n");
    }

    printf("Нажмите Enter...");
    getchar();

    // игровой цикл
    while (1) {
        clear_screen();
        printf("\n========== КРЕСТИКИ-НОЛИКИ ==========\n");
        printf("Вы: %c, Компьютер: %c\n", player, computer);
        print_tictactoe(field);

        if (current == 0) {
            // ход игрока
            player_turn(field, player);

            if (check_win(field, player)) {
                clear_screen();
                printf("\n========== КРЕСТИКИ-НОЛИКИ ==========\n");
                print_tictactoe(field);
                printf("Вы выиграли!\n");
                return 1; // победа игрока
            }

            current = 1;
        } else {
            // ход компьютера
            printf("Ход компьютера...\n");
            computer_turn(field, computer, player, difficulty);

            if (check_win(field, computer)) {
                clear_screen();
                printf("\n========== КРЕСТИКИ-НОЛИКИ ==========\n");
                print_tictactoe(field);
                printf("Компьютер выиграл!\n");
                return 0; // победа компьютера
            }

            current = 0;
        }

        // проверка на ничью
        if (check_draw(field)) {
            clear_screen();
            printf("\n========== КРЕСТИКИ-НОЛИКИ ==========\n");
            print_tictactoe(field);
            printf("Ничья!\n");
            return 2; // ничья
        }
    }
}

/* ========== GAME 5: ТЕТРИС ========== */

// координаты блоков фигур (относительные)
// 7 классических фигур тетриса: I, O, T, L, J, S, Z
static const int SHAPES[7][4][2] = {
    {{0,0}, {0,1}, {0,2}, {0,3}},   // I - палка
    {{0,0}, {0,1}, {1,0}, {1,1}},   // O - квадрат
    {{0,1}, {1,0}, {1,1}, {1,2}},   // T - тэшка
    {{0,0}, {1,0}, {1,1}, {1,2}},   // L
    {{0,2}, {1,0}, {1,1}, {1,2}},   // J
    {{0,1}, {0,2}, {1,0}, {1,1}},   // S
    {{0,0}, {0,1}, {1,1}, {1,2}}    // Z
};

// выводим поле тетриса
void print_tetris(char** field, int rows, int cols) {
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

// проверяем, можно ли разместить фигуру
int can_place(char** field, int rows, int cols, int shape[4][2], int row, int col) {
    int i;
    for (i = 0; i < 4; i++) {
        int r = row + shape[i][0];
        int c = col + shape[i][1];
        if (r < 0 || r >= rows || c < 0 || c >= cols || field[r][c] != '.') {
            return 0;
        }
    }
    return 1;
}

// поворот фигуры на 90 градусов по часовой (кроме квадрата)
void rotate_shape(int shape[4][2], int shape_type) {
    int i, temp;
    if (shape_type == 1) return; // квадрат не крутится

    // поворот: (r, c) -> (c, -r) относительно центра
    for (i = 0; i < 4; i++) {
        temp = shape[i][0];
        shape[i][0] = shape[i][1];
        shape[i][1] = -temp;
    }

    // нормализация - сдвигаем к 0
    int min_r = shape[0][0], min_c = shape[0][1];
    for (i = 1; i < 4; i++) {
        if (shape[i][0] < min_r) min_r = shape[i][0];
        if (shape[i][1] < min_c) min_c = shape[i][1];
    }
    for (i = 0; i < 4; i++) {
        shape[i][0] -= min_r;
        shape[i][1] -= min_c;
    }
}

// удаляем заполненные линии и возвращаем количество
int remove_lines(char** field, int rows, int cols) {
    int i, j, k, cleared = 0;
    for (i = rows - 1; i >= 0; i--) {
        int full = 1;
        for (j = 0; j < cols; j++) {
            if (field[i][j] == '.') {
                full = 0;
                break;
            }
        }
        if (full) {
            cleared++;
            for (k = i; k > 0; k--) {
                for (j = 0; j < cols; j++) {
                    field[k][j] = field[k - 1][j];
                }
            }
            for (j = 0; j < cols; j++) {
                field[0][j] = '.';
            }
            i++;
        }
    }
    return cleared;
}

int start_tetris_game(Difficulty difficulty) {
    int rows = 16, cols = 10;
    int goal, cleared = 0;
    char** field;
    char input;
    int i, j;

    // цель по сложности
    goal = (difficulty == DIFF_EASY) ? 5 : (difficulty == DIFF_MEDIUM) ? 10 : 15;

    // создаём пустое поле
    field = create_field(rows, cols);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            field[i][j] = '.';
        }
    }

    printf("\n========== ТЕТРИС ==========\n");
    printf("Цель: собрать %d линий\n", goal);
    printf("Управление: A - влево, D - вправо, S - вниз, W - поворот, Q - выход\n");
    printf("Нажмите Enter...");
    getchar();

    // игровой цикл
    while (cleared < goal) {
        // выбираем случайную фигуру
        int shape_type = rand() % 7;
        int shape[4][2];
        for (i = 0; i < 4; i++) {
            shape[i][0] = SHAPES[shape_type][i][0];
            shape[i][1] = SHAPES[shape_type][i][1];
        }

        int row = 0, col = cols / 2 - 2;

        // проверяем, можно ли разместить
        if (!can_place(field, rows, cols, shape, row, col)) {
            break; // игра окончена
        }

        // фигура падает
        while (1) {
            clear_screen();
            printf("\n========== ТЕТРИС ==========\n");
            printf("Линий: %d/%d\n", cleared, goal);

            // рисуем фигуру на поле
            for (i = 0; i < 4; i++) {
                field[row + shape[i][0]][col + shape[i][1]] = '#';
            }
            print_tetris(field, rows, cols);
            // убираем фигуру с поля
            for (i = 0; i < 4; i++) {
                field[row + shape[i][0]][col + shape[i][1]] = '.';
            }

            printf("\nКоманда (A-влево, D-вправо, S-вниз, W-поворот, Q-выход): ");
            scanf(" %c", &input);
            while (getchar() != '\n');

            if (input == 'Q' || input == 'q') {
                free_field(field, rows);
                return 0;
            }

            // движение влево
            if ((input == 'A' || input == 'a') && can_place(field, rows, cols, shape, row, col - 1)) {
                col--;
            }
            // движение вправо
            else if ((input == 'D' || input == 'd') && can_place(field, rows, cols, shape, row, col + 1)) {
                col++;
            }
            // поворот
            else if (input == 'W' || input == 'w') {
                int temp_shape[4][2];
                for (i = 0; i < 4; i++) {
                    temp_shape[i][0] = shape[i][0];
                    temp_shape[i][1] = shape[i][1];
                }
                rotate_shape(temp_shape, shape_type);
                if (can_place(field, rows, cols, temp_shape, row, col)) {
                    for (i = 0; i < 4; i++) {
                        shape[i][0] = temp_shape[i][0];
                        shape[i][1] = temp_shape[i][1];
                    }
                }
            }

            // автоматическое падение или ручное ускорение
            if (can_place(field, rows, cols, shape, row + 1, col)) {
                row++;
            } else {
                // фиксируем фигуру
                for (i = 0; i < 4; i++) {
                    field[row + shape[i][0]][col + shape[i][1]] = '#';
                }
                cleared += remove_lines(field, rows, cols);
                break;
            }
        }
    }

    clear_screen();
    printf("\n========== ТЕТРИС ==========\n");
    print_tetris(field, rows, cols);
    free_field(field, rows);

    if (cleared >= goal) {
        printf("\nПОБЕДА! Собрано %d линий!\n", cleared);
        return 1;
    } else {
        printf("\nИГРА ОКОНЧЕНА! Собрано: %d/%d\n", cleared, goal);
        return 0;
    }
}
