#include <iostream>
#include <utility>

template<unsigned... args> struct ArrayHolder {
    static const unsigned data[sizeof...(args)];
};

template<unsigned... args>
const unsigned ArrayHolder<args...>::data[sizeof...(args)] = { args... };

template<size_t N, template<size_t> class F, unsigned... args>
struct generate_array_impl {
    typedef typename generate_array_impl<N-1, F, F<N>::value, args...>::result result;
};

template<template<size_t> class F, unsigned... args>
struct generate_array_impl<0, F, args...> {
    typedef ArrayHolder<F<0>::value, args...> result;
};

template<size_t N, template<size_t> class F>
struct generate_array {
    typedef typename generate_array_impl<N-1, F>::result result;
};
