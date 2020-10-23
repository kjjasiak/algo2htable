#ifndef LLIST_H
#define LLIST_H

using std::cout;
using std::endl;

// implementacja listy i jej interfejsu
template <typename T>
class Element {
	T d;

public:
	Element<T>* prev;
	Element<T>* next;

	Element(const T& d) : d(d)
	{
		this->prev = nullptr;
		this->next = nullptr;
	}

	T& getData() {
		return this->d;
	}

	void setData(const T& d) {
		this->d = d;
	}
};

template <typename T>
class LinkedList {
	Element<T>* head;
	Element<T>* tail;
	int size;

public:
	LinkedList() {
		head = nullptr;
		tail = nullptr;
		size = 0;
	}

	~LinkedList() {
		Element<T>* tmp;

		for (int k = 0; k < size; k++) {
			tmp = head;
			head = tmp->next;
			delete tmp;
		}
	}

	int& getSize() {
		return size;
	}

	Element<T>* getHead() {
		return head;
	}

	Element<T>* getTail() {
		return tail;
	}

	void addToTail(const T&);
	void addToHead(const T&);
	void deleteTail();
	void deleteHead();
	T* getByIndex(int);
	T& operator[](int);
	bool setByIndex(int, T&);
	Element<T>* search(const T&, int(*compare)(const T&, const T&));
	bool searchDelete(const T&, int(*compare)(const T&, const T&));
	void addInOrder(const T&, int(*compare)(const T&, const T&));
	void clearList();
	void showList();
	void showList(int);
	string listToStr(int numOfEl = -1);
};

template <typename T>
void LinkedList<T>::addToTail(const T& data) {
	if (size > 0) {
		Element<T>* el = new Element<T>(data);
		el->next = nullptr;
		el->prev = tail;
		tail->next = el;
		tail = el;
	}
	else {
		Element<T>* el = new Element<T>(data);
		head = el;
		tail = el;
	}

	size++;
	//cout << "Pomyslnie dodano element na koncu listy!" << endl << endl;
}

template <typename T>
void LinkedList<T>::addToHead(const T& data) {
	if (size > 0) {
		Element<T>* el = new Element<T>(data);
		el->next = head;
		el->prev = nullptr;
		head->prev = el;
		head = el;
	}
	else {
		Element<T>* el = new Element<T>(data);
		head = el;
		tail = el;
	}

	size++;
	//cout << "Pomyslnie dodano element na poczatku listy!" << endl << endl;
}

template <typename T>
void LinkedList<T>::deleteTail() {
	if (size > 1) {
		Element<T>* tmp = tail;

		tail = tail->prev;
		tail->next = nullptr;

		delete tmp;
		tmp = nullptr;

		size--;
		//cout << "Pomyslnie usunieto ostatni element!" << endl << endl;
	}
	else if (size == 1) {
		delete tail;
		tail = nullptr;
		head = nullptr;

		size = 0;
		//cout << "Pomyslnie usunieto ostatni element!" << endl << endl;
	}
	else {
		cout << "Brak elementow na liscie!" << endl << endl;
	}
}

template <typename T>
void LinkedList<T>::deleteHead() {
	if (size > 1) {
		Element<T>* tmp = head;

		head = head->next;
		head->prev = nullptr;

		delete tmp;
		tmp = nullptr;

		size--;
		//cout << "Pomyslnie usunieto pierwszy element!" << endl << endl;
	}
	else if (size == 1) {
		delete head;
		head = nullptr;
		tail = nullptr;

		size = 0;
	}
	else {
		cout << "Brak elementow na liscie!" << endl << endl;
	}
}

template <typename T>
T& LinkedList<T>::operator[](int i)
{
	return *getByIndex(i);
}

template <typename T>
T* LinkedList<T>::getByIndex(int i) {
	if (size > 0) {
		Element<T>* tmp = head;

		for (int k = 0; k < i; k++) {
			tmp = tmp->next;

			if (tmp == nullptr) {
				cout << "Indeks elementu #" << i << " jest spoza zakresu!" << endl << endl;
				return nullptr;
			}
		}

		//cout << "Element o indeksie #" << i << ": " << endl << tmp->getData() << endl << endl;
		return &tmp->getData();
	}
	else {
		//cout << "Brak elementow na liscie!" << endl << endl;
		return nullptr;
	}
}

template <typename T>
bool LinkedList<T>::setByIndex(int i, T& newData) {
	if (size > 0) {
		Element<T>* tmp = head;

		for (int k = 0; k < i; k++) {
			tmp = tmp->next;

			if (tmp == nullptr) {
				//cout << "Indeks elementu #" << i << " jest spoza zakresu!" << endl << endl;
				return false;
			}
		}

		tmp->setData(newData);
		//cout << "Nowe dane:" << endl << tmp->getData() << endl;
		//cout << "Pomyslnie ustawiono dane elementu o indeksie #" << i << "!" << endl << endl;
		return true;
	}
	else {
		//cout << "Brak elementow na liscie!" << endl << endl;
		return false;
	}
}

