/* 实验17 Dijkstra 最短路径（带路径输出）

有向图，n 个顶点 m 条边，求从源点 s 到所有点的最短路径长度，并输出到某个指定终点 t 的一条具体路径。边权非负。
数据范围：1 ≤ n ≤ 1000，1 ≤ m ≤ 10^5。要求堆优化 Dijkstra。
输入示例：
4 5 1 4
1 2 2
1 3 4
2 3 1
2 4 7
3 4 3

输出示例：
6
1 2 3 4
（到各点距离可先输出一行，再输出路径）*/

/*
 解题思路（写实验报告）
1. 存储结构：邻接表存储有向图，适合 m=1e5 大规模数据，不能使用邻接矩阵。
2. 手写小根堆，保存距离d，顶点u，每次取出当前距离最小顶点。
3. dist[]数组记录源点到各点最短距离，初始为 INF；源点距离置 0 并入堆。
4. 堆优化 Dijkstra 核心：弹出堆顶，如果堆内距离大于 dist [u]，代表该记录是过期失效，直接跳过。
5. 松弛操作：对 u 的每一条出边 u→v，若dist[v] > dist[u]+w，更新 dist [v]，设置pre[v]=u记录 v 的前驱，将新状态压入堆。
6. 路径还原：从终点 t 沿着 pre 数组反向回溯得到逆序路径，数组反转得到源点到终点正向路径。
7. 复杂度 O(mlog n)；前提条件：所有边权非负。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN        1005        //顶点最大数目
#define MAXM        100010      //边最大数目1e5
#define MAXHEAP     200010      //堆最大容量，m条边最多m次入堆
#define INF         0x3f3f3f3f  //无穷大

//邻接表 边节点
typedef struct Edge {
    int to;
    int w;
    struct Edge *next;
} Edge;

Edge* adj[MAXN];   // adj[u]：u的边链表头

//小根堆：存储 (距离d,顶点u)
typedef struct HeapNode {
    int d;
    int u;
} HeapNode;

HeapNode heap[MAXHEAP];
int heapSize;

//堆向上调整（小根堆）
void heapUp(int idx)
{
    while(idx > 1)
    {
        int fa = idx / 2;
        if(heap[fa].d <= heap[idx].d)
            break;
        //交换父子
        HeapNode t = heap[fa];
        heap[fa] = heap[idx];
        heap[idx] = t;
        idx = fa;
    }
}

// 堆向下调整（小根堆）
void heapDown(int idx)
{
    while(idx * 2 <= heapSize)
    {
        int lc = idx * 2;
        int rc = idx * 2 + 1;
        int small = lc;
        if(rc <= heapSize && heap[rc].d < heap[lc].d)
            small = rc;
        if(heap[idx].d <= heap[small].d)
            break;
        HeapNode t = heap[idx];
        heap[idx] = heap[small];
        heap[small] = t;
        idx = small;
    }
}

// 堆插入节点
void heapPush(int d, int u)
{
    heapSize++;
    heap[heapSize].d = d;
    heap[heapSize].u = u;
    heapUp(heapSize);
}

// 弹出堆顶最小元素
HeapNode heapPop(void)
{
    HeapNode top = heap[1];
    heap[1] = heap[heapSize];
    heapSize--;
    heapDown(1);
    return top;
}

int heapEmpty(void)
{
    return heapSize == 0;
}

// 添加一条有向边 u -> v 权w
void addEdge(int u, int v, int w)
{
    Edge* e = (Edge*)malloc(sizeof(Edge));
    e->to = v;
    e->w = w;
    e->next = adj[u]; 
    adj[u] = e;
}

int dist[MAXN];   // dist[i]源点到i最短距离
int pre[MAXN];    // pre[i]记录i的前驱节点，用于路径回溯

int main(void)
{
    int n, m, s, t;
    printf("输入 n m source target:\n");
    scanf("%d%d%d%d", &n, &m, &s, &t);

    //初始化邻接表
    memset(adj, 0, sizeof(adj));  //把 adj 整块内存全部字节置 0。
    for(int i = 1; i <= n; i++)
    {
        dist[i] = INF;
        pre[i] = -1;
    }
    heapSize = 0;

    printf("输入m条有向边 u v w:\n");
    for(int i = 0; i < m; i++)
    {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        addEdge(u, v, w);
    }

    // Dijkstra初始化
    dist[s] = 0;
    heapPush(0, s);
    printf("\n=====Dijkstra开始=====\n");

    while(!heapEmpty())
    {
        HeapNode cur = heapPop();
        int d = cur.d;
        int u = cur.u;
        printf("[堆弹出] u=%d,堆中距离d=%d  dist[u]=%d\n", u, d, dist[u]);

        // 关键：堆中存的是旧的失效记录，直接跳过
        if(d > dist[u])
        {
            printf("    >>失效旧记录，跳过\n");
            continue;
        }

        //遍历u所有邻接边
        for(Edge* p = adj[u]; p != NULL; p = p->next)
        {
            int v = p->to;
            int w = p->w;
            printf("    松弛边 %d->%d w=%d  dist[v]=%d , dist[u]+w=%d\n",
                    u, v, w, dist[v], dist[u]+w);

            if(dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;
                pre[v] = u;   //记录v的前驱是u
                heapPush(dist[v], v);
                printf("        >>>更新dist[%d]=%d , pre[%d]=%d ,入堆(%d,%d)\n",
                        v, dist[v], v, pre[v], dist[v], v);
            }
        }
    }

    // 输出源点到所有点距离
    printf("\n====各点最短距离====\n");
    for(int i = 1; i <= n; i++)
    {
        if(i > 1) printf(" ");
        if(dist[i] == INF)
            printf("dist[%d]=INF", i);
        else
            printf("dist[%d]=%d", i, dist[i]);
    }
    printf("\n");

    // 输出终点t距离
    printf("%d\n", dist[t]);

    // 回溯还原路径：从t反向走到s，存入path数组
    int path[MAXN];
    int pathCnt = 0;
    int cur = t;
    while(cur != -1)
    {
        path[pathCnt++] = cur;
        cur = pre[cur];
    }
    //反转path得到s到t正向路径
    for(int i = 0; i < pathCnt/2; i++)
    {
        int tmp = path[i];
        path[i] = path[pathCnt - 1 - i];
        path[pathCnt -1 -i] = tmp;
    }

    printf("====s到t路径====\n");
    for(int i = 0; i < pathCnt; i++)
    {
        if(i>0) printf(" ");
        printf("%d", path[i]);
    }
    printf("\n");

    //释放边内存
    for(int i=1;i<=n;i++){
        Edge *p=adj[i],*q;
        while(p){
            q=p->next;
            free(p);
            p=q;
        }
    }
    return 0;
}

