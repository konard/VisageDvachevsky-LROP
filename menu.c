#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "games.h"

/* Display main menu */
void show_main_menu(void) {
    printf("\n========== ГЛАВНОЕ МЕНЮ ==========\n");
    printf("1. Играть\n");
    printf("2. Настройки\n");
    printf("0. Выход\n");
    printf("==================================\n");
    printf("Ваш выбор: ");
}

/* Main menu loop */
void run_main_menu(int* score, Difficulty* difficulty) {
    int choice;

    while (1) {
        clear_screen();
        show_main_menu();
        choice = read_int_in_range(0, 2);

        switch (choice) {
            case 1:
                run_games_menu(score, *difficulty);
                break;
            case 2:
                run_settings_menu(difficulty);
                break;
            case 0:
                return; /* Exit to main */
            default:
                break;
        }
    }
}

/* Games menu */
void run_games_menu(int* score, Difficulty difficulty) {
    int choice;
    int result;

    while (1) {
        clear_screen();
        printf("\n========== МЕНЮ ИГР ==========\n");
        printf("1. Счастливчик\n");
        printf("2. Найди пары\n");
        printf("3. Сапёр\n");
        printf("4. Крестики-нолики\n");
        printf("5. Тетрис\n");
        printf("6. Назад\n");
        printf("0. Выход\n");
        printf("==============================\n");
        printf("Текущие очки: %d\n", *score);
        printf("Ваш выбор: ");

        choice = read_int_in_range(0, 6);

        switch (choice) {
            case 1:
                result = start_lucky_game(difficulty);
                if (result == 1) {
                    *score += 10;
                    printf("\n[+10 очков] Победа!\n");
                } else if (result == 0) {
                    *score -= 10;
                    printf("\n[-10 очков] Поражение!\n");
                }
                printf("Нажмите Enter для продолжения...");
                getchar();
                break;

            case 2:
                result = start_find_game(difficulty);
                if (result == 1) {
                    *score += 10;
                    printf("\n[+10 очков] Победа!\n");
                } else if (result == 0) {
                    *score -= 10;
                    printf("\n[-10 очков] Поражение!\n");
                }
                printf("Нажмите Enter для продолжения...");
                getchar();
                break;

            case 3:
                result = start_minesweeper_game(difficulty);
                if (result == 1) {
                    *score += 10;
                    printf("\n[+10 очков] Победа!\n");
                } else if (result == 0) {
                    *score -= 10;
                    printf("\n[-10 очков] Поражение!\n");
                }
                printf("Нажмите Enter для продолжения...");
                getchar();
                break;

            case 4:
                result = start_tictactoe_game(difficulty);
                if (result == 1) {
                    *score += 10;
                    printf("\n[+10 очков] Победа!\n");
                } else if (result == 0) {
                    *score -= 10;
                    printf("\n[-10 очков] Поражение!\n");
                } else {
                    printf("\nНичья! Очки не изменились.\n");
                }
                printf("Нажмите Enter для продолжения...");
                getchar();
                break;

            case 5:
                result = start_tetris_game(difficulty);
                if (result == 1) {
                    *score += 10;
                    printf("\n[+10 очков] Победа!\n");
                } else if (result == 0) {
                    *score -= 10;
                    printf("\n[-10 очков] Поражение!\n");
                }
                printf("Нажмите Enter для продолжения...");
                getchar();
                break;

            case 6:
                return; /* Back to main menu */

            case 0:
                exit(0); /* Exit program */

            default:
                break;
        }
    }
}

/* Settings menu */
void run_settings_menu(Difficulty* difficulty) {
    int choice;

    while (1) {
        clear_screen();
        printf("\n========== НАСТРОЙКИ ==========\n");
        printf("1. Сложность\n");
        printf("2. Назад\n");
        printf("0. Выход\n");
        printf("===============================\n");
        printf("Ваш выбор: ");

        choice = read_int_in_range(0, 2);

        switch (choice) {
            case 1:
                run_difficulty_menu(difficulty);
                break;
            case 2:
                return; /* Back to main menu */
            case 0:
                exit(0); /* Exit program */
            default:
                break;
        }
    }
}

/* Difficulty selection menu */
void run_difficulty_menu(Difficulty* difficulty) {
    int choice;
    const char* diff_names[] = {"", "EASY", "MEDIUM", "HARD"};

    while (1) {
        clear_screen();
        printf("\n========== ВЫБОР СЛОЖНОСТИ ==========\n");
        printf("1. HARD\n");
        printf("2. MEDIUM\n");
        printf("3. EASY\n");
        printf("4. Назад\n");
        printf("0. Выход\n");
        printf("=====================================\n");
        printf("[Текущая сложность: %s]\n", diff_names[*difficulty]);
        printf("Ваш выбор: ");

        choice = read_int_in_range(0, 4);

        switch (choice) {
            case 1:
                *difficulty = DIFF_HARD;
                printf("Сложность установлена: HARD\n");
                printf("Нажмите Enter для продолжения...");
                getchar();
                break;
            case 2:
                *difficulty = DIFF_MEDIUM;
                printf("Сложность установлена: MEDIUM\n");
                printf("Нажмите Enter для продолжения...");
                getchar();
                break;
            case 3:
                *difficulty = DIFF_EASY;
                printf("Сложность установлена: EASY\n");
                printf("Нажмите Enter для продолжения...");
                getchar();
                break;
            case 4:
                return; /* Back to settings menu */
            case 0:
                exit(0); /* Exit program */
            default:
                break;
        }
    }
}
