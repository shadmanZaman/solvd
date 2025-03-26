#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// LEETCODE 621
typedef struct Element // char not required
{
    int freq, gap;
} Element;

// Min heap helper functions
void swap(Element *a, Element *b)
{
    Element temp = *a;
    *a = *b;
    *b = temp;
}

void reduceGap(Element heap[], int l)
{
    for (int i = 0; i < l; i++)
        heap[i].gap--;
}

// Min heapify function
void heapify(Element heap[], int size, int i)
{
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left].gap < heap[smallest].gap)
        smallest = left;
    if (right < size && heap[right].gap < heap[smallest].gap)
        smallest = right;

    if (smallest != i)
    {
        swap(&heap[i], &heap[smallest]);
        heapify(heap, size, smallest);
    }
}

// Insert into the min heap
void insertHeap(Element heap[], int *heapSize, Element *value) // tasklenghth and index
{
    heap[(*heapSize)++] = *value;

    int i = *heapSize - 1;
    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (heap[parent].gap <= heap[i].gap)
            break;

        swap(&heap[i], &heap[parent]); // swap
        i = parent;                    // move up
    }
}

Element heapPop(Element heap[], int *size)
{
    Element root = heap[0];
    heap[0] = heap[--(*size)];
    heapify(heap, *size, 0); // restore heap property

    return root;
}

void taskScheduler(Element heap[], int *heapSize, int n, int *count)
{
    if (heap[0].freq < 1) // if its out
    {
        heapPop(heap, heapSize);
        return;
    }

    while (heap[0].gap > 0) // idle states
    {
        reduceGap(heap, *heapSize);
        printf("IDLE\n");
        (*count)++; // direct? instead of multiple iteration?
    }

    Element temp = heapPop(heap, heapSize);

    printf("TASK PERFORMED\n");
    temp.gap = n; // restore back to original count;
    temp.freq--;  // task performed;
    (*count)++;

    reduceGap(heap, *heapSize);        // n shift due to task performed
    insertHeap(heap, heapSize, &temp); // push back
}

int main()
{
    char tasks[] = "AAABBB";
    int dict[100] = {0}, n = 2; // 100 since all alphabets map to this range
    int heapSize = 0, count = 0, taskLength = strlen(tasks);

    for (int i = 0; i < taskLength; i++)
    {
        if (!dict[tasks[i]])
            heapSize++;

        dict[tasks[i]]++;
    }

    Element heap[heapSize];
    taskLength = heapSize; // size of num tasks
    heapSize = 0;

    for (int i = 50; i < 100; i++) // insertion phase
    {
        if (dict[i])
        {
            heap[heapSize++] = (Element){dict[i] - 1, n - (taskLength - heapSize)};
            count++;
        }
    }
    // heap is already min-heapified in the beginning

    // post insertion phase
    while (heapSize > 0)
    {
        taskScheduler(heap, &heapSize, n, &count);
    }
    printf("\nMin CPU Intervals: %d", count);
}
