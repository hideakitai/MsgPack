#pragma once

#ifndef ARX_SMART_PTR_SHARED_PTR_H
#define ARX_SMART_PTR_SHARED_PTR_H

#include <Arduino.h>
#include "detail/shared_count.h"

// imported and simplified from https://github.com/boostorg/smart_ptr

namespace arx {

    template<class T> class shared_ptr;

    namespace detail
    {
        typedef decltype(nullptr) sp_nullptr_t;

        template<class T> T&& move(T& t) { return static_cast<T&&>(t); }

        template<class T> T forward(T t) { return t; }

        // sp_element, element_type

        template<class T> struct sp_element { typedef T type; };
        template<class T> struct sp_element<T[]> { typedef T type; };
        template<class T, size_t N> struct sp_element<T[N]> { typedef T type; };

        // sp_dereference, return type of operator*

        template<class T> struct sp_dereference { typedef T& type; };
        template<> struct sp_dereference<void> { typedef void type; };
        template<> struct sp_dereference<void const> { typedef void type; };
        template<> struct sp_dereference<void volatile> { typedef void type; };
        template<> struct sp_dereference<void const volatile> { typedef void type; };
        template<class T> struct sp_dereference<T[]> { typedef void type; };
        template<class T, size_t N> struct sp_dereference<T[N]> { typedef void type; };

        // sp_member_access, return type of operator->

        template<class T> struct sp_member_access { typedef T* type; };
        template<class T> struct sp_member_access<T[]> { typedef void type; };
        template<class T, size_t N> struct sp_member_access<T[N]> { typedef void type; };

        // sp_array_access, return type of operator[]

        template<class T> struct sp_array_access { typedef void type; };
        template<class T> struct sp_array_access<T[]> { typedef T& type; };
        template<class T, size_t N> struct sp_array_access<T[N]> { typedef T& type; };

        template <class T>
        void swap(T& a, T& b)
        {
            T t = move(a);
            a = move(b);
            b = move(t);
        }

    }

    template<class T> class shared_ptr
    {
    public:

        typedef typename detail::sp_element<T>::type element_type;

    private:

        typedef shared_ptr<T> this_type;

        template<class Y> friend class shared_ptr;

        element_type* px;
        detail::shared_count pn;

    public:

        shared_ptr() : px(0), pn()
        {
        }

        shared_ptr(detail::sp_nullptr_t) : px(0), pn()
        {
        }

        template<class Y>
        explicit shared_ptr(Y* p): px(p), pn()
        {
            detail::shared_count(p).swap(pn);
        }

        shared_ptr(const shared_ptr& r) : px(r.px), pn(r.pn)
        {
        }

        template<class Y>
        shared_ptr(const shared_ptr<Y>& r)
        : px(r.px), pn(r.pn)
        {
        }

        template<class Y>
        shared_ptr(const shared_ptr<Y>& r, element_type* p) : px(p), pn(r.pn)
        {
        }

        shared_ptr& operator= (const shared_ptr& r)
        {
            this_type(r).swap(*this);
            return *this;
        }

        template<class Y>
        shared_ptr& operator= (const shared_ptr<Y>& r)
        {
            this_type(r).swap(*this);
            return *this;
        }

        shared_ptr(shared_ptr&& r) : px(r.px), pn()
        {
            pn.swap(r.pn);
            r.px = 0;
        }

        template<class Y>
        shared_ptr(shared_ptr<Y>&& r)
        : px(r.px), pn()
        {
            pn.swap( r.pn );
            r.px = 0;
        }

        shared_ptr& operator= (shared_ptr&& r)
        {
            this_type(static_cast<shared_ptr&&>(r)).swap(*this);
            return *this;
        }

        template<class Y>
        shared_ptr & operator= (shared_ptr<Y>&& r)
        {
            this_type(static_cast<shared_ptr<Y>&&>(r)).swap(*this);
            return *this;
        }

        template<class Y>
        shared_ptr(shared_ptr<Y>&& r, element_type* p) : px(p), pn()
        {
            pn.swap( r.pn );
            r.px = 0;
        }

        shared_ptr& operator= (detail::sp_nullptr_t)
        {
            this_type().swap(*this);
            return *this;
        }

        void reset()
        {
            this_type().swap(*this);
        }

        template<class Y>
        void reset(Y* p)
        {
            this_type(p).swap(*this);
        }

        template<class Y>
        void reset(const shared_ptr<Y>& r, element_type* p)
        {
            this_type(r, p).swap(*this);
        }

        template<class Y>
        void reset(shared_ptr<Y>&& r, element_type* p)
        {
            this_type(static_cast<shared_ptr<Y>&&>(r), p).swap(*this);
        }

        typename detail::sp_dereference<T>::type operator* () const
        {
            return *px;
        }

        typename detail::sp_member_access<T>::type operator-> () const
        {
            return px;
        }

        typename detail::sp_array_access<T>::type operator[] (size_t i) const
        {
            return static_cast<typename detail::sp_array_access<T>::type>(px[i]);
        }

        element_type* get() const
        {
            return px;
        }

        explicit operator bool () const
        {
            return px != 0;
        }

        bool operator! () const
        {
            return px == 0;
        }

        bool unique() const
        {
            return pn.unique();
        }

        long use_count() const
        {
            return pn.use_count();
        }

        void swap(shared_ptr& other)
        {
            detail::swap(px, other.px);
            pn.swap(other.pn);
        }

    };  // shared_ptr



