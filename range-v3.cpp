/*
 * range-v3.cpp
 *
 *  Created on: Nov 3, 2019
 *      Author: frank
 */

#include <iostream>
#include <range/v3/all.hpp>
#include <range/v3/experimental/utility/generator.hpp>

using namespace ranges;
using namespace ranges::experimental;

generator<unsigned short> ushorts()
{
  unsigned short u = 0;
  do { co_yield u; } while (++u);
}

int main()
{
  auto evens = ushorts()
             | views::filter([](auto i) {
                   return (i % 2) == 0; })
  	  	  	 | views::take(10);

  copy( evens, ostream_iterator<>(std::cout, "\n") );
}



