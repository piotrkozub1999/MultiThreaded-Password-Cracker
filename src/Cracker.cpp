#include "pch.h"
#include "Cracker.h"


/// Konstruktor:
/// - przypisanie wybranego algorytmu
/// - przypisanie wybranej ilości rdzeni
Cracker::Cracker(int threadCount, Algorithm *algorithm) : threadCount(threadCount) {
	this->algorithm = algorithm;
	this->threadPool = std::vector<std::thread>(threadCount);
	return;
}


/// Destruktor:
/// - usunięcie obiektu zainicjowanego algorytmu
Cracker::~Cracker()
{
	delete algorithm;
	return;
}


/// Funkcja "start" rozpoczyna proces hashowania
/// Przydzielenie zadań do poszczególnych wątków
/// Zwrócenia znalezionego słowa - "cracked"
std::string Cracker::start(std::string hash){
	complete = false; // Ustawienie zmiennej atomowej zatrzymującej wszystkie wątki na False przy każdym nowym hashu
	std::unique_lock<std::mutex> lck(dictionaryMutex); 
	while (dictionary.size() <= 0) { // Czekanie na załadowanie słownika
		std::cout << "Najpierw zaladuj slownik!\n";
		cv.wait(lck); 
	}
	
	std::cout << std::left << std::setw(20) << "Watki: " << std::setw(4) << threadCount << std::endl;
	std::cout << std::left << std::setw(20) << "Algorytm: " << std::setw(4) << *algorithm << std::endl;
	std::cout << std::left << std::setw(20) << "Hash: " << std::setw(4) << hash << std::endl;

	std::transform(hash.begin(), hash.end(), hash.begin(), ::tolower); // zmiana wszystkich liter na małe

	for (unsigned int threadID = 0, startIndex = 0; threadID < threadCount; threadID++) // dla każdego wątku
	{
		addWorker([=] {
			compareHashes(dictionary, hash, threadID, threadCount); // przydzielenie zadania
		});
	}

	for (auto&& thread : threadPool) // przejście przez wszystkie wątki w puli
		if (thread.joinable()) // funkcja joinable zwraca prawde jeśli wątek jest aktywny
			thread.join(); // konczy prace wątku w momencie zakończenia aktywnego zadania
	
	return cracked;
}


/// Funkcja "compareHashes" znajdująca zahashowane słowo
/// Wszystkie słowa w słowniku są hashowane
/// Każdy wątek zaczyna od hashowania innego słowa i jest iterowany przez liczbę wątków
/// Jeśli któryś z wątków uzyska identyczny hash jak ten podany na początku, program kończy pracę i zwraca słowo
void Cracker::compareHashes(std::vector<std::string>dictionarySlice, std::string hash, unsigned int threadID, int threadCount)
{
	Algorithm* algorithmClone = algorithm->clone(); // stworzenie osobnego obiektu algorytmu dla każdego wątku w celu bezpieczeństwa
	std::string password; // zmienna przechowująca aktualnie sprawdzane słowo

	// pętla przechodząca przez słownik aż do momentu znalezienia hasła
	// każdy wątek zaczyna iterację od indeksu równemu jego id, po czym przechodzi do słowa oddalonego o liczbę równą liczbie wykorzystywanych wątków
	// jeśli hasło się nie znajdzie (błąd użytkownka), program zakończy działanie po przejściu przez cały słownk
	for (int index = threadID; index < dictionarySlice.size() && !complete; index+=threadCount) {

		
		if (index < dictionarySlice.size()) { 
			password = dictionarySlice[index];

			/// Tylko do podglądania
			// std::cout << "Thread" << threadID << ": checking " << password << std::endl;
		}

		if (algorithmClone->hash(password) == hash) { // porównanie spradzanego hasha z hashem szukanym
			cracked = password; // przypisanie znalezionego słowa do zwracanej zmiennej
			complete = true; // ustawienie zmiennej atomowej na True przerywa prace wszystkich wątków
			std::cout << std::left << std::setw(20) << "Znaleziono na watku nr: " << std::setw(4) << threadID+1 << std::endl;
			break; // wyjście z pętli
		}
	}

	delete algorithmClone; // usunięcie sklonowanego algorytmu
	return;
}

/// Funkcja ładująca słownik txt do vectora
/// W trakcie jej działania mutex jest blokowany
bool Cracker::loadDictionary(const std::string dictionaryPath)
{
	std::ifstream input(dictionaryPath, std::ios::in); // otwarcie słownika
	std::ifstream::pos_type fileSize;
	if (!input.is_open()) {
		std::cout << "Nie udalo sie otworzyc slownika: " << dictionaryPath << std::endl;
		input = std::ifstream();
		return false; 
	}
	else {
		fileSize = std::ifstream(dictionaryPath, std::ifstream::ate | std::ifstream::binary).tellg(); // set file size variable
		std::cout << std::left << std::setw(20) << "Slownik: " << std::setw(4) << dictionaryPath << " (" << fileSize / 1048576 << "MB)" << std::endl;
	}

	std::string line;
	while (getline(input, line)) { // załadowanie wszystkich słów jedno po drugim
		if (line != "") {
			dictionary.emplace_back(line);
		}
	}

	input.close(); // zamknięcie słownika
	std::cout << std::endl << std::endl;
	
	std::unique_lock<std::mutex> lck(dictionaryMutex); // odblokowanie mutexu pozwalające na rozpoczęcie pracy przez wątki
	cv.notify_all();

	return dictionary.size() > 0;
}


/// Funkcja addWorker przypisuje zadania poszczególnym wątkom
template<typename Worker> void Cracker::addWorker(Worker&& function)
{
	// Szukanie aktywnych wątków
	for (auto&& thread : threadPool)
	{
		if (thread.joinable()) // jeśli wątek jest aktywny
			continue; // przejdź do następnego wątku

		// Jeśli program nie przeszedł do następnego wątku
		thread = std::thread(function); // rozpoczyna wykonywanie funkcji na znalezionym wątku
		return;
	}
	
	// Szukanie nieaktywnych wątków
	for (auto&& thread : threadPool)
	{
		if (!thread.joinable()) // Jeśli wątek nie jest aktywny
			continue; // przejdź do następnego wątku

		// Jeśli program nie przeszedł do następnego wątku
		thread.join(); // aktywuje ten wątek
		thread = std::thread(function); // rozpoczyna wykonywanie funkcji na znalezionym wątku
		return;
	}
	return;
}
