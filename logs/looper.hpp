/*实现异步工作器*/

#ifndef __M_LOOPER_H__
#define __M_LOOPER_H__

#include "buffer.hpp"
#include <condition_variable>
#include <functional>
#include <atomic>

namespace zx
{
    using Functor = std::function<void(Buffer &)>;
    enum class AsyncType
    {
        ASYNC_SAFE,  // 安全状态, 满了就阻塞, 避免资源耗尽
        ASYNC_UNSAFE // 非安全转态, 无限扩容, 用于性能测试
    };

    class AsyncLooper
    {
    public:
        using ptr = std::shared_ptr<AsyncLooper>;
        AsyncLooper(const Functor &callback, AsyncType looper_type = AsyncType::ASYNC_SAFE)
            : _stop(false),
              _looper_type(looper_type),
              _thread(std::thread(&AsyncLooper::threadEntry, this)),
              _callBack(callback) {}

        ~AsyncLooper() { stop(); }

        void stop()
        {
            _stop = true;
            _cond_con.notify_all(); // 唤醒所有工作线程
            _thread.join();         // 等待工作线程退出
        }

        void push(const char *data, size_t len)
        {
            // 1. 无线扩容 --> 非安全;  2. 固定大小 --> 生产缓冲区满了就阻塞
            std::unique_lock<std::mutex> lock(_mutex);
            // 条件变量为空, 缓冲区剩余空间大于数据长度, 添加数据
            if (_looper_type == AsyncType::ASYNC_SAFE)
                _cond_pro.wait(lock, [&]()
                               { return _pro_buf.writeAbleSize() >= len; });
            // 添加数据
            _pro_buf.push(data, len);
            // 唤醒消费者缓冲区
            _cond_con.notify_one();
        }

    private:
        // 线程入口函数
        void threadEntry()
        {
            while (1)
            {
                // 判断生产缓冲区有无数据, 有则交换, 无则阻塞
                {
                    // 互斥锁的生命周期
                    std::unique_lock<std::mutex> lock(_mutex);
                    // 退出标志被设置, 且生产缓冲区已经没有数据了再退出
                    if (_stop && _pro_buf.empty())
                        break;

                    _cond_con.wait(lock, [&]()
                                   { return _stop || !_pro_buf.empty(); });
                    _con_buf.swap(_pro_buf);
                    // 唤醒生产者
                    if (_looper_type == AsyncType::ASYNC_SAFE)
                        _cond_pro.notify_all();
                }
                // 唤醒后, 对消费缓冲区进行数据处理
                _callBack(_con_buf);
                // 初始化消费缓冲区
                _con_buf.reset();
            }
        }

    private:
        Functor _callBack;

    private:
        AsyncType _looper_type;
        std::atomic<bool> _stop; // 工作器停止标志
        Buffer _pro_buf;         // 生产缓冲区
        Buffer _con_buf;         // 消费缓冲区
        std::mutex _mutex;
        std::condition_variable _cond_pro;
        std::condition_variable _cond_con;
        std::thread _thread; // 异步工作器对应的工作线程
    };
} // namespace zx

#endif