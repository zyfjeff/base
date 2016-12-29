//
// Created by tianqian.zyf on 12/17/16.
//

#ifndef CODEBASE_SINGLETON_H_H
#define CODEBASE_SINGLETON_H_H
#include "macros.h"
#include <thread>

/*
 *  desc: 一个单例模版类的实现，借助于std::call_once仅调用一次的语义来保证单例线程安全，传统的加锁方式性能是个问题，
 *  使用Double Check的话在C++98没有内存模型支撑的情况下其正确性无法保证，基于静态成员的方法在C++98是非线程安全的，直到C++11
 *  其行为才是正确的。
 */
template<typename T>
class Singleton {
 public:
    static T* Instance() {
        std::call_once(once_init_, [&](){
            data_ = new T();
        });
        return data_;
    }

    class SingleGc {
     public:
        ~SingleGc() {
            if (Singleton::data_) {
                delete Singleton::data_;
            }
        }
    };
 private:
    DISALLOW_IMPLICIT_CONSTRUCTORS(Singleton);
    static std::once_flag once_init_;
    static T* data_;
    static SingleGc gc_;
};

template <typename T>
std::once_flag Singleton<T>::once_init_;

template <typename T>
T* Singleton<T>::data_ = nullptr;

template <typename T>
typename Singleton<T>::SingleGc Singleton<T>::gc_;

#endif //CODEBASE_SINGLETON_H_H
