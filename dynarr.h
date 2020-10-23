#ifndef DYNARR_H
#define DYNARR_H

using std::cout;
using std::endl;
using std::move;
using std::string;
using std::stringstream;

// implementacja tablicy i interfejsu
template <typename T>
class DynArr {
	int length;
	int capacity;
	float rRatio;
	T* arr;
public:
	DynArr() {
		length = 0;
		capacity = 1;
		rRatio = 2.0;
		arr = new T[capacity];
	}

	DynArr(int l) {
		length = l;
		capacity = l;
		rRatio = 2.0;
		arr = new T[capacity];
	}

	~DynArr() {
		delete[] arr;
	}

	int getLength() {
		return length;
	}

	void setLength(int length) {
		this->length = length;
	}

	int getCapacity() {
		return capacity;
	}

	void add(const T&);
	void deleteLast();
	T* getByIndex(int);
	bool setByIndex(int, const T&);
	void clearArray();
	string arrayToStr(int numOfEl = -1);
	string arrayToStrFull(int numOfEl = -1);
	void showArray(int numOfEl = -1);
};

template <typename T>
void DynArr<T>::add(const T& data) {
	if (length >= capacity) {
		T* tmp = new T[int(capacity * rRatio)]; // alokacja 200%

		for (int i = 0; i < length; i++) { // przepisanie
			tmp[i] = move(arr[i]);
		}
		delete[] arr; // uwolnienie pamieci
		arr = tmp;
		tmp = nullptr;
		capacity *= rRatio;
	}

	arr[length] = data; // dodanie elementu
	length++;
	//cout << "Pomyslnie dodano element" << endl;
};

template <typename T>
void DynArr<T>::deleteLast() {
	if (length > 0)
		length--;
	else
		cout << "Brak elementow w tablicy!" << endl;
}

template <typename T>
T* DynArr<T>::getByIndex(int i) {
	if (i < 0) {
		cout << "Indeks nie moze byc ujemny!" << endl;
		return nullptr;
	}

	if (length > 0) {
		if (i < length) {
			//cout << "Zwracany element o indeksie " << i << ":" << endl << arr[i] << endl;
			return &arr[i];
		}
		else {
			cout << "Indeks #" << i << " jest spoza zakresu!" << endl;
			return nullptr;
		}

	}
	else {
		cout << "Brak elementow w tablicy!" << endl;
		return nullptr;
	}
};

template <typename T>
bool DynArr<T>::setByIndex(int i, const T& newData) {
	if (i < 0) {
		cout << "Indeks nie moze byc ujemny!" << endl;
		return false;
	}

	if (length > 0) {
		if (i < length) {
			arr[i] = newData;
			return true;
		}
		else {
			cout << "Indeks #" << i << " jest spoza zakresu!" << endl;
			return false;
		}
	}
	else {
		cout << "Brak elementow w tablicy!" << endl;
		return false;
	}
};

template <typename T>
void DynArr<T>::clearArray() {
	if (length > 0) {
		length = 0;
		cout << "Pomyslnie wyczyszczono tablice!" << endl << endl;
	}
	else {
		cout << "Brak elementow w tablicy!" << endl;
	}
}

template <typename T>
string DynArr<T>::arrayToStr(int numOfEl) {
	stringstream s;

	if (length > 0) {
		int k;

		if ((numOfEl < 0) || (numOfEl > length)) {
			k = length;
		}
		else {
			k = numOfEl;
		}

		for (int i = 0; i < k; i++) {
			s << "#" << i << ": ";
			s << arr[i] << endl;
		}
	}
	else {
		s << "Brak elementow w tablicy!" << endl;
	}

	return s.str();
}

template <typename T>
string DynArr<T>::arrayToStrFull(int numOfEl) {
	stringstream s;

	if (length > 0) {
		int k;

		if ((numOfEl < 0) || (numOfEl > length)) {
			k = length;
		}
		else {
			k = numOfEl;
		}

		s << "Rozmiar tablicy: " << length << endl;
		s << "Maksymalny rozmiar tablicy: " << capacity << endl;
		s << "Adres tablicy: " << this << endl << endl;
		s << "Zawartosc tablicy:" << endl;

		for (int i = 0; i < k; i++) {
			s << "Element #" << i << ":" << endl;
			s << arr[i] << endl;
		}
	}
	else {
		s << "Brak elementow w tablicy!" << endl;
		s << "Maksymalny rozmiar tablicy: " << capacity << endl;
		s << "Adres tablicy: " << this << endl << endl;
	}

	return s.str();
}

template <typename T>
void DynArr<T>::showArray(int numOfEl) {
	cout << this->arrayToStrFull(numOfEl);
}

#endif