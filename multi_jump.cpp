/*
 * multi_jump.cpp
 *
 *  Created on: Nov 12, 2019
 *      Author: frank
 */
#include <iostream>
#include <string>
#include <cassert>
#include <boost/context/detail/fcontext.hpp>

using namespace std;
using namespace boost::context::detail;

const int times = 10000000;

void f1(transfer_t t) {
	int* p = (int*)t.data;
	t = jump_fcontext(t.fctx, nullptr);
	for(int i = 1; i < times; ++i) {
		*p = i;
		t = jump_fcontext(t.fctx, nullptr);
	}
}

int main() {
    string buff(4096, '\0');
    fcontext_t ctx = make_fcontext(&buff[4095], buff.length(), f1);
    int value1 = 0;
    transfer_t t = jump_fcontext(ctx, &value1);
    int i;
    for(i = 1; i < times; ++i) {
    	t = jump_fcontext(t.fctx, &value1);
    	assert(value1 == i);
    }
    assert(i == times);
}

