#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Функция для слияния двух подмассивов
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void generateArray(int arr[], int size, int type) {
    if (type == 0) { // Отсортированный массив
        for (int i = 0; i < size; i++) {
            arr[i] = i;
        }
    } else if (type == 1) { // Обратно отсортированный массив
        for (int i = 0; i < size; i++) {
            arr[i] = size - i - 1;
        }
    } else { // Случайный массив
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % 100000;
        }
    }
}

void measureTime(int arr[], int size, int type) {
    clock_t start, end;
    double total_time = 0;
    
    for (int i = 0; i < 10; i++) {
        int *copy = (int *)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            copy[j] = arr[j];
        }

        start = clock();
        mergeSort(copy, 0, size - 1);
        end = clock();

        total_time += (double)(end - start) / CLOCKS_PER_SEC;

        free(copy);
    }

    printf("Размер: %d, Тип: %d, Среднее время: %.6f секунд\n", size, type, total_time / 10);
}

int main() {
    srand(time(NULL));

    int sizes[] = {100, 1000, 10000, 100000, 1000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int t = 0; t < 3; t++) { // 0 - отсортированный, 1 - обратно отсортированный, 2 - случайный
        for (int s = 0; s < num_sizes; s++) {
            int size = sizes[s];
            int *arr = (int *)malloc(size * sizeof(int));

            generateArray(arr, size, t);
            measureTime(arr, size, t);

            free(arr);
        }
    }

    return 0;
}
