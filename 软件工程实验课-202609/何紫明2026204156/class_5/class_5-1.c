/* 实验5
单向链表、双向链表、单向循环链表的建立、插入和删除操作。
建立一个单向链表，输入一串随机指定位数的整数并排序（升序或者降序），
同时可以实现插入节点、删除节点、节点显示等操作，
编写一个菜单，由用户选择执行哪一种操作(插入/删除/按值查询)，并输出链表。 

示例：
输入：8位整数： 9 5 6 3 4 4 7 0
生成链表：0 -> 3 -> 4 -> 4 -> 5 -> 6 -> 7 -> 9
插入节点：2
生成链表：0 -> 2 -> 3 -> 4 -> 4 -> 5 -> 6 -> 7 -> 9
删除节点：5
生成链表：0 -> 2 -> 3 -> 4 -> 4 -> 6 -> 7 -> 9
查询节点：4
结果：节点存在，4位于链表第4和第5个位置
删除节点：4
生成链表：0 -> 2 -> 3 -> 6 -> 7 -> 9
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义单向链表节点结构体
typedef struct Node {
    int score; // 节点数据
    struct Node* next; // 指向下一个节点的指针
} Linklist;

// 创建一个新的节点
Linklist* createNode(int data) {
    Linklist* newNode = (Linklist*)malloc(sizeof(Linklist)); // 分配内存
    if (!newNode) { // 检查内存分配是否成功
        printf("内存分配失败!\n");
        exit(1);
    }
    newNode->score = data; // 设置节点数据
    newNode->next = NULL; // 初始化下一个节点指针为NULL
    return newNode; // 返回新创建的节点
}

// 插入节点到链表中（升序插入）
void insertNode(Linklist** head, int data) {
    Linklist* newNode = createNode(data); // 创建新节点
    Linklist* pre = NULL; // 从头节点开始遍历
    Linklist* current = *head; // 从头节点开始遍历

    // 找到插入位置
    while (current != NULL && current->score < data) {
        pre = current;
        current = current->next; // 移动到下一个节点
    }
    if (pre == NULL) { // 插入到头部
        newNode->next = *head; // 新节点指向当前头节点
        *head = newNode; // 更新头节点为新节点
    } else { 
        pre->next = newNode; 
        newNode->next = current; // 新节点指向当前节点
    }
}

// 删除指定值的节点
void deleteNode(Linklist** head, int data) {
    Linklist* pre = NULL; 
    Linklist* current = *head; 
    // 遍历链表查找要删除的节点,所有等于data的节点都删除
    while (current != NULL) {
        if (current->score == data) { // 找到要删除的节点
            if (pre == NULL) { // 删除头节点
                *head = current->next; // 更新头节点为下一个节点
                free(current); // 释放当前节点内存
                current = *head; // 移动到新的头节点
            } else { 
                pre->next = current->next; // 前一个节点指向当前节点的下一个节点
                free(current); // 释放当前节点内存
                current = pre->next; // 移动到下一个节点
            }
        } else { 
            pre = current; 
            current = current->next; // 移动到下一个节点
        }
    }
    printf("删除节点 %d 完成。\n", data); 
}  

// 查询指定值的节点
void searchNode(Linklist* head, int data) {
    Linklist* current = head;
    int position = 1; // 节点位置计数器
    int found = 0; // 标记是否找到节点
    while (current != NULL) {
        if (current->score == data) {
            printf("节点 %d 存在，位于链表第 %d 个位置。\n", data, position);
            found = 1; // 标记为找到
        }
        current = current->next; // 移动到下一个节点
        position++; // 位置计数器加1
    }
    if (!found) { // 如果没有找到节点
        printf("节点 %d 不存在。\n", data);
    }
}

// 显示链表
void displayList(Linklist* head) {
    Linklist* current = head; // 从头节点开始遍历
    if (current == NULL) { // 如果链表为空
        printf("链表为空。\n");
        return;
    }
    printf("链表内容：");
    while (current != NULL) {
        printf("%d", current->score); // 输出当前节点的数据
        if (current->next != NULL) { // 如果不是最后一个节点
            printf(" -> "); // 输出箭头表示链表连接
        }
        current = current->next; // 移动到下一个节点
    }
    printf("\n"); // 换行
}

// 主函数
int main() {
    Linklist* head = NULL; // 初始化链表头指针为NULL
    insertNode(&head, 9); //9 5 6 3 4 4 7 0
    insertNode(&head, 5);
    insertNode(&head, 6);
    insertNode(&head, 3);
    insertNode(&head, 4);
    insertNode(&head, 4);
    insertNode(&head, 7);
    insertNode(&head, 0);
    displayList(head);

    int choice, data; // 用户选择和数据输入变量
    while (1) { // 无限循环，直到用户选择退出
        printf("\n菜单:\n");
        printf("1. 插入节点\n");
        printf("2. 删除节点\n");
        printf("3. 查询节点\n");
        printf("4. 显示链表\n");
        printf("5. 退出\n");
        printf("请输入您的选择: ");
        scanf("%d", &choice); // 读取用户选择
        switch (choice) { // 根据用户选择执行相应操作
            case 1:
                printf("请输入要插入的整数: ");
                scanf("%d", &data); // 读取要插入的整数
                insertNode(&head, data); // 调用插入函数
                displayList(head); // 显示链表
                break;
            case 2:
                printf("请输入要删除的整数: ");
                scanf("%d", &data); // 读取要删除的整数
                deleteNode(&head, data); // 调用删除函数
                displayList(head); // 显示链表
                break;
            case 3:
                printf("请输入要查询的整数: ");
                scanf("%d", &data); // 读取要查询的整数
                searchNode(head, data); // 调用查询函数
                break;
            case 4:
                displayList(head); // 显示链表
                break;
            case 5:
                printf("退出程序。\n");
                // 释放链表内存
                while (head != NULL) {
                    Linklist* temp = head; // 临时指针保存当前头节点
                    head = head->next; // 移动头指针到下一个节点
                    free(temp); // 释放当前节点的内存
                }
                printf("链表内存已释放。\n");
                return 0; // 退出程序
            default:
                printf("无效选择，请重新输入。\n"); // 提示无效选择
        }
    }
    return 0; // 返回0表示程序正常结束
}



