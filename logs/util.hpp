#ifndef __M_UTIL_H__
#define __M_UTIL_H__
/*
 *  实用工具类的实现
 *    1. 获取系统时间
 *    2. 判断文件是否存在
 *    3. 获取文件所在路径
 *    4. 创建目录
 */

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <ctime>
// #include <unistd.h>

namespace zx
{
    namespace util
    {
        // 时间类
        class Date
        {
        public:
            static size_t now()
            {
                return (size_t)time(nullptr);
            }
        };

        // 文件类
        class File
        {
        public:
            // 判断文件是否存在
            static bool exists(const std::string &pathname)
            {
                struct stat st;
                if (stat(pathname.c_str(), &st) < 0)
                    return false;
                return true;

                // 系统调用接口, 不能跨平台使用
                // return (access(pathname.c_str(), F_OK) == 0);
            }

            // 获取文件所在路径
            static std::string path(const std::string &pathname)
            {
                size_t pos = pathname.find_last_of("/\\");
                // 当前目录
                if (pos == std::string::npos)
                    return ".";
                return pathname.substr(0, pos + 1);
            }

            // 创建目录
            static void createDirectory(const std::string &pathname)
            {
                // ./abc/bcd/def/a.txt
                size_t pos = 0, idx = 0;
                while (idx < pathname.size())
                {
                    pos = pathname.find_first_of("/\\", idx);
                    if (pos == std::string::npos)
                        mkdir(pathname.c_str(), 0777);
                    std::string parent_dir = pathname.substr(0, pos + 1);
                    // if (parent_dir == "." || parent_dir == "..")
                    // {
                    //     idx = pos + 1;
                    //     continue;
                    // }

                    if (exists(parent_dir) == true)
                    {
                        idx = pos + 1;
                        continue;
                    }
                    mkdir(parent_dir.c_str(), 0777);
                    idx = pos + 1;
                }
            }
        };
    }
}

#endif