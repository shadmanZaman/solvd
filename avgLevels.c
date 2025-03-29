#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for a tree node
typedef struct Node
{
    int data;
    struct Node *left, *right;
} Node;

// Utility struct
typedef struct Average
{
    int total, count;
} Average;

void averageOfLevels(Node *node, Average *arr, int level)
{
    if (node == NULL)
        return;

    arr[level].total += node->data;
    arr[level].count++;

    averageOfLevels(node->left, arr, level + 1);
    averageOfLevels(node->right, arr, level + 1);
}

// Function to create a new tree node
Node *createNode(int value)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node *treeify(int arr[], int index, int n)
{
    if (index >= n)
        return NULL;

    Node *root = createNode(arr[index]);

    // Recursively build the left and right subtrees
    root->left = treeify(arr, 2 * index + 1, n);
    root->right = treeify(arr, 2 * index + 2, n);

    return root;
}

int main()
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int n = sizeof(arr) / sizeof(arr[0]);

    Node *root = treeify(arr, 0, n);

    Average avgArr[n];
    memset(avgArr, 0, sizeof(avgArr));
    averageOfLevels(root, avgArr, 0);

    for (int i = 0; i < n; i++)
        if (avgArr[i].count)
            printf("Avg: %.3f\n", (double)avgArr[i].total / avgArr[i].count);
}