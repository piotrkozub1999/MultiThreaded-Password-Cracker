#include "pch.h"
#include "SHA1Algorithm.h"


/// Konstruktor:
/// - przypisuje nazwę algorytmu
/// - przypisuje długość hasha
SHA1Algorithm::SHA1Algorithm()
{
	this->algorithmName = "SHA1";
	this->hashLength = 40;
}

/// Destruktor
SHA1Algorithm::~SHA1Algorithm()
{
	delete output;
}

/// Funkcja zwracjąca zahashowane słowo
std::string SHA1Algorithm::hash(std::string &password) {
	mtx.lock();
	SHA1_Init(&context);
	SHA1_Update(&context, password.c_str(), password.length());
	SHA1_Final(digest, &context);

	for (int n = 0; n < 20; ++n) {
		snprintf(&(output[n * 2]), 20 * 2, "%02x", (unsigned int)digest[n]); // express the digest in hex 
	}
	mtx.unlock();
	return output;
}

/// Klonowanie algorytmu SHA1
Algorithm * SHA1Algorithm::clone()
{
	return new SHA1Algorithm();
}
