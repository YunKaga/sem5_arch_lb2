#include <cstdio>
#include <chrono>
#include <string>

long int sum_cpp_func(long int N) {
    long int sum = 0;
    for (long int i = 1; i <= N; ++i) {
        if (i % 3 == 0)
            sum += i;
    }

    return sum;
}

long int sum_asm_func(long int N) {
    long int sum = 0;
    
    __asm__ __volatile__ ( 
        ".intel_syntax noprefix \n\t"
        "mov rcx, 1 \n\t" // Инициализация счётчика цикла
        "mov %0, 0 \n\t" // Обнуление переменной суммы
        "cmp rcx, %1 \n\t" // Сравнение счётчика с верхней границей N
        "jg 3f \n\t"
        
        "1: \n\t"
        "mov rax, rcx \n\t" // Подготовка перед делением
        "cqo \n\t" // Расширение rax в rdx
        "mov rsi, 3 \n\t" // Загрузка делителя
        "idiv rsi \n\t" // Деление
        "test rdx, rdx \n\t" // Проверка на нулевой остаток
        "jnz 2f \n\t" // Если остаток есть, пропускаем сложение
        
        "add %0, rcx \n\t" // Если остаток ноль, то прибавляем в итоговую сумму
        
        "2: \n\t"
        "inc rcx \n\t" // Инкримент счётчика цикла
        "cmp rcx, %1 \n\t" // Повторное сравнение счётчика с N
        "jle 1b \n\t" // Если меньше, прыгаем на первый шаг
        
        "3: \n\t"
        ".att_syntax prefix \n\t" // Восстанавливаем привычный GCC синтаксис
        
        : "=&r" (sum) // & - модификатор раннего изменения. Регистр будет затронут до чтения входных
        : "r" (N)
        : "rax", "rcx", "rdx", "rsi", "cc"
    );
    
    return sum;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Не хватает параметра строки\n");
        return 1;
    }

    long int N = std::stoi(argv[1]);

    int result_cpp = 0;
    long int result_asm = 0;

    // Замер cpp
    auto start_cpp = std::chrono::high_resolution_clock::now();
    result_cpp = sum_cpp_func(N);
    auto end_cpp = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> cpp_time = end_cpp - start_cpp;

    // Замер asm
    auto start_asm = std::chrono::high_resolution_clock::now();
    result_asm = sum_asm_func(N);
    auto end_asm = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> asm_time = end_asm - start_asm;

    std::printf("\nN = %ld\n", N);
    std::printf("Результат C++: %d | Time: %lf ms\n", result_cpp, cpp_time.count());
    std::printf("Результат ASM: %ld | Time: %lf ms\n", result_asm, asm_time.count());

    return 0;
}
