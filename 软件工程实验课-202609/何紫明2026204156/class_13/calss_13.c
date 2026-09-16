/* 实验13
最长上升子序列（LIS）
给定一个长度为 n 的整数序列，求其最长严格上升子序列的长度，并输出任意一个这样的子序列。
数据范围：1 ≤ n ≤ 1000，|a_i| ≤ 10^9。
要求：O(n²) DP 即可，进阶可尝试 O(n log n)。

输入示例：
8
10 9 2 5 3 7 101 18

输出示例：
4
2 3 7 101
（或其它合法 LIS）
*/

/* 解题思路
 O(n^2) DP 解题思路

1. 状态定义
dp[i]：以第 i 个元素结尾的最长严格上升子序列的长度（下标从 0 开始）。
pre[i]：记录前驱下标，用来回溯还原完整 LIS 子序列。pre[i]表示 LIS 中a[i]前面那个元素的下标；1 代表没有前驱。
2. 状态转移
    对于每一个位置 i：
    1. 初始：dp[i] = 1，子序列至少包含自己本身；pre[i] = -1。
    2. 遍历 j 从 0 到 i-1：
    如果 a[j] < a[i]（严格上升），并且 dp[j]+1 > dp[i]：
    dp[i] = dp[j]+1
    更新 pre[i] = j，记录前驱。
3. 求最长长度
遍历 dp 数组，找到最大值maxLen，同时记录最大值对应的下标pos（LIS 的末尾元素下标）。
4. 回溯还原子序列
从pos出发，沿着pre[]向前回溯，把下标依次存入数组；回溯得到的是逆序，最后反转数组得到最终 LIS。
*/


#include <stdio.h>
#define MAXN 1005

int a[MAXN];     //原始输入数组
int dp[MAXN];    //dp[i]:以i结尾的最长严格上升子序列长度
int pre[MAXN];   //pre[i]：i在LIS中的前驱下标，-1表示无前驱
int res[MAXN];   //保存回溯得到的LIS结果

int main(void)
{
    int n;
    printf("请输入长度n：");
    //读取输入
    scanf("%d", &n);
    printf("请输n个整数：");
    for(int i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }

    //初始化dp与pre数组
    for(int i = 0; i < n; i++)
    {
        dp[i] = 1;    //子序列至少包含自己
        pre[i] = -1;  //初始没有前驱
    }

    //DP状态转移 O(n^2)
    for(int i = 0; i < n; i++)
    {
        //遍历i前面所有j
        for(int j = 0; j < i; j++)
        {
            //严格上升：a[j] < a[i]
            if(a[j] < a[i])
            {
                //如果接在j后面，可以得到更长子序列
                if(dp[j] + 1 > dp[i])
                {
                    dp[i] = dp[j] + 1;
                    pre[i] = j; //记录前驱下标
                }
            }
        }
    }

    //寻找dp数组最大值，以及对应末尾元素下标pos
    int maxLen = 0;
    int pos = 0;
    for(int i = 0; i < n; i++)
    {
        if(dp[i] > maxLen)
        {
            maxLen = dp[i];
            pos = i;
        }
    }

    //回溯：从pos向前，沿着pre数组找回整个LIS，得到逆序结果
    int cnt = 0;
    int cur = pos;
    while(cur != -1)
    {
        res[cnt++] = a[cur];
        cur = pre[cur];
    }

    //输出最长长度
    printf("最长上升子序列（LIS）长度为：%d\n", maxLen);
    printf("最长上升子序列（LIS）为：");

    //回溯得到的res是逆序，反向打印
    for(int i = cnt - 1; i >= 0; i--)
    {
        if(i != cnt - 1) printf(" ");
        printf("%d", res[i]);
    }
    printf("\n");

    return 0;
}
