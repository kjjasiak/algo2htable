#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <cmath>
#include <string>
#include <sstream>
#include "dynarr.h"
#include "llist.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::stringstream;

template <typename T>
struct entry {
	string key;
	T value;

	entry(string key, const T& value) : key(key), value(value) { }
};

template <typename T>
int cmpKeys(const entry<T>& e1, const entry<T>& e2) {
	return !(e1.key == e2.key);
}

template <typename T>
class HashTable {
	DynArr<LinkedList<entry<T>>>* arr;
	double maxLoad;
	int numOfElem;
	int arrInitLength;

public:
	HashTable() {
		arrInitLength = 5;
		arr = new DynArr<LinkedList<entry<T>>>(arrInitLength);
		maxLoad = 0.75;
		numOfElem = 0;
	}

	~HashTable () {
		delete arr;
	}

	void add(string, const T&);
	entry<T>* search(string);
	bool remove(string);
	void clear();
	string toString(int numOfEl = -1);
	string stats();
	int hash(string);
private:
	void rehash();
};

template <typename T>
void HashTable<T>::add(string key, const T& value) {
	int arrPlace = this->hash(key);

	LinkedList<entry<T>>* list = arr->getByIndex(arrPlace);

	Element<entry<T>>* elem = list->search(entry<T>(key, 0), cmpKeys);

	if (elem == nullptr) {
		// dodajemy
		list->addToTail(entry<T>(key, value));
		numOfElem++;

		double arrLoad = numOfElem / (double)arr->getLength();
		//cout << "Dodawanie, arrLoad: " << arrLoad << endl;

		if (arrLoad >= maxLoad)
			rehash();
	}
	else {
		// aktualizujemy dane
		//cout << "Klucz \"" << key << "\" istnieje, podmiana danych..." << endl;
		elem->setData(entry<T>(key, value));
	}
}

template <typename T>
entry<T>* HashTable<T>::search(string key) {
	int arrPlace = hash(key);
	LinkedList<entry<T>>* list = arr->getByIndex(arrPlace);

	Element<entry<T>>* elem = list->search(entry<T>(key, 0), cmpKeys);

	//cout << "Szukany klucz: \"" << key << "\"" << endl;

	if (elem == nullptr) {
		//cout << "Nie znaleziono klucza" << endl;
		return nullptr;
	}

	//cout << "Znaleziono klucz" << endl;
	//cout << "\t[" << arrPlace << "] " << elem->getData() << endl;
	return &elem->getData();
}

template <typename T>
bool HashTable<T>::remove(string key) {
	int arrPlace = hash(key);
	LinkedList<entry<T>>* list = arr->getByIndex(arrPlace);

	//cout << "Usuwany klucz: \"" << key << "\"" << endl;

	bool result = list->searchDelete(entry<T>(key, 0),cmpKeys);

	if (result) {
		//cout << "Usunieto element" << endl;
		numOfElem--;
	}
	else {
		//cout << "Brak elementu o podanym kluczu" << endl;
	}

	return result; // TODO: czy rehash konieczny?
}

template <typename T>
void HashTable<T>::clear() {
	for (int i = 0; i < arr->getLength(); i++) {
		LinkedList<entry<T>>* list = arr->getByIndex(i);
		list->clearList();
	}
	numOfElem = 0;
}

template <typename T>
string HashTable<T>::toString(int numOfEl) {
	int k;

	if ((numOfEl < 0) || (numOfEl > arr->getLength()))
		k = arr->getLength();
	else
		k = numOfEl;

	stringstream s;

	s << "Aktualny rozmiar: " << numOfElem << endl;
	s << "Maksymalny rozmiar: " << arr->getLength() << endl;
	s << "Stopien zapelnienia: " << numOfElem / (double)arr->getLength() << endl << endl;
	s << "Tablica (" << arr->getLength() << " list";
	
	if (k != arr->getLength())
		s << ", skrot " << k << " elementow";
	
	s << "):" << endl;

	if (numOfElem > 0) {
		for (int i = 0; i < arr->getLength(); i++) {
			LinkedList<entry<T>>* list = arr->getByIndex(i);

			if (list->getSize() > 0) {
				s << "\t[" << i << "] ";
				s << list->listToStr() << endl;
				k--;
			}

			if (k == 0)
				break;
		}
	}
	else {
		s << "\tBrak elementow" << endl;
	}
	//s << endl;
	//s << this->stats();
	
	return s.str();
}

template <typename T>
string HashTable<T>::stats() {
	stringstream s;

	int listCount = 0, nonEmptyLists = 0, sumListSize = 0;
	int minListSize = arr->getByIndex(0)->getSize();
	int maxListSize = arr->getByIndex(0)->getSize();

	for (int i = 0; i < arr->getLength(); i++) {
		LinkedList<entry<T>>* list = arr->getByIndex(i);

		if (list->getSize() > 0)
			nonEmptyLists++;
		
		if (list->getSize() < minListSize)
			minListSize = list->getSize();

		if (list->getSize() > maxListSize)
			maxListSize = list->getSize();

		sumListSize += list->getSize();
		listCount++;
	}

	s << "Statystyki:" << endl;
	s << "\tliczba list: " << listCount << endl;
	s << "\tmin rozmiar listy: " << minListSize << endl;
	s << "\tmax rozmiar listy: " << maxListSize << endl;
	s << "\tliczba niepustych list: " << nonEmptyLists << endl;
	s << "\tsredni rozmiar listy: " << sumListSize / (double)arr->getLength() << endl;

	return s.str();
}

template <typename T>
int HashTable<T>::hash(string key) {
	string s = key;
	int q = key.length();
	unsigned int n = arr->getLength();

	int h = 0;
	int j = 0;

	for (int i = (q - 1); i >= 0; i--) {
		h = h + s[j] * int(pow(31, i));
		//cout << "s[j]: " << s[j] << ", h: " << h << endl;
		j++;
	}
	h = h % n;
	//cout << "n: " << n << ", h % n: " << h << endl;

	return h;
}

template <typename T>
void HashTable<T>::rehash() {
	//cout << "Rehash i rozszerzenie..." << endl;

	DynArr<LinkedList<entry<T>>>* tmp = arr;
	arr = nullptr;
	numOfElem = 0;

	arr = new DynArr<LinkedList<entry<T>>>(tmp->getLength() * 2);

	for (int i = 0; i < tmp->getLength(); i++) {
		// komorka tablicy
		LinkedList<entry<T>>* list = tmp->getByIndex(i);

		// elementy listy
		for (Element<entry<T>>* i = list->getHead(); i != nullptr; i = i->next) {
			entry<T> e = i->getData();
			add(e.key, e.value);
		}
	}

	delete tmp;
}

#endif
