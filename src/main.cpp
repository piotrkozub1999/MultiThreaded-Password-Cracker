#include "pch.h"
#include <iostream>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;


/// Funkcja pobierająca ścieżke głownego folderu projektu
/// Pobiera lokalizacja kompilacji i zwraca całą ścięzkę jeden folder wyżej
std::string getPath(std::string filename){
    std::filesystem::path tmp;
    for(fs::path p1 : {fs::absolute(filename) })
        tmp = p1.parent_path();

    std::string path_string{tmp.parent_path().u8string()};
    std::cout << "Sciezka pliku "<< filename << " to: "<< path_string << std::endl;
	std::string filePath = path_string + "/" + filename;

	return filePath;
}

/// Przepisanie listy hashy z pliku txt do vectora
std::vector<std::string> getAllHashes(std::string filePath){
	std::ifstream MyReadFile(filePath);
	std::string line;
	std::vector<std::string> allHashes;

	// Odczytanie hashy z pliku linijka po linijce
    while (getline (MyReadFile, line))
		allHashes.emplace_back(line); 

	return allHashes;
}


int main(int argc, char** argv) {

	// Pobranie listy hashy
	std::string dictionaryPath = getPath("dictionary.txt");
	std::vector<std::string> MD5Hashes = getAllHashes(getPath("MD5.txt"));
	std::vector<std::string> SHA1Hashes = getAllHashes(getPath("SHA1.txt"));
	std::vector<std::string> SHA256Hashes = getAllHashes(getPath("SHA256.txt"));
	std::vector<std::string> activeHashes;
	unsigned int threadCount;
	Algorithm *algorithm = NULL;
    std::string test[10];

	// Wybieranie algorytmu
	while (algorithm == NULL) {
		std::cout << "Wybierz algorytm: " << std::endl;
		std::cout << "MD5 (1), SHA1 (2), SHA256 (3)" << std::endl << "> ";
		std::string algorithmString;
		std::cin >> algorithmString;
		try {
			switch (stoi(algorithmString)) {
			case 1:
				algorithm = new MD5Algorithm();
				activeHashes = MD5Hashes;
				continue;
			case 2:
				algorithm = new SHA1Algorithm();
				activeHashes = SHA1Hashes;
				continue;
			case 3:
				algorithm = new SHA256Algorithm();
				activeHashes = SHA256Hashes;
				continue;
			default:
				break;
			}
		}
		catch (std::invalid_argument e) { // Trzymanie wyjątków
			// kontynuuj pętle
		}

		std::cout << "Wprowadz poprawne ID algorytmu" << std::endl;
		continue;
	}

	// Ustawienie liczby wątków
	std::cout << "Podaj liczbe watkow: " << std::endl << "> ";
	std::cin >> threadCount;


	auto startTime = std::chrono::high_resolution_clock::now(); // Czas początkowy
	Cracker cracker(threadCount, algorithm); // Stworzenie obiektu hashującego

	if (cracker.loadDictionary(dictionaryPath)) { // Załadowanie słownika
		auto loadDictionaryFinish = std::chrono::high_resolution_clock::now(); // Czas po załadowaniu słownika

		for(int i = 0; i<activeHashes.size(); i++){			
			std::string password = cracker.start(activeHashes[i]);
			if (password.size() > 0)
				std::cout << std::endl << std::left << std::setw(20) << "Znalezione slowo: " << std::setw(4) << password << std::endl << std::endl << std::endl;
			else
				std::cout << std::endl << "Nie znaleziono takiego hasla!" << std::endl << std::endl << std::endl;
		}

		auto finishTime = std::chrono::high_resolution_clock::now(); // Czas po załadowaniu słownika i znalezeieniu haseł 

		std::cout << std::left << std::setw(22) << "Wyniki:" << std::endl;
		std::cout << std::left << std::setw(22) << "Ladowanie slownika: " << std::setw(7) << std::chrono::duration_cast<std::chrono::milliseconds>(loadDictionaryFinish - startTime).count() << "ms" << std::endl;
		std::cout << std::left << std::setw(22) << "Hashowanie: " << std::setw(7) << std::chrono::duration_cast<std::chrono::milliseconds>(finishTime - loadDictionaryFinish).count() << "ms" << std::endl;
		std::cout << std::left << std::setw(22) << "Czas calkowity: " << std::setw(7) << std::chrono::duration_cast<std::chrono::milliseconds>(finishTime - startTime).count() << "ms" << std::endl;

		int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finishTime - startTime).count();
		int seconds = milliseconds / 1000;
		milliseconds %= 1000;
		int minutes = seconds / 60;
		seconds %= 60;
		if (seconds<10)
			std::cout << std::left << std::setw(22) << "Czas calkowity: " << minutes << ":0" << std::setw(4) << seconds << "minutes" << std::endl;
		else
			std::cout << std::left << std::setw(22) << "Czas calkowity: " << minutes << ":" << std::setw(5) << seconds << "minutes" << std::endl;

	}
	return 0;
}
