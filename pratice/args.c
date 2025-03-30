#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/*不定参宏函数的使用*/
#define LOG(fmt, ...) printf("[%s:%d]" fmt, __FILE__, __LINE__, ##__VA_ARGS__);

/*C语言中不定参函数的使用, 不定参数据的访问*/
void printNum(int count, ...)
{
    va_list ap;
    // 获取指定参数的起始地址, 获取count参数之后的第一个参数的起始地址
    va_start(ap, count);
    for (int i = 0; i < count; i++)
    {
        int num = va_arg(ap, int);
        printf("param[%d]:%d\n", i, num);
    }
    // 将ap指针置空
    va_end(ap);
}

// 模拟实现printf函数
void myprintf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char *res;
    int ret = vasprintf(&res, fmt, ap);
    if (ret != -1)
    {
        puts(res);
        free(res);
    }
    va_end(ap);
}

int main()
{
    LOG("测试代码");
    printNum(3, 111, 222, 333);
    printNum(5, 1, 2, 3, 4, 5);
    myprintf("%s-%d", "测试自定义代码", 666);
    return 0;
}