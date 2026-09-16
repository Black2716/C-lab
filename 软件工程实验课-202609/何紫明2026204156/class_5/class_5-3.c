/*  单向循环链表的建立、插入和删除操作。
示例：
输入：8位整数
生成：9 5 6 3 4 4 7 0
链表：0->3->4->4->5->6->7->9->0
插入：2
链表：0->2->3->4->4->5->6->7->9->0
删除：5
链表：0->2->3->4->4->6->7->9->0
查询：4
结果：4位于链表第4和5位
删除：4
链表：0->2->3->6->7->9->0

*/

#include <stdio.h>
#include <stdlib.h>

// 定义单向循环链表节点结构体
typedef struct Node {
    int score; // 节点数据
    struct Node* next; // 指向下一个节点的指针
} Linklist;    

// 创建一个新的单向循环链表节点
Linklist* createNode(int data) {
    Linklist* newNode = (Linklist*)malloc(sizeof(Linklist));
    if (!newNode) {
        printf("内存分配失败!\n");
        exit(1);
    }
    newNode->score = data; // 设置节点数据
    newNode->next = NULL; // 初始化下一个节点指针为NULL
    return newNode; // 返回新创建的节点
}

// 插入节点到单向循环链表中（升序插入）
void insertNode(Linklist** head, int data) {
    Linklist* newNode = createNode(data); // 创建新节点
    if (*head == NULL) { // 如果链表为空
        *head = newNode; // 新节点成为头节点
        newNode->next = newNode; // 新节点指向自身，形成循环
        return;
    }
    Linklist* current = *head; // 从头节点开始遍历
    Linklist* pre = NULL; // 前一个节点指针
    // 找到插入位置
    do {
        if (current->score >= data) { // 找到插入位置
            break;
        }
        pre = current; // 更新前一个节点
        current = current->next; // 移动到下一个节点
    } while (current != *head); // 循环直到回到头节点
    if (pre == NULL) { // 插入到头部
        newNode->next = *head; // 新节点指向当前头节点
        // 找到最后一个节点，更新其next指针
        Linklist* last = *head;
        while (last->next != *head) {
            last = last->next; // 移动到下一个节点
        }
        last->next = newNode; // 最后一个节点指向新节点
        *head = newNode; // 更新头节点为新节点
    } else {
        pre->next = newNode; // 前一个节点指向新节点
        newNode->next = current; // 新节点指向当前节点
    }
}

// 删除指定值的单向循环链表节点
void deleteNode(Linklist** head, int data) {
    if (*head == NULL) {
        printf("链表为空，无法删除节点 %d。\n", data);
        return;
    }

    Linklist* current = *head;
    Linklist* pre = NULL;

    // 使用 do-while 遍历循环链表，保证头节点被访问
    // current = *head 从头节点开始遍历，普通while第一次判断就会失败
    int count = 0;
    do {
        if (current->score == data) {
            count++;
            Linklist* temp = current;
            if (current->next == current) {
                // 链表只有一个节点
                *head = NULL;
            } else if (pre == NULL) {
                // 删除头节点（链表有多个节点）
                // 找到尾节点
                Linklist* last = *head;
                while (last->next != *head) {
                    last = last->next;
                }

                *head = current->next;
                last->next = *head;
                current = *head;
                pre = NULL;
            } else {
                // 删除中间/尾节点
                pre->next = current->next;
                current = current->next;
            }
            free(temp);
            continue;
        }
        pre = current;
        current = current->next;
    } while (current != *head);
    if (count == 0) {
        printf("节点 %d 不存在。\n", data);
    }
}


// 查询指定值的单向循环链表节点
void queryNode(Linklist* head, int data) {
    if (head == NULL) {
        printf("链表为空，无法查询节点 %d。\n", data);
        return;
    }

    Linklist* current = head;
    int count = 0;
    do {
        if (current->score == data) {
            printf("节点 %d 位于链表第 %d 位。\n", data, count + 1);
        }
        count++;
        current = current->next;
    } while (current != head);
    if (count == 0) {
        printf("节点 %d 不存在。\n", data);
    }
}

// 打印单向循环链表
void printList(Linklist* head) {
    if (head == NULL) {
        printf("链表为空。\n");
        return;
    }

    Linklist* current = head;
    do {
        printf("%d", current->score);
        current = current->next;
        printf("->"); // 输出箭头表示链表连接
        if (current == head) {
            printf("%d", current->score);
        }

    } while (current != head);
    printf("\n");
}

// 主函数
int main() {
    Linklist* head = NULL; // 初始化链表为空
    //9 5 6 3 4 4 7 0
    insertNode(&head, 9);
    insertNode(&head, 5);
    insertNode(&head, 6);
    insertNode(&head, 3);
    insertNode(&head, 4);
    insertNode(&head, 4);
    insertNode(&head, 7);
    insertNode(&head, 0);
    printList(head);

    int choice, data; // 用户选择和数据输入变量
    while (1) { // 无限循环，直到用户选择退出
        printf("\n单向循环链表菜单:\n");
        printf("1. 插入节点\n");
        printf("2. 删除节点\n");
        printf("3. 查询节点\n");
        printf("4. 打印链表\n");
        printf("5. 退出\n");
        printf("请输入您的选择：");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("请输入要插入的节点值：");
                scanf("%d", &data);
                insertNode(&head, data);
                printList(head);
                break;
            case 2:
                printf("请输入要删除的节点值：");
                scanf("%d", &data);
                deleteNode(&head, data);
                printList(head);
                break;
            case 3:
                printf("请输入要查询的节点值：");
                scanf("%d", &data);
                queryNode(head, data);
                break;
            case 4:
                printList(head);
                break;
            case 5:
                printf("退出程序！\n");
                //释放链表内存
                Linklist* start = head;
                do {
                    Linklist* temp = head;
                    head = head->next;
                    free(temp);
                } while (head != start);
                printf("链表内存已释放。\n");
                return 0;
            default:
                printf("无效的选择，请重新输入。\n");
                break;
        }
    }
    return 0;
}