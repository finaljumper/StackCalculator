template <class T>
class ArrayList {
public:

	Array(int = 10);

	~Array();
	void add(const T &element) {
		if (count == size) {
			size += 10;
			T * newCollection = new T[size];
			for (int i = 0; i < size; i++) {
				newCollection[i] = collection[i];
			}
			delete[] collection;
			this->collection = newCollection;
		}
		collection[count] = element;
		count++;
	}

	void add(T * element) {
		if (count == size) {
			T * newCollection = new T[size + 10];
			for (int i = 0; i < size; i++) {
				newCollection[i] = collection[i];
			}
			delete[] collection;
			this->collection = newCollection;
		}
		collection[count] = *element;
		count++;
	}

	void remove(int index) {
		int i = index;
		if (index < this->getSize()) {
			while (i != getSize()) {
				collection[i] = collection[i + 1];
				i++;
			}
			count--;
		}
		else
			count--;
	}

	void erase() {
		collection = NULL;
		count = 0;
	}

	T at(int index) {
		return collection[index];
	}

	int getSize() {
		return count;
	}
private:
	T * collection;
	int size;
	int count;
};

template<class T>
Array<T>::Array(int size)
{
}

template<class T>
Array<T>::~Array()
{
}
