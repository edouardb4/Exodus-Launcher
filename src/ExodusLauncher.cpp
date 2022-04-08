#include "ExodusLauncher.h"
#include "md5.h"
#include "zstr.h"
#include "Utils.h"
#include <filesystem>

bool ExodusLauncher::downloadContent() {
    std::string ret = getContent("http://exoduspvp.org/updates.txt");
    std::vector<std::string> vec = split(ret, ":");
    MD5 md5;
    std::string path = std::filesystem::current_path().generic_string();
    bool exists = std::ifstream(path + "/1.8/ExodusClient.jar").good();
    if (exists && vec[1] != md5.digestFile((path + "/1.8/ExodusClient.jar").c_str())) {
        std::filesystem::remove(path + "/1.8/ExodusClient.jar");
    }
    exists = std::ifstream(path + "/1.8/ExodusClient.jar").good();
    if (!exists) {
        std::cout << "Downloading Client... " << std::endl;
        downloadFile("http://exoduspvp.org/ExodusClient.jar", (path + "/1.8/ExodusClient.jar").c_str());
        std::cout << "Finished Downloading Client..." << std::endl;
    }
    return true;
}