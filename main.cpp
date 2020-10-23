#include <iostream>
#include <string>
#include <time.h>
#include "hashtable.h"

using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::to_string;

// wypisanie danych struktury
ostream& operator<<(ostream& out, const entry<int>& n) {
	out << "| " << n.key << ": " << n.value << " ";
	return out;
}

string generateKey(int length) {
	string key = "";
	char c;

	for (int i = 0; i < length; i++) {
		c = rand() % ('z' - 'a' + 1) + 'a'; // a - z
		key += c;
	}
	return key;
}

void test() {
	HashTable<int>* htable = new HashTable<int>();

	cout << htable->toString() << endl << endl;

	htable->add("adada", 12);
	htable->add("qwer", 16);
	cout << htable->toString() << endl << endl;

	htable->add("wfwaf", 22);
	htable->add("pdkpd", 243);
	htable->add("gdhrh", 876);
	cout << htable->toString() << endl << endl;

	htable->add("fdfer", 34);
	htable->add("zdwff", 999);
	cout << htable->toString() << endl << endl;

	htable->add("wolnls", 123);
	cout << htable->toString() << endl << endl;

	htable->search("wfwaf");
	cout << endl << endl;

	htable->search("fffff");
	cout << endl << endl;

	htable->remove("fdfer");
	cout << htable->toString() << endl << endl;

	htable->remove("wfwaf");
	cout << htable->toString() << endl << endl;

	htable->remove("aaa");
	cout << htable->toString() << endl << endl;

	htable->add("wolnls", 999999);
	cout << htable->toString(3) << endl << endl;

	//htable->clear();
	//cout << htable->toString() << endl << endl;

	htable->stats();

	delete htable;
}

int main() {
	//test();
	
	const int testStages[] = {
		10,
		100,
		1000,
		10000,
		100000,
		1000000,
		10000000
	};
	double elapsedTime = 0;
	int numOfEl = 10; // ograniczenie liczby wypisywanych elementow w petlach

	HashTable<int>* htable = new HashTable<int>();

	for (int i = 0; i < sizeof(testStages) / sizeof(testStages[0]); i++) {

		// dodawanie
		clock_t start = clock();
		for (int j = 0; j < testStages[i]; j++) {
			htable->add(generateKey(6),j);
		}
		clock_t stop = clock();

		cout << endl << htable->toString(numOfEl) << endl;

		elapsedTime = double(stop - start) / (CLOCKS_PER_SEC);
		printf("Dodanie %d elementow do tablicy zajelo %.6f s (%.3f ms)\n", testStages[i], elapsedTime, elapsedTime * 1000);
		printf("Dodanie pojedynczego elementu zajelo %.8f s (%.5f ms)\n\n", elapsedTime / testStages[i], (elapsedTime / testStages[i]) * 1000);

		// wyszukiwanie
		const int numOfSearches = 10000;
		int numOfHits = 0;

		start = clock();
		for (int i = 0; i < numOfSearches; i++) {
			entry<int>* e = htable->search(generateKey(6));

			if (e != nullptr)
				numOfHits++;
		}
		stop = clock();

		elapsedTime = double(stop - start) / (CLOCKS_PER_SEC);
		printf("\nWyszukanie w tablicy elementow o losowym kluczu (%d prob) zajelo %.4f s (%.2f ms)\n", numOfSearches, elapsedTime, elapsedTime * 1000);
		printf("Wyszukanie pojedynczego elementu zajelo %.9f s (%.6f ms)\n", elapsedTime / numOfSearches, (elapsedTime / numOfSearches) * 1000);
		printf("Liczba trafien: %d\n\n", numOfHits);

		// statystyki
		cout << htable->stats() << endl;
	}

	delete htable;

	cout << "Nacisnij dowolny klawisz, aby zakonczyc..." << endl;
	getchar();
	return 0;
}