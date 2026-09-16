/* 实验10 
现有4个城市，以及3条双向公路连接着某些城市，公路的分布及过路费如下图所示：
现在有一部电影在这4个城市上映，票价分别为：50,20,50,30。
一个人可以选择在本市看电影，也可以选择去其他城市看，如果前往其他城市观看，则不必把回程的路费计入。
分别计算这4个城市的人看电影最少需要多少钱？
 */


 /*# 解题思路

1. 建立带权无向图，存储城市之间路费；
2. 使用 Floyd 算法 / Dijkstra，求出任意两点之间最短路费 dist [u][v]；
- Floyd：多源最短路径，一次性算出所有点 ↔ 所有点最短距离
- Dijkstra：单源最短路径，给定一个起点，算出这个起点到其余所有点最短距离
    Floyd 算法核心思想
        动态规划思想：`dist[i][j]` 代表 i 到 j 的最短距离。
        引入中转点 k：`i → ... →k → ... →j`
        判断：i 直接走到 j， 和 i 先走到 k，再 k 走到 j，哪个更近
        dist[i][j] = min( dist[i][j], dist[i][k] + dist[k][j] )

    Dijkstra 算法核心思想
        1. dis[]数组：dis[v]代表起点 s 到 v 的当前最短距离；
        2. visited[]标记：该点是否已经确定最短路径；
        3. 每次：从没有确定最短路径的点中选出距离起点最近的点 u，标记 u 的最短路径确定；
        4. 拿 u 去松弛它所有邻接点 v：`dis[v] = min(dis[v], dis[u] + w[u][v])`

3. 对每个起点 u，遍历全部目标城市 v，计算`dist[u][v] + price[v]`；
4. 取最小值，就是 u 城市居民看电影最少花费。*/


#include <stdio.h>
#define N 4
#define INF 99999999

int main(){
    //dist
    int dist[N+1][N+1];
    //price
    int price[N+1] = {0,50,20,50,30};

    //初始化dist
    for(int i = 1; i <= N ; i++){
        for(int j = 1; j <= N ; j++){
            if(i == j){
                dist[i][j] = 0;
            }else{
                dist[i][j] = INF;
            }
        }
    }

    //输入边
    dist[1][3] = 9; dist[3][1] = 9;
    dist[2][3] = 5; dist[3][2] = 5;
    dist[2][4] = 8; dist[4][2] = 8;

    //floyd算法
    for(int k = 1 ; k <= N ; k++){
        for(int i = 1; i <= N ; i++){
            for(int j = 1; j <= N ; j++){
                dist[i][j] = dist[i][j] < dist[i][k] + dist[k][j] ? dist[i][j] : dist[i][k] + dist[k][j];
            }
        }
    }

    //计算最少花费
    for(int i = 1; i <= N ; i++){
        int minCost = INF;
        for(int j = 1; j <= N ; j++){
            int cost = dist[i][j] + price[j]; //计算从i到j的最少花费
            if(cost < minCost){
                minCost = cost;
            }
        }
        printf("城市%d看电影最少花费：%d\n",i,minCost);
    }
    return 0;    
}