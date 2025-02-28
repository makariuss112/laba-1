#include <stdio.h>
#include <string.h>
#include <limits.h>

int main()
{
    int number, numSum = 0, numCount = 0, maxNum = INT_MIN, minNum = INT_MAX, counter = 0, minNumIndex = 0, maxNumIndex = 0;
    float average = 0;
    FILE *file, *tempFile;
    char filename[100];

    printf("Enter the filename (without extension): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    strcat(filename, ".bin"); // объединение строк название файла и .bin

    file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return 1;
    }
    printf("Enter integers. To complete, enter any character (except a number).\n");
    while (1)
    {
        if (scanf("%d", &number) == 1)
        {
            fwrite(&number, sizeof(int), 1, file);
        }
        else
        {
            while (getchar() != '\n')
                ;
            break;
        }
    }
    fclose(file);

    file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
        return 1;
    }

    while (fread(&number, sizeof(int), 1, file) == 1)
    {
        if (number <= minNum)
        {
            minNum = number;
            minNumIndex = counter;
        }

        if (number >= maxNum)
        {
            maxNum = number;
            maxNumIndex = counter;
        }
        counter++;
    }
    fclose(file);

    file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file for reading again.\n");
        return 1;
    }

    tempFile = fopen("tempFile.bin", "wb");
    if (tempFile == NULL)
    {
        printf("Error opening temporary file for writing.\n");
        return 1;
    }

    counter = 0;
    while (fread(&number, sizeof(int), 1, file) == 1)
    {
        if (counter != minNumIndex && counter != maxNumIndex)
        {
            fwrite(&number, sizeof(int), 1, tempFile);
        }
        counter++;
    }

    fclose(file);
    fclose(tempFile);
    remove(filename);
    rename("tempFile.bin", filename);

    file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file for reading final result.\n");
        return 1;
    }

    printf("\nRemaining numbers: ");

    while (fread(&number, sizeof(int), 1, file) == 1)
    {
        numSum += number;
        numCount++;
        printf("%d ", number);
    }
    fclose(file);
    printf("\nSum of elements: %d", numSum);

    if (numCount > 0)
    {
        average = (float)numSum / numCount;
        printf("\nAverage of remaining elements: %.2f\n", average);
    }
    else
    {
        printf("\nNo remaining elements to calculate average.\n");
    }
 
    maxNum = INT_MIN;
    minNum = INT_MAX;

    file = fopen(filename, "rb");
    while (fread(&number, sizeof(int), 1, file) == 1)
    {
        if (number <= minNum)
        {
            minNum = number;
        }

        if (number >= maxNum)
        {
            maxNum = number;
        }
        counter++;
    }
    fclose(file);

    file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file for reading again.\n");
        return 1;
    }

    tempFile = fopen("tempFile.bin", "wb");
    if (tempFile == NULL)
    {
        printf("Error opening temporary file for writing.\n");
        return 1;
    }

    while (fread(&number, sizeof(int), 1, file) == 1)
    {
        if (number == minNum)
        {
            fwrite(&maxNum, sizeof(int), 1, tempFile);
        }
        else if (number == maxNum)
        {
            fwrite(&minNum, sizeof(int), 1, tempFile);
        }
        else
        {
            fwrite(&number, sizeof(int), 1, tempFile);
        }
    }
    fclose(file);
    fclose(tempFile);
    remove(filename);
    rename("tempFile.bin", filename);

    file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file for reading final result.\n");
        return 1;
    }

    printf("\nChanged number of elements: ");

    while (fread(&number, sizeof(int), 1, file) == 1)
    {
        printf("%d ", number);
    }
    fclose(file);

    file = fopen(filename, "rb+");
    if (file == NULL)
    {
        printf("Error opening file for reading and writing.\n");
        return 1;
    }

    int swapped;
    do
    {
        swapped = 0;
        fseek(file, 0, SEEK_SET);

        // Чтение и сравнение чисел
        for (int i = 0; i < numCount - 1; i++)
        {
            fseek(file, i * sizeof(int), SEEK_SET);
            fread(&number, sizeof(int), 1, file);
            int current = number;

            fseek(file, (i + 1) * sizeof(int), SEEK_SET);
            int next;
            fread(&next, sizeof(int), 1, file);

            // Сравнение и замена
            if (current > next)
            {
                fseek(file, i * sizeof(int), SEEK_SET);
                fwrite(&next, sizeof(int), 1, file);
                fwrite(&current, sizeof(int), 1, file);
                swapped = 1;
            }
        }
    } while (swapped);

    fclose(file);

    file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file for reading final result.\n");
        return 1;
    }

    printf("\nSorteded elements: ");
    while (fread(&number, sizeof(int), 1, file) == 1)
    {
        printf("%d ", number);
    }
    fclose(file);

    getchar();
    return 0;
}
