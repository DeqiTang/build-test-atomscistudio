#include "config_manager.h"

#include <cstdlib>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

ConfigManager::ConfigManager() {
    this->home_dir = get_home_dir();

    // if config files do not exist, generate them
#if defined(__linux__) || defined(__APPLE__)
    if (false == fs::exists(fs::path(this->home_dir) / ".atomscistudio")) {
        fs::create_directory(fs::path(this->home_dir) / ".atomscistudio");
    }
#elif defined(_WIN32)
    if (false == fs::exists(fs::path(this->home_dir) / "atomscistudio")) {
        fs::create_directory(fs::path(this->home_dir) / "atomscistudio");
    }
#endif

}


std::string ConfigManager::get_home_dir() {
    std::string home_dir;

#if defined(__linux__) || defined(__APPLE__)
    char* home = std::getenv("HOME");
    home_dir = std::string(home);
#elif defined(_WIN32)
    char* home_path = std::getenv("HOMEPATH");
    home_dir = std::string(home_path);
#endif
    return home_dir;

}
