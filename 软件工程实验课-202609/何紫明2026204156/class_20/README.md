# 命令行任务管理系统

这是一个基于 C 语言实现的命令行待办事项管理系统，支持：

- 添加任务
- 查看任务
- 完成任务
- 删除任务
- 数据持久化

## 编译

```bash
gcc -std=c11 -Wall -Wextra -o todo todo.c
```

## 使用方式

```bash
./todo add "学习C语言"
./todo list
./todo done 1
./todo delete 2
```

也可以直接启动交互模式：

```bash
./todo
```

输入：

```text
todo> add 学习C语言
todo> list
todo> done 1
todo> delete 2
todo> quit
```