    template<class T, class U>
    inline bool operator== (const shared_ptr<T>& a, const shared_ptr<U>& b)
    {
        return a.get() == b.get();
    }

    template<class T, class U>
    inline bool operator!= (const shared_ptr<T>& a, const shared_ptr<U>& b)
    {
        return a.get() != b.get();
    }

    template<class T>
    inline bool operator== (const shared_ptr<T>& p, detail::sp_nullptr_t)
    {
        return p.get() == 0;
    }

    template<class T>
    inline bool operator== (detail::sp_nullptr_t, const shared_ptr<T>& p )
    {
        return p.get() == 0;
    }

    template<class T>
    inline bool operator!= (const shared_ptr<T>& p, detail::sp_nullptr_t)
    {
        return p.get() != 0;
    }

    template<class T>
    inline bool operator!= (detail::sp_nullptr_t, const shared_ptr<T>& p )
    {
        return p.get() != 0;
    }

    template<class T>
    inline void swap(const shared_ptr<T>& a, const shared_ptr<T>& b)
    {
        a.swap(b);
    }

    template<class T, class U>
    shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r)
    {
        (void)static_cast<T*>(static_cast<U*>(0));

        typedef typename shared_ptr<T>::element_type E;
        E* p = static_cast<E*>(r.get());
        return shared_ptr<T>(r, p);
    }

    template<class T, class U>
    shared_ptr<T> const_pointer_cast(const shared_ptr<U>& r)
    {
        (void)const_cast<T*>(static_cast<U*>(0));

        typedef typename shared_ptr<T>::element_type E;
        E* p = const_cast<E*>(r.get());
        return shared_ptr<T>(r, p);
    }

    template<class T, class U>
    shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r)
    {
        (void)dynamic_cast<T*>(static_cast<U*>(0));

        typedef typename shared_ptr<T>::element_type E;
        E* p = dynamic_cast<E*>(r.get());
        return p ? shared_ptr<T>(r, p): shared_ptr<T>();
    }

    template<class T, class U>
    shared_ptr<T> reinterpret_pointer_cast(const shared_ptr<U>& r)
    {
        (void)reinterpret_cast<T*>(static_cast<U*>(0) );

        typedef typename shared_ptr<T>::element_type E;
        E* p = reinterpret_cast<E*>(r.get());
        return shared_ptr<T>(r, p);
    }

    template<class T, class U>
    shared_ptr<T> static_pointer_cast(shared_ptr<U>&& r)
    {
        (void)static_cast<T*>(static_cast<U*>(0));

        typedef typename shared_ptr<T>::element_type E;
        E* p = static_cast<E*>(r.get());
        return shared_ptr<T>(detail::move(r), p);
    }

    template<class T, class U>
    shared_ptr<T> const_pointer_cast(shared_ptr<U>&& r)
    {
        (void)const_cast<T*>(static_cast<U*>(0));

        typedef typename shared_ptr<T>::element_type E;
        E* p = const_cast<E*>(r.get());
        return shared_ptr<T>(detail::move(r), p);
    }

    template<class T, class U>
    shared_ptr<T> dynamic_pointer_cast(shared_ptr<U>&& r)
    {
        (void)dynamic_cast<T*>(static_cast<U*>(0));

        typedef typename shared_ptr<T>::element_type E;
        E* p = dynamic_cast<E*>(r.get());
        return p ? shared_ptr<T>(detail::move(r), p) : shared_ptr<T>();
    }

    template<class T, class U>
    shared_ptr<T> reinterpret_pointer_cast(shared_ptr<U>&& r)
    {
        (void)reinterpret_cast<T*>(static_cast<U*>(0));

        typedef typename shared_ptr<T>::element_type E;
        E* p = reinterpret_cast<E*>(r.get());
        return shared_ptr<T>(detail::move(r), p);
    }


    // make_shared : for compatibility, just an alias of constructor

    template<class T>
    shared_ptr<T> make_shared()
    {
        return shared_ptr<T>(new T);
    }

    template<class T, class... Args>
    shared_ptr<T> make_shared(Args&&... args)
    {
        return shared_ptr<T>(new T(detail::forward<Args>(args)...));
    }

} // arx

#endif // ARX_SMART_PTR_SHARED_PTR_H
