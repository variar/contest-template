#pragma once

#include <memory>
#include <logger_export.h>

namespace logger
{
    class LOGGER_EXPORT ILogger 
    {
    public:
        virtual ~ILogger() {};
        virtual void Trace(const std::string& message) = 0;
        virtual void Error(const std::string& message) = 0;
    };
    
    LOGGER_EXPORT ILogger* createMainLogger();
    LOGGER_EXPORT ILogger* getLogger(const std::string& module);
}
