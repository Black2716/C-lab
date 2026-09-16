/* 实验8
舞伴配对问题。假设在大学生的周末舞会上，男、女学生各自排成一队。
舞会开始时，依次从男队和女队的队头各出一人配成舞伴。--
如果两队初始人数不等，则较长的那一队中未配对者等待下一轮舞曲。

要求【男、女学生人数】及其【姓名】以及【舞会的轮数】由用户从键盘输入，
屏幕输出每一轮舞伴的配对名单，如果在该轮中有未配对的，则要求能够从屏幕显示下一轮第一个出场的未配对者姓名。
 */

/*
解题思路总结：
1. 配对过的人不会离场，只是循环排队，每一轮继续参与；
2. 每一轮实际可配对对数 = 男生队列长度、女生队列长度两者的较小值；
3. 一轮结束后，比较两队长度，选取长度更大的队列，输出其队头姓名，作为未配对第一个出场人员；
4. 需要兼容三种场景：男生人数多于女生、女生人数多于男生、男女人数相等。
*/
#include <stdio.h>
#include <string.h>
#define MAXSIZE 50
#define NAMELEN 20

typedef struct {
    char name[MAXSIZE][NAMELEN]; // 存储姓名
    int front;
    int rear;
} Queue;

void initQueue(Queue *q) {
    q->front = 0;
    q->rear = 0;
}

int isEmpty(Queue *q) {
    return q->front == q->rear;
}

//获取队列元素个数
int getSize(Queue *q) {
    return (q->rear - q->front + MAXSIZE) % MAXSIZE;
}

int enQueue(Queue *q, char *name) { 
    if ((q->rear +1)%MAXSIZE == q->front) return 0;
    strcpy(q->name[q->rear], name); //strcpy 使用方法：将字符串name复制到 队尾姓名 q->name[q->rear]中
    q->rear = (q->rear+1)%MAXSIZE;
    return 1; 
}

int deQueue(Queue *q, char *out) {
    if(isEmpty(q)) return 0;
    strcpy(out, q->name[q->front]); //strcpy 使用方法：将队头姓名 q->name[q->front]复制到 out 中
    q->front = (q->front+1)%MAXSIZE;
    return 1;
}

int getFront(Queue *q, char *out) {
    if(isEmpty(q)) return 0;
    strcpy(out, q->name[q->front]); 
    return 1;
}

int main(void)
{
    Queue manQ, womanQ;
    initQueue(&manQ);
    initQueue(&womanQ);

    int manCnt, womanCnt, round;
    char tmp[NAMELEN]; // 临时存储姓名

    printf("男队：\n");
    printf("输入跳舞人数：");
    scanf("%d", &manCnt);
    printf("输入跳舞者的姓名：");
    for(int i=0; i<manCnt; i++){ 
        scanf("%s", tmp);
        enQueue(&manQ, tmp);
    }

    printf("女队：\n");
    printf("输入跳舞人数：");
    scanf("%d", &womanCnt);
    printf("输入跳舞者的姓名：");
    for(int i=0; i<womanCnt; i++){
        scanf("%s", tmp);
        enQueue(&womanQ, tmp);
    }

    printf("请输入舞会的轮数：");
    scanf("%d", &round);

    for(int r=0; r<round; r++)
    {
        //本轮最多配对对数：取两队较小值
        int pairNum = getSize(&manQ) < getSize(&womanQ) ? getSize(&manQ): getSize(&womanQ);
        for(int i=0; i < pairNum; i++)
        {
            char m[NAMELEN],w[NAMELEN]; // 临时存储舞者姓名
            deQueue(&manQ, m); 
            deQueue(&womanQ, w);
            printf("配对的舞者：%s\t%s\n", m, w);
            //配对完放回各自队列尾部循环
            enQueue(&manQ, m);
            enQueue(&womanQ, w);
        }
        //输出第一个未配对出场者，从最长队列开始
        getFront( getSize(&manQ) < getSize(&womanQ)? &womanQ : &manQ , tmp); 
        printf("第一个出场的未配对舞者的姓名：%s\n", tmp);
    }
    return 0;
}
