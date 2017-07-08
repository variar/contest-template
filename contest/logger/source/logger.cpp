#include <logger/logger.hpp>

#include <P7_Trace.h>

namespace logger
{
    class Logger : public ILogger
    {
    public:
        explicit Logger(const std::string& module, bool isShared);
        ~Logger();
        
        void Trace(const std::string& message) override;
        void Error(const std::string& message) override;
        
    private:
        IP7_Client* m_client;
        IP7_Trace* m_trace;
        void* m_module;
    };
    
    ILogger* createMainLogger()
    {
        return new Logger("main", false);
    }
    
    ILogger* getLogger(const std::string& module)
    {
        return new Logger(module, true);
    }
    
Logger::Logger(const std::string& module, bool isShared)
    : m_client {nullptr}
    , m_trace {nullptr}
    , m_module {nullptr}
{
    if (!isShared)
    {
        P7_Set_Crash_Handler();
        
        m_client =  P7_Create_Client(TM("/P7.Sink=Console /P7.Format=%tm\t[%mn]\t{%ti}\t%ms"));
        m_trace = P7_Create_Trace(m_client, TM("Trace"));
        
        m_trace->Register_Module(TM("main"), &m_module);
        
        m_trace->Share(TRACE_DEFAULT_SHARED_NAME);
    }
    else 
    {
        m_trace = P7_Get_Shared_Trace(TRACE_DEFAULT_SHARED_NAME);
        m_trace->Register_Module(module.c_str(), &m_module);
    }
}

Logger::~Logger()
{
    if (m_trace)
    {
        m_trace->Release();
    }
    
    if (m_client)
    {
        m_client->Release();
    }
}

void Logger::Trace(const std::string& message)
{
    m_trace->P7_INFO(m_module, TM("%s"), message.c_str());
}

void Logger::Error(const std::string& message)
{
    m_trace->P7_ERROR(m_module, TM("%s"), message.c_str());
}

}
