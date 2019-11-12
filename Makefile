all:
	clang++ -std=c++2a -fcoroutines-ts -stdlib=libc++ generator1.cpp -o generator1
	clang++ -std=c++2a -fcoroutines-ts -stdlib=libc++ generator2.cpp -o generator2
	clang++ -std=c++2a -fcoroutines-ts -stdlib=libc++ generator3.cpp -o generator3
	clang++ -std=c++2a -fcoroutines-ts -lstdc++ co_vs_callback.cpp -o co_vs_call_back
	clang++ -std=c++2a -fcoroutines-ts -lstdc++ range-v3.cpp -o range-v3
	clang++ -std=c++2a -lstdc++ entry_function.cpp -o entry_function -lboost_context-mt
	
clean:
	rm -f generator1 generator2 generator3 co_vs_call_back range-v3 entry_function
	


