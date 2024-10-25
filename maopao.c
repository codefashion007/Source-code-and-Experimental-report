#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int arr[100000]; // 定义一个包含100000个元素的数组
    int n = sizeof(arr) / sizeof(arr[0]);
    int i;

    // 设置随机数生成器的种子
    srand((unsigned int)time(NULL));

    // 生成随机数组
    for (i = 0; i < n; i++) {
        arr[i] = rand() % 1000; // 生成0到999之间的随机数
    }

    // 打印整个随机数组
    printf("随机数组:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    bubbleSort(arr, n);

    // 打印整个排序后的数组
    printf("排序后的数组:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}