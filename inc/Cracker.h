#include <thread>
#pragma once
class Cracker
{
public:

	Cracker(const int maximumThreads, Algorithm *algorithm);
	~Cracker();
	std::string start(std::string hash);
	bool loadDictionary(const std::string filePath);

private:
	
	void compareHashes(std::vector<std::string>dictionarySlice, std::string hash, unsigned int threadID, int threadCount);
	
	template<typename Worker> void addWorker(Worker &&job);

	/// Liczba wątków
	const int threadCount;

	/// Vector przechowujący wszystkie wątki
	std::vector<std::thread> threadPool;

	/// Zmienna atomowa typu bool przerywająca prace programu
	std::atomic<bool> complete = { false };


	/// Mutex i zmienna warunkowa do upewnienia się przed rozpoczęciem pracy na niezaładowanym słowiku
	std::mutex dictionaryMutex;
	std::condition_variable cv;

	/// Słownik
	std::vector<std::string> dictionary;
	
	/// Wskaźnik na algorytm hashujący słowa ze słownika
	Algorithm *algorithm;
	

	/// Zmienna przechowująca znalezione słowo
	std::string cracked;
	
};

