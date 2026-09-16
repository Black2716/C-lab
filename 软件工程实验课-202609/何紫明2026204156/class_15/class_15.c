/*实验15 区间调度
有 n 个活动，第 i 个活动开始时间 s[i]、结束时间 e[i]。
选择尽可能多的活动，使它们两两不重叠（结束时间 = 开始时间视为不重叠）。
输出最多活动数及任意一组方案的编号（1-based）。
数据范围：1 ≤ n ≤ 10^5，0 ≤ s_i < e[i] ≤ 10^9。要求 O(n log n)。
输入示例：
4
1 3
2 4
3 5
0 6

输出示例：
2
1 3
*/

#include <stdio.h>
#include <stdlib.h>

#define MAXN 10010

//活动结构体
typedef struct {
    long long s; //开始时间
    long long e; //结束时间
    int id; //原始编号，1-based
}Activity;

Activity act[MAXN];
int res_id[MAXN]; //保存选中的活动编号

//qsort比较函数：按照时间e从小到大排序
int cmpActivity(const void *a, const void *b){
    Activity *p1 = (Activity *)a;
    Activity *p2 = (Activity *)b;
    if (p1->e < p2->e) return -1;
    else if(p1->e > p2->e) return 1;
    else return 0;
}

int main(void)
{
    int n;
    printf("请输入活动数量n：");
    scanf("%d", &n);

    printf("请依次输入每个活动的开始时间 结束时间：\n");
    for(int i = 0; i < n; i++)
    {
        scanf("%lld %lld", &act[i].s, &act[i].e);
        act[i].id = i + 1;  //原始编号从1开始
    }

    printf("\n====原始活动列表====\n");
    for(int i = 0; i < n; i++)
    {
        printf("id=%d  s=%lld e=%lld\n", act[i].id, act[i].s, act[i].e);
    }

    //qsort快速排序，按结束时间升序 O(nlogn)
    qsort(act, n, sizeof(Activity), cmpActivity);

    printf("\n====按结束时间升序排序之后====\n");
    for(int i = 0; i < n; i++)
    {
        printf("id=%d  s=%lld e=%lld\n", act[i].id, act[i].s, act[i].e);
    }

    int select_cnt = 0;
    //last_end记录上一个被选中活动的结束时间，初始极小值
    long long last_end = -1;

    //贪心遍历所有排好序的活动
    for(int i = 0; i < n; i++)
    {
        printf("【遍历】当前活动id=%d s=%lld e=%lld，上一个结束last_end=%lld\n",
                act[i].id, act[i].s, act[i].e, last_end);

        //当前活动开始 >=上一个结束，可以选；等于也允许
        if(act[i].s >= last_end)
        {
            printf("    >>满足条件，选中活动 %d\n", act[i].id);
            res_id[select_cnt++] = act[i].id;
            last_end = act[i].e;  //更新上一个结束时间
        }
        else
        {
            printf("    >>时间冲突，跳过该活动\n");
        }
    }

    //输出结果
    printf("\n====最终结果====\n");
    printf("最多活动数：%d\n", select_cnt);
    printf("活动编号：");

    for(int k = 0; k < select_cnt; k++)
    {
        if(k > 0) printf(" ");
        printf("%d", res_id[k]);
    }
    printf("\n");

    return 0;
}
