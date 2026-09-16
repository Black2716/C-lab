/*实验7
    逆波兰表达式求值。逆波兰表达式，即后缀表示法，例如：
    中缀表示的 8+(1+2)*4 的后缀表示为 8 1 2 + 4 * +
    用栈计算逆波兰表达式的基本思路是：按顺序遍历整个表达式，
    若遇到操作数 (假设都是二元运算符) 则入栈；若遇到操作符 (+ - * /)
    连续弹出两个操作数并执行相应的运算，然后将其运算结果入栈。
    重复以上过程，直到表达式遍历完，栈内只剩下一个操作数时，那
    就是最终的运算结果，弹出打印即可。
    输入用例至少 5 个操作符，每个操作符之间用空格隔开。
    输出用例为一个整数，即最终的运算结果。
示例：
输入：8 1 2 + 4 * +
中缀：8+(1+2)*4
运算结果：20
示例：
输入：4 15 5 / +
中缀：4+(15/5)
运算结果：7**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ===== 字符栈：用于中缀转后缀，保存运算符、括号 char
typedef struct CharStack {
    int top;
    char data[100];
} CharStack;

CharStack initCharStack() {
    CharStack s;
    s.top = -1;
    return s;
}

void charPush(CharStack* s, char ch) {
    s->top++;
    s->data[s->top] = ch;
}

char charPop(CharStack* s) {
    return s->data[s->top--];
}

char charPeek(CharStack* s) { 
    return s->data[s->top];
}

// ===== 数字栈：后缀表达式求值，保存运算整数 int
typedef struct NumStack {
    int top;
    int data[100];
} NumStack;

NumStack initNumStack() {
    NumStack s;
    s.top = -1;
    return s;
}

void numPush(NumStack* s, int val) {
    s->top++;
    s->data[s->top] = val;
}

int numPop(NumStack* s) {
    return s->data[s->top--];
}

// 获取运算符优先级
int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0; // ( 返回0
}

// 中缀表达式转后缀逆波兰表达式
void InfixToPostfix(char* infix, char* postfix) {
    CharStack s = initCharStack();
    int i = 0, j = 0;
    while (infix[i] != '\0') {
        if (isspace(infix[i])) { //跳过所有空格、tab
            i++;
        } else if (isdigit(infix[i])) {
            //处理多位数字，直接复制到 postfix
            while (isdigit(infix[i])) {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' '; //数字后面加空格分隔
        } else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/') {
            //运算符：弹出优先级>=当前的栈顶运算符
            while (s.top != -1 && charPeek(&s) != '('
                   && getPrecedence(charPeek(&s)) >= getPrecedence(infix[i])) {
                postfix[j++] = charPop(&s);
                postfix[j++] = ' ';
            }
            charPush(&s, infix[i++]);
        } else if (infix[i] == '(') {
            charPush(&s, infix[i++]);
        } else if (infix[i] == ')') {
            //弹出直到左括号
            while (s.top != -1 && charPeek(&s) != '(') {
                postfix[j++] = charPop(&s);
                postfix[j++] = ' ';
            }
            charPop(&s); //丢弃 '('
            i++;
        } else {
            //非法字符，跳过
            i++;
        }
    }
    //栈中剩余运算符输出
    while (s.top != -1) {
        postfix[j++] = charPop(&s);
        postfix[j++] = ' ';
    }
    if(j>0) j--; //删掉末尾多余空格
    postfix[j] = '\0';
}

// 逆波兰后缀表达式求值
int evaluatePostfix(char* postfix) {
    NumStack s = initNumStack();
    int i = 0;
    while (postfix[i] != '\0') {
        if (isspace(postfix[i])) {
            i++;
        } else if (isdigit(postfix[i])) {
            //解析多位数字
            int num = 0;
            while (isdigit(postfix[i])) {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            numPush(&s, num);
        } else {
            //运算符
            char op = postfix[i];
            i++;
            //检查栈是否有两个操作数
            if(s.top < 1){
                printf("表达式错误，操作数不足!\n");
                return 0;
            }
            int b = numPop(&s);
            int a = numPop(&s);
            int res;
            switch(op){
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/':
                    if(b == 0){
                        printf("运行错误：除数为0！\n");
                        return 0;
                    }
                    res = a / b;
                    break;
                default: res = 0;
            }
            numPush(&s, res);
        }
    }
    if(s.top != 0){
        printf("后缀表达式格式错误！栈剩余元素异常\n");
        return 0;
    }
    return numPop(&s);
}

int main() {
    char infix[100];
    char postfix[100];
    printf("请输入中缀表示法表达式：");
    fgets(infix, sizeof(infix), stdin);  //读入中缀表达式，fgets使用方法：fgets(缓冲区, 缓冲区大小, 输入流)
    //去掉fgets读入的换行符
    infix[strcspn(infix, "\n")] = '\0'; // strcspn() 函数用于查找字符串中不包含指定字符集的最长前缀的长度

    InfixToPostfix(infix, postfix);
    printf("转换为后缀表示法表达式：%s\n", postfix);
    int ans = evaluatePostfix(postfix);
    printf("逆波兰表达式运算结果：%d\n", ans);
    return 0;
}
