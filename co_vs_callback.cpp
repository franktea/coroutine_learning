/*
 * co_vs_callback.cpp
 *
 *  Created on: Oct 31, 2019
 *      Author: frank
 */
#include <iostream>
#include <thread>
#include <experimental/coroutine>
#include <chrono>
#include <functional>

// clang++ -std=c++2a -fcoroutines-ts -lstdc++ co_vs_callback.cpp

using call_back = std::function<void(int)>;
void Add100ByCallback(int init, call_back f) // 异步调用
{
	std::thread* t = new std::thread([init, f]() {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		f(init + 100);
	});
	t->detach();
}

struct Add100AWaitable
{
	Add100AWaitable(int init):init_(init) {}
	bool await_ready() const { return false; }
	int await_resume() { return result_; }
	void await_suspend(std::experimental::coroutine_handle<> handle)
	{
		auto f = [handle, this](int value) mutable {
			result_ = value;
			handle.resume();
		};
		Add100ByCallback(init_, f); // 调用原来的异步调用
	}
	int init_;
	int result_;
};

struct Task
{
	struct promise_type {
		auto get_return_object() { return Task{this}; }
		auto initial_suspend() { return std::experimental::suspend_never{}; }
		auto final_suspend() { return std::experimental::suspend_never{}; }
		void unhandled_exception() { std::terminate(); }
		//void return_void() {}
		int return_value(int v) { return v; }
		std::experimental::coroutine_handle<> coro() {
			return std::experimental::coroutine_handle<promise_type>::from_promise(*this);
		}
	};
	promise_type* p_ = nullptr;
	Task(promise_type* p):p_(p){
		std::cout<<"constructing................\n";
	}
	~Task() {
		std::cout<<"destructing------------------\n";
		if(p_ && p_->coro().done())
		{
			std::cout<<"destroy~~~~========================>\n";
			p_->coro().destroy();
		}
	}
};

Task Add100ByCoroutine(int init, call_back f)
{
	int ret = co_await Add100AWaitable(init);
	ret = co_await Add100AWaitable(ret);
	ret = co_await Add100AWaitable(ret);
	f(ret);
	co_return ret;
}

int main()
{
	Add100ByCallback(5, [](int value){ std::cout<<"get result from normal: "<<value<<"\n"; });
	Add100ByCoroutine(10, [](int value){ std::cout<<"get result from coroutine1: "<<value<<"\n"; });
	Add100ByCoroutine(20, [](int value){ std::cout<<"get result from coroutine2: "<<value<<"\n"; });
	Add100ByCoroutine(30, [](int value){ std::cout<<"get result from coroutine3: "<<value<<"\n"; });
	Add100ByCoroutine(40, [](int value){ std::cout<<"get result from coroutine4: "<<value<<"\n"; });
	getchar();
}





