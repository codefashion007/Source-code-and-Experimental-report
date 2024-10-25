#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

// 定义斐波那契堆中的节点结构
typedef struct Node
{
    int key;                                    // 节点的键值
    struct Node* parent, * child, * left, * right; // 指向父节点、子节点和兄弟节点的指针
    unsigned degree : 7;                        // 节点的度数，使用7位足以存储最大度数
    unsigned mark : 1;                          // 标记节点是否被切断过
} Node;

// 定义斐波那契堆结构
typedef struct
{
    Node* min; // 指向最小节点的指针
    int size;  // 堆中节点的数量
} FibonacciHeap;

void consolidate(FibonacciHeap* heap);
void _consolidate(FibonacciHeap* heap);

// debug
void printHeap(FibonacciHeap* heap)
{
    if (heap->min == NULL)
    {
        printf("Heap is empty\n");
        return;
    }
    printf("Heap nodes: ");
    Node* x = heap->min;
    do
    {
        printf("%d ", x->key);
        x = x->right;
    } while (x != heap->min);
    printf("\n");
}

// 创建一个新节点
Node* createNode(int key)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->parent = node->child = NULL;
    node->left = node->right = node; // 初始化为一个循环链表
    node->degree = 0;
    node->mark = 0;
    return node;
}

// 创建一个空的斐波那契堆
FibonacciHeap* createHeap()
{
    FibonacciHeap* heap = (FibonacciHeap*)malloc(sizeof(FibonacciHeap));
    heap->min = NULL;
    heap->size = 0;
    return heap;
}

// 向斐波那契堆中插入一个新节点
void insert(FibonacciHeap* heap, int key)
{

    // NoBugs
    // printHeap(heap);

    Node* node = createNode(key);
    // 如果堆为空，新节点就是最小节点
    if (heap->min == NULL)
    {
        heap->min = node;
    }
    else
    {
        // 将新节点插入到根节点列表中
        node->right = heap->min->right;
        node->left = heap->min;
        heap->min->right->left = node;
        heap->min->right = node;
        // 更新最小节点
        if (key < heap->min->key)
        {
            heap->min = node;
        }
    }
    heap->size++;
}

// 合并两个斐波那契堆
void merge(FibonacciHeap* heap1, FibonacciHeap* heap2)
{
    if (heap2->min == NULL)
        return;
    if (heap1->min == NULL)
    {
        *heap1 = *heap2;
        free(heap2);
        return;
    }
    // 合并两个堆的根节点列表
    heap1->min->left->right = heap2->min->right;
    heap2->min->right->left = heap1->min->left;
    heap1->min->left = heap2->min;
    heap2->min->right = heap1->min;
    // 更新最小节点
    if (heap2->min->key < heap1->min->key)
    {
        heap1->min = heap2->min;
    }
    free(heap2);
}

// 提取最小节点
Node* extractMin(FibonacciHeap* heap)
{
    Node* z = heap->min;
    if (z != NULL)
    {
        // 将最小节点的每个子节点添加到根链表中
        if (z->child != NULL)
        {
            Node* child = z->child;
            do
            {
                Node* next = child->right;
                child->left = heap->min;
                child->right = heap->min->right;
                heap->min->right->left = child;
                heap->min->right = child;
                child = next;
            } while (child != z->child);
        }

        // 移除最小节点
        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right)
        {
            heap->min = NULL;
        }
        else
        {
            heap->min = z->right;
            // 调整堆
            _consolidate(heap); // 需要实现合并堆
        }
        heap->size--;
    }
    return z;
}

void fibHeapLink(FibonacciHeap* heap, Node* y, Node* x)
{
    // 将y链接为x的子节点
    y->left->right = y->right;
    y->right->left = y->left;

    y->parent = x;

    if (x->child == NULL)
    {
        x->child = y;
        y->left = y->right = y;
    }
    else
    {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }

    x->degree++;
    y->mark = 0;
}

