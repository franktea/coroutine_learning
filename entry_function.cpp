#include <iostream>
#include <boost/context/detail/fcontext.hpp>

using namespace std;
using namespace boost::context::detail;

int value1;

void f1( ctx::transfer_t t) {
    ++value1;
    ctx::jump_fcontext( t.fctx, t.data);
}

int main() {

}
