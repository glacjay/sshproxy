#ifndef HEADER__CONSTANTS_HPP
#define HEADER__CONSTANTS_HPP

enum SshStatus
{
    StatStopped,
    StatConnecting,
    StatConnected,
    StatSleeping,
};

enum LogLevel
{
    LogDebug,
    LogInfo,
    LogWarn,
    LogError,
};

const int MaxTotalLog = 700;

#endif
