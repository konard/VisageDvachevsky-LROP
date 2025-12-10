/*
 * Test program to verify UTF-8 Russian text output
 * This should be compiled and run on Windows to verify the fix
 */

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

int main(void) {
#ifdef _WIN32
    /* Set console code page to UTF-8 for correct Russian text display */
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    printf("\n========================================\n");
    printf("Тест отображения русского текста\n");
    printf("========================================\n");
    printf("\nМеню:\n");
    printf("1. Играть\n");
    printf("2. Настройки\n");
    printf("0. Выход\n");
    printf("\nСообщения:\n");
    printf("[+10 очков] Победа!\n");
    printf("[-10 очков] Поражение!\n");
    printf("Ваш выбор: \n");
    printf("\nИгры:\n");
    printf("- Счастливчик\n");
    printf("- Найди пары\n");
    printf("- Сапёр\n");
    printf("- Крестики-нолики\n");
    printf("- Тетрис\n");
    printf("\nДополнительный текст:\n");
    printf("Сложность установлена: HARD\n");
    printf("Текущие очки: 100\n");
    printf("Нажмите Enter для продолжения...\n");
    printf("\nПсевдографика (ASCII):\n");
    printf("+---------------------+\n");
    printf("|   ГЛАВНОЕ МЕНЮ      |\n");
    printf("+---------------------+\n");
    printf("| 1. Играть           |\n");
    printf("| 2. Настройки        |\n");
    printf("| 0. Выход            |\n");
    printf("+---------------------+\n");
    printf("\nЕсли весь русский текст выше отображается корректно,\n");
    printf("то проблема с кодировкой решена!\n");
    printf("========================================\n");

    return 0;
}
