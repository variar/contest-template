#include <dummy/dummy.h>

#include <boost/any.hpp>

#include <pplx/pplxtasks.h>
#include <iostream>

Dummy::Dummy() 
{
    auto task = pplx::create_task([](){ std::cout << "PPL task" << std::endl;});
    task.get();
}
