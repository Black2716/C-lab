#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "student.dat"
#define NAME_LENGTH 30

typedef struct {
    int id;
    char name[NAME_LENGTH];
    float score;
} Student;

typedef struct {
    Student *items; // 存生数组
    size_t count; // 当前学生数量
    size_t capacity; // 数组容量
} StudentList;

// 初始化学生列表
static void init_list(StudentList *list) {
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

// 释放学生列表内存
static void free_list(StudentList *list) {
    free(list->items);
    init_list(list);
}

// 确保学生列表容量足够
static int ensure_capacity(StudentList *list) {
    if (list->count < list->capacity) {
        return 1;
    }

    size_t new_capacity = list->capacity == 0 ? 8 : list->capacity * 2; // 初始容量为 8，后续每次增加一倍
    Student *new_items = realloc(list->items, new_capacity * sizeof(*new_items));
    if (new_items == NULL) {
        fprintf(stderr, "内存分配失败，无法继续操作。\n");
        return 0;
    }
    list->items = new_items;
    list->capacity = new_capacity;
    return 1;
}

// 读取用户输入的整数
static int read_line(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) { // 读取失败
        return 0;
    }
    buffer[strcspn(buffer, "\r\n")] = '\0'; // 移除换行符 strcspn 函数返回第一个非换行符的索引，将该索引后的字符设置为 '\0' 即可移除换行符
    return 1;
}


static int read_int(const char *prompt, int *value) {
    char line[64];
    char extra;
    if (!read_line(prompt, line, sizeof(line)) ||
        sscanf(line, " %d %c", value, &extra) != 1) {
        printf("请输入有效的整数。\n");
        return 0;
    }
    return 1;
}

static int read_score(const char *prompt, float *score) {
    char line[64];
    char extra;
    if (!read_line(prompt, line, sizeof(line)) ||
        sscanf(line, " %f %c", score, &extra) != 1 ||
        *score < 0.0f || *score > 100.0f) {
        printf("成绩必须是 0 到 100 之间的数字。\n");
        return 0;
    }
    return 1;
}

static int find_student(const StudentList *list, int id) {
    for (size_t i = 0; i < list->count; i++) {
        if (list->items[i].id == id) {
            return (int)i;
        }
    }
    return -1;
}

static int load_students(StudentList *list, const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return 1; /* 首次运行时数据文件不存在是正常情况。 */
    }

    Student student;
    while (fread(&student, sizeof(student), 1, file) == 1) {
        if (!ensure_capacity(list)) {
            fclose(file);
            return 0;
        }
        list->items[list->count++] = student;
    }

    if (ferror(file)) {
        fprintf(stderr, "读取数据文件失败。\n");
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}

static int save_students(const StudentList *list, const char *path) {
    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        perror("无法保存数据");
        return 0;
    }

    if (list->count > 0 &&
        fwrite(list->items, sizeof(*list->items), list->count, file) != list->count) {
        fprintf(stderr, "写入数据文件失败。\n");
        fclose(file);
        return 0;
    }
    if (fclose(file) != 0) {
        perror("关闭数据文件失败");
        return 0;
    }
    return 1;
}

static void print_student(const Student *student) {
    printf("%-8d %-30s %6.2f\n", student->id, student->name, student->score);
}

static void show_students(const StudentList *list) {
    if (list->count == 0) {
        printf("暂无学生信息。\n");
        return;
    }
    printf("\n学号     姓名                           成绩\n");
    printf("-----------------------------------------------\n");
    for (size_t i = 0; i < list->count; i++) {
        print_student(&list->items[i]);
    }
}

