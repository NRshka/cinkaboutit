#define _CRT_SECURE_NO_WARNINGS
#ifndef _DENSE_H
#define _DENSE_H
#define alph 26
#define _long_alph 62

#include <fstream>
#include <utility>

using namespace std;

namespace bd {
const char table[63] = { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" };
class key {
public:
#ifdef _LONG_ALPH_
	static key& convert(unsigned long long x) {
		unsigned long long p = x;
		int n = 0;
		while (p) {
			p /= _long_alph;
			++n;
		}
		int len = n;
		char *res = new char[n + 1];
		res[n] = 0;
		unsigned long long _x = x;
		while (n>=0) {
			res[--n] = table[x % _long_alph];
			x /= _long_alph;
		}
		return *(new key(res, len + 1, _x));
	}
	void convertInt(char x[], int n) {
		num = 0;
		int k;
		for (int i = 0; i < n - 1; i++) {
			for (k = 0; k < _long_alph; k++) {
				if (table[k] == x[n - i - 2])
					break;
			}
			num += pow(_long_alph, i) * k;
		}
	}
#endif
private:
	char *str = nullptr;
	int len;
	unsigned long long num = 0;//�������� �������������
	key(char *x, int n, unsigned long long _num) :len(n), num(_num) {
		str = new char[len];
		for (int i = 0; i < len - 1; i++)
			str[i] = x[i];
		str[len - 1] = 0;
	}
public:
	key(int n) :len(n) {
		refresh();
	}
	key(char *x): len(strlen(x) + 1) {
		convertInt(x, len);
		str = new char[len];
		for (int i = 0; i < len - 1; i++)
			str[i] = x[i];
		str[len - 1] = 0;
	}
	key(char *x, int n) :len(n) {
		convertInt(x, n);
		str = new char[len];
		for (int i = 0; i < len - 1; i++)
			str[i] = x[i];
		str[len - 1] = 0;
	}
	key(key& x) {
		len = x.len;
		str = new char[len];
		for (int i = 0; i < len; i++)
			str[i] = x.str[i];
	}
	void operator++(int) {
		++num;
		int i = len - 2;
		char x = str[i];
		for (; i > 0; i--) {
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
		}
		str[i] = x;
		++i;
		for (; i < len - 1; i++) {
			str[i] = 'a';
		}
	}
	void operator--(int){
		for(int i=len-1;i>=0;i--){
			if(str[i] == '0'){
				str[i] = 'Z';
				break;
			}else{
				if(str[i] == 'A'){
					str[i] = 'z';
					break;
				}else{
					if(str[i] == 'a'){
						str[i] = '9';
						continue;
					}
				}
			}
		}
	}
	int operator%(int x){
		return num%x;
	}
	char& operator[](int x) {
		if (x >= len || x < 0)
			throw out_of_range("Bad size");
		return str[x];
	}
	key& operator+(key& x) {
		char *result = new char[len];
		char x1 = 0;
		char x2 = 0;
		char r = 0;
		char a = 0;
		for (int k = len - 2; k >= 0; k--) {
			for (int i = 0; i < _long_alph; i++) {
				if (table[i] == str[k])
					x1 = i;
				if (table[i] == x[k])
					x2 = i;
			}
			r = x1 + x2 + a;
			a = 0;
			if (r < _long_alph) {
				result[k] = table[r];
			}
			else {
				result[_long_alph - k] = table[r];
				++a;
			}
		}
		result[len - 1] = 0;
		return *(new key(result, len, num + x.num));
	}
	key& operator=(key& x) {
		delete[]str;
		len = x.len;
		str = new char[len];
		for (int i = 0; i < len; i++)
			str[i] = x.str[i];
		return *this;
	}
	key& operator-(key& x) {
		char *result = new char[len];
		char x1 = 0;
		char x2 = 0;
		char r = 0;
		char a = 0;
		for (int k = len - 2; k >= 0; k--) {
			for (int i = 0; i < _long_alph; i++) {
				if (table[i] == str[k])
					x1 = i;
				if (table[i] == x[k])
					x2 = i;
			}
			r = x1 - x2 - a;
			a = 0;
			if (r >= 0) {
				result[k] = table[r];
			}
			else {
				result[_long_alph - k] = table[r];
				++a;
			}
		}
		result[len - 1] = 0;
		return *(new key(result, len, num-x.num));
	}
	key& operator/(key &x) {
		return convert(num / x.num);
	}
	void Resize(int i) {
		if (i < 0)
			throw bad_array_new_length();
		if (i == len)
			return;
		if (i < len) {
			char *res = new char[i];
			for (int k = i - 1, l = 0; k >= 0; k--, l++)
				res[k] = str[len - 1 - l];
			delete[]str;
			str = res;
		}
		else {
			if (i > len) {
				char *res = new char[i + 1];
				int l = 0;
				for (int k = 0; k < len; k++, l++)
					res[i - l] = str[len - k - 1];
				for (int k = 0; k <= i - l; k++)
					res[k] = 'a';
				delete[]str;
				str = res;
			}
		}
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
	~key() {
		delete[]str;
	}
};

template<class T>
class Dense {
	friend class key;
public://just for func

private:
	int n = 0;//����� �������
	int listSize = 0;//����� ������� �� ��������
	int indexK = 0;//����� ����� (����� ��� �������� ����� ���������� ������)
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
	char* Add(T _thing) {
		if (ind < listSize) {
			olololo:
			int l = strlen(last_key->get_index()) + 1;
			arr[ind].first = new char[l];
			strcpy_s(arr[ind].first, l, last_key->get_index());
			arr[ind].second = _thing;
			++ind;
			(*last_key)++;
			return arr[ind-1].first;
		}
		else {
			for (int i = 0; i < listSize; i++)
				delete[]arr[i].first;
			//last_key->refresh();
			ind = 0;
			goto olololo;
		}
	}
	void hrya() {
		key a("aaaC5", 6);
		key &w = a - "aaab1";
	}
	void WritePage() {//������-�� ���� ������������ � �� �����, ���� ����, � �� ��������������, �� ���� � ��� �����
		dataFile.open(file_path, ios::out | ios::binary | ios::app);
		keysFile.open(keys_path, ios::out | ios::binary | ios::app);
		if (!(dataFile.is_open() && keysFile.is_open()))
			throw "Smth about genocide";
		for (int i = 0; i < listSize; i++) {
			dataFile.write((char*)&arr[i].second, sizeof(T));
			keysFile.write(arr[i].first, indexK * sizeof(char));
			int s = (pages * listSize + i) * sizeof(T);//��������� �� ������ � dataFile
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
	T Search(key& x) {
		//�������� ���� ��� �� ���-�� ������ �� ����� ��������
		//��������� �������� �� ���������� ������
		//����� �� �������� ������ ������
		//???
		//PROFIT!
		key& page = last_key / listSize;
		int elem = last_key/listSize;
		ReadPage(page.num);
		return arr[elem].first;
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
	int GetKeySize() {
		return indexK;
	}
	int Remove() {
		if (keys_path && file_path)
			return std::remove(keys_path) + std::remove(file_path);
		return -1;
	}
};

}
#endif // !_DENSE_H
