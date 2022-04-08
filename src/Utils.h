#pragma once
#include <string>
#include <iostream>  
#include <sstream>
#include <vector>

namespace {
    bool doesPathExist(const std::string& s)
    {
        struct stat buffer;
        return (stat(s.c_str(), &buffer) == 0);
    }

    std::vector<std::string> split(std::string toSplit, std::string delimiter) {
        std::vector<std::string> vec;
        size_t pos = 0;
        while ((pos = toSplit.find(delimiter)) != std::string::npos) {
            vec.push_back(toSplit.substr(0, pos));
            toSplit = toSplit.substr(pos + 1);
        }
        return vec;
    }

    size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
        size_t written;
        written = fwrite(ptr, size, nmemb, stream);
        return written;
    }

    size_t write_string_data(char* ptr, size_t size, size_t nmemb, void* userdata) {
        std::ostringstream* stream = (std::ostringstream*)userdata;
        size_t count = size * nmemb;
        stream->write(ptr, count);
        return count;
    }
}