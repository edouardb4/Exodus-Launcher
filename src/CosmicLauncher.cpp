#include "CosmicLauncher.h"
#include "md5.h"
#include "zstr.h"
#include "Utils.h"
#include <filesystem>

bool CosmicLauncher::downloadContent() {
    std::string ret = getContent("http://cdn.cosmicclient.com/checkforupdates");
    std::vector<std::string> vec = split(ret, ":");
    MD5 md5;
    std::string path = std::filesystem::current_path().generic_string();
    bool exists = std::ifstream(path + "/1.8/CosmicClient.jar").good();
    if (exists && vec[0] != md5.digestFile((path + "/1.8/CosmicClient.jar").c_str())) {
        std::filesystem::remove(path + "/1.8/CosmicClient.jar");
    }
    exists = std::ifstream(path + "/1.8/CosmicClient.jar").good();
    if (!exists) {
        std::cout << "Downloading Client... " << std::endl;
        downloadFile("http://cdn.cosmicclient.com/download/CosmicClient-1.0-SNAPSHOT.jar", (path + "/1.8/CosmicClient.jar").c_str());
    }
    return true;
}