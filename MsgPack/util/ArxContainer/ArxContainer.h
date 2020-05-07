#pragma once

#ifndef ARX_RINGBUFFER_H
#define ARX_RINGBUFFER_H

#include <Arduino.h>

#if defined(ARDUINO_ARCH_AVR)\
 || defined(ARDUINO_ARCH_MEGAAVR)\
 || defined(ARDUINO_ARCH_SAMD)\
 || defined(ARDUINO_spresense_ast)
    #define ARX_SMART_PTR_DISABLED
#endif

#ifdef ARX_SMART_PTR_DISABLED

#ifndef ARX_VECTOR_DEFAULT_SIZE
#define ARX_VECTOR_DEFAULT_SIZE 16
#endif // ARX_VECTOR_DEFAULT_SIZE

#ifndef ARX_DEQUE_DEFAULT_SIZE
#define ARX_DEQUE_DEFAULT_SIZE 16
#endif // ARX_DEQUE_DEFAULT_SIZE

#ifndef ARX_MAP_DEFAULT_SIZE
#define ARX_MAP_DEFAULT_SIZE 16
#endif // ARX_MAP_DEFAULT_SIZE

namespace arx {

    template<typename T, size_t N>
    class RingBuffer
    {
        T queue_[N];
        int head_ {0};
        int tail_ {0};

    public:

        virtual ~RingBuffer() {}

        using iterator = T*;

        size_t capacity() const { return N; };
        size_t size() const { return tail_ - head_; }
        inline const T* data() const { return &(get(head_)); }
        T* data() { return &(get(head_)); }
        bool empty() const { return tail_ == head_; }
        void clear() { head_ = 0; tail_ = 0; }

        void pop() { pop_front(); }
        void pop_front()
        {
            if (size() == 0) return;
            if (size() == 1) clear();
            else head_++;
        }
        void pop_back()
        {
            if (size() == 0) return;
            if (size() == 1) clear();
            else tail_--;
        }

        void push(const T& data) { push_back(data); }
        void push(T&& data) { push_back(data); }
        void push_back(const T& data)
        {
            get(tail_++) = data;
            if (size() > N) ++head_;
        };
        void push_back(T&& data)
        {
            get(tail_++) = data;
            if (size() > N) ++head_;
        };
        void push_front(const T& data)
        {
            get(head_--) = data;
            if (size() > N) --tail_;
        };
        void push_front(T&& data)
        {
            get(head_--) = data;
            if (size() > N) --tail_;
        };
        void emplace(const T& data) { push(data); }
        void emplace(T&& data) { push(data); }
        void emplace_back(const T& data) { push_back(data); }
        void emplace_back(T&& data) { push_back(data); }

        const T& front() const { return get(head_); };
        T& front() { return get(head_); };

        const T& back() const { return get(tail_ - 1); }
        T& back() { return get(tail_ - 1); }

        const T& operator[] (size_t index) const { return get(head_ + (int)index); }
        T& operator[] (size_t index) { return get(head_ + (int)index); }

        iterator begin() { return ptr(head_); }
        iterator end() { return (queue_ + tail_); }

        iterator erase(iterator p)
        {
            if (p == end()) return p;
            for (T* pos = p + 1; pos != end(); ++pos)
                *(pos - 1) = *pos;
            --tail_;
            return p;
        }

        void resize(size_t sz)
        {
            size_t s = size();
            if (sz > size())
            {
                for (size_t i = 0; i < sz - s; ++i) push(T());
            }
            else if (sz < size())
            {
                for (size_t i = 0; i < s - sz; ++i) pop();
            }
        }

        void assign(const iterator first, const iterator end)
        {
            clear();
            const char* p = first;
            while (p != end) push(*p++);
        }

    private:

        T& get(const int i)
        {
            if (i >= 0) return queue_[i % N];
            else        return queue_[N - abs(i % N)];
        }

