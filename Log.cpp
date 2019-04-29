#include "Log.h"

namespace g3d {
void CLogger::logError(const std::string &str) {
    std::cerr << "[ERROR] " << str << '\n';
    exit(-1);
}

void CLogger::logWarning(const std::string &str) {
    std::cout << "[WARN] " << str << '\n';
}

void CLogger::logInfo(const std::string &str) {
    std::cout << "[INFO] " << str << '\n';
}
}; 
