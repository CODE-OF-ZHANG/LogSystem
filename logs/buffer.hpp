/*
    实现异步日志缓冲区
*/

#ifndef __M_BUFFER_H__
#define __M_BUFFER_H__
#include "logger.hpp"

namespace zx
{
#define DEFAULT_BUFFER_SIZE (1 * 1024 * 1024)
#define THRESHOLD_BUFFER_SIZE (8 * 1024 * 1024)
#define INCREMENT_BUFFER_SIZE (1 * 1024 * 1024)
    class Buffer
    {
    public:
        Buffer() : _buffer(DEFAULT_BUFFER_SIZE), _reader_idx(0), _write_idx(0) {};
        // 向缓冲区写入数据
        void push(const char *data, size_t len)
        {
            /*
                缓冲区剩余空间不够的情况
                    1. 扩容
                        1. 固定大小, 则直接返回
                    2. 动态空间, 用于极限性能测试 --> 扩容

            */
            // if (len > writeAbleSize())
            //     return;
            ensureEnoughSize(len);
            // 将数据拷贝进缓冲区
            std::copy(data, data + len, &_buffer[_write_idx]);
            // 将当前写入位置向后偏移
            moveWriter(len);
        }

        size_t writeAbleSize()
        {
            // 该接口仅对固定缓冲区大小提供
            return (_buffer.size() - _write_idx);
        }

        // 返回可读数据的起始地址
        const char *begin()
        {
            return &_buffer[_reader_idx];
        }

        // 返回可读数据的长度
        size_t readAbleSize()
        {
            return (_write_idx - _reader_idx);
        }

        // 对读指针进行向后偏移的操作
        void moveReader(size_t len)
        {
            assert(len <= readAbleSize());
            _reader_idx += len;
        }

        // 重置读写位置
        void reset() { _write_idx = _reader_idx = 0; }

        // 对Buffer实现交换操作
        void swap(Buffer &buffer)
        {
            _buffer.swap(buffer._buffer);
            std::swap(_reader_idx, buffer._reader_idx);
            std::swap(_write_idx, buffer._write_idx);
        }

        // 判断缓冲区是否为空S
        bool empty()
        {
            return (_reader_idx == _write_idx);
        }

    private:
        // 对写指针进行向后偏移的操作
        void moveWriter(size_t len)
        {
            assert((len + _write_idx) <= _buffer.size());
            _write_idx += len;
        }

        // 对空间进行扩容
        void ensureEnoughSize(size_t len)
        {
            if (len <= writeAbleSize())
                return;
            size_t new_size = 0;
            if (_buffer.size() < THRESHOLD_BUFFER_SIZE)
                new_size = _buffer.size() * 2 + len;
            else
                new_size = _buffer.size() + INCREMENT_BUFFER_SIZE + len;
            _buffer.resize(new_size);
        }

    private:
        std::vector<char> _buffer;
        size_t _reader_idx; // 当前刻度数据的指针
        size_t _write_idx;  // 当前可写数据的指针
    };
} // namespace zx

#endif