static void add_student(StudentList *list) {
    Student student;
    if (!read_int("请输入学号：", &student.id)) {
        return;
    }
    if (find_student(list, student.id) >= 0) {
        printf("学号已存在，不能重复添加。\n");
        return;
    }
    if (!read_line("请输入姓名：", student.name, sizeof(student.name)) ||
        student.name[0] == '\0') {
        printf("姓名不能为空。\n");
        return;
    }
    if (!read_score("请输入成绩：", &student.score)) {
        return;
    }
    if (!ensure_capacity(list)) {
        return;
    }
    list->items[list->count++] = student;
    printf("添加成功。\n");
}

static void delete_student(StudentList *list) {
    int id;
    if (!read_int("请输入要删除的学号：", &id)) {
        return;
    }
    int index = find_student(list, id);
    if (index < 0) {
        printf("未找到该学生。\n");
        return;
    }
    memmove(&list->items[index], &list->items[index + 1],
            (list->count - (size_t)index - 1) * sizeof(*list->items));
    list->count--;
    printf("删除成功。\n");
}

static void query_student(const StudentList *list) {
    int id;
    if (!read_int("请输入要查询的学号：", &id)) {
        return;
    }
    int index = find_student(list, id);
    if (index < 0) {
        printf("未找到该学生。\n");
        return;
    }
    printf("\n学号     姓名                           成绩\n");
    printf("-----------------------------------------------\n");
    print_student(&list->items[index]);
}

static void modify_score(StudentList *list) {
    int id;
    float score;
    if (!read_int("请输入要修改成绩的学号：", &id)) {
        return;
    }
    int index = find_student(list, id);
    if (index < 0) {
        printf("未找到该学生。\n");
        return;
    }
    if (!read_score("请输入新成绩：", &score)) {
        return;
    }
    list->items[index].score = score;
    printf("成绩修改成功。\n");
}

static int compare_score_desc(const void *left, const void *right) {
    const Student *a = left;
    const Student *b = right;
    if (a->score < b->score) {
        return 1;
    }
    if (a->score > b->score) {
        return -1;
    }
    return a->id - b->id;
}

static void sort_students(StudentList *list) {
    qsort(list->items, list->count, sizeof(*list->items), compare_score_desc);
    printf("已按成绩从高到低排序。\n");
    show_students(list);
}

static void print_menu(void) {
    printf("\n========== 学生信息管理系统 ==========\n");
    printf("1. 添加学生\n");
    printf("2. 删除学生\n");
    printf("3. 修改学生成绩\n");
    printf("4. 查询学生\n");
    printf("5. 显示全部学生\n");
    printf("6. 按成绩排序\n");
    printf("7. 保存数据\n");
    printf("8. 读取数据\n");
    printf("0. 退出\n");
    printf("=======================================\n");
}

int main(void) {
    StudentList list;
    init_list(&list);
    if (!load_students(&list, DATA_FILE)) {
        free_list(&list);
        return EXIT_FAILURE;
    }
    printf("已自动读取 %zu 条学生记录。\n", list.count);

    int choice;
    while (1) {
        print_menu();
        if (!read_int("请选择操作：", &choice)) {
            continue;
        }
        switch (choice) {
            case 1:
                add_student(&list);
                break;
            case 2:
                delete_student(&list);
                break;
            case 3:
                modify_score(&list);
                break;
            case 4:
                query_student(&list);
                break;
            case 5:
                show_students(&list);
                break;
            case 6:
                sort_students(&list);
                break;
            case 7:
                printf(save_students(&list, DATA_FILE) ? "保存成功。\n" : "保存失败。\n");
                break;
            case 8:
                free_list(&list);
                init_list(&list);
                printf(load_students(&list, DATA_FILE) ? "读取成功，共 %zu 条记录。\n" : "读取失败。\n",
                       list.count);
                break;
            case 0:
                if (!save_students(&list, DATA_FILE)) {
                    free_list(&list);
                    return EXIT_FAILURE;
                }
                free_list(&list);
                printf("数据已保存，程序退出。\n");
                return EXIT_SUCCESS;
            default:
                printf("无效选项，请重新选择。\n");
        }
    }
}
