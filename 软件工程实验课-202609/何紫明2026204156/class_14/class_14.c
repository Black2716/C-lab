/*实验14 0-1 背包
有 n 件物品，第 i 件重量 w[i]、价值 v[i]，背包容量 W。
求能装下的最大价值，并输出选择的物品编号（1-based，任意一组最优解）。
数据范围：1 ≤ n ≤ 100，1 ≤ W ≤ 1000，1 ≤ w[i],v[i] ≤ 100。

输入示例：
4 5
1 2
2 3
3 4
4 5

输出示例：
7
1 4
（或其它最优组合）

*/

/*解题思路（二维 DP + 回溯输出方案）

状态定义
dp[i][j]：前 i 件物品，背包容量为 j 时，可以获得的最大价值。
i：考虑前 i 个物品（0 <= i <= n）
j：背包已用容量（0 <= j <= W）

状态转移方程
1. 不选第 i 件物品：dp[i][j] = dp[i-1][j]
2. 选第 i 件物品（前提 j >= w[i]）：dp[i][j] = dp[i-1][j-w[i]] + v[i]
初始化：dp[0][j]=0，0 件物品无论容量多少价值都是 0。

回溯找选中物品
从 i=n，j=W 倒推：
如果 dp[i][j] > dp[i-1][j]：代表选了第 i 件物品，记录该物品编号；然后 j = j - w[i]；i=i-1。
否则：没有选第 i 件，直接 i=i-1。
回溯得到的物品编号是逆序，最后反向输出。
范围：n≤100，W≤1000；二维数组大小 101 × 1001，内存完全够用。
*/



#include <stdio.h>

#define MAXN 105   //物品最大数量 n<=100
#define MAXW 1005  //背包最大容量 W<=1000

int w[MAXN];   //w[i] 第i件物品重量，i从1开始
int v[MAXN];   //v[i] 第i件物品价值
//dp[i][j]：前i件物品，背包容量j的最大价值
int dp[MAXN][MAXW];
int select[MAXN]; //保存回溯出来选中的物品编号
int cnt;          //选中物品的个数

int main(void)
{
    int n, W;
    //读入物品数量n，背包容量W
    printf("请输入物品个数 n 和背包容量 w ：");
    scanf("%d%d", &n, &W);

    //读入每件物品重量、价值，物品下标从1开始，方便处理
    printf("请输入每件物品重量和价值：\n");
    for(int i = 1; i <= n; i++)
    {
        scanf("%d%d", &w[i], &v[i]);
    }

    //dp数组初始化：0件物品，任何容量价值都是0
    for(int j = 0; j <= W; j++)
    {
        dp[0][j] = 0;
    }

    //DP填表
    for(int i = 1; i <= n; i++)   //遍历前i个物品
    {
        for(int j = 0; j <= W; j++) //遍历背包容量
        {
            //情况1：不选第i件物品
            dp[i][j] = dp[i-1][j];

            //情况2：可以选第i件物品，背包容量足够放下w[i]
            if(j >= w[i])
            {
                //选和不选取价值更大的
                if(dp[i-1][j - w[i]] + v[i] > dp[i][j])
                {
                    dp[i][j] = dp[i-1][j - w[i]] + v[i];
                }
            }
        }
    }

    // =========回溯，找出哪些物品被选中=========
    cnt = 0;
    int cur_i = n;
    int cur_j = W;
    while(cur_i >= 1)
    {
        //dp[cur_i][cur_j] > dp[cur_i-1][cur_j] 说明选了cur_i号物品
        if(dp[cur_i][cur_j] > dp[cur_i-1][cur_j])
        {
            select[cnt++] = cur_i;   //记录物品编号（1‑based）
            cur_j = cur_j - w[cur_i];//背包容量减去该物品重量
        }
        cur_i = cur_i - 1; //处理上一件物品
    }

    //回溯得到的select数组是逆序，反向打印
    printf("选择的一组物品编号为：");

    for(int k = cnt - 1; k >= 0; k--)
    {
        if(k != cnt-1)
            printf(" ");
        printf("%d", select[k]);
    }
    printf("\n");

    //输出最大价值
    int max_val = dp[n][W];
    printf("%d\n", max_val);


    return 0;
}
