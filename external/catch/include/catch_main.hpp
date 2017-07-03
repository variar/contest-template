#pragma once
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "tmock.hpp"
namespace {
void setMockReporter() {
    trompeloeil::set_reporter([](
    trompeloeil::severity s,
    const char* file,
    unsigned long line,
    const std::string& msg)
  {
    std::ostringstream os;
    if (line) os << file << ':' << line << '\n';
    os << msg;
    auto failure = os.str();
    if (s == trompeloeil::severity::fatal)
    {
      FAIL(failure);
    }
    else
    {
      CAPTURE(failure);
      CHECK(failure.empty());
    }
  });
}
}

#if defined(WIN32) && defined(_UNICODE) && !defined(DO_NOT_USE_WMAIN)
// Standard C/C++ Win32 Unicode wmain entry point
extern "C" int wmain (int argc, wchar_t * argv[], wchar_t * []) {
#else
// Standard C/C++ main entry point
int main (int argc, char * argv[]) {
#endif
    setMockReporter();
    int result = Catch::Session().run( argc, argv );
    return ( result < 0xff ? result : 0xff );
}


