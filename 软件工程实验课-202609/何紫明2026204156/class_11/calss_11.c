/*实验11
现在有一个飞机制造商，它在n天里每天都要生产一架飞机。
如果第i架飞机生产后没有售出，则每滞留一天都要消耗c​的存储成本.
由于贸易战等影响，前m天都无法出售任何飞机，但是飞机制造商依然会生产飞机，
之后的每天中，都能恰好售出当前已经生产好的一架飞机，那么在第n+m天后,就能把所有的飞机全部出售。
现在请你计算出，在多种出售顺序中，飞机制造商存储成本之和最低的需要多少？

输入格式：
第一行：n  m
第二行：n 个数字，是每架飞机的单位存储成本 c [i]！不是全局统一 c！！

    1、n 天，每天造 1 架飞机，共 n 架；第 i 架飞机，滞留一天代价是c[i]。
    2、前 m 天完全不能卖飞机，照常生产。
    3、m 天之后，每天卖出 1 架；售卖日期：m+1, m+2 … m+n，每个售卖日卖恰好一架，可以自由选择出售顺序。
    4、每一架飞机 i，如果在s天卖出，总存储代价 = (s-i) * c[i]
    5、求：总存储代价总和的最小值。
    
大的(c[i])，分配给小的售卖时间；
小的(c[i])，分配给大的售卖时间。
注意：飞机不能在未生产之前卖出。
*/


#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int c;
    int day;
}Plane;

//qsort：按出厂day升序
int cmpByDay(const void *a, const void *b)
{
    Plane *pa = (Plane*)a;
    Plane *pb = (Plane*)b;
    return pa->day - pb->day;
}

//打印堆内容，调试用
void printHeap(int heap[], int heapSize)
{
    printf("[堆内c值]: ");
    for(int i=0;i<heapSize;i++){
        printf("%d ", heap[i]);
    }
    printf("\n");
}

int main(void)
{
    FILE *fp = fopen("input4.txt", "r");
    if(fp == NULL){
        printf("打开input.txt失败\n");
        return -1;
    }
    int n, m;
    fscanf(fp, "%d %d", &n, &m);
    Plane* planes = (Plane*)malloc(sizeof(Plane)*n);
    for(int i = 0; i < n; i++)
    {
        fscanf(fp, "%d", &planes[i].c);
        planes[i].day = i+1;
    }
    fclose(fp);

    qsort(planes, n, sizeof(Plane), cmpByDay); //按出厂day升序排序

    int* heap = (int*)malloc(sizeof(int)*n);
    int heapSize = 0;

    int sum_sc = 0;
    int sum_ic = 0;
    for(int i = 0; i < n; i++)
    {
        sum_ic += planes[i].c * planes[i].day;
    }

    int planePtr = 0;
    printf("=====开始模拟售卖=====\n");
    for(int idx = 0; idx < n; idx++)
    {
        int sell = m + 1 + idx;
        printf("\n---售卖时刻 sell = %d ---\n", sell);

        //把所有 出厂day <= sell 的飞机加入最大堆
        while(planePtr < n && planes[planePtr].day <= sell)
        {
            int insertC = planes[planePtr].c;
            int insertDay = planes[planePtr].day;
            printf("  将飞机(c=%d,day=%d)加入堆\n", insertC, insertDay);

            heap[heapSize++] = insertC;
            //向上调整 最大堆
            int pos = heapSize - 1; //新加入的元素位置
            while(pos > 0)
            {
                int father = (pos - 1)/2; //父节点位置
                if(heap[pos] > heap[father]) //如果新加入的元素大于父节点，交换
                {
                    int t = heap[pos];
                    heap[pos] = heap[father];
                    heap[father] = t;
                    pos = father; //继续向上调整
                }else break;
            }
            planePtr++; //指针指向下一个飞机
        }
        printHeap(heap, heapSize); //打印堆内容，调试用

        //取出堆顶（当前c最大）卖出
        int maxC = heap[0];
        sum_sc += maxC * sell; 

        // 找这个c对应的出厂day，用于打印单架代价（仅调试打印使用，不参与计算）
        int realDay = 0;
        for(int k=0;k<n;k++){
            //简单找，仅调试打印，不影响算法
            if(planes[k].c == maxC && planes[k].day <= sell){
                realDay = planes[k].day;
                break;
            }
        }
        int singleCost = maxC * (sell - realDay);
        printf("  >>卖出 c=%d,出厂day=%d，售卖时间%d，本架存储代价：%d\n",maxC,realDay,sell,singleCost);

        //删除堆顶
        heap[0] = heap[heapSize-1];
        heapSize--;
        int pos = 0; //堆顶位置，需要向下调整，维护最大堆性质，确保堆顶元素最大
        while(1) 
        {
            int left = pos*2 + 1;
            int right = pos*2 + 2;
            int maxIdx = pos;
            if(left < heapSize && heap[left] > heap[maxIdx]) maxIdx = left; //如果左子节点大于堆顶元素，更新最大Idx
            if(right < heapSize && heap[right] > heap[maxIdx]) maxIdx = right; //如果右子节点大于堆顶元素，更新最大Idx
            if(maxIdx == pos) break; //如果最大Idx没有改变，说明堆顶元素最大，无需调整
            int t = heap[pos]; //交换堆顶元素与最大Idx对应的元素
            heap[pos] = heap[maxIdx];
            heap[maxIdx] = t;
            pos = maxIdx; //继续向下调整
        }
    }

    int ans = sum_sc - sum_ic;
    printf("\n================\n");
    printf("最小存储代价总和为：%d\n", ans);

    free(planes);
    free(heap);
    return 0;
}
