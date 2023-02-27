#pragma once

class MD5Algorithm : public Algorithm
{
public:
	MD5Algorithm();
	~MD5Algorithm();

	virtual std::string hash(std::string &password);
	virtual Algorithm* clone();
private:
	std::mutex mtx;
	MD5_CTX context;
	unsigned char digest[16];
	char *output = (char*)malloc(33);
};