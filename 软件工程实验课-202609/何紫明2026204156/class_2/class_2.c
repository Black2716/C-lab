/* 实验2 
编程模拟斗地主的发牌过程。
一副扑克有54张牌，每张扑克包括花色(Suit)和牌面(Face)两个部分，花色包括：黑桃(spades)、红桃(hearts)、草花(clubs)、方块(diamonds) 、red、black。
黑桃，红桃，草花和方块又有13张牌面：2、3……10、Jack（J）、Queen（Q）、King（K）、A。
red 和 black 仅有1种牌面Joker（JK）。每位玩家17张牌，剩余3张底牌
要求随机发5次牌，输出每次发牌后，每位玩家的牌和剩余的底牌：*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>

// 定义牌的结构体
typedef struct {
    char suit[12]; // 花色
    char face[4]; // 牌面
} Card;

#define NUM_CARDS 54 // 扑克总数
#define NUM_PLAYERS 3 // 玩家数量   
#define NUM_CARDS_PER_PLAYER 17 // 每位玩家的牌数
#define NUM_BOTTOM_CARDS 3 // 底牌数量

// 初始化扑克牌
void initialize_deck(Card deck[]) {
    const char *suits[] = {"黑桃", "红桃", "草花", "方块", "大王", "小王"};
    const char *faces[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    
    int index = 0;
    for (int i = 0; i < 4; i++) { // 前四种花色
        for (int j = 0; j < 13; j++) { // 每种花色的牌面
            strcpy(deck[index].suit, suits[i]);
            strcpy(deck[index].face, faces[j]);
            index++;
        }
    }
    // 添加两张Joker牌
    strcpy(deck[index].suit, suits[4]); // red Joker
    strcpy(deck[index].face, "JK");
    index++;
    
    strcpy(deck[index].suit, suits[5]); // black Joker
    strcpy(deck[index].face, "JK");
}

// 洗牌函数
void shuffle_deck(Card deck[]) {
    //不能写进循环内，循环速度极快，都在同一秒内，种子相同无法实现真正随机
    //srand(time(NULL)); // 使用当前时间作为随机数种子。

    //i从后往前遍历，保证每张牌都能被随机交换
    for (int i = NUM_CARDS - 1; i > 0; i--) {   
        int j = rand() % (i + 1); // 生成一个0到i之间的随机数，表示要交换的牌的位置
        // 交换牌的位置
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}


// 发牌函数
void deal_cards(Card deck[], Card players[][NUM_CARDS_PER_PLAYER], Card bottom_cards[]) {
    for (int i = 0; i < NUM_PLAYERS; i++) {
        for (int j = 0; j < NUM_CARDS_PER_PLAYER; j++) {
            players[i][j] = deck[i * NUM_CARDS_PER_PLAYER + j]; // 给每位玩家发17张牌
        }
    }
    // 剩余3张牌作为底牌
    for (int k = 0; k < NUM_BOTTOM_CARDS; k++) {
        bottom_cards[k] = deck[NUM_PLAYERS * NUM_CARDS_PER_PLAYER + k];
    }
}

// 打印玩家的牌和底牌
void print_cards(Card players[][NUM_CARDS_PER_PLAYER], Card bottom_cards[]) {
    for (int i = 0; i < NUM_PLAYERS; i++) {
        printf("玩家%d的牌: ", i + 1);
        for (int j = 0; j < NUM_CARDS_PER_PLAYER; j++) {
            printf("[%s_%s] ", players[i][j].suit, players[i][j].face);
        }
        printf("\n");
    }
    printf("底牌: ");
    for (int k = 0; k < NUM_BOTTOM_CARDS; k++) {
        printf("[%s_%s] ", bottom_cards[k].suit, bottom_cards[k].face);
    }
    printf("\n");
}

// 主函数
int main() {
    Card deck[NUM_CARDS]; // 扑克牌数组
    Card players[NUM_PLAYERS][NUM_CARDS_PER_PLAYER]; // 玩家牌数组
    Card bottom_cards[NUM_BOTTOM_CARDS]; // 底牌数组

    srand((unsigned)time(NULL));// 使用当前时间作为随机数种子
    for (int round = 1; round <= 5; round++) {
        printf("第%d轮发牌:\n", round);
        initialize_deck(deck);
        shuffle_deck(deck);
        deal_cards(deck, players, bottom_cards);
        print_cards(players, bottom_cards);
        printf("\n");
    }
    return 0;
}