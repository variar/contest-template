#pragma once

namespace plog
{
    enum Severity
    {
        none = 0,
        fatal = 1,
        error = 2,
        warning = 3,
        info = 4,
        debug = 5,
        verbose = 6
    };

    inline const char* severityToString(Severity severity)
    {
        switch (severity)
        {
        case fatal:
            return "FAT";
        case error:
            return "ERR";
        case warning:
            return "WRN";
        case info:
            return "INF";
        case debug:
            return "DBG";
        case verbose:
            return "ALL";
        default:
            return "NON";
        }
    }

    inline Severity severityFromString(const char* str)
    {
        for (Severity severity = fatal; severity <= verbose; severity = static_cast<Severity>(severity + 1))
        {
            if (severityToString(severity)[0] == str[0])
            {
                return severity;
            }
        }

        return none;
    }
}
