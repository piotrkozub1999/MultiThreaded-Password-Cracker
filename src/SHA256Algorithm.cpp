#include "pch.h"
#include "SHA256Algorithm.h"



/// Konstruktor:
/// - przypisuje nazwę algorytmu
/// - przypisuje długość hasha
SHA256Algorithm::SHA256Algorithm()
{
	this->algorithmName = "SHA256";
	this->hashLength = 64;
}

/// Destruktor
SHA256Algorithm::~SHA256Algorithm()
{
	delete output;
}

/// Funkcja zwracjąca zahashowane słowo
std::string SHA256Algorithm::hash(std::string &password)
{
	mtx.lock();
	SHA256_Init(&context);
	SHA256_Update(&context, password.c_str(), password.length());
	SHA256_Final(digest, &context);

	for (int n = 0; n < 32; ++n) {
		snprintf(&(output[n * 2]), 32 * 2, "%02x", (unsigned int)digest[n]); // express the digest in hex 
	}
	mtx.unlock(); 

	return output;
}

/// Klonowanie algorytmu MD5
Algorithm * SHA256Algorithm::clone()
{
	return new SHA256Algorithm();
}

