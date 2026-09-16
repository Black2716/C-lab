/*
        数据规模 (词)	        优化前时间空间	                     优化后时间空间
        1w	            时间：较慢；空间：静态数组	            时间：快；空间：哈希表 + 链表
        10w	            时间：慢；空间：静态数组	            时间：较快；空间：哈希表 + 链表
        100w        	时间：很慢；空间：数组容易超限	         时间：尚可；空间：动态堆内存
        1000w       	时间：极慢，几乎不可用；数组溢出	     时间：可运行；堆内存占用较高
*/

/*
实验18 AI辅助程序优化——英文文本统计分析系统【优化版本】
优化内容：
1. 只读取文件一次，读取过程同步统计字符总数，消除rewind二次遍历文件
2. 实现简易字符串哈希表，单词查找从线性O(n)优化为平均O(1)
3. 使用标准库qsort快速排序替代冒泡排序 O(n log n)
4. 动态链表存储单词，取消固定MAX_WORD_NUM上限，避免数组越界
5. 使用大块缓冲区fread读取文件，减少fgetc频繁系统IO调用
6. 增加单词缓冲区溢出保护，程序鲁棒性提升
7. 程序结束释放堆内存，避免内存泄漏
输入：article.text
输出：文件字符数、单词数量、不同单词数量、出现频率最高10个单词
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define WORD_LEN        100     // 单个单词最大长度
#define HASH_TABLE_SIZE 2048    // 哈希表槽位数量，2的幂
#define BUF_SIZE        4096    // 文件块读取缓冲区大小

// 哈希链表节点：保存单个单词信息
typedef struct HashNode
{
    char word[WORD_LEN];
    int count;
    struct HashNode *next;
} HashNode;

// 哈希表，数组每个元素是链表头指针
HashNode* hash_table[HASH_TABLE_SIZE];

// 存放所有不重复单词，用于后续qsort排序
HashNode** word_list;
int diff_word_cnt = 0;   // 不同单词总数量

/**
 * @brief 简单字符串哈希函数，将单词映射到哈希槽下标
 * @param s 待哈希单词
 * @return 哈希表下标 0 ~ HASH_TABLE_SIZE‑1
 */
unsigned int hash_func(const char *s)
{
    unsigned int hash = 0;
    while (*s != '\0')
    {
        hash = hash * 31 + (*s);
        s++;
    }
    return hash % HASH_TABLE_SIZE;
}

/**
 * @brief 在哈希表查找/插入单词
 * @param word 小写单词
 */
void hash_insert(const char *word)
{
    unsigned int idx = hash_func(word);
    HashNode *p = hash_table[idx];

    // 遍历该槽链表，查找单词是否已经存在
    while (p != NULL)
    {
        if (strcmp(p->word, word) == 0)
        {
            p->count++;
            return;
        }
        p = p->next;
    }

    // 没有找到，新建节点插入链表头部
    HashNode *new_node = (HashNode*)malloc(sizeof(HashNode));
    strncpy(new_node->word, word, WORD_LEN - 1);
    new_node->word[WORD_LEN - 1] = '\0';
    new_node->count = 1;
    new_node->next = hash_table[idx];
    hash_table[idx] = new_node;
    diff_word_cnt++;
}

/**
 * @brief qsort的比较回调函数：按count降序排序
 */
int cmp_node(const void *a, const void *b)
{
    HashNode *na = *(HashNode**)a;
    HashNode *nb = *(HashNode**)b;
    return nb->count - na->count;
}

int main(void)
{
    FILE *fp;
    long char_cnt = 0;          // 文件总字符数
    int total_word = 0;         // 总单词数量
    char block_buf[BUF_SIZE];   // 文件块读缓冲区

    // 初始化哈希表全部置空
    memset(hash_table, 0, sizeof(hash_table));

    fp = fopen("article.text", "r");
    if (fp == NULL)
    {
        printf("无法打开 article.text，请检查文件位置！\n");
        return 1;
    }

    char word_buf[WORD_LEN];
    int buf_idx = 0;

    // ========== 只读取文件1次：块读取 ==========
    size_t read_len;
    while ((read_len = fread(block_buf, sizeof(char), BUF_SIZE, fp)) > 0)
    {
        for (size_t i = 0; i < read_len; i++)
        {
            char ch = block_buf[i];
            char_cnt++;  // 同步统计文件字符数，不再二次读文件

            if (isalpha(ch))
            {
                // 单词缓冲区溢出保护，防止越界
                if (buf_idx < WORD_LEN - 1)
                {
                    word_buf[buf_idx++] = tolower(ch);
                }
            }
            else
            {
                // 遇到分隔符，完成一个单词
                if (buf_idx > 0)
                {
                    word_buf[buf_idx] = '\0';
                    hash_insert(word_buf);
                    total_word++;
                    buf_idx = 0;
                }
            }
        }
    }
    // 处理文件末尾残留最后一个单词
    if (buf_idx > 0)
    {
        word_buf[buf_idx] = '\0';
        hash_insert(word_buf);
        total_word++;
    }
    fclose(fp);

    // ========== 将哈希表中全部节点收集到数组，用于快速排序 ==========
    word_list = (HashNode**)malloc(sizeof(HashNode*) * diff_word_cnt);
    int pos = 0;
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        HashNode *p = hash_table[i];
        while (p != NULL)
        {
            word_list[pos++] = p;
            p = p->next;
        }
    }

    // qsort快速排序，按出现次数降序
    qsort(word_list, diff_word_cnt, sizeof(HashNode*), cmp_node);

    // ========== 输出结果 ==========
    printf("文件字符数：%ld\n", char_cnt);
    printf("单词数量：%d\n", total_word);
    printf("不同单词数量：%d\n", diff_word_cnt);
    printf("出现频率最高的10个单词：\n");

    int show_cnt = diff_word_cnt < 10 ? diff_word_cnt : 10;
    for (int i = 0; i < show_cnt; i++)
    {
        printf("%-15s %d\n", word_list[i]->word, word_list[i]->count);
    }

    // ========== 释放所有堆内存，防止内存泄漏 ==========
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        HashNode *p = hash_table[i];
        while (p != NULL)
        {
            HashNode *temp = p;
            p = p->next;
            free(temp);
        }
    }
    free(word_list);

    return 0;
}