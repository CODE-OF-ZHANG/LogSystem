// #include "util.hpp"
// #include "level.hpp"
// #include "format.hpp"
// #include "sink.hpp"
// #include "logger.hpp"
// #include "buffer.hpp"
#include "../logs/bitlog.h"
#include <unistd.h>

// void test_log()
// {
//     // zx::Logger::ptr logger = zx::LoggerManager::getInstance().getLogger("async_logger");
//     // logger->debug("%s", "测试日志...");
//     // logger->info("%s", "测试日志...");
//     // logger->warn("%s", "测试日志...");
//     // logger->error("%s", "测试日志...");
//     // logger->fatal("%s", "测试日志...");

//     // size_t cursize = 0, count = 0;
//     // while (cursize < 1024 * 1024 * 10)
//     // {
//     //     logger->fatal("测试日志-%d", count++);
//     //     cursize += 20;
//     // }

//     DEBUG("%s", "测试日志...");
//     INFO("%s", "测试日志...");
//     WARN("%s", "测试日志...");
//     ERROR("%s", "测试日志...");
//     FATAL("%s", "测试日志...");

//     size_t count = 0;
//     while (count < 50000)
//     {
//         FATAL("测试日志-%d", count++);
//     }
// }

int main()
{
    // std::ifstream ifs("./logfile/test.log", std::ios::binary);
    // if (ifs.is_open() == false)
    // {
    //     std::cout << "open faild\n";
    //     return -1;
    // }
    // ifs.seekg(0, std::ios::end);
    // size_t fsize = ifs.tellg();
    // ifs.seekg(0, std::ios::beg);
    // std::string body;
    // body.resize(fsize);
    // ifs.read(&body[0], fsize);
    // if (ifs.good() == false)
    // {
    //     std::cout << "read error\n";
    //     return -1;
    // }
    // ifs.close();

    // std::cout << fsize << std::endl;
    // zx::Buffer buffer;
    // for (int i = 0; i < body.size(); i++)
    // {
    //     buffer.push(&body[i], 1);
    // }
    // std::cout << buffer.readAbleSize() << std::endl;
    // std::ofstream ofs("./logfile/tmp.log", std::ios::binary);

    // // ofs.write(buffer.begin(), buffer.readAbleSize());
    // size_t rsize = buffer.readAbleSize();
    // for (int i = 0; i < rsize; i++)
    // {
    //     ofs.write(buffer.begin(), 1);
    //     if (ofs.good() == false)
    //     {
    //         std::cout << "write error!\n";
    //         return -1;
    //     }
    //     buffer.moveReader(1);
    // }

    // // if (ofs.good() == false)
    // // {
    // //     std::cout << "write error!\n";
    // //     return -1;
    // // }
    // ofs.close();

    std::unique_ptr<zx::LoggerBuilder> builder(new zx::GlobalLoggerBuilder());
    builder->buildLoggerName("async_logger");
    builder->buildLoggerLevel(zx::LogLevel::value::WARN);
    builder->buildFormatter("[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n");
    builder->buildLoggerType(zx::LoggerType::LOGGER_ASYNC);
    builder->buildEnableUnSafeAsync();
    builder->buildSink<zx::FileSink>("./logfile/async.log");
    builder->buildSink<zx::StdoutSink>();
    builder->build();

    // test_log();

    // std::string logger_name = "sync_logger";
    // zx::LogLevel::value limit = zx::LogLevel::value::WARN;
    // zx::Formatter::ptr fmt(new zx::Formatter("[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n"));
    // zx::LogSink::ptr stdout_lsp = zx::SinkFactory::create<zx::StdoutSink>();
    // zx::LogSink::ptr file_lsp = zx::SinkFactory::create<zx::FileSink>("./logfile/test.log");
    // zx::LogSink::ptr roll_lsp = zx::SinkFactory::create<zx::FileBySizeSink>("./logfile/roll-", 1024 * 1024);
    // std::vector<zx::LogSink::ptr> sinks = {stdout_lsp, file_lsp, roll_lsp};
    // zx::Logger::ptr logger(new zx::SyncLogger(logger_name, limit, fmt, sinks));

    

    // zx::LogMsg msg(zx::LogLevel::value::INFO, "main.cc", 53, "root", "格式化功能测试...");
    // zx::Formatter fmt;
    // std::string str = fmt.format(msg);
    // // std::cout << str << std::endl;

    // zx::LogSink::ptr time_lsp = zx::SinkFactory::create<zx::FileByTimeSink>("./logfile/roll-", zx::TimeGap::GAP_SECOND);
    // time_t old = zx::util::Date::now();
    // while (zx::util::Date::now() < old + 5)
    // {
    //     time_lsp->log(str.c_str(), str.size());
    //     usleep(1000);
    // }

    // zx::LogSink::ptr stdout_lsp = zx::SinkFactory::create<zx::StdoutSink>();
    // zx::LogSink::ptr file_lsp = zx::SinkFactory::create<zx::FileSink>("./logfile/test.log");
    // zx::LogSink::ptr roll_lsp = zx::SinkFactory::create<zx::FileBySizeSink>("./logfile/roll-", 1024 * 1024);
    // stdout_lsp->log(str.c_str(), str.size());
    // file_lsp->log(str.c_str(), str.size());
    // size_t cursize = 0;
    // size_t count = 0;
    // while (cursize < 1024 * 1024 * 10)
    // {
    //     std::string tmp = str + std::to_string(count++);
    //     roll_lsp->log(str.c_str(), str.size());
    //     cursize += tmp.size();
    // }

    // std::cout << zx::LogLevel::toString(zx::LogLevel::value::DEBUG) << std::endl;
    // std::cout << zx::LogLevel::toString(zx::LogLevel::value::INFO) << std::endl;
    // std::cout << zx::LogLevel::toString(zx::LogLevel::value::WARN) << std::endl;
    // std::cout << zx::LogLevel::toString(zx::LogLevel::value::ERROR) << std::endl;
    // std::cout << zx::LogLevel::toString(zx::LogLevel::value::FATAL) << std::endl;
    // std::cout << zx::LogLevel::toString(zx::LogLevel::value::OFF) << std::endl;

    // std::cout << zx::util::Date::now() << std::endl;
    // std::string pathname = "./abc/bcd/a.txt";
    // zx::util::File::createDirectory(zx::util::File::path(pathname));
    return 0;
}