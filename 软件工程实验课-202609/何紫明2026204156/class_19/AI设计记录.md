# 第十九次实验：学生信息管理系统开发

## 1. 实验目标

设计并实现一个命令行学生信息管理系统，使用动态内存保存学生记录，并通过 `student.dat` 文件实现数据持久化。

## 2. 数据结构

```c
typedef struct {
    int id;
    char name[30];
    float score;
} Student;
```

学生数组使用 `StudentList` 管理。数组容量不足时通过 `realloc` 扩容，满足挑战要求中的动态内存使用。

## 3. 功能模块与接口

| 函数 | 作用 |
| --- | --- |
| `load_students` | 程序启动或选择“读取数据”时从二进制文件加载记录 |
| `save_students` | 将内存中的全部记录保存到 `student.dat` |
| `add_student` | 校验学号、姓名和成绩后添加学生 |
| `delete_student` | 按学号删除学生并移动后续数组元素 |
| `query_student` | 按学号查询并显示学生 |
| `modify_score` | 按学号修改成绩 |
| `sort_students` | 使用 `qsort` 按成绩降序排列 |
| `show_students` | 显示全部学生 |

## 4. 设计说明

- 学号作为学生记录的唯一标识，不允许重复添加。
- 成绩限制在 `0.0` 到 `100.0`，避免无效数据进入系统。
- 数据文件不存在时按首次运行处理，不作为错误。
- 选择退出时自动保存；程序启动时自动读取。
- 所有动态内存由 `free_list` 统一释放，避免内存泄漏。

## 5. 编译与运行

```bash
cc -std=c11 -Wall -Wextra -pedantic student_manager.c -o student_manager
./student_manager
```

## 6. 测试记录

已验证以下场景：

1. 首次启动时无 `student.dat`，程序正常进入菜单。
2. 添加学生、重复学号校验、成绩范围校验。
3. 查询、删除、修改成绩和成绩降序排序。
4. 选择保存或退出后生成 `student.dat`。
5. 再次启动能够自动读取之前保存的记录。
6. 删除最后一条记录、空列表显示和无效菜单输入。
