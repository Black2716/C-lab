#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_TASKS 1024
#define MAX_CONTENT_LEN 256
#define DATA_FILE_ENV "TODO_DATA_FILE"

typedef struct {
    int id;
    char content[MAX_CONTENT_LEN];
    int done;
} Task;

typedef struct {
    Task tasks[MAX_TASKS];
    int count;
} TaskList;

static char *get_data_path(void) {
    const char *env_path = getenv(DATA_FILE_ENV);
    if (env_path != NULL && env_path[0] != '\0') {
        return strdup(env_path);
    }
    return strdup("todo_data.txt");
}

static void trim_newline(char *s) {
    size_t len = strlen(s); //`size_t`：无符号整数类型（`unsigned`），专门用来表示内存大小、字符串长度。
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}

static void trim_spaces(char *s) {
    char *start = s;
    char *end;
    while (*start != '\0' && isspace((unsigned char)*start)) {
        start++;
    }
    if (start != s) {
        memmove(s, start, strlen(start) + 1);
    }
    end = s + strlen(s);
    while (end > s && isspace((unsigned char)*(end - 1))) {
        *(--end) = '\0';
    }
}

static void tokenize_command(const char *input, char *argv[], int max_args, int *argc_out) {
    char buffer[512];
    char *dst = buffer;
    int in_quote = 0;
    int arg_count = 0;
    const char *p = input;

    while (*p != '\0') {
        if (*p == '"') {
            in_quote = !in_quote;
            p++;
            continue;
        }

        if (isspace((unsigned char)*p) && !in_quote) {
            if (dst != buffer) {
                *dst = '\0';
                argv[arg_count++] = strdup(buffer);
                dst = buffer;
            }
            while (*p != '\0' && isspace((unsigned char)*p)) {
                p++;
            }
            continue;
        }

        if (arg_count >= max_args) {
            break;
        }
        *dst++ = *p++;
    }

    if (dst != buffer) {
        *dst = '\0';
        argv[arg_count++] = strdup(buffer);
    }

    *argc_out = arg_count;
}

static void free_argv(char *argv[], int argc) {
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
}

static int load_tasks(TaskList *list, const char *path) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        list->count = 0;
        return 0;
    }

    list->count = 0;
    char line[512];
    while (fgets(line, sizeof(line), fp) != NULL) {
        trim_newline(line);
        if (line[0] == '\0') {
            continue;
        }

        char *saveptr = NULL;
        char *id_str = strtok_r(line, "|", &saveptr);
        char *content = strtok_r(NULL, "|", &saveptr);
        char *done_str = strtok_r(NULL, "|", &saveptr);

        if (id_str == NULL || content == NULL || done_str == NULL) {
            continue;
        }

        Task task;
        task.id = atoi(id_str);
        snprintf(task.content, sizeof(task.content), "%s", content);
        task.done = atoi(done_str);

        if (list->count < MAX_TASKS) {
            list->tasks[list->count++] = task;
        }
    }

    fclose(fp);
    return 0;
}

static int save_tasks(const TaskList *list, const char *path) {
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        fprintf(stderr, "无法写入任务文件：%s\n", path);
        return -1;
    }

    for (int i = 0; i < list->count; i++) {
        fprintf(fp, "%d|%s|%d\n", list->tasks[i].id, list->tasks[i].content, list->tasks[i].done);
    }

    fclose(fp);
    return 0;
}

static void print_usage(void) {
    printf("Usage:\n");
    printf("  todo add \"完成软件工程作业\"\n");
    printf("  todo list\n");
    printf("  todo done 1\n");
    printf("  todo delete 2\n");
    printf("  todo help\n");
}

static int add_task(TaskList *list, const char *content) {
    if (content == NULL || content[0] == '\0') {
        fprintf(stderr, "任务内容不能为空。\n");
        return 1;
    }

    Task task;
    task.id = list->count == 0 ? 1 : list->tasks[list->count - 1].id + 1;
    snprintf(task.content, sizeof(task.content), "%s", content);
    task.done = 0;

    if (list->count >= MAX_TASKS) {
        fprintf(stderr, "任务数量已达上限。\n");
        return 1;
    }

    list->tasks[list->count++] = task;
    return 0;
}

static int list_tasks(const TaskList *list) {
    if (list->count == 0) {
        printf("暂无任务。\n");
        return 0;
    }

    printf("ID  状态  内容\n");
    printf("---------------------\n");
    for (int i = 0; i < list->count; i++) {
        printf("%2d  %s  %s\n",
               list->tasks[i].id,
               list->tasks[i].done ? "[√]" : "[ ]",
               list->tasks[i].content);
    }
    return 0;
}

static int mark_done(TaskList *list, int id) {
    for (int i = 0; i < list->count; i++) {
        if (list->tasks[i].id == id) {
            list->tasks[i].done = 1;
            printf("任务 %d 已完成。\n", id);
            return 0;
        }
    }

    fprintf(stderr, "未找到任务 ID=%d。\n", id);
    return 1;
}