template <typename T>
Element<T>* LinkedList<T>::search(const T& cmpData, int(*compare)(const T&, const T&)) {
	if (size > 0) {
		//cout << "Dane szukanego elementu:" << endl << *cmpData << endl;

		for (Element<T>* i = head; i != nullptr; i = i->next) {
			if (compare(cmpData, i->getData()) == 0) {
				//cout << "Element znaleziony pod adresem: " << i << endl << "o zawartosci: " << endl << i->getData() << endl << endl;
				return i;
			}
		}

		//cout << "Nie znaleziono pasujacego elementu!" << endl << endl;
		return nullptr;
	}
	else {
		//cout << "Brak elementow na liscie" << endl << endl;
		return nullptr;
	}
}

template <typename T>
bool LinkedList<T>::searchDelete(const T& cmpData, int(*compare)(const T&, const T&)) {
	if (size > 0) {
		Element<T>* tmp;
		//cout << "Dane elementu do usuniecia:" << endl << *cmpData << endl;

		for (Element<T>* i = head; i != nullptr; i = i->next) {
			if (compare(cmpData, i->getData()) == 0) {
				//cout << "Element znaleziony pod adresem: " << i << endl << "o zawartosci: " << endl << i->getData() << endl << endl;

				// usuwanie
				if (size > 1) {
					if (i == head) {
						tmp = i;
						i = tmp->next;
						i->prev = nullptr;
						head = i;
						tmp->prev = nullptr;
						tmp->next = nullptr;
						delete tmp;
					}
					else if (i == tail) {
						tmp = i;
						i = tmp->prev;
						i->next = nullptr;
						tail = i;
						tmp->prev = nullptr;
						tmp->next = nullptr;
						delete tmp;
					}
					else {
						tmp = i;
						i = tmp->prev;
						i->next = tmp->next;
						i = tmp->next;
						i->prev = tmp->prev;
						tmp->next = nullptr;
						tmp->prev = nullptr;
						delete tmp;
					}
					size--;
				}
				else {
					delete head;
					head = nullptr;
					tail = nullptr;

					size = 0;
				}
				
				//cout << "Pomyslnie usunieto szukany element!" << endl << endl;
				return true;
			}
		}

		//cout << "Nie znaleziono pasujacego elementu!" << endl << endl;
		return false;
	}
	else {
		//cout << "Brak elementow na liscie!" << endl << endl;
		return false;
	}
}

template <typename T>
void LinkedList<T>::addInOrder(const T& data, int(*compare)(const T&, const T&)) {
	if (size > 0) {
		Element<T>* tmp;
		Element<T>* newEl = new Element<T>(data);
		int result;

		for (Element<T>* i = head; i != nullptr; i = i->next) {
			result = compare(newEl->getData(), i->getData());

			if (result <= 0) {
				if (i == head) {
					i->prev = newEl;
					newEl->next = i;
					newEl->prev = nullptr;
					head = newEl;

					break;
				}
				else {
					tmp = i->prev;
					i->prev = newEl;
					newEl->next = i;
					newEl->prev = tmp;
					tmp->next = newEl;

					break;
				}
			}
			else if ((result > 0) && (i == tail)) {
				i->next = newEl;
				newEl->prev = i;
				newEl->next = nullptr;
				tail = newEl;

				break;
			}
		}

		//cout << "Pomyslnie dodano element!" << endl << endl;
		size++;
	}
	else {
		Element<T>* newEl = new Element<T>(data);
		newEl->next = nullptr;
		newEl->prev = nullptr;
		head = newEl;
		tail = newEl;

		size++;
	}
}

template <typename T>
void LinkedList<T>::clearList() {
	if (size > 0) {
		Element<T>* tmp;

		for (int k = 0; k < size; k++) {
			tmp = head;
			head = tmp->next;
			delete tmp;
		}
		size = 0;
		head = nullptr;
		tail = nullptr;

		cout << "Pomyslnie wyczyszczono liste!" << endl << endl;
	}
	else {
		cout << "Brak elementow na liscie!" << endl << endl;
	}
}

template <typename T>
void LinkedList<T>::showList() {
	cout << "Rozmiar listy: " << size << endl;
	cout << "Adres listy w pamieci: " << this << endl << endl;
	
	int k = 0;
	for (Element<T>* i = head; i != nullptr; i = i->next) {
		cout << "Element #" << k << ":" << endl;
		cout << i->getData() << endl;
		k++;
	}

	cout << endl;
}

template <typename T>
void LinkedList<T>::showList(int numEl) {
	cout << "Rozmiar listy: " << size << endl;
	cout << "Adres listy w pamieci: " << this << endl << endl;

	int k = 0;
	for (Element<T>* i = head; i != nullptr; i = i->next) {
		if (k < numEl) {
			cout << "Element #" << k << ":" << endl;
			cout << i->getData() << endl;
			k++;
		}
		else {
			break;
		}
	}

	cout << endl;
}

template <typename T>
string LinkedList<T>::listToStr(int numOfEl) { // TODO: poprawic warunki wyswietlania
	stringstream s;

	if (size > 0) {
		int k;

		if ((numOfEl < 0) || (numOfEl > size)) {
			k = size;
		}
		else {
			k = numOfEl;
		}

		for (Element<T>* i = head; i != nullptr; i = i->next) {
			if (k > 0) {
				s << i->getData();
				k--;
			}
			else {
				break;
			}
		}
	}
	else {
		s << "Brak elementow na liscie!";
	}	

	return s.str();
}

#endif