        const T& get(const int i) const
        {
            if (i >= 0) return queue_[i % N];
            else        return queue_[N - abs(i % N)];
        }

        T* ptr(const int i)
        {
            if (i >= 0) return (T*)(queue_ + i % N);
            else        return (T*)(queue_ + N - abs(i % N));
        }

        const T* ptr(const int i) const
        {
            if (i >= 0) return (T*)(queue_ + i % N);
            else        return (T*)(queue_ + N - abs(i % N));
        }

    };


    template <typename T, size_t N = ARX_VECTOR_DEFAULT_SIZE>
    struct vector : public RingBuffer<T, N>
    {
        virtual ~vector() {}
    private:
        using RingBuffer<T, N>::pop;
        using RingBuffer<T, N>::pop_front;
        using RingBuffer<T, N>::push;
        using RingBuffer<T, N>::push_front;
        using RingBuffer<T, N>::emplace;
    };

    template <typename T, size_t N = ARX_DEQUE_DEFAULT_SIZE>
    struct deque : public RingBuffer<T, N>
    {
        virtual ~deque() {}
    private:
        using RingBuffer<T, N>::capacity;
        using RingBuffer<T, N>::pop;
        using RingBuffer<T, N>::push;
        using RingBuffer<T, N>::emplace;
        using RingBuffer<T, N>::assign;
        using RingBuffer<T, N>::begin;
        using RingBuffer<T, N>::end;
    };

    template <class T1, class T2>
    struct pair
    {
        T1 first;
        T2 second;
    };

    template <class T1, class T2>
    pair<T1, T2> make_pair(const T1& t1, const T2& t2)
    {
        return {t1, t2};
    };

    template <class Key, class T, size_t N = ARX_MAP_DEFAULT_SIZE>
    struct map : public RingBuffer<pair<Key, T>, N>
    {
        using iterator = typename RingBuffer<pair<Key, T>, N>::iterator;

        virtual ~map() {}

        iterator find(const Key& key)
        {
            for (size_t i = 0; i < this->size(); ++i)
            {
                iterator it = this->begin() + i;
                if (key == it->first)
                    return it;
            }
            return this->end();
        }

        pair<iterator, bool> insert(const Key& key, const T& t)
        {
            bool b {false};
            iterator it = find(key);
            if (it == this->end())
            {
                this->push(make_pair(key, t));
                b = true;
                it = this->end() - 1;
            }
            return {it, b};
        }

        pair<iterator, bool> insert(const pair<Key, T>& p)
        {
            bool b {false};
            iterator it = find(p.first);
            if (it == this->end())
            {
                this->push(p);
                b = true;
                it = this->end() - 1;
            }
            return {it, b};
        }

        pair<iterator, bool> emplace(const Key& key, const T& t)
        {
            return insert(key, t);
        }

        pair<iterator, bool> emplace(const pair<Key, T>& p)
        {
            return insert(p);
        }

        const T& at(const Key& key) const
        {
            // iterator it = find(key);
            // if (it != this->end()) return it->second;
            // return T();
            return find(key)->second;
        }

        T& at(const Key& key)
        {
            // iterator it = find(key);
            // if (it != this->end()) return it->second;
            // return T();
            return find(key)->second;
        }

        iterator erase(const iterator it)
        {
            iterator i = find(it->first);
            if (i != this->end())
            {
                return this->erase(i);
            }
            return this->end();
        }

        iterator erase(const size_t index)
        {
            if (index < this->size())
            {
                iterator it = this->begin() + index;
                return this->erase(it);
            }
            return this->end();
        }

        inline T& operator[] (const Key& key)
        {
            iterator it = find(key);
            if (it != this->end()) return it->second;

            insert(::arx::make_pair(key, T()));
            return this->back().second;
        }
    };

} // namespace arx

template<typename T, size_t N>
using ArxRingBuffer = arx::RingBuffer<T, N>;


#endif // ARX_SMART_PTR_DISABLED
#endif // ARX_RINGBUFFER_H
