#pragma once
#include "DownloadContent.h"

class ExodusLauncher : public DownloadContent {
public:
	bool downloadContent() override;
};