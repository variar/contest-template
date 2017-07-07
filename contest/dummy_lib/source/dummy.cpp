#include <dummy/dummy.h>

#include <boost/any.hpp>

#include <iostream>

#include <logger/logger.hpp>

Dummy::Dummy() 
{
    std::unique_ptr<logger::ILogger> logger {logger::getLogger("dummy")};
    logger->Trace("Dummy lib");
}
