#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>

class ConfigManager {
public:
    ConfigManager();

    std::string get_home_dir();
};

#endif // CONFIGMANAGER_H
