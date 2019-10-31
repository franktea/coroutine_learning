/*
 * generator1.cpp
 *
 *  Created on: Oct 31, 2019
 *      Author: frank
 */
#include <experimental/coroutine>
#include <memory>
#include <iostream>
#include <optional>

using namespace std;
using namespace std::experimental;

template <typename T>
struct generator {
    struct promise_type {
        suspend_never initial_suspend() { return {}; }
        suspend_always final_suspend() { return {}; }

        auto yield_value(T v) {
             val.emplace(std::move(v));
             return suspend_always();
        }

        auto get_return_object() { return generator(this); }

        void return_void() {}
        void unhandled_exception() { throw; }

        auto coro() { return coroutine_handle<promise_type>::from_promise(*this); }

        std::optional<T> val;
    };

    generator(generator&& source) : p(std::exchange(source.p, nullptr)) {}
    explicit generator(promise_type* p) :p(p) {}
    ~generator() {
        if (p) p->coro().destroy();
    }

    // generator<T> is a Range. You can use it in a range-for loop.
    struct EndSentinel{};
    auto end() { return EndSentinel(); }
    auto begin() {
        struct Iter {
            // You probably want to add iterator_tag and friends.
            // That isn't needed if you only want to support range-for.
            bool operator!=(EndSentinel) const { return !p->coro().done(); }
            void operator++() { p->coro().resume(); }
            T& operator*() const { return *p->val; }

            promise_type* p;
        };
        return Iter{p};
    }

    promise_type* p;
};

generator<int> f()
{
    for(int i = 0; i < 10; ++i)
    {
        co_yield i;
    }
}

int main()
{
    auto ints = f();
    for(auto&& i: ints)
    {
        cout<<i<<", ";
    }
    cout<<"\n";
    return 0;
}




