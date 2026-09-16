#include <stdio.h>
#include <stdlib.h>
#define MAXVEX 100

//边表节点
typedef struct EdgeNode {
    int adjvex; //邻接顶点下标
    struct EdgeNode *next;
} EdgeNode;

typedef struct VertexNode {
    int data;
    EdgeNode *firstOut;   //正邻接链表：出边 i -> x
    EdgeNode *firstIn;    //逆邻接链表：入边 x -> i
} VertexNode;

typedef struct {
    VertexNode adjList[MAXVEX];
    int numVertexes;
    int numEdges;
} Graph;

//BFS队列
typedef struct {
    int data[MAXVEX];
    int front;
    int rear;
} Queue;

void initQueue(Queue *q) {
    q->front = 0;
    q->rear = 0;
}

void enQueue(Queue *q, int val) {
    q->data[q->rear++] = val;
}

int deQueue(Queue *q) {
    return q->data[q->front++];
}

int isEmpty(Queue *q) {
    return q->front == q->rear;
}

//创建图，同时构建正邻接、逆邻接链表
void createGraph(Graph *g) {
    printf("请输入顶点个数：");
    scanf("%d", &g->numVertexes);
    printf("请输入边数：");
    scanf("%d", &g->numEdges);

    // 初始化顶点表
    // 输入顶点信息，初始化边表为空
    printf ("请输入%d个顶点（用空格分隔）：", g->numVertexes);
    for(int i = 0; i < g->numVertexes; i++){
        scanf("%d", &g->adjList[i].data);
        g->adjList[i].firstOut = NULL;
        g->adjList[i].firstIn = NULL;
    }

    int isUndirected;
    printf("是否无向图(1是，0否):");
    scanf("%d",&isUndirected);

    printf("输入每条边 i j (下标从0开始)\n");
    for(int k=0;k<g->numEdges;k++){
        int i,j;
        scanf("%d%d",&i,&j);
        if(i<0||i>=g->numVertexes||j<0||j>=g->numVertexes){
            printf("下标非法，跳过\n");
            continue;
        }

        //=====正邻接链表 i -> j，i的出边=====
        EdgeNode *eOut = (EdgeNode*)malloc(sizeof(EdgeNode));
        eOut->adjvex = j;
        eOut->next = g->adjList[i].firstOut;
        g->adjList[i].firstOut = eOut;

        //=====逆邻接链表 j <- i，j的入边=====
        EdgeNode *eIn = (EdgeNode*)malloc(sizeof(EdgeNode));
        eIn->adjvex = i;
        eIn->next = g->adjList[j].firstIn;
        g->adjList[j].firstIn = eIn;

        //无向图：双向都要再补一组
        if(isUndirected == 1){
            // j -> i 出边
            EdgeNode *eOut2 = (EdgeNode*)malloc(sizeof(EdgeNode));
            eOut2->adjvex = i;
            eOut2->next = g->adjList[j].firstOut;
            g->adjList[j].firstOut = eOut2;

            // i <- j 的入边
            EdgeNode *eIn2 = (EdgeNode*)malloc(sizeof(EdgeNode));
            eIn2->adjvex = j;
            eIn2->next = g->adjList[i].firstIn;
            g->adjList[i].firstIn = eIn2;
        }
    }
}

//打印正邻接表（出边）
void printOutAdj(Graph *g){
    printf("\n====正邻接链表(出边表)====\n");
    for(int i=0;i<g->numVertexes;i++){
        printf("v%d(%d): ",i,g->adjList[i].data);
        EdgeNode *p = g->adjList[i].firstOut;
        while(p){
            printf("-> v%d(%d) ",p->adjvex , g->adjList[p->adjvex].data);
            p=p->next;
        }
        printf("\n");
    }
}

//打印逆邻接表（入边表）
void printInAdj(Graph *g){
    printf("\n====逆邻接链表(入边表)====\n");
    for(int i=0;i<g->numVertexes;i++){
        printf("v%d(%d): ",i,g->adjList[i].data);
        EdgeNode *p = g->adjList[i].firstIn;
        while(p){
            printf("-> v%d(%d) ",p->adjvex , g->adjList[p->adjvex].data);
            p=p->next;
        }
        printf("\n");
    }
}

//DFS，使用正邻接（出边）
void DFS(Graph *g, int v, int visited[]){
    visited[v]=1;
    printf("%d ",g->adjList[v].data);
    EdgeNode *p = g->adjList[v].firstOut;
    while(p){
        if(!visited[p->adjvex]){
            DFS(g,p->adjvex,visited);
        }
        p=p->next;
    }
}

void DFSTraverse(Graph *g){
    int visited[MAXVEX]={0};
    printf("DFS遍历：");
    for(int i=0;i<g->numVertexes;i++){
        if(!visited[i]) DFS(g,i,visited);
    }
    printf("\n");
}

//BFS，使用正邻接（出边）
void BFSTraverse(Graph *g){
    int visited[MAXVEX]={0};
    Queue q;
    initQueue(&q);
    printf("BFS遍历：");
    for(int i=0;i<g->numVertexes;i++){
        if(!visited[i]){
            visited[i]=1;
            printf("%d ",g->adjList[i].data);
            enQueue(&q,i);
            while(!isEmpty(&q)){
                int v = deQueue(&q);
                EdgeNode *p = g->adjList[v].firstOut;
                while(p){
                    if(!visited[p->adjvex]){
                        visited[p->adjvex]=1;
                        printf("%d ",g->adjList[p->adjvex].data);
                        enQueue(&q,p->adjvex);
                    }
                    p=p->next;
                }
            }
        }
    }
    printf("\n");
}

//释放正、逆邻接链表所有节点
void freeGraph(Graph *g){
    for(int i=0;i<g->numVertexes;i++){
        EdgeNode *p = g->adjList[i].firstOut;
        while(p){
            EdgeNode *t=p; p=p->next; free(t);
        }
        p = g->adjList[i].firstIn;
        while(p){
            EdgeNode *t=p; p=p->next; free(t);
        }
    }
}

int main(void){
    Graph g;
    createGraph(&g);
    printOutAdj(&g);
    printInAdj(&g);
    DFSTraverse(&g);
    BFSTraverse(&g);
    freeGraph(&g);
    return 0;
}
