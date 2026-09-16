/*实验16 全排列生成 + 剪枝

给定 n 个互不相同的整数，生成它们的所有全排列，并按字典序从小到大输出。
额外要求：如果某个排列中存在任意两个相邻数字的差的绝对值为 1，则该排列直接剪枝，不予输出。
数据范围：1 ≤ n ≤ 9。

输入示例：
3
1 2 4

输出示例：
1 4 2
2 4 1
*/

/*
解题思路（回溯 DFS + 剪枝 + 字典序）
1. 全排列回溯原理
    用递归 DFS 构建排列：
    维护path[]：当前正在构建的排列；
    维护used[]标记数组：标记原数组中该数字是否已经被选入 path，避免重复选取。
    递归出口：path长度等于 n，代表生成一个完整排列，判断是否合法，合法则打印。
2. 字典序输出要求
    想要得到字典序从小到大，必须先把原始输入数组升序排序，递归时按数组顺序依次尝试选取元素。
    递归依次尝试 a [0],a [1],a [2]…，生成的排列自然就是字典序。
3. 剪枝条件（核心）
题目：排列中任意两个相邻数字差绝对值等于 1 就舍弃。
提前剪枝，不要等到整组排列生成完才判断
4. 递归步骤：
① 将输入数组升序排序，保证字典序；
② dfs (path,used):
  如果 path 长度 ==n：输出 path，return；
  遍历每一个下标 i：
   如果 used [i]==true，已经选过，跳过；
   如果 path 不为空，且abs(path.back() - a[i]) ==1 →剪枝，continue，不进入下层递归；
   标记 used [i]=true；a [i] 加入 path；
   递归 dfs；
   回溯：path 弹出末尾；used [i]=false；
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 10

int n;
int a[MAXN];        //原始输入数组
int used[MAXN];     //标记数组，used[i]=1代表a[i]已经被选
int path[MAXN];     //当前递归正在构建的排列
int pathLen;        //path当前有效长度

//qsort比较函数，升序排序，保证输出字典序
int cmp(const void *x, const void *y)
{
    int p1 = *(int*)x;
    int p2 = *(int*)y;
    return p1 - p2;
}

// dfs回溯生成全排列，带剪枝
void dfs()
{
    int i;
    //递归出口：path长度等于n，得到完整合法排列
    if(pathLen == n)
    {
        printf("[完整合法排列]  ");
        for(i = 0; i < n; i++)
        {
            if(i>0) printf(" ");
            printf("%d", path[i]);
        }
        printf("\n");
        return;
    }

    //依次尝试选取每一个未被使用的数字
    for(i = 0; i < n; i++)
    {
        if(used[i] == 1)
        {
            printf("    dfs层pathLen=%d: a[%d]=%d 已被使用，跳过\n",pathLen,i,a[i]);
            continue;
        }

        // =========剪枝判断=========
        //path不为空：准备把a[i]接在path末尾；相邻差绝对值等于1，直接剪枝，不走下一层
        if(pathLen > 0)
        {
            int last = path[pathLen - 1];
            if( abs(last - a[i]) == 1 )
            {
                printf("    dfs层pathLen=%d:尝试选a[%d]=%d, last=%d, |%d‑%d|=1 【剪枝！放弃该分支】\n",
                        pathLen,i,a[i],last,last,a[i]);
                continue; //直接跳过，不递归
            }
        }

        //走到这里，可以选择a[i]
        printf("    dfs层pathLen=%d:选中a[%d]=%d，进入下层递归\n",pathLen,i,a[i]);
        used[i] = 1;
        path[pathLen++] = a[i];

        dfs(); //递归向下

        //回溯，恢复现场
        pathLen--;
        used[i] = 0;
        printf("    dfs回溯返回 pathLen=%d，撤销选择a[%d]=%d\n",pathLen,i,a[i]);
    }
}

int main(void)
{
    printf("请输入n：");
    scanf("%d",&n);
    printf("请输入%d个互不相同整数：\n",n);
    for(int i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
    }

    //先升序排序，保证dfs生成字典序全排列
    qsort(a, n, sizeof(int), cmp);
    printf("\n排序之后数组：");
    for(int i=0;i<n;i++) printf("%d ",a[i]);
    printf("\n====开始DFS回溯生成带剪枝全排列====\n\n");

    //初始化
    pathLen = 0;
    for(int i=0;i<n;i++) used[i]=0;

    dfs();

    return 0;
}
