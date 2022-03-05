#include "config_manager.h"

#include <cstdlib>

ConfigManager::ConfigManager() {

    this->home_dir = get_home_dir();
}


std::string ConfigManager::get_home_dir() {
    std::string home_dir;
#ifdef _WIN32
    char* home_path = std::get_env("HOMEPATH");
    home_dir = std::string(home_paht);
#elif defined(__linux__) || defined(__APPLE__)
    char* home = std::getenv("HOME");
    home_dir = std::string(home);
#endif
    return home_dir;
}
