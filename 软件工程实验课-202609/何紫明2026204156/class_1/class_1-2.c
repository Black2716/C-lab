/* 实验1
练习2：挑战类型表示的极限——大数存储问题。编程计算并输出1~40之间的所有数的阶乘。 */
/* 类型	存储大小	值范围
char	1 字节	-128 到 127 或 0 到 255
unsigned char	1 字节	0 到 255
signed char	1 字节	-128 到 127
int	2 或 4 字节	-32,768 到 32,767 或 -2,147,483,648 到 2,147,483,647
unsigned int	2 或 4 字节	0 到 65,535 或 0 到 4,294,967,295
short	2 字节	-32,768 到 32,767
unsigned short	2 字节	0 到 65,535
long	4 字节	-2,147,483,648 到 2,147,483,647
unsigned long	4 字节	0 到 4,294,967,295 */


//原理：用数组每一位存一位十进制数字，模拟手工乘法，解决超出内置类型的大数存储。
#include <stdio.h>
#define MAX 100
int main() {
    int n, i, j, carry, result[MAX];
    result[0] = 1; // 阶乘的初始值为1
    int result_len = 1; // 阶乘结果的位数
    
    for ( n = 0; n < 40; n++)
    {
        carry = 0; // 每次计算前清零进位
        for ( i = 0; i < result_len; i++) {
            int temp = result[i] * (n + 1) + carry; // 计算当前位的乘积
            result[i] = temp % 10 ; // 当前位的结果
            carry = temp / 10; // 更新进位
        }
        // 处理进位
        while (carry != 0) {
            result[result_len] = carry % 10; // 将进位的每一位存入结果数组
            carry /= 10;
            result_len++; // 阶乘结果的位数增加
        }
        // 输出当前阶乘结果
        printf("%d! = " , n + 1);
        for ( j = result_len - 1; j >= 0; j--) {
            printf("%d", result[j]); // 从高位到低位输出结果
        }
        printf("\n");
    }
    return 0;
}