#pragma once
#include <iostream>
#include <string>

namespace g3d {
class CLogger {
  public:
    static void logError(const std::string &str);
    static void logWarning(const std::string &str);
    static void logInfo(const std::string &str);
};
}; 

#define ENGINE_LOG_ERROR(...) CLogger::logError(std::string(__VA_ARGS__))
#define ENGINE_LOG_WARN(...) CLogger::logWarning(std::string(__VA_ARGS__))
#define ENGINE_LOG_INFO(...) CLogger::logInfo(std::string(__VA_ARGS__))
