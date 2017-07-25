#include <dll_template/dummy_dll.h>
#include <plog/Log.h>

#include <boost/stacktrace.hpp>

DummyDll::DummyDll() 
{
    LOG_INFO << "Dummy dll constructed";

    LOG_INFO << "Stack from dll " << boost::stacktrace::stacktrace();
}
