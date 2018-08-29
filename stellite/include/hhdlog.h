#pragma once

#include <string>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <cstdarg>
#include <unistd.h>

#if defined(ANDROID) || defined(__ANDROID__)
#include <android/log.h>
#endif

#define HHDLOG(szFormat, ...) \
  HhdLog::PrintLog(__FILE__, __LINE__, __FUNCTION__, szFormat, ##__VA_ARGS__);

class HhdLog
{
public:
  static void PrintLog(std::string strFilePath, int nLineNum, std::string strFuncName, std::string strFormat, ...)
  {
    static std::mutex mtx;
    std::lock_guard<std::mutex> lg(mtx);

    auto curTime = std::time(nullptr);
    char szCurTime[1024] = {
        0,
    };
    std::strftime(szCurTime, sizeof(szCurTime), "%m-%d %H:%M:%S", std::localtime(&curTime));

    char szLog[1024] = {
        0,
    };
    va_list args;
    va_start(args, strFormat);
    vsprintf(szLog, strFormat.c_str(), args);
    va_end(args);
    char szPrettyLog[1024] = {
        0,
    };
    std::string strFileName;

    if (not strFilePath.empty())
    {
      auto iStart = strFilePath.find_last_of('/');

      if (iStart == std::string::npos)
      {
        iStart = strFilePath.find_last_of('\\');
      }

      strFileName = strFilePath.substr(iStart + 1, strFilePath.size() - iStart - 1);
    }

#if defined(ANDROID) || defined(__ANDROID__)
    std::snprintf(
        szPrettyLog,
        1024,
        "%s:%d:%s %s",
        strFileName.c_str(),
        nLineNum,
        strFuncName.c_str(),
        szLog);

    __android_log_write(ANDROID_LOG_DEBUG, "HHDLOG", szPrettyLog);
#else
    auto now = std::chrono::system_clock::now();
    auto ms = (int)(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000).count();
    auto pid = getpid();
    auto tid = (int)std::hash<std::thread::id>{}(std::this_thread::get_id());

    std::snprintf(
        szPrettyLog,
        1024,
        "%s.%d %d %d D HHDLOG: %s:%d:%s %s",
        szCurTime,
        ms,
        pid,
        tid,
        strFileName.c_str(),
        nLineNum,
        strFuncName.c_str(),
        szLog);

    std::cout << szPrettyLog << std::endl;
#endif
  }

private:
};