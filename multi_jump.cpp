/*
 * multi_jump.cpp
 *
 *  Created on: Nov 12, 2019
 *      Author: frank
 */
#include <iostream>
#include <string>
#include <boost/context/detail/fcontext.hpp>

using namespace std;
using namespace boost::context::detail;

void f1(transfer_t t) {
	string* p = (string*)t.data;
    p->operator[](0) = 'f';
    p->operator[](1) = '1';
    jump_fcontext( t.fctx, t.data);
}

int main() {
    string buff(4096, '\0');
    fcontext_t ctx = make_fcontext(&buff[0], buff.length(), f1);
    string value1 = "hello";
    jump_fcontext(ctx, &value1);
    assert(value1 == "f1llo");
}

