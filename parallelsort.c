#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Функция для слияния двух подмассивов
void merge(int* arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSort(int* arr, int left, int right, int depth) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if (depth < 4) { // Параллельность на верхних уровнях
            #pragma omp parallel sections
            {
                #pragma omp section
                mergeSort(arr, left, mid, depth + 1);

                #pragma omp section
                mergeSort(arr, mid + 1, right, depth + 1);
            }
        } else {
            mergeSort(arr, left, mid, depth + 1);
            mergeSort(arr, mid + 1, right, depth + 1);
        }

        merge(arr, left, mid, right);
    }
}

void generateArray(int* arr, int n, int type) {
    if (type == 0) { // Случайный 
        for (int i = 0; i < n; i++) arr[i] = rand() % 1000000;
    } else if (type == 1) { // Упорядоченный 
        for (int i = 0; i < n; i++) arr[i] = i;
    } else if (type == 2) { // Обратно упорядоченный 
        for (int i = 0; i < n; i++) arr[i] = n - i;
    }
}

void testSorting(int n, int maxThreads) {
    int* arr = (int*)malloc(n * sizeof(int));
    int* original = (int*)malloc(n * sizeof(int));
    char* types[] = {"Случайный", "Упорядоченный", "Обратно упорядоченный"};

    printf("Размер массива: %d\n", n);

    for (int type = 0; type < 3; type++) {
        // Генерация исходного массива
        generateArray(original, n, type);

        for (int threads = 1; threads <= maxThreads; threads++) {
            double totalTime = 0.0;

            for (int test = 0; test < 10; test++) { // 10 запусков
                for (int i = 0; i < n; i++) arr[i] = original[i];

                omp_set_num_threads(threads);

                double start = omp_get_wtime();
                mergeSort(arr, 0, n - 1, 0);
                double end = omp_get_wtime();

                totalTime += (end - start);
            }

            double averageTime = totalTime / 10.0;
            printf("Тип массива: %s, Потоки: %d, Среднее время: %f секунд\n",
                   types[type], threads, averageTime);
        }
    }

    free(arr);
    free(original);
}

int main() {
    int sizes[] = { 100, 1000, 10000, 100000, 1000000};
    int maxThreads = 8;

    for (int i = 0; i < 5; i++) {
        testSorting(sizes[i], maxThreads);
    }

    return 0;
}
