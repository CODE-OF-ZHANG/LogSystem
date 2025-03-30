/*
    1. 定义枚举类 --> 枚举出日志等级
    2. 提供转换接口 --> 将枚举类型转换成对应的字符串
*/
/*防止头文件重复包含*/
#ifndef __M_LEVEL_H__
#define __M_LEVEL_H__

namespace zx
{
    // 定义枚举类
    class LogLevel
    {
    public:
        // 枚举日志等级
        enum class value
        {
            UNKNOW = 0,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            OFF
        };

        // 日志等级转换接口
        static const char* toString(LogLevel::value level)
        {
            switch(level)
            {
                case LogLevel::value::DEBUG:
                    return "DEBUG";
                case LogLevel::value::INFO:
                    return "INFO";
                case LogLevel::value::WARN:
                    return "WARN";
                case LogLevel::value::ERROR:
                    return "ERROR";
                case LogLevel::value::FATAL:
                    return "FATAL";
                case LogLevel::value::OFF:
                    return "OFF";
            }
            return "UNKNOW";
        }
    };
} // namespace zx
#endif