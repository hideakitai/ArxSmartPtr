#pragma once

#ifndef ARX_SMART_PTR_H
#define ARX_SMART_PTR_H

#include <Arduino.h>

#if defined(ARDUINO_ARCH_AVR)\
 || defined(ARDUINO_ARCH_MEGAAVR)\
 || defined(ARDUINO_ARCH_SAMD)\
 || defined(ARDUINO_spresense_ast)
    #define ARX_SMART_PTR_DISABLED
#endif

#ifdef ARX_SMART_PTR_DISABLED

// reference : https://setuna-kanata.hatenadiary.org/entry/20081128/1227890991

namespace arx {

    class sp_counted_base
    {
        long count;

    public:

        sp_counted_base() : count(1) {}
        virtual ~sp_counted_base() {}

        void addref() { ++count; }
        void release()
        {
            if(--count == 0)
            {
                dispose();
                delete this;
            }
        }

        long use_count() const { return count; }

        virtual void dispose() = 0;
    };


    template <typename T>
    class sp_counted_impl : public sp_counted_base
    {
        T* ptr;
    public:
        sp_counted_impl(T* ptr) : ptr(ptr) {}
        virtual void dispose() override { delete ptr; }
    };


    template <typename T, typename Deleter>
    class sp_counted_impl_del : public sp_counted_base
    {
        T* ptr;
        Deleter deleter;
    public:
        sp_counted_impl_del(T* ptr, Deleter deleter)
        : ptr(ptr)
        , deleter(deleter)
        {}
        virtual void dispose() override { deleter(ptr); }
    };


    class shared_count
    {
        sp_counted_base* pimpl;

    public:

        shared_count() : pimpl(0) {}

        template <typename T>
        explicit shared_count( T* ptr )
        : pimpl(new sp_counted_impl<T>(ptr))
        {}

        template <typename T, typename Deleter>
        explicit shared_count(T* ptr, Deleter deleter )
        : pimpl(new sp_counted_impl_del<T, Deleter>(ptr, deleter))
        {}

        ~shared_count()
        {
            if(pimpl) pimpl->release();
        }

        shared_count(shared_count const& sc)
        : pimpl(sc.pimpl)
        {
            if(pimpl) pimpl->addref();
        }

        long use_count() const { return pimpl->use_count(); }

        shared_count& operator=(shared_count const& sc)
        {
            sp_counted_base* temp = sc.pimpl;
            if(temp != pimpl)
            {
                if(temp)  temp->addref();
                if(pimpl) pimpl->release();
                pimpl = temp;
            }
            return *this;
        }
    };


    template <typename T>
    struct shared_ptr_traits
    {
        typedef T& reference;
    };
    template<>
    struct shared_ptr_traits<void>
    {
        typedef void reference;
    };


    template <typename T>
    class shared_ptr
    {
        T* ptr;
        shared_count count;

    public:

        typedef typename shared_ptr_traits< T >::reference reference;
        template <typename U> friend class shared_ptr;

        shared_ptr()
        : ptr(nullptr)
        , count()
        {}

        template <typename U>
        explicit shared_ptr(U* ptr)
        : ptr(ptr)
        , count(ptr)
        {}

        template <typename U>
        shared_ptr(shared_ptr<U> const& sp)
        : ptr(sp.ptr)
        , count(sp.count)
        {}

        template <typename U, typename Deleter>
        explicit shared_ptr(U* ptr, Deleter deleter)
        : ptr(ptr)
        , count(ptr, deleter)
        {}

        T* get() const { return ptr; }

        long use_count() const { return count.use_count(); }

        explicit operator bool() const { return ptr != nullptr; }
        reference operator*() const { return *ptr; }
        T* operator->() const { return ptr; }
    };


    // just an alias of shared_ptr<T>(new T())
    template<typename T, typename... Args>
    inline shared_ptr<T> make_shared(Args&&... args)
    {
        return shared_ptr<T>(new T(args)...);
    }

} // arx

#endif // ARX_SMART_PTR_DISABLED
#endif // ARX_SMART_PTR_H
