#include <map>
#include <iostream>
#include <chrono>
#include <thread>
#include <coroutine>

using namespace std::chrono_literals;

class TimerScheduler {
public:
    // Run，在主循环里面调用
    void Run() {
        auto now = std::chrono::steady_clock::now();
        while(! timer_map_.empty() && timer_map_.begin()->first <= now) {
            timer_map_.begin()->second.resume();
            timer_map_.erase(timer_map_.begin());
        }
    }

    void Insert(std::chrono::steady_clock::duration d, std::coroutine_handle<> h) {
        auto t = std::chrono::steady_clock::now() + d;
        TimerScheduler::Instance().timer_map_.insert(std::make_pair(t, h));
    }

    static TimerScheduler& Instance() {
        static TimerScheduler tm;
        return tm;
    }
private:
    TimerScheduler() {}
private:
    std::map<std::chrono::time_point<std::chrono::steady_clock>, std::coroutine_handle<>> timer_map_;
};

// 重载运算符co_await，这样就可以支持co_await 10ms这样的语法了
auto operator co_await(std::chrono::steady_clock::duration d) noexcept {
    struct awaitable_timer {
        std::chrono::steady_clock::duration d;
        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<> h) noexcept {
            // 将写成handle保存在timer scheduler中，
            // 使得scheduler可以在必要的时候调用h.resume()
            TimerScheduler::Instance().Insert(d, h);
        }
        void await_resume() noexcept { }
    };
    return awaitable_timer { d };
}

struct Task {
    struct promise_type {
        promise_type() = default;
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; } 
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
    };
};

Task TimerTest() {
    std::cout<<"begin\n";

    co_await 100ms;
    std::cout<<"after 100ms\n";

    co_await 1s;
    std::cout<<"after 1s\n";
};

int main() {
    TimerTest();

    while(1) { // 主循环
        TimerScheduler::Instance().Run(); // 在这里run

        // 如果还有其它scheduler，也在这里run

        // 做其它事情，这个例子没啥事做，就sleep一下吧
        std::this_thread::sleep_for(1ms);
    }
}
