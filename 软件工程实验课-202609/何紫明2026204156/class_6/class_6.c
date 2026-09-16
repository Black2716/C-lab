/* 实验6
二叉搜索树的建立、插入、删除以及遍历。 */

#include <stdio.h> // 打印函数
#include <stdlib.h> // 动态分配内存
#include <string.h> // 字符串操作函数

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 建立二叉搜索树
/*
 * 将有序数组 nums[left..right] 递归构建为一棵平衡的二叉搜索树
 * 分治思想：每次取中间元素作为根，左半建左子树，右半建右子树
 * 时间复杂度 O(n)，空间复杂度 O(log n)（递归栈深度）
 */
TreeNode* buildBST(int* nums, int left, int right) {
    if (left > right) {
        return NULL;
    }
    int mid = (left + right) / 2;
    TreeNode* root = (TreeNode*)malloc(sizeof(TreeNode));
    root->val = nums[mid];
    root->left = buildBST(nums, left, mid - 1);
    root->right = buildBST(nums, mid + 1, right);
    return root;
}

// 插入节点
TreeNode* insertBST(TreeNode* root, int val) {
    if (root == NULL) { // 树为空，直接插入新节点
        TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode)); 
        newNode->val = val;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (val == root->val) {
        printf("节点 %d 已存在，无法插入。\n", val);
        return root;
    } else if (val < root->val) {
        root->left = insertBST(root->left, val);
    } else {
        root->right = insertBST(root->right, val);
    }
    return root;
}

// 删除节点
TreeNode* deleteBST(TreeNode* root, int val) {
    if (root == NULL) {
        return root;
    }
    if (val < root->val) {
        root->left = deleteBST(root->left, val);
    } else if (val > root->val) {
        root->right = deleteBST(root->right, val);
    } else {
        if (root->left == NULL) { // 只有一个右子树
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) { // 只有一个左子树
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }
        // 有左右子树，找到后继节点
        TreeNode* successor = root->right;
        while (successor->left != NULL) {
            successor = successor->left; // 找到后继节点的最左子节点
        }
        root->val = successor->val; // 把后继的值搬到当前节点
        root->right = deleteBST(root->right, successor->val); // 删除后继节点
    }
    return root;
}

//释放内存
void freeBST(TreeNode* root) { // 后序遍历，释放内存
    if (root == NULL) {
        return;
    }
    freeBST(root->left); // 先释放左子树
    freeBST(root->right); // 再释放右子树
    free(root); // 最后释放根节点
}

// 打印二叉搜索树，中序遍历
void printBST(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    printBST(root->left);
    printf("%d ", root->val);
    printBST(root->right);
}

// 打印二叉搜索树，前序遍历
void printPreorderBST(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", root->val);
    printPreorderBST(root->left);
    printPreorderBST(root->right);
}

// 打印二叉搜索树，后序遍历 
void printPostorderBST(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    printPostorderBST(root->left);
    printPostorderBST(root->right);
    printf("%d ", root->val);
}

// 打印二叉搜索树，层序遍历，使用队列实现
void printLevelorderBST(TreeNode* root) {
    if (root == NULL) return;
    // 用数组模拟队列
    TreeNode* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = root; // 根节点入队, 初始化队列
    while (front < rear) {  // 队列不为空，继续遍历
        TreeNode* node = queue[front++]; // 出队，打印节点值
        printf("%d ", node->val);
        if (node->left)  queue[rear++] = node->left;
        if (node->right) queue[rear++] = node->right;
    }
}



// 打印二叉搜索树，深度优先遍历, 显式栈的 DFS（非递归前序）
void printDepthorderBST(TreeNode* root) {
    if (root == NULL) return;
    TreeNode* stack[100];
    int top = 0;
    stack[top++] = root;             // 根入栈
    while (top > 0) {
        TreeNode* node = stack[--top]; // 出栈
        printf("%d ", node->val);
        // 先推右孩子（后出），再推左孩子（先出）
        // 这样出栈顺序就是 根-左-右，和前序一致
        if (node->right) stack[top++] = node->right;
        if (node->left)  stack[top++] = node->left;
    }
}


//主函数
int main() {
    int nums[] = {1, 3, 4, 5, 6, 9, 10, 12, 13};
    int numsSize = sizeof(nums) / sizeof(nums[0]);
    TreeNode* root = buildBST(nums, 0, numsSize - 1);
    printf("二叉树搜索已建立。\n");
    printf("当前二叉搜索树：");
    printBST(root);
    printf("\n");
    int choice, val;
    while (1) { // 循环操作，直到用户选择退出
        printf("二叉搜索树操作菜单\n");
        printf("1. 插入节点\n");
        printf("2. 删除节点\n");
        printf("3. 打印二叉搜索树，中序遍历\n");
        printf("4. 打印二叉搜索树，前序遍历\n");
        printf("5. 打印二叉搜索树，后序遍历\n");
        printf("6. 打印二叉搜索树，层序遍历\n");
        printf("7. 打印二叉搜索树，深度优先遍历\n");
        printf("8. 退出\n");
        printf("请输入要进行的操作：");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("请输入要插入的节点值：");
                scanf("%d", &val);
                root = insertBST(root, val);
                printf("当前二叉搜索树：");
                printBST(root);
                printf("\n");
                break;
            case 2:
                printf("请输入要删除的节点值：");
                scanf("%d", &val);
                root = deleteBST(root, val);
                printf("删除节点 %d 成功。\n", val);
                printf("当前二叉搜索树：");
                printBST(root);
                printf("\n");
                break;
            case 3:
                printf("当前二叉搜索树，中序遍历：");
                printBST(root);
                printf("\n");
                break;
            case 4:
                printf("当前二叉搜索树，前序遍历：");
                printPreorderBST(root);
                printf("\n");
                break;
            case 5:
                printf("当前二叉搜索树，后序遍历：");
                printPostorderBST(root);
                printf("\n");
                break;
            case 6:
                printf("当前二叉搜索树，层序遍历：");
                printLevelorderBST(root);
                printf("\n");
                break;
            case 7:
                printf("当前二叉搜索树，深度优先遍历：");
                printDepthorderBST(root);
                printf("\n");
                break;
            case 8:
                printf("程序退出。\n");
                // 释放内存
                freeBST(root);
                root = NULL;
                printf("二叉搜索树内存已释放。\n");
                return 0;
            default:
                printf("无效操作，请重新输入。\n");
                break;
        }
        printf("\n");
    }
    return 0;
}