static int delete_task(TaskList *list, int id) {
    int index = -1;
    for (int i = 0; i < list->count; i++) {
        if (list->tasks[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        fprintf(stderr, "未找到任务 ID=%d。\n", id);
        return 1;
    }

    for (int i = index; i < list->count - 1; i++) {
        list->tasks[i] = list->tasks[i + 1];
    }
    list->count--;

    for (int i = 0; i < list->count; i++) {
        list->tasks[i].id = i + 1;
    }

    printf("任务 %d 已删除。\n", id);
    return 0;
}

static int handle_command(TaskList *list, int argc, char *argv[], const char *path) {
    if (argc == 0) {
        print_usage();
        return 0;
    }

    const char *command = argv[0];

    if (strcmp(command, "help") == 0) {
        print_usage();
        return 0;
    }

    if (strcmp(command, "add") == 0) {
        if (argc < 2) {
            fprintf(stderr, "add 命令需要提供任务内容。\n");
            return 1;
        }

        char content[MAX_CONTENT_LEN] = {0};
        snprintf(content, sizeof(content), "%s", argv[1]);
        for (int i = 2; i < argc; i++) {
            strncat(content, " ", sizeof(content) - strlen(content) - 1);
            strncat(content, argv[i], sizeof(content) - strlen(content) - 1);
        }

        if (add_task(list, content) == 0) {
            if (save_tasks(list, path) == 0) {
                printf("任务已添加：%s\n", content);
            }
            return 0;
        }
        return 1;
    }

    if (strcmp(command, "list") == 0) {
        return list_tasks(list);
    }

    if (strcmp(command, "done") == 0) {
        if (argc < 2) {
            fprintf(stderr, "done 命令需要提供任务 ID。\n");
            return 1;
        }
        int id = atoi(argv[1]);
        if (mark_done(list, id) == 0 && save_tasks(list, path) == 0) {
            return 0;
        }
        return 1;
    }

    if (strcmp(command, "delete") == 0) {
        if (argc < 2) {
            fprintf(stderr, "delete 命令需要提供任务 ID。\n");
            return 1;
        }
        int id = atoi(argv[1]);
        if (delete_task(list, id) == 0 && save_tasks(list, path) == 0) {
            return 0;
        }
        return 1;
    }

    fprintf(stderr, "未知命令：%s\n", command);
    print_usage();
    return 1;
}

static void interactive_loop(const char *path) {
    TaskList list;
    load_tasks(&list, path);

    char input[512];
    while (1) {
        printf("todo> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n再见！\n");
            break;
        }
        trim_newline(input);
        trim_spaces(input);
        if (strcmp(input, "") == 0) {
            continue;
        }
        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            printf("再见！\n");
            break;
        }

        char *argv[8] = {0};
        int argc = 0;
        tokenize_command(input, argv, 8, &argc);

        if (argc == 0) {
            free_argv(argv, argc);
            continue;
        }

        if (strcmp(argv[0], "help") == 0) {
            print_usage();
            free_argv(argv, argc);
            continue;
        }

        if (strcmp(argv[0], "list") == 0) {
            list_tasks(&list);
            free_argv(argv, argc);
            continue;
        }

        if (strcmp(argv[0], "add") == 0) {
            if (argc < 2) {
                fprintf(stderr, "add 命令需要提供任务内容。\n");
                free_argv(argv, argc);
                continue;
            }

            char content[MAX_CONTENT_LEN] = {0};
            snprintf(content, sizeof(content), "%s", argv[1]);
            for (int i = 2; i < argc; i++) {
                strncat(content, " ", sizeof(content) - strlen(content) - 1);
                strncat(content, argv[i], sizeof(content) - strlen(content) - 1);
            }

            if (add_task(&list, content) == 0 && save_tasks(&list, path) == 0) {
                printf("任务已添加：%s\n", content);
            }
            free_argv(argv, argc);
            continue;
        }

        if (strcmp(argv[0], "done") == 0) {
            if (argc < 2) {
                fprintf(stderr, "done 命令需要提供任务 ID。\n");
                free_argv(argv, argc);
                continue;
            }
            int id = atoi(argv[1]);
            if (mark_done(&list, id) == 0 && save_tasks(&list, path) == 0) {
                // no-op
            }
            free_argv(argv, argc);
            continue;
        }

        if (strcmp(argv[0], "delete") == 0) {
            if (argc < 2) {
                fprintf(stderr, "delete 命令需要提供任务 ID。\n");
                free_argv(argv, argc);
                continue;
            }
            int id = atoi(argv[1]);
            if (delete_task(&list, id) == 0 && save_tasks(&list, path) == 0) {
                // no-op
            }
            free_argv(argv, argc);
            continue;
        }

        fprintf(stderr, "未知命令：%s\n", argv[0]);
        print_usage();
        free_argv(argv, argc);
    }
}

int main(int argc, char *argv[]) {
    char *data_path = get_data_path();
    TaskList list;
    load_tasks(&list, data_path);

    if (argc == 1) {
        interactive_loop(data_path);
        free(data_path);
        return 0;
    }

    int handled = handle_command(&list, argc - 1, argv + 1, data_path);
    free(data_path);
    return handled == 0 ? 0 : 1;
}
