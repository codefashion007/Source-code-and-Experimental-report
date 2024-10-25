#include <stdio.h>
#include <stdlib.h>

// 交换数组中的两个元素
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 堆调整，将数组arr从index开始的heapSize个元素调整为最大堆
void heapify(int arr[], int n, int i) {
    int largest = i;      // 初始化最大值为根
    int left = 2 * i + 1; // 左子节点
    int right = 2 * i + 2; // 右子节点

    // 如果左子节点比根大，将最大值设置为左子节点
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    // 如果右子节点比最大值大，将其设置为最大值
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // 如果最大值不是根，将其与根交换，然后继续调整
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

// 堆排序
void heapSort(int arr[], int n) {
    // 构建最大堆
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // 一个个从堆中取出元素
    for (int i = n - 1; i >= 0; i--) {
        // 将当前的根（最大值）与最后一个元素交换
        swap(&arr[0], &arr[i]);

        // 调整剩余的堆
        heapify(arr, i, 0);
    }
}

// 打印数组
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 主函数
int main() {
    int arr[100000];
    int n = sizeof(arr) / sizeof(arr[0]);

    // 生成随机数组
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000; // 生成0到999之间的随机数
    }

    printf("原始数组:\n");
    printArray(arr, n);

    // 进行堆排序
    heapSort(arr, n);

    printf("排序后的数组:\n");
    printArray(arr, n);

    return 0;
}