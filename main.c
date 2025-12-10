#include <stdio.h>
#include "utilities.h"
#include "menu.h"

int main(void) {
    int score = 0;
    Difficulty difficulty = DIFF_MEDIUM;

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
