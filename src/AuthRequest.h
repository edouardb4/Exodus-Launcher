#pragma once

#include <cstdint>
#include <string>

enum class RequestType : uint8_t {
	AUTHENTICATE
};

class AuthRequest {
public:
	AuthRequest(uint8_t type);
	void post();
	void setUserCredentials(std::string userCredentials) {
		this->userCredentials = userCredentials;
	}
	void setPassword(std::string password) {
		this->password = password;
	}
protected:
	uint8_t type;
	std::string userCredentials, password;
};