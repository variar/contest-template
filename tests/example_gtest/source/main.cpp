#include <logger/logger.hpp>
#include <gmock/gmock.h>

int main(int argc, char *argv[])
{
	::testing::InitGoogleMock(&argc, argv);
    std::unique_ptr<logger::ILogger> logger{logger::createMainLogger()};
	return RUN_ALL_TESTS();
}
