#pragma warning(disable:4996)
#include "AuthRequest.h"
#include "AuthConstants.h"
#include "json.hpp"
#include <iostream>
#include <cpr/cpr.h>
#include <cstdio>
#include <curl/curl.h>

using json = nlohmann::json;

AuthRequest::AuthRequest(uint8_t type) : type(type) {
}

size_t write_data(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::ostringstream* stream = (std::ostringstream*)userdata;
    size_t count = size * nmemb;
    stream->write(ptr, count);
    return count;
}

void AuthRequest::post() {

    if (type == static_cast<uint8_t>(RequestType::AUTHENTICATE)) {
        json j;
        j["agent"]["name"] = "Minecraft";
        j["agent"]["version"] = 1;
        j["username"] = userCredentials;
        j["password"] = password;
        j["requestUser"] = true;
        std::ostringstream stream;
        std::string jsonString = j.dump();
        CURL* curl = curl_easy_init();
        struct curl_slist* slist = NULL;
        slist = curl_slist_append(slist, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE); 
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);
        curl_easy_setopt(curl, CURLOPT_URL, "https://authserver.mojang.com/authenticate");
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, TRUE);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonString.length());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);
        CURLcode result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        json k = json::parse(stream.str());
        std::cout << stream.str() << std::endl;
        std::cout << k["user"]["id"] << std::endl;
    }
}