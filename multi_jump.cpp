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

void f1(transfer_t t) {
	string* p = (string*)t.data;
    p->append("|1");
    transfer_t t2 = jump_fcontext(t.fctx, nullptr);
    p->append("|2");
    jump_fcontext(t2.fctx, nullptr);
}

int main() {
    string buff(4096, '\0');
    fcontext_t ctx = make_fcontext(&buff[4095], buff.length(), f1);
    string value1 = "hello";
    transfer_t t = jump_fcontext(ctx, &value1);
    assert(value1 == "hello|1");
    jump_fcontext(t.fctx, &value1);
    assert(value1 == "hello|1|2");
}

