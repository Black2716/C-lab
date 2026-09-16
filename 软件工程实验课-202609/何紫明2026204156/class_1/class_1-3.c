//实验1
//练习3：用函数编程计算并输出如图所示的杨辉三角，行数由用户输入。

#include <stdio.h>
void print_pascals_triangle(int rows) {
    int triangle[rows][rows]; // 创建一个二维数组来存储杨辉三角的值
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= i; j++) {
            if (j == 0 || j == i) {
                triangle[i][j] = 1; // 每行的第一个和最后一个元素为1
            } else {
                triangle[i][j] = triangle[i - 1][j - 1] + triangle[i - 1][j]; // 其他元素为上一行的两个相邻元素
            }
        }
    }
    // 输出杨辉三角
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= i; j++) {
            printf("%d ", triangle[i][j]); // 输出当前行的元素
        }
        printf("\n"); // 换行输出下一行
    }
} 

int main() {
    int rows;
    printf("请输入杨辉三角的行数: ");
    scanf("%d", &rows); // 用户输入行数
    print_pascals_triangle(rows); // 调用函数打印杨辉三角
    return 0;
}