#include <iostream>
/*单例模式 -- 饿汉模式 --> 以空间换时间*/

// class Singleton
// {
// private:
//     int _data;

// private:
//     // 声明单例对象
//     static Singleton _eton;
//     // 默认构造私有化
//     Singleton() : _data(666) { std::cout << "单例对象构造！" << std::endl; }
//     Singleton(const Singleton &) = delete;
//     ~Singleton() {}

// public:
//     //  提供对外访问接口
//     static Singleton &getInstance()
//     {
//         return _eton;
//     }
//     int getData() { return _data; }
// };

// Singleton Singleton::_eton;

/*单例模式 -- 懒汉模式 --> 延迟加载的思想 --> 一个对象在用的时候再实例化*/
class Singleton
{
private:
    Singleton() : _data(666) { std::cout << "单例模式-懒汉模式！" << std::endl; }
    Singleton(const Singleton &) = delete;
    ~Singleton() {}

private:
    int _data;

public:
    static Singleton &getInstance()
    {
        static Singleton _eton;
        return _eton;
    }
    int getData() { return _data; }
};

int main()
{
    std::cout << Singleton::getInstance().getData() << std::endl;
    // std::cout << Singleton::getInstance().getData() << std::endl;
    return 0;
}