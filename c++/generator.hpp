#ifndef AOC_GENERATOR_HPP
#define AOC_GENERATOR_HPP

#include <cassert>
#include <concepts>
#include <coroutine>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>

// A simple stopgap implementation of std::generator<> from C++23

namespace aoc {

template <typename T>
class [[nodiscard]] generator
    : public std::ranges::view_interface<generator<T>>
{
public:
    struct promise_type {
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        auto get_return_object()
        {
            return generator { *this };
        }

        void return_void() {}

        void unhandled_exception()
        {
            throw;
        }

        auto yield_value(T val)
        {
            curr_val = val;
            return std::suspend_always {};
        }

        T curr_val;
    };

    generator() = delete;

    generator(generator && src) noexcept
        : coro(std::exchange(src.coro, {}))
    {}

    generator & operator=(generator && src) noexcept
    {
        generator tmp(std::move(src));
        std::swap(coro, tmp.coro);
        return *this;
    }

    ~generator()
    {
        if (coro)
            coro.destroy();
    }

    class iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;

        bool operator==(std::default_sentinel_t) const
        {
            return coro.done();
        }

        iterator & operator++()  // prefix
        {
            coro.resume();
            return *this;
        }

        void operator++(int)  // postfix
        {
            operator++();
        }

        T operator*() const
        {
            return coro.promise().curr_val;
        }

    private:
        explicit iterator(std::coroutine_handle<promise_type> coro) noexcept
            : coro(coro)
        {}

        std::coroutine_handle<promise_type> coro;
        friend class generator;
    };

    iterator begin()
    {
        assert(coro);
        coro.resume();
        return iterator { coro };
    }

    auto end() const noexcept
    {
        return std::default_sentinel;
    }

private:
    explicit generator(promise_type & p)
        : coro(std::coroutine_handle<promise_type>::from_promise(p))
    {}

    std::coroutine_handle<promise_type> coro;
};

}

#endif
