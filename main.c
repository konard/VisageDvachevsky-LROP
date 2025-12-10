#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "utilities.h"
#include "menu.h"

int main(void) {
    int score = 0;
    Difficulty difficulty = DIFF_MEDIUM;

#ifdef _WIN32
    /* Set console code page to UTF-8 for correct Russian text display */
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    /* Initialize random number generator */
    init_random();

    /* Run main menu */
    run_main_menu(&score, &difficulty);

    /* Display final score */
    clear_screen();
    printf("\n========================================\n");
    printf("      СПАСИБО ЗА ИГРУ!              \n");
    printf("========================================\n");
    printf("Ваши итоговые очки: %d\n", score);
    printf("========================================\n");

    return 0;
}
