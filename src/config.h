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
  fs::path config_path_{};
};


#endif // CONFIG_H
