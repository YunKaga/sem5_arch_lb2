#include <cstdio>
#include <string>
#include <chrono>
int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Ошибка. Не хватает аргументов строки\n");
        return 1;
    }
    int a = std::stoi(argv[1]); // Делимое
    int b = std::stoi(argv[2]); // Делитель

    auto start_cpp = std::chrono::high_resolution_clock::now();
    int res_cpp = a % b; // Результат c++
    auto end_cpp = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> cpp_time = end_cpp - start_cpp;

    int res_asm = 0; // Результат assembler
    auto start_asm = std::chrono::high_resolution_clock::now();
    __asm__ ( ".intel_syntax noprefix \n\t"
            "mov eax, %1 \n\t"
            "cdq \n\t"
            "idiv %2 \n\t"
            "mov %0, edx \n\t"
            : "=r" (res_asm) // выходной операнд
            : "r" (a), "r" (b) // входные операнды
            : "%eax", "%edx" // Разрушаемые регистры
            );
    auto end_asm = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> asm_time = end_cpp - start_cpp;
    printf("C++ результат: %i | Время: %lf\n", res_cpp, asm_time.count());
    printf("ASM результат: %i | Время: %lf\n", res_asm, cpp_time.count());

    return 0;
}
