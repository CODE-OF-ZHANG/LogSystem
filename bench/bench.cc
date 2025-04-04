#include "../logs/bitlog.h"
#include <chrono>

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
    // 1. 获取日志器
    zx::Logger::ptr logger = zx::getLogger(logger_name);
    if (logger.get() == nullptr)
        return;
    std::cout << "测试日志: " << msg_count << " 条, 总大小" << (msg_count * msg_len) / 1024 << "KB\n";
    // 2. 组织指定长度的日志消息
    std::string msg(msg_len - 1, 'A'); // 少一个字节, 是为了到时候末尾添加换行
    // 3. 创建指定数量的线程
    std::vector<std::thread> threads;
    std::vector<double> cost_array(thr_count);
    // 总日志数量除以线程数量就是每个线程要输出的日志数量
    size_t msg_per_thr = msg_count / thr_count;
    for (int i = 0; i < thr_count; i++)
    {
        threads.emplace_back([&, i]()
                             {
                                 // 4. 线程函数内部开始计时
                                 auto start = std::chrono::high_resolution_clock::now();
                                 // 5. 开始循环写日志
                                 for (int j = 0; i < msg_per_thr; j++)
                                 {
                                     logger->fatal("%s", msg.c_str());
                                 }
                                 // 6. 线程函数内部结束计时
                                 auto end = std::chrono::high_resolution_clock::now();
                                 std::chrono::duration<double> cost = end - start;
                                 cost_array[i] = cost.count();
                                 std::cout << "线程: " << i << ": " << "\t输出数量:"
                                           << msg_per_thr << ", 耗时: " << cost.count() << "s" << std::endl; });
    }
    for (int i = 0; i < thr_count; i++)
    {
        threads[i].join();
    }
    // 7. 计算总耗时 --> 在多线程中, 线程是并发处理的, 所以所有线程中, 耗时最长的即为总时间
    double max_cost = cost_array[0];
    for (int i = 0; i < thr_count; i++)
        max_cost = max_cost < cost_array[i] ? cost_array[i] : max_cost;
    size_t msg_per_sec = msg_count / max_cost;
    size_t size_per_sec = (msg_count * msg_len) / (max_cost * 1024);
    // 8. 进行输出打印
    std::cout << "\t总耗时: " << max_cost << std::endl;
    std::cout << "\t每条输出日志数量: " << msg_per_sec << "条\n";
    std::cout << "\t每秒输出日志大小: " << size_per_sec << "KB\n";
}

void sync_bench()
{
    std::unique_ptr<zx::LoggerBuilder> builder(new zx::GlobalLoggerBuilder());
    builder->buildLoggerName("sync_logger");
    builder->buildFormatter("%m%n");
    builder->buildLoggerType(zx::LoggerType::LOGGER_SYNC);
    builder->buildSink<zx::FileSink>("./logfile/sync.log");
    builder->build();
    bench("sync_logger", 1, 1, 100);
}

void async_bench()
{
    std::unique_ptr<zx::LoggerBuilder> builder(new zx::GlobalLoggerBuilder());
    builder->buildLoggerName("async_logger");
    builder->buildFormatter("%m%n");
    builder->buildLoggerType(zx::LoggerType::LOGGER_ASYNC);
    builder->buildEnableUnSafeAsync();
    builder->buildSink<zx::FileSink>("./logfile/async.log");
    builder->build();
    bench("sync_logger", 1, 1, 100);
}

int main()
{
    sync_bench();
    // async_bench();
    return 0;
}
