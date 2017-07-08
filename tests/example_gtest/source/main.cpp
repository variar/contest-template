#include <logger/logger.hpp>
#include <gmock/gmock.h>

#include <memory>

int main(int argc, char *argv[])
{
	::testing::InitGoogleMock(&argc, argv);
    std::unique_ptr<logger::ILogger> logger{logger::createMainLogger()};
    logger->Trace("test message");
	return RUN_ALL_TESTS();
}
