#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
namespace fs = std::filesystem;

class ConfigManager {
  public:
    ConfigManager();
    ~ConfigManager();

    bool loadConfig();
  private:


};



#endif //CONFIG_H
