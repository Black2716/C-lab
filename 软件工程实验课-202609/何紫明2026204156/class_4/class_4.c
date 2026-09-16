/* 实验4
给出一个有序的整数数组a 和有序的整数数组b，请将数组b合并到数组a中，变成一个有序的升序数组。
数据范围： 0<= n,m < 100, |ai| <= 100, |bi| <= 100
示例1:
输入：[4,5,6],[1,2,3]
输出：[1,2,3,4,5,6]
示例2:
输入：[1,2,3],[2,5,6]
输出：[1,2,2,3,5,6]
*/

#include <stdio.h>
void merge(int a[], int n, int b[], int m, int result[]){
    int i = 0, j = 0, k = 0;
    while (i < n && j < m) {
        if (a[i] < b[j]) {
            result[k++] = a[i++];
        } else {
            result[k++] = b[j++];
        }
    }
    while (i < n) {
        result[k++] = a[i++];
    }
    while (j < m) {
        result[k++] = b[j++];
    }
}

// 测试函数
int main() {
    int a[100], b[100], result[200];
    int n, m;
    printf("请输入数组a的长度：");
    scanf("%d", &n);
    printf("请输入数组a的元素：");
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    printf("请输入数组b的长度：");
    scanf("%d", &m);
    printf("请输入数组b的元素：");
    for (int i = 0; i < m; i++) {
        scanf("%d", &b[i]);
    }
    merge(a, n, b, m, result);
    printf("合并后的数组为：");
    for (int i = 0; i < n + m; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    return 0;
}