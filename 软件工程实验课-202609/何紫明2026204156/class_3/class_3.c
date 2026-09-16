/*  实验3
编写一个函数来验证输入的字符串是否是有效的 IPv4 / IPv6 / MAC 地址,并指出具体哪位数存在问题。
IPv4 地址由十进制数和点来表示，每个地址包含4个十进制数，其范围为 0 - 255， 用(“.”)分割。
比如：172.16.254.1；同时，IPv4 地址内的数不会以 0 开头。比如，地址 172.16.254.01 是不合法的。
IPv6 地址由8组16进制的数字来表示，每组表示 16 比特。这些组数字通过 (":")分割。
比如,  2001:0db8:85a3:0000:0000:8a2e:0370:7334 是一个有效的地址。
而且，我们可以加入一些以 0 开头的数字，字母可以使用大写，也可以是小写。
所以，2001:db8:85a3:0:0:8A2E:0370:7334 也是一个有效的 IPv6 address地址 (即，忽略 0 开头，忽略大小写)。
然而，我们不能因为某个组的值为0，而使用一个空的组，以至于出现 (::) 的情况。
比如， 2001:0db8:85a3::8A2E:0370:7334 是无效的 IPv6 地址。
同时，在 IPv6 地址中，多余的 0 也是不被允许的。比如， 02001:0db8:85a3:0000:0000:8a2e:0370:7334 是无效的。
MAC：6 组十六进制，`-`或者`:`分隔，每组 2 位十六进制，例如`00‑1A‑2B‑3C‑4D‑5E`、`00:1a:2b:3c:4d:5e`
*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/*
  辅助函数：手动分割地址，逐个字符扫描，检测连续分隔符，捕获空段
  参数说明
    addr:原始地址字符串
    sep:分隔符（IPv4是'.'，IPv6是':'，MAC支持':'和'-'）
    segments:输出，存放各段字符串
    maxSeg:最大段数量
    errSeg:输出出错段下标
  返回：段数量；如果出现空段/非法字符返回负数
*/
int splitAddress(char *addr, char sep, char segments[][50], int maxSeg, int *errSeg)
{
    int segCnt = 0;
    int pos = 0;
    *errSeg = -1;
    int len = strlen(addr);
    char buf[50];
    int bufIdx = 0;
    for(int i=0; i<=len; i++)
    {
        char ch = addr[i];
        if(ch == sep || ch == '\0')
        {
            buf[bufIdx] = '\0';
            // 遇到分隔符：保存当前段
            if (segCnt >= maxSeg)
            {
                *errSeg = segCnt;
                return -1;
            }
            strcpy(segments[segCnt], buf);
            // 空段检测：bufIdx == 0，连续分隔符！
            if(bufIdx == 0)
            {
                *errSeg = segCnt;
                return -1;
            }
            segCnt++;
            bufIdx = 0;
        }
        else
        {
            buf[bufIdx++] = ch;
            if(bufIdx >= 49)
            {
                *errSeg = segCnt;
                return -1;
            }
        }
    }
    return segCnt;
}
/*检查IPv4地址是否合法 IPv4 规则
  4 段十进制，`.`分割；每段范围`0‑255`
  不能前导 0：`172.16.254.01`非法；但单独`0`合法
  段数必须正好 4 段，不能空段，不能有非数字字符
  返回值：1合法 0非法
  errSeg ：输出参数，保存出错段下标（从0开始），-1表示没有错误
*/
// IPv4校验
int isValidIPv4(char *ip, int *errSeg) {
    char seg[10][50];
    int cnt = splitAddress(ip, '.', seg, 10, errSeg);
    if(cnt !=4)
    {
        if(*errSeg == -1) *errSeg = cnt;
        return 0;
    }
    for(int i=0; i<4; i++)
    {
        char *token = seg[i];
        // 检查全数字
        for(int j=0;j<strlen(token);j++)
        {
            if(!isdigit(token[j]))
            {
                *errSeg = i;
                return 0;
            }
        }
        //前导0：长度>1，首字符0
        if(strlen(token)>1 && token[0] == '0')
        {
            *errSeg = i;
            return 0;
        }
        int num = atoi(token);
        if(num <0 || num>255)
        {
            *errSeg = i;
            return 0;
        }
    }
    return 1;
}
/*检查IPv6地址是否合法 IPv6 规则
  8 段十六进制，`:`分割；每段范围`0‑FFFF`
  可以前导 0：`2001:0db8:85a3:0000:0000:8a2e:0370:7334`合法
  不允许空段，不能有非十六进制字符
*/
// IPv6校验
int isValidIPv6(char *ip, int *errSeg) {
    char seg[10][50];
    int cnt = splitAddress(ip, ':', seg, 10, errSeg);
    if(cnt !=8)
    {
        if(*errSeg == -1) *errSeg = cnt;
        return 0;
    }
    for(int i=0;i<8;i++)
    {
        char *token = seg[i];
        int slen = strlen(token);
        if(slen>4)
        {
            *errSeg = i;
            return 0;
        }
        for(int j=0;j<slen;j++)
        {
            if(!isxdigit(token[j]))
            {
                *errSeg = i;
                return 0;
            }
        }
    }
    return 1;
}
/*检查MAC地址是否合法 MAC 规则
  6 段十六进制，`-`或`:`分割；每段范围`00‑FF`
  每段必须为两位十六进制数
*/
// MAC地址校验，支持 : 或 - 两种分隔符
int isValidMAC(char *mac, int *errSeg) {
    *errSeg = -1;
    int len = strlen(mac);
    char seg[10][50];
    int segCnt=0;
    char buf[50];
    int bufIdx=0;
    for(int i=0;i<=len;i++)
    {
        char ch = mac[i];
        if(ch == ':' || ch == '-' || ch == '\0')
        {
            buf[bufIdx] = '\0';
            if(segCnt >= 10)
            {
                *errSeg = segCnt;
                return 0;
            }
            strcpy(seg[segCnt], buf);
            if(bufIdx == 0)
            {
                *errSeg = segCnt;
                return 0;
            }
            segCnt++;
            bufIdx=0;
        }
        else
        {
            buf[bufIdx++] = ch;
        }
    }
    if(segCnt !=6)
    {
        *errSeg = segCnt;
        return 0;
    }
    for(int i=0;i<6;i++)
    {
        char *token = seg[i];
        if(strlen(token)!=2)
        {
            *errSeg = i;
            return 0;
        }
        for(int j=0;j<2;j++)
        {
            if(!isxdigit(token[j]))
            {
                *errSeg = i;
                return 0;
            }
        }
    }
    return 1;
}
int main() {
    char input[100];
    printf("请输入一个地址（IPv4 / IPv6 / MAC）：");
    scanf("%s", input);
    int err4 = -1, err6 = -1, errMac = -1;
    int is4 = 0, is6 = 0, isMac = 0;
    // 一次性全部校验
    is4 = isValidIPv4(input, &err4);
    is6 = isValidIPv6(input, &err6);
    isMac = isValidMAC(input, &errMac);
    if (is4) {
        printf("这是一个有效的 IPv4 地址。\n");
    } else if (is6) {
        printf("这是一个有效的 IPv6 地址。\n");
    } else if (isMac) {
        printf("这是一个有效的 MAC 地址。\n");
    } else {
        printf("输入的地址无效。\n");
        // 优先根据分隔符选择对应的错误信息，而不是优先IPv4
        if(strchr(input, '.') != NULL)
        {
            printf("IPv4校验失败，错误段下标：%d\n", err4);
        }
        else if(strchr(input, ':') != NULL && strchr(input, '-') == NULL)
        {
            printf("IPv6校验失败，错误段下标：%d\n", err6);
        }
        else if(strchr(input, ':') != NULL || strchr(input, '-') != NULL)
        {
            printf("MAC校验失败，错误段下标：%d\n", errMac);
        }
    }
    return 0;
}
