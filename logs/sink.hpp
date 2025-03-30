/*
    日志落地模块的实现
        1. 抽象落地基类
        2. 派生子类 --> (根据不同的落地方向进行派生)
        3. 使用工厂模式进行创建与表示的分离
*/
#ifndef __M_SINK_H__
#define __M_SINK_H__

#include "format.hpp"
#include <fstream>

namespace zx
{
    // 抽象落地基类
    class LogSink
    {
    public:
        using ptr = std::shared_ptr<LogSink>;
        LogSink() {}
        virtual ~LogSink() {}
        virtual void log(const char *data, size_t len) = 0;
    };

    /*
        落地方向
            1. 标准输出
            2. 指定文件
            3. 滚动文件 --> (以大小进行滚动)
    */

    // 1. 标准输出
    class StdoutSink : public LogSink
    {
    public:
        // 将日志消息写入到标准输出
        void log(const char *data, size_t len)
        {
            std::cout.write(data, len);
        }
    };

    // 2. 指定文件
    class FileSink : public LogSink
    {
    public:
        // 构造时传入文件名, 并打开文件, 将操作句柄给管理起来
        FileSink(const std::string &pathname) : _pathname(pathname)
        {
            // 1. 创建日志文件所在的目录
            util::File::createDirectory(util::File::path(_pathname));
            // 2. 创建并打开文件
            _ofs.open(_pathname, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }
        // 将日志消息写入指定文件
        void log(const char *data, size_t len)
        {
            _ofs.write(data, len);
            assert(_ofs.good());
        }

    private:
        std::string _pathname;
        std::ofstream _ofs;
    };

    // 3. 滚动文件 --> (以文件大小进行滚动)
    class FileBySizeSink : public LogSink
    {
    public:
        // 构造时传入文件名, 并打开文件, 将操作句柄给管理起来
        FileBySizeSink(const std::string &basename, size_t max_size)
            : _basename(basename), _max_fsize(max_size), _cur_fsize(0), _name_count(0)
        {
            std::string pathname = createNewFile();
            // 1. 创建日志文件所在的目录
            util::File::createDirectory(util::File::path(pathname));
            // 2. 创建并打开文件
            _ofs.open(pathname, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }
        // 将日志消息写入指定文件
        void log(const char *data, size_t len)
        {
            if (_cur_fsize > _max_fsize)
            {
                // 关闭已经打开的文件
                _ofs.close();
                std::string pathname = createNewFile();
                _ofs.open(pathname, std::ios::binary | std::ios::app);
                assert(_ofs.is_open());
                _cur_fsize = 0;
            }
            _ofs.write(data, len);
            assert(_ofs.good());
            _cur_fsize += len;
        }

    private:
        // 判断文件大小, 超过指定大小就创建新文件
        std::string createNewFile()
        {
            // 获取系统时间, 以时间来构造文件扩展名
            time_t t = util::Date::now();
            struct tm lt;
            localtime_r(&t, &lt);
            std::stringstream filename;
            filename << _basename;
            filename << lt.tm_year + 1900;
            filename << lt.tm_mon + 1;
            filename << lt.tm_mday;
            filename << lt.tm_hour;
            filename << lt.tm_min;
            filename << lt.tm_sec;
            filename << "-";
            filename << _name_count++;
            filename << ".log";
            return filename.str();
        }

    private:
        std::string _basename; // 基础文件名  --> 文件名 = 基础文件名 + 扩展文件名
        std::ofstream _ofs;
        size_t _max_fsize; // 指定文件可写入的最大大小
        size_t _cur_fsize; // 当前文件大小
        size_t _name_count;
    };

    /*
        扩展一个以时间作为日志文件滚动切换类型的日志文件
            1. 以时间进行文件滚动, 实际上是以时间段进行滚动
                实现思想: 以当前系统时间, 取模时间段大小, 可以得到当前时间段是第几个时间段
                    每次以当前系统时间取模, 判断与当前文件的时间段是否一致, 不一致代表不是同一个时间段
    */

    enum class TimeGap
    {
        GAP_SECOND,
        GAP_MINUTE,
        GAP_HOUR,
        GAP_DAY
    };

    // 3. 滚动文件扩展 --> (以时间进行滚动)
    class FileByTimeSink : public zx::LogSink
    {
    public:
        // 构造时传入文件名, 并打开文件, 将操作句柄给管理起来
        FileByTimeSink(const std::string &basename, TimeGap gap_type)
            : _basename(basename)
        {
            switch (gap_type)
            {
            case TimeGap::GAP_SECOND:
                _gap_size = 1;
                break;
            case TimeGap::GAP_MINUTE:
                _gap_size = 60;
                break;
            case TimeGap::GAP_HOUR:
                _gap_size = 3600;
                break;
            case TimeGap::GAP_DAY:
                _gap_size = 3600 * 24;
                break;
            default:
                break;
            }
            // 获取当前是第几个时间段
            _cur_gap = _gap_size == 1 ? zx::util::Date::now() : zx::util::Date::now() % _gap_size;
            std::string filename = createNewFile();
            // 1. 创建日志文件所在的目录
            zx::util::File::createDirectory(zx::util::File::path(filename));
            // 2. 创建并打开文件
            _ofs.open(filename, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }

        // 将日志消息写入指定文件
        void log(const char *data, size_t len)
        {
            time_t cur = zx::util::Date::now();
            if ((cur % _gap_size) != _cur_gap)
            {
                // 关闭已经打开的文件
                _ofs.close();
                std::string filename = createNewFile();
                _ofs.open(filename, std::ios::binary | std::ios::app);
                assert(_ofs.is_open());
            }
            _ofs.write(data, len);
            assert(_ofs.good());
        }

    private:
        std::string createNewFile()
        {
            // 获取系统时间, 以时间来构造文件扩展名
            time_t t = zx::util::Date::now();
            struct tm lt;
            localtime_r(&t, &lt);
            std::stringstream filename;
            filename << _basename;
            filename << lt.tm_year + 1900;
            filename << lt.tm_mon + 1;
            filename << lt.tm_mday;
            filename << lt.tm_hour;
            filename << lt.tm_min;
            filename << lt.tm_sec;
            filename << ".log";
            return filename.str();
        }

    private:
        std::string _basename; // 基础文件名  --> 文件名 = 基础文件名 + 扩展文件名
        std::ofstream _ofs;
        size_t _cur_gap;  // 当前是第几个时间段
        size_t _gap_size; // 时间段的大小
    };

    // 简单工厂模式
    class SinkFactory
    {
    public:
        template <typename SinkType, typename... Args>
        static LogSink::ptr create(Args &&...args)
        {
            return std::make_shared<SinkType>(std::forward<Args>(args)...);
        }
    };
} // namespace zx

#endif