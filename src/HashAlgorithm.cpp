#include "pch.h"
#include "HashAlgorithm.h"


std::string Algorithm::hash(std::string &password)
{
	return password;
}

/// Funkcja zwracająca nowy obiekt algorytmu hashującego
Algorithm * Algorithm::clone()
{
	return new Algorithm();
}

std::string Algorithm::getName() const
{
	return algorithmName;
}


int Algorithm::getLength() const
{
	return hashLength;
}

/// Operator przeciążony służacy do printowania nazwy algorytmu
std::ostream & operator<<(std::ostream & output, const Algorithm &algorithm)
{
	output << algorithm.algorithmName;
	return output; 
}