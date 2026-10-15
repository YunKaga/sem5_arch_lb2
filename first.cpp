#include <cstdio>
#include <string>
int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Ошибка. Не хватает аргументов строки\n");
        return 1;
    }
    int a = std::stoi(argv[1]); // Делимое
    int b = std::stoi(argv[2]); // Делитель

    int res_cpp = a % b; // Результат c++
    int res_asm = 0; // Результат assembler

    __asm__ ( ".intel_syntax noprefix \n\t"
            "mov eax, %1 \n\t"
            "cdq \n\t"
            "idiv %2 \n\t"
            "mov %0, edx \n\t"
            : "=r" (res_asm) // выходной операнд
            : "r" (a), "r" (b) // входные операнды
            : "%eax", "%edx" // Разрушаемые регистры
            );
    printf("C++ результат: %i\n", res_cpp);
    printf("ASM результат: %i\n", res_asm);

    return 0;
}
