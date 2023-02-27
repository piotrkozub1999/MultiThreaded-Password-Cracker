#include "pch.h"
#include "MD5Algorithm.h"


/// Konstruktor:
/// - przypisuje nazwę algorytmu
/// - przypisuje długość hasha
MD5Algorithm::MD5Algorithm()
{
	this->algorithmName = "MD5";
	this->hashLength = 32;
}

/// Destruktor
MD5Algorithm::~MD5Algorithm()
{
	delete output;
}

/// Funkcja zwracjąca zahashowane słowo
std::string MD5Algorithm::hash(std::string &password)
{
	mtx.lock(); // lock mutex
	MD5_Init(&context); 
	MD5_Update(&context, password.c_str(), password.length());
	MD5_Final(digest, &context);

	for (int n = 0; n < 16; ++n) {
		snprintf(&(output[n * 2]), 16 * 2, "%02x", (unsigned int)digest[n]);
	}

	mtx.unlock();
	return output;
}

/// Klonowanie algorytmu MD5
Algorithm * MD5Algorithm::clone()
{
	return new MD5Algorithm();
}
