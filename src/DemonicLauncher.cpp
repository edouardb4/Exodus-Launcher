#include "DemonicLauncher.h"
#include "md5.h"
#include "zstr.h"
#include "Utils.h"
#include <filesystem>

bool DemonicLauncher::downloadContent() {
    std::string ret = getContent("http://demonicpvp.org:8080/updates");
    std::vector<std::string> vec = split(ret, ":");
    MD5 md5;
    std::string path = std::filesystem::current_path().generic_string();
    bool exists = std::ifstream(path + "/1.8/DemonicClient.jar").good();
    if (exists && vec[0] != md5.digestFile((path + "/1.8/DemonicClient.jar").c_str())) {
        std::filesystem::remove(path + "/1.8/DemonicClient.jar");
    }
    exists = std::ifstream(path + "/1.8/DemonicClient.jar").good();
    if (!exists) {
        std::cout << "Downloading Client... " << std::endl;
        downloadFile("http://demonicpvp.org:8080/CosmicClient-1.8.9-Old.jar", (path + "/1.8/DemonicClient.jar").c_str());
    }
    return true;
}