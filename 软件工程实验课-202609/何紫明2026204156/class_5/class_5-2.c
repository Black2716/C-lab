
/*双向链表的建立、插入和删除操作。
示例：
输入：8位整数
生成：9 5 6 3 4 4 7 0
链表（Next）：0->3->4->4->5->6->7->9
链表（Prior）：9->7->6->5->4->4->3->0
插入：2
链表（Next）：0->2->3->4->4->5->6->7->9
链表（Prior）： 9->7->6->5->4->4->3->2->0
删除：5
链表（Next）：0->2->3->4->4->6->7->9
链表（Prior）：9->7->6->4->4->3->2->0
查询：4
结果：4位于正向链表第4和5位,位于反向链表第4和5位
删除：4
链表（Next）：0->2->3->6->7->9
链表（Prior）：9->7->6->3->2->0
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct DNode {
    int score; // 节点数据
    struct DNode* next; // 指向下一个节点的指针
    struct DNode* prior; // 指向前一个节点的指针
} DLinklist;

// 创建一个新的双向链表节点
DLinklist* createDNode(int data) {
    DLinklist* newNode = (DLinklist*)malloc(sizeof(DLinklist));
    if (!newNode) {
        printf("内存分配失败!\n");
        exit(1);
    }
    newNode->score = data; // 设置节点数据
    newNode->next = NULL; // 初始化下一个节点指针为NULL
    newNode->prior = NULL; // 初始化前一个节点指针为NULL
    return newNode; // 返回新创建的节点
}   

// 插入节点到双向链表中（升序插入）
void insertDNode(DLinklist** head, int data) {
    DLinklist* newNode = createDNode(data); // 创建新节点
    DLinklist* pre = NULL; // 从头节点开始遍历
    DLinklist* current = *head; // 从头节点开始遍历
    // 找到插入位置
    while (current != NULL && current->score < data) {
        pre = current;
        current = current->next; // 移动到下一个节点
    }
    if (pre == NULL) { // 插入到头部
        newNode->next = *head; // 新节点指向当前头节点
        if (*head != NULL) {
            (*head)->prior = newNode; // 当前头节点的前一个节点指向新节点
        }
        *head = newNode; // 更新头节点为新节点
    } else {
        pre->next = newNode; // 前一个节点指向新节点
        newNode->prior = pre; // 新节点的前一个节点指向前一个节点
        newNode->next = current; // 新节点指向当前节点
        if (current != NULL) {
            current->prior = newNode; // 当前节点的前一个节点指向新节点
        }
    }
}

// 删除指定值的双向链表节点
void deleteDNode(DLinklist** head, int data) {
    DLinklist* current = *head; // 从头节点开始遍历
    int found = 0; // 标记是否找到节点
    while (current != NULL) {
        if (current->score == data) { // 找到要删除的节点
            found = 1; // 标记为找到
            DLinklist* temp = current; // 临时指针保存当前节点
            if (current->prior == NULL) { // 删除头节点
                *head = current->next; // 更新头节点为下一个节点
                if (*head != NULL) {
                    (*head)->prior = NULL; // 新头节点的前一个节点指向NULL
                }
                free(current); // 释放当前节点内存
                current = *head; // 移动到新的头节点
            } else {
                current->prior->next = current->next; // 前一个节点指向当前节点的下一个节点
                if (current->next != NULL) {
                    current->next->prior = current->prior; // 下一个节点的前一个节点指向当前节点的前一个节点
                }
                current = current->next; // 移动到下一个节点
                free(temp); // 释放当前节点内存
            }
        } else {
            current = current->next; // 移动到下一个节点
        }
    }
    if (!found) {
        printf("节点 %d 不存在。\n", data);
    } else {
        printf("删除节点 %d 完成。\n", data);
    }
}

// 查询指定值的双向链表节点
void searchDNode(DLinklist* head, int data) {
    DLinklist* current = head; // 从头节点开始遍历
    int position = 1; // 节点位置计数器
    int found = 0; // 标记是否找到节点
    while (current != NULL) {
        if (current->score == data) {
            printf("节点 %d 存在，位于正向链表第 %d 个位置。\n", data, position);
            found = 1; // 标记为找到
        }
        current = current->next; // 移动到下一个节点
        position++; // 位置计数器加1
    }
    if (!found) { // 如果没有找到节点
        printf("节点 %d 不存在。\n", data);
    }
}

// 显示双向链表
void displayDList(DLinklist* head) {
    DLinklist* current = head; // 从头节点开始遍历
    if (current == NULL) { // 如果链表为空
        printf("链表为空。\n");
        return;
    }
    printf("链表（Next）：");
    while (current != NULL) {
        printf("%d", current->score); // 输出当前节点的数据
        if (current->next != NULL) { // 如果不是最后一个节点
            printf(" -> "); // 输出箭头表示链表连接
        }
        current = current->next; // 移动到下一个节点
    }
    printf("\n链表（Prior）：");
    // 显示反向链表
    current = head; // 重新从头节点开始遍历
    while (current->next != NULL) { // 移动到最后一个节点
        current = current->next; // 移动到下一个节点
    }
    while (current != NULL) { // 从最后一个节点开始向前遍历
        printf("%d", current->score); // 输出当前节点的数据
        if (current->prior != NULL) { // 如果不是第一个节点
            printf(" -> "); // 输出箭头表示链表连接
        }
        current = current->prior; // 移动到前一个节点
    }
    printf("\n"); // 换行
}

// 主函数
int main() {
    DLinklist* head = NULL; // 初始化双向链表头指针为NULL
    insertDNode(&head, 9); //9 5 6 3 4 4 7 0
    insertDNode(&head, 5);
    insertDNode(&head, 6);
    insertDNode(&head, 3);
    insertDNode(&head, 4);
    insertDNode(&head, 4);
    insertDNode(&head, 7);
    insertDNode(&head, 0);
    displayDList(head);

    int choice, data; // 用户选择和数据输入变量
    while (1) { // 无限循环，直到用户选择退出
        printf("\n双向链表菜单:\n");
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
                insertDNode(&head, data); // 调用插入函数
                displayDList(head); // 显示双向链表
                break;
            case 2:
                printf("请输入要删除的整数: ");
                scanf("%d", &data); // 读取要删除的整数
                deleteDNode(&head, data); // 调用删除函数
                displayDList(head); // 显示双向链表
                break;
            case 3:
                printf("请输入要查询的整数: ");
                scanf("%d", &data); // 读取要查询的整数
                searchDNode(head, data); // 调用查询函数
                break;
            case 4:
                displayDList(head); // 显示双向链表
                break;
            case 5:
                printf("退出程序。\n");
                // 释放双向链表内存
                while (head != NULL) {
                    DLinklist* temp = head; // 临时指针保存当前头节点
                    head = head->next; // 移动头指针到下一个节点
                    free(temp); // 释放当前节点的内存
                }
                printf("双向链表内存已释放。\n");
                return 0; // 退出程序
            default:
                printf("无效选择，请重新输入。\n"); // 提示无效选择
        }
    }
    return 0; 
}
