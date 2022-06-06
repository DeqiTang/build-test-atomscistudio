/************************************************************************
 *
 * Atom Science Studio
 * Copyright (C) 2022  Deqi Tang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "config_manager.h"

#include <cstdlib>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

ConfigManager::ConfigManager() {
    this->home_dir = get_home_dir();
    this->config_dir = get_config_dir();

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

    init_json();
}

std::string ConfigManager::get_home_dir() {

    std::string home_dir;

#if defined(__linux__) || defined(__APPLE__)
    char* home = std::getenv("HOME");
    home_dir = std::string(home);
#elif defined(_WIN32)
    char* home_drive = std::getenv("HOMEDRIVE");
    char* home_path = std::getenv("HOMEPATH");
    home_dir = (fs::path(std::string(home_drive)) / std::string(home_path)).string();
#endif
    return home_dir;
}

std::string ConfigManager::get_config_dir() {

    std::string config_dir;

#if defined(__linux__) || defined(__APPLE__)
    config_dir = (fs::path(this->home_dir) / ".atomscistudio").string();
#elif defined(_WIN32)
    config_dir = (fs::path(this->home_dir) / "atomscistudio").string();
#endif

    return config_dir;
}

void ConfigManager::init_json() {

    if (false == fs::exists(fs::path(this->get_config_dir()) / "config.json")) {
        config_ptree.add("version", "0.0.0");
        pt::write_json((fs::path(this->get_config_dir()) / "config.json").string(), this->config_ptree);
    } else {
        pt::read_json((fs::path(this->get_config_dir()) / "config.json").string(), this->config_ptree);
    }

}
