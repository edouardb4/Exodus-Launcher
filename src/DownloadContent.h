#pragma once
#pragma warning(disable:4996)
#include <string>

class DownloadContent {
public:
	void downloadFile(const char* url, const char* fname);
	std::string getContent(const char* url);
	virtual bool downloadContent();
};