// v2.0
// 合并堆的根链表，确保每个度数唯一
void _consolidate(FibonacciHeap* heap)
{
    int D = (int)(log(heap->size) / log(2)) + 1;
    // FibHeapNode *A[D];
    //  动态分配 A 数组 Windows的Mingw不支持c99
    Node** A = (Node**)malloc(D * sizeof(Node*));
    // FibHeapNode *A = (FibHeapNode *) malloc(sizeof(FibHeapNode *[D]));

    for (int i = 0; i < D; i++)
    {
        A[i] = NULL;
    }

    Node* x = heap->min;
    int numRoots = 0;
    if (x != NULL)
    {
        numRoots++;
        x = x->right;
        while (x != heap->min)
        {
            numRoots++;
            x = x->right;
        }
    }
    while (numRoots > 0)
    {
        int d = x->degree;
        Node* next = x->right;

        while (A[d] != NULL)
        {
            Node* y = A[d]; // y与x度数相同
            if (x->key > y->key)
            { // 交换x与y
                Node* temp = x;
                x = y;
                y = temp;
            }
            fibHeapLink(heap, y, x);
            A[d] = NULL;
            d++;
        }

        A[d] = x;
        x = next;
        numRoots--;
    }

    heap->min = NULL;

    for (int i = 0; i < D; i++)
    {
        if (A[i] != NULL)
        {
            if (heap->min == NULL)
            {
                heap->min = A[i];
                heap->min->left = heap->min;
                heap->min->right = heap->min;
            }
            else
            {
                A[i]->left = heap->min;
                A[i]->right = heap->min->right;
                heap->min->right->left = A[i];
                heap->min->right = A[i];
                if (A[i]->key < heap->min->key)
                {
                    heap->min = A[i];
                }
            }
        }
    }
    // a small bug i forgot
    free(A);
}

// 维护堆结构的辅助函数
void consolidate(FibonacciHeap* heap)
{
    Node* A[50] = { NULL }, * x, * y, * w;
    int i, l;

    // 使用do-while循环替代原来的for循环，避免无限循环问题
    Node* start = heap->min;
    x = start;
    do
    {
        l = x->degree;
        while (A[l] != NULL)
        {
            y = A[l];
            // 这里怎么有个空指针访问
            if (x->key > y->key)
            {
                Node* temp = x;
                x = y;
                y = temp;
            }
            // 合并两个子堆
            w = x->child;
            y->parent = x;
            x->child = y;
            x->degree++;
            if (w != NULL)
            {
                y->left->right = w->right;
                w->right->left = y->left;
                y->right = w;
                w->left = y;
            }
            A[l] = NULL;
            l++;
        }
        A[l] = x;
        x = x->right;
    } while (x != start); // 遍历所有根节点

    heap->min = NULL;
    // 重新构建根节点列表并找到最小节点
    for (i = 0; i < 50; i++)
    {
        if (A[i] != NULL)
        {
            A[i]->left = A[i]->right = A[i]; // 重新建立循环链表结构
            if (heap->min == NULL || A[i]->key < heap->min->key)
            {
                heap->min = A[i];
            }
        }
    }
}

// 斐波那契堆排序
void fibonacciHeapSort(FibonacciHeap* heap)
{
    Node* minNode;
    while ((minNode = extractMin(heap)) != NULL)
    {
        printf("%d ", minNode->key);
        free(minNode);
    }
}

int main() {
    FibonacciHeap* heap = createHeap();
    srand(time(NULL)); // 设置随机种子

    // 生成随机数据并插入斐波那契堆
    for (int i = 0; i < 100000; i++) {
        int key = rand() % 1000; // 生成0到999之间的随机数
        insert(heap, key);
    }

    printf("Sorted keys:\n ");
    fibonacciHeapSort(heap);
    printf("\n ");

    // 释放斐波那契堆占用的内存
    while (extractMin(heap) != NULL)
        ;
    free(heap);
    return 0;
}