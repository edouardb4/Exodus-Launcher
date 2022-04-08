#include "DownloadContent.h"
#include "md5.h"
#include "zstr.h"
#include "Utils.h"
#include <fstream>
#include <stdio.h>
#include <filesystem>
#include <cpr/cpr.h>
#include <cstdio>
#include <atlstr.h>
#include <curl/curl.h>
#include <vector>
#include <zlib.h>
#include<direct.h>
#include <filesystem>
#include <memory>

void DownloadContent::downloadFile(const char* url, const char* fname) {
    CURL* curl;
    FILE* fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(fname, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}

std::string DownloadContent::getContent(const char* url) {
    std::ostringstream stream;
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_string_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);
    CURLcode result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return stream.str();
}

bool DownloadContent::downloadContent() {
    std::string ret = getContent("http://exoduspvp.org/updates.txt");
    std::vector<std::string> vec = split(ret, ":");
    MD5 md5;
    std::string path = std::filesystem::current_path().generic_string();
    if (std::ifstream(path + "/old-Launcher.exe").good()) {
        std::filesystem::remove((path + "/old-Launcher.exe").c_str());
    }
    if (md5.digestFile((path + "/Launcher.exe").c_str()) != vec[0]) {
        std::cout << "Downloading Launcher Update... " << std::endl;
        std::rename((path + "/Launcher.exe").c_str(), (path + "/old-Launcher.exe").c_str());
        downloadFile("http://exoduspvp.org/Launcher.exe", (path + "/Launcher.exe").c_str());
    }
    
    if (std::ifstream(path + "/res/").good()) {
        std::cout << "Downloading Content... " << std::endl;
  //      std::filesystem::remove_all((path + "/res/").c_str());
    }
    if (!std::ifstream(path + "/unzip.jar").good()) {
        downloadFile("http://exoduspvp.org/unzip.jar", (path + "/unzip.jar").c_str());
    }
  //  downloadFile("http://exoduspvp.org/res.zip", (path + "/res.zip").c_str());
  //  std::system("java -jar unzip.jar res.zip res");
  //  std::filesystem::remove(path + "/res.zip");
    if (!std::ifstream(path + "/1.8")) {
        _mkdir((path + "/1.8").c_str());
    }
    if (!doesPathExist(path + "/1.8/bin-1.8")) {
        std::cout << "Downloading Bin... " << std::endl;
        downloadFile("http://exoduspvp.org/bin-1.8.zip", (path + "/1.8/bin-1.8.zip").c_str());
        std::cout << "Extracting Bin" << std::endl;
        std::system("java -jar unzip.jar 1.8/bin-1.8.zip 1.8");
        std::remove((path + "/1.8/bin-1.8.zip").c_str());
    }
    if (!doesPathExist(path + "/1.8/library-1.8")) {
        std::cout << "Downloading Libraries... " << std::endl;
        downloadFile("http://exoduspvp.org/library-1.8.zip", (path + "/1.8/library-1.8.zip").c_str());
        std::cout << "Extracting Libraries" << std::endl;
        std::system("java -jar unzip.jar 1.8/library-1.8.zip 1.8");
        std::remove((path + "/1.8/library-1.8.zip").c_str());
    }
    if (!doesPathExist(path + "/assets_18")) {
        std::cout << "Downloading Assets... " << std::endl;
        downloadFile("http://exoduspvp.org/assets_18.zip", (path + "/assets_18.zip").c_str());
        std::cout << "Extracting Assets" << std::endl;
        std::system("java -jar unzip.jar assets_18.zip assets_18");
        std::remove((path + "/assets_18.zip").c_str());
    }
    if (!doesPathExist(path + "/jre_64")) {
        std::cout << "Downloading JRE... " << std::endl;
        downloadFile("http://exoduspvp.org/jre_64.zip", (path + "/jre_64.zip").c_str());
        std::cout << "Extracting JRE" << std::endl;
        std::system("java -jar unzip.jar jre_64.zip jre_64");
        std::remove((path + "/jre_64.zip").c_str());
    }
    std::filesystem::remove(path + "/unzip.jar");
    return true;
}