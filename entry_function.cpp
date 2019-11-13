#include <iostream>
#include <string>
#include <cassert>
#include <boost/context/detail/fcontext.hpp>

using namespace std;
using namespace boost::context::detail;

void f1(transfer_t t) {
    int* p = (int*)t.data;
    ++(*p);
    jump_fcontext( t.fctx, t.data);
}

int main() {
    string buff(4096, '\0');
    fcontext_t ctx = make_fcontext(&buff[4095], buff.size(), f1);
    int value1 = 0;
    jump_fcontext(ctx, &value1);
    assert(value1 == 1);
}
