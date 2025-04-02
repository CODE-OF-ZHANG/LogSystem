/*C++风格的不定参函数使用*/
#include <iostream>
void xprintf()
{
    std::cout << std::endl;
}

template <typename T, typename... Args>
void xprintf(const T &t, Args &&...args)
{
    std::cout << t;
    if ((sizeof...(args)) > 0)
        xprintf(std::forward<Args>(args)...);
    else
        xprintf();
}

int main()
{
    xprintf("C++代码测试");
    xprintf("C++代码测试", "不定参函数");
    xprintf("C++代码测试", "不定参函数", 666);
    return 0;
}