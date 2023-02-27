#pragma once

class SHA1Algorithm : public Algorithm
{
public:
	SHA1Algorithm();
	~SHA1Algorithm();
	virtual std::string hash(std::string &password);
	virtual Algorithm *clone();
private:
	std::mutex mtx;
	SHA_CTX context;
	unsigned char digest[20];
	char *output = (char*) malloc(41);
};
