#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void write_numbers_to_file(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }
    
    int num, count = 0;
    printf("Введите целые числа (0 для завершения ввода):\n");
    while (1) {
        scanf("%d", &num);
        if (num == 0) break;
        fwrite(&num, sizeof(int), 1, file);
        count++;
    }
    fclose(file);
}

void read_numbers_from_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }
    
    int num;
    printf("Содержимое файла:\n");
    while (fread(&num, sizeof(int), 1, file)) {
        printf("%d ", num);
    }
    printf("\n");
    fclose(file);
}

void calculate_sports_score(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }
    
    int num, min = INT_MAX, max = INT_MIN, sum = 0, count = 0;
    while (fread(&num, sizeof(int), 1, file)) {
        if (num < min) min = num;
        if (num > max) max = num;
        sum += num;
        count++;
    }
    fclose(file);
    
    if (count <= 2) {
        printf("Недостаточно данных для вычисления балла спортсмена.\n");
        return;
    }
    
    double avg = (double)(sum - min - max) / (count - 2);
    printf("Средний балл спортсмена: %.2f\n", avg);
}

void swap_min_max(const char *filename) {
    FILE *file = fopen(filename, "r+b");
    if (!file) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }
    
    int num, min = INT_MAX, max = INT_MIN;
    long min_pos = -1, max_pos = -1, pos = 0;
    while (fread(&num, sizeof(int), 1, file)) {
        if (num < min) {
            min = num;
            min_pos = pos;
        }
        if (num > max) {
            max = num;
            max_pos = pos;
        }
        pos++;
    }
    
    if (min_pos != -1 && max_pos != -1) {
        fseek(file, min_pos * sizeof(int), SEEK_SET);
        fwrite(&max, sizeof(int), 1, file);
        
        fseek(file, max_pos * sizeof(int), SEEK_SET);
        fwrite(&min, sizeof(int), 1, file);
    }
    
    fclose(file);
}

void sort_numbers_in_file(const char *filename) {
    FILE *file = fopen(filename, "r+b");
    if (!file) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }
    
    int num1, num2, swapped;
    long count = 0;
    fseek(file, 0, SEEK_END);
    count = ftell(file) / sizeof(int);
    
    for (long i = 0; i < count - 1; i++) {
        swapped = 0;
        for (long j = 0; j < count - i - 1; j++) {
            fseek(file, j * sizeof(int), SEEK_SET);
            fread(&num1, sizeof(int), 1, file);
            fread(&num2, sizeof(int), 1, file);
            
            if (num1 > num2) {
                fseek(file, j * sizeof(int), SEEK_SET);
                fwrite(&num2, sizeof(int), 1, file);
                fwrite(&num1, sizeof(int), 1, file);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
    
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <имя файла>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    const char *filename = argv[1];
    write_numbers_to_file(filename);
    read_numbers_from_file(filename);
    calculate_sports_score(filename);
    swap_min_max(filename);
    printf("После замены минимальных и максимальных элементов:\n");
    read_numbers_from_file(filename);
    sort_numbers_in_file(filename);
    printf("После сортировки:\n");
    read_numbers_from_file(filename);
    
    return EXIT_SUCCESS;
}
