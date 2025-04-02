#include "../logs/bitlog.h"

namespace zx
{
    void bench(const std::string &logger_name, size_t thr_count, size_t msg_count, size_t msg_len)
    {
        /*
            1. 获取日志器
            2. 组织指定长度的日志消息
            3. 创建指定数量的线程
            4. 线程函数内部开始计时
            5. 开始循环写日志
            6. 线程函数内部结束计时
            7. 计算总耗时
            8. 进行输出打印
        */
    }

    void sync_bench();
    void async_bench();
} // namespace zx
