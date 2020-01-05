/*
 * hanoi.cpp
 *
 *  Created on: Jan 5, 2020
 *      Author: frank
 */

#include <experimental/coroutine>
#include <memory>
#include <iostream>
#include <string>

using namespace std;

template<typename T>
class generator {
public:
    struct promise_type;
    using handle_type = std::experimental::coroutine_handle<promise_type>;
private:
    handle_type coro;
public:
    explicit generator(handle_type h) :
            coro(h) {
    }
    generator(const generator &) = delete;
    generator(generator &&oth) noexcept :
            coro(oth.coro) {
        oth.coro = nullptr;
    }
    generator &operator=(const generator &) = delete;
    generator &operator=(generator &&other) noexcept {
        coro = other.coro;
        other.coro = nullptr;
        return *this;
    }
    ~generator() {
        if (coro) {
            coro.destroy();
        }
    }

    bool next() {
        coro.resume();
        return not coro.done();
    }

    T get_value() {
        return coro.promise().current_value;
    }

    struct promise_type {
    private:
        T current_value { };
        friend class generator;
    public:
        promise_type() = default;
        ~promise_type() = default;
        promise_type(const promise_type&) = delete;
        promise_type(promise_type&&) = delete;
        promise_type &operator=(const promise_type&) = delete;
        promise_type &operator=(promise_type&&) = delete;

        auto initial_suspend() {
            return std::experimental::suspend_always { };
        }

        auto final_suspend() {
            return std::experimental::suspend_always { };
        }

        auto get_return_object() {
            return generator { handle_type::from_promise(*this) };
        }

        auto return_void() {
        	std::cout<<"return void\n";
            return std::experimental::suspend_never { };
        }

        auto yield_value(T some_value) {
            current_value = some_value;
            return std::experimental::suspend_always { };
        }

        void unhandled_exception() {
            std::exit(1);
        }
    };
};

struct DiskInfo {
    int disk_number;
    char from_rod; // 此处柱子的编号简单用char类型'A' 'B' 'C'表示
    char to_rod;
    string ToString() {
    	return std::to_string(disk_number) + string(1, from_rod) + string(1, to_rod);
    }
};

struct HanoiStep : public std::experimental::suspend_never {
	int n;
	char from_rod;
	char to_rod;
	char aux_rod;
};

generator<DiskInfo> TowerOfHanoi(int n, char from_rod,
                    char to_rod, char aux_rod)
{
	std::cout<<"call "<<n<<", "<<from_rod<<", "<<to_rod<<", "<<aux_rod<<"\n";
    if (n == 1)
    {
        co_yield DiskInfo { 1, from_rod, to_rod };
        co_return;
    }

    auto f1 = TowerOfHanoi(n - 1, from_rod, aux_rod, to_rod);
    co_yield DiskInfo { n, from_rod, to_rod };
    co_await TowerOfHanoi(n - 1, aux_rod, to_rod, from_rod);
}

int main()
{
    auto it = TowerOfHanoi(6, 'A', 'B', 'C');
    while(it.next())
    {
        cout<<it.get_value().ToString()<<"\n";
    }
    return 0;
}



