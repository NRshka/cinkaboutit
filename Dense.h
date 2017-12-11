#define _CRT_SECURE_NO_WARNINGS
#ifndef _DENSE_H
#define _DENSE_H
#define alph 26

#include <fstream>
#include <utility>

using namespace std;

namespace bd {

template<class T>
class Dense {
private://just for func
	class key {
		friend class Dense;
	private:
		//char table[63] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		char *str = nullptr;
		const int len;
		key(int n, char *_str):str(_str), len(n) {
		}
	public:
		key(int n):len(n) {
			refresh();
		}
		void operator++(int) {
			int i = len - 2;
			char x;
			/*for (; i > 0; i--) {
				if ((x >= 65 && x < 90) || (x >= 97 && x < 122) || (x >= 48 && x < 57)) {
					++x;
					break;
				}
				else {
					if (x == 122) {
						x = 65;
						break;
					}
					else {
						if (x == 57)
							continue;
						else {
							if (x == 90) {
								x = 48;
								break;
							}
						}
					}
				}
			}*/

			for (; i >= 0; i--) {
				x = str[i];
				if (x == 'z')
					str[i] = 'a';
				else {
					++str[i];
					break;
				}
			}

			/*str[i] = x;
			++i;
			for (; i < len - 1; i++) {
				str[i] = 'a';
			}*/
		}
		char* get_index() {
			return str;
		}
		void refresh() {
			if (str)
				delete[]str;
			str = new char[len];
			for (int i = 0; i < len - 1; i++)
				str[i] = 'a';
			str[len - 1] = 0;
		}
		key* operator-(key *x) {
			char *res = new char[len];
			int a;
			for (int q = x.len - 2; q >= 0; q--) {
				char c1 = str[q];
				char c2 = x.str[q];
				int i = 0;
				for (; table[i] != c1 && i < 62; i++);
				int j = 0;
				for (; table[j] != c2 && j < 62; j++);
				int r = i - j - a;
				a = 0;
				if (r > 0)
					res[q] = r;
				else {
					res[62 - q] = r;
					a--;
				}
			}
			res[len - 1] = 0;
			return new key(len, res);
		}
		/*key* operator-(char x[]) {
			char *res = new char[len];
			int a = 0;
			for (int q = len - 2; q >= 0; q--) {
				char c1 = str[q];
				char c2 = x[q];
				int i = 0;
				for (; table[i] != c1 && i < 62; i++);
				int j = 0;
				for (; table[j] != c2 && j < 62; j++);
				int r = i - j - a;
				a = 0;
				if (r > 0)
					res[q] = r;
				else {
					res[62 - q] = r;
					a--;
				}
			}
			res[len - 1] = 0;
			return new key(len, res);
		}*/
		key* operator-(char x[]) {
			char *res = new char[len];
			int a = 0;
			for (int i = len - 2; i >= 0; i--) {
				if (str[i] >= x[i]) {
					res[i] = str[i] - x[i] + 97 + a;
					a = 0;
				}
				else {
					--a;
					res[i] = 'z' + str[i] - x[i];
				}
			}
			res[len - 1] = 0;
			return new key(len, res);
		}
		bool operator>(char x[]) {
			for (int i = 0; i < len; i++) {
				if (str[i] == x[i])
					continue;
				if (str[i] > x[i])
					return 1;
				return 0;
			}
			return 0;
		}
		~key() {
			delete[]str;
		}
	};
private:
	int n = 0;//число записей
	int listSize = 0;//число записей на странице
	int indexK = 0;//длина ключа (чтобы все страницы имели одинаковый размер)
	fstream dataFile, keysFile;
	char *file_path = nullptr;
	char *keys_path = nullptr;
	pair<char*, T> *arr;
	key *last_key;
	int ind = 0;
	int pages = 0;
public:
	Dense(const char x[]) {
		file_path = new char[len(x) + 1];
		strcpy(file_path, x);
	}
	Dense(int _n, int _m) {
		if (_n < 0)
			throw out_of_range("Number of entires can't be negative!");
		if (_m < 0)
			throw out_of_range("Length of page can't be negative!");
		n = _n;
		listSize = _m;
		arr = new pair<char*, T>[_m];
		indexK = n*listSize / alph + 2;// \0 Hail NULL! \0
		last_key = new key(indexK);
	}
	void SetPath(char x[]) {
		delete[]file_path;
		int lenstr = strlen(x);
		file_path = new char[lenstr + 1];
		strcpy_s(file_path, lenstr + 1, x);
		int j = lenstr;
		for (; j >= 0 && x[j] != '.'; j--);
		keys_path = new char[j + 5];//.kys\0
		for (int i = 0; x[i] != '.'; i++)
			keys_path[i] = x[i];
		keys_path[j] = '.';
		keys_path[j + 1] = 'k';
		keys_path[j + 2] = 'y';
		keys_path[j + 3] = 's';
		keys_path[j + 4] = 0;
		//strcat_s(keys_path, 4, "kys");
	}
	int Add(T _thing) {
		if (ind < listSize) {
			olololo:
			int l = strlen(last_key->get_index()) + 1;
			arr[ind].first = new char[l];
			strcpy_s(arr[ind].first, l, last_key->get_index());
			arr[ind].second = _thing;
			++ind;
			(*last_key)++;
			return 0;
		}
		else {
			for (int i = 0; i < listSize; i++)
				delete[]arr[i].first;
			//last_key->refresh();
			ind = 0;
			goto olololo;
		}
		return -1;
	}
	void WritePage() {//вообще-то надо дозаписывать в то место, куда надо, а не перезаписывать, но пока и так сойдёт
		dataFile.open(file_path, ios::out | ios::binary | ios::app);
		keysFile.open(keys_path, ios::out | ios::binary | ios::app);
		if (!(dataFile.is_open() && keysFile.is_open()))
			throw "Smth about genocide";
		for (int i = 0; i < listSize; i++) {
			dataFile.write((char*)&arr[i].second, sizeof(T));
			keysFile.write(arr[i].first, indexK * sizeof(char));
			int s = (pages * listSize + i) * sizeof(T);//указатель на запись в dataFile
			keysFile.write((char*)&s, sizeof(int));
			if (dataFile.fail() || keysFile.fail())
				throw "Ich Singer Bister tag Ervaght!";
		}
		++pages;
		dataFile.close();
		keysFile.close();
	}
	void ReadPage(int _n) {
		if (_n * listSize > n || _n < 0)
			throw "It haven't page by this index";
		dataFile.open(file_path, ios::in | ios::binary);
		keysFile.open(keys_path, ios::in | ios::binary);
		if (!(dataFile.is_open() && keysFile.is_open()))
			throw "Smth about genocide";
		int i = 0;

		keysFile.seekg(_n * listSize * (indexK*sizeof(char) + sizeof(int)), ios_base::beg);
		while (i < listSize) {
			arr[i].first = new char[indexK];
			keysFile.read(arr[i].first, indexK * sizeof(char));
			int k = 0;
			keysFile.read((char*)&k, sizeof(int));
			dataFile.seekg(k, ios_base::beg);
			dataFile.read((char*)&arr[i].second, sizeof(T));
			++i;
		}
		dataFile.close();
		keysFile.close();
	}
	T SearchByKey(char x[]) {
		int i = 0;
		key *fkey = last_key;
		for (int k = 0; k < n*listSize; k++)
			(*fkey)++;
		while (*fkey > x) {
			fkey = (*fkey) - x;
			++i;
		}
		ReadPage(i);
		for (int i = 0; i < listSize; i++) {
			if (strcmp(arr[i].first, fkey->str))
				return arr[i].second;
		}
	}
	void test() {
		for (int i = 0; i < listSize; i++)
			cout << arr[i].second << endl;
	}
	int Insert(T _thing) {
		
	}
	~Dense() {
		delete[]file_path;
		delete[]keys_path;
		for (int i = 0; i < ind; i++) {
			delete[]arr[i].first;
		}
		delete[]arr;
		//delete[]last_key;
	}
};

}
#endif // !_DENSE_H
