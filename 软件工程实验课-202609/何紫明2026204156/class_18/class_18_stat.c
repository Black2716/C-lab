/*实验18 AI辅助程序优化——英文文本统计分析系统

编写程序，实现英文文本统计。
输入：article.text
输出：
文件字符数：
单词数量：
不同单词数量：
出现频率最高的10个单词：


基本要求：
1.文件读取
2.单词提取
3.单词统计
4.排序输出
5.数据结构不限
*/

/*解题思路
1. 打开并读取 article.text 文件，逐个读字符
2. 统计文件总字符数（空格、换行、标点都算字符，不是字母）
3. 分割提取英文单词：把文本里的单词拿出来，忽略标点，统一大小写（Hello 和 hello 算同一个单词）
4. 保存每个单词 + 统计出现次数
5. 统计：总单词数、不重复单词数量
6. 按出现次数降序排序，取出前 10 高频单词
*/



#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_NUM 10000 //最多支持10000个不同单词
#define WORD_LEN 100

//单词统计结构体
typedef struct
{
    char word[WORD_LEN];
    int count;
} WordItem;

WordItem word_table[MAX_WORD_NUM];
int diff_word_cnt = 0; //不同单词数量

//在word_tabel查找单词，找到返回下标
int find_word(char *w)
{
    int i;
    for(i = 0; i < diff_word_cnt; i++)
    {
        if(strcmp(word_table[i].word, w) == 0) //两个字符串完全相等
        {
            return i;
        }
    }
    return -1;
}

//冒泡排序，按count降序
void sort_words(WordItem arr[], int n)
{
    int i,j;
    WordItem temp;
    for(i = 0; i < n-1; i++)
    {
        for(j = 0; j < n-1-i; j++)
        {
            if(arr[j].count < arr[j+1].count)
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main(void)
{
    FILE *fp;
    int ch;  //fget返回int，保存读取的字符
    long char_cnt = 0;
    int total_word = 0;

    //打开文件
    fp = fopen("article.text", "r");
    if(fp == NULL)
    {
        printf("无法打开 article.text，请检查文件位置！\n");
        return 1;
    }

    // 1.统计文件字符总数
    while( (ch = fgetc(fp)) != EOF )
    {
        char_cnt++;
    }

    // 重置文件指针回到开头，提取单词
    rewind(fp);
    char word_buf[WORD_LEN];
    int buf_idx = 0;
    diff_word_cnt = 0;
    total_word = 0;

    while( (ch = fgetc(fp)) != EOF )
    {
        if( isalpha(ch) ) //判断是否字母
        {
            word_buf[buf_idx++] = tolower(ch);
        }
        else
        {
            //遇到分隔符，缓冲区有内容说明该单词结束
            if(buf_idx > 0)
            {
                word_buf[buf_idx] = '\0'; //字符串结束符
                int pos = find_word(word_buf);
                if(pos != -1)
                {
                    word_table[pos].count++;
                }
                else
                {
                    strcpy(word_table[diff_word_cnt].word, word_buf);
                    word_table[diff_word_cnt].count = 1;
                    diff_word_cnt++;
                }
                total_word++;
                buf_idx = 0;
            }
        }
    }
    // 处理文件末尾残留单词
    if(buf_idx > 0)
    {
        word_buf[buf_idx] = '\0';
        int pos = find_word(word_buf);
        if(pos != -1)
            word_table[pos].count++;
        else
        {
            strcpy(word_table[diff_word_cnt].word, word_buf);
            word_table[diff_word_cnt].count = 1;
            diff_word_cnt++;
        }
        total_word++;
    }

    fclose(fp);

    //排序
    sort_words(word_table, diff_word_cnt);

    //输出结果
    printf("文件字符数：%ld\n", char_cnt);
    printf("单词数量：%d\n", total_word);
    printf("不同单词数量：%d\n", diff_word_cnt);
    printf("出现频率最高的10个单词：\n");
    int show_cnt = diff_word_cnt < 10 ? diff_word_cnt : 10;
    for(int i = 0; i < show_cnt; i++)
    {
        printf("%-15s %d\n", word_table[i].word, word_table[i].count);
    }

    return 0;
}
