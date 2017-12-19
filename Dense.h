#define _CRT_SECURE_NO_WARNINGS
#ifndef _DENSE_H
#define _DENSE_H
#define alph 26
#ifdef _EXTENDED_ALPHABET_KEY_BD_
#define _long_alph 62
#endif

#include <fstream>
#include <utility>

using namespace std;

namespace bd {
#ifdef _EXTENDED_ALPHABET_KEY_BD_
const char table[63] = { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" };
#endif

char* move_left(char* x, int delta, int len, int fir) {
	char* res = new char[len - delta];
	for (int i = 0; i < len - delta; i++) {
		res[i] = x[fir + i];
	}
	delete[]x;
	return res;
}

class key {
public:
#ifdef _long_alph
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
	int convertInt(char x[], int n) {
		num = 0;
		int k;
		for (int i = 0; i < n - 1; i++) {
			for (k = 0; k < _long_alph; k++) {
				if (table[k] == x[n - i - 2])
					break;
			}
			num += pow(_long_alph, i) * k;
		}
		return num;
	}
#endif
private:
	char *str = nullptr;
	int len;
	key(char *x, int n, unsigned long long _num) :len(n), num(_num) {
		str = new char[len + 1];
		for (int i = 0; i < len; i++)
			str[i] = x[i];
		str[len - 1] = 0;
	}
public:
	long long num = 0;//численное представление
	/*explicit key(int n) :len((n > 0) ? n : 0) {
		if (n > 0)
			refresh();
	}*/
	key(int x) {
		unsigned long long p = x;
		num = x;
		int n = 0;
		while (p) {
			p /= _long_alph;
			++n;
		}
		//int len = n;
		char *res = new char[n + 1];
		this->len = n;
		res[n] = 0;
		unsigned long long _x = x;
		while (n > 0) {
			res[--n] = table[x % _long_alph];
			x /= _long_alph;
		}
		str = res;
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
#ifdef _long_alph
	void operator++(int) {
		++num;
		int i = len - 1;
		char x = str[i];
		for (; i >= 0; i--) {
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
					if (x == 57) {
						str[i] = 'a';
						if (i == 0) {
							char* res = new char[len + 2];
							res[0] = 'b';
							for (int k = 0; k <= len; k++)
								res[k + 1] = str[k];
							delete[]str;
							str = res;
							++len;
						}
						continue;
					}
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
	}
	void operator--(int){
		--num;
		for (int i = len - 1; i >= 0; i--) {
			if (str[i] == '0') {
				str[i] = 'Z';
				break;
			}
			else {
				if (str[i] == 'A') {
					str[i] = 'z';
					break;
				}
				else {
					if (str[i] == 'a') {
						str[i] = '9';
						continue;
					}
					else {
						if (str[i] == 'b') {
							if (i == 0) {
								char* res = new char[len];
								for (int k = 0; k < len; k++)
									res[k] = str[k + 1];
								delete[]str;
								str = res;
								--len;
								break;
							}
						}
					}
				}
			}
		}
	}
	int operator%(int x){
		return num%x;
	}
	key& operator+(key& x) {
		char x1 = 0;//цифра первого числа
		char x2 = 0;//цифра второго числа
		char r = 0;//результат вычисления
		char a = 0;//перенос на следующий разряд
		char *result;
		int max_len = (len >= x.len) ? len : x.len;
		int max = 0;
		if (len >= x.len) {
			max = len;
			result = new char[max + 2];
			for (int i = 1; i < max + 1 - x.len; i++)//перекопировать в результат самый длинный список, чтобы не надо было дописывать потом
				*(result + i) = *(str + i - 1);//за исключением последних символов, которые в любом случае изменятся из-за сложения
		}
		else {
			max = x.len;
			result = new char[max + 2];
			for (int i = 1; i < max + 2 - len; i++)
				*(result + i) = *(x.str + i - 1);
		}
		int min_len = (len <= x.len) ? len : x.len;

		for (int k1 = len - 1, k2 = x.len - 1; min_len > 0; min_len--, k1--, k2--, max--) {
			for (int i = 0; i < _long_alph; i++) {
				if (table[i] == str[k1])
					x1 = i;
				if (table[i] == x[k2])
					x2 = i;
			}
			r = x1 + x2 + a;
			a = 0;
			if (r < _long_alph) {
				result[max] = table[r];
			}
			else {
				result[max] = table[_long_alph - r];
				a += r / _long_alph;
			}
		}
		result[max_len + 1] = 0;
		bool z = false;//увеличилась ли длина (не делать перенос)
		while (a > 0) {//перенос на старший разряд
			for (int i = 0; i < _long_alph; i++) {
				if (result[max] == table[i]) {
					x1 = i;
					break;
				}
			}
			r = x1 + a;
			if (r < _long_alph) {
				result[--max] = table[r];
				a = 0;
				z = true;
			}
			else {
				result[--max] = table[_long_alph - r];
				a += r / _long_alph;
			}
		}
		if (z)//test it!
			result = move_left(result, 1, max_len + 2, 1);
		return *(new key(result, len, num + x.num));
	}
	key& operator-(key& x) {
		char x1 = 0;//цифра первого числа
		char x2 = 0;//цифра второго числа
		char r = 0;//результат вычисления
		char a = 0;//перенос на следующий разряд
		char *result;
		int max_len = (len >= x.len) ? len : x.len;
		int max = 0;
		if (len >= x.len) {
			max = len - 1;
			result = new char[max + 2];
			for (int i = 0; i < max + 2 - x.len; i++)//перекопировать в результат самый длинный список, чтобы не надо было дописывать потом
				*(result + i) = *(str + i);//за исключением последних символов, которые в любом случае изменятся из-за сложения
		}
		else {
			max = x.len;
			result = new char[max + 2];
			for (int i = 0; i < max + 2 - len; i++)
				*(result + i) = *(x.str + i);
		}
		int min_len = (len <= x.len) ? len : x.len;

		for (int k1 = len - 1, k2 = x.len - 1; min_len > 0; min_len--, k1--, k2--, max--) {
			for (int i = 0; i < _long_alph; i++) {
				if (table[i] == str[k1])
					x1 = i;
				if (table[i] == x[k2])
					x2 = i;
			}
			r = x1 - x2 - a;
			a = 0;
			if (r > 0) {
				result[max] = table[r];
			}
			else {
				result[max] = table[_long_alph + r];
				a += 1;
			}
		}
		result[max_len] = 0;
		bool z = false;//увеличиласт ли длина (не делать перенос)
		while (a > 0) {//перенос на старший разряд
			for (int i = 0; i < _long_alph; i++) {
				if (result[max] == table[i]) {
					x1 = i;
					break;
				}
			}
			r = x1 - a;
			if (r < _long_alph) {
				result[max--] = table[r];
				a = 0;
				z = true;
			}
			else {
				result[max--] = table[_long_alph - r];
				a += 1;
			}
		}
		int i = 0;
		while (result[i++] == 'a');
		if(i - 1)
			result = move_left(result, i - 1, max_len + 1, 1);
		return *(new key(result, len, num - x.num));
	}
	key& operator*(key& x) {
		int x1 = 0;//цифра первого числа
		int x2 = 0;//цифра второго числа
		int r = 0;//результат вычисления
		int a = 0;//перенос на следующий разряд
		char *result;
		//int max_len = (len >= x.len) ? len : x.len;
		int index = len + x.len + 1;
		int max = index;
		result = new char[index];
		int min_len = (len <= x.len) ? len : x.len;
		for (int i = 0; i < index; i++)
			result[i] = 1;
		int max_index = index;
		for (int k1 = len - 1, i = 0; k1 >= 0; k1--, index = max_index - i, i++) {
			for (int k2 = x.len - 1; k2 >= 0; k2--, index--) {
				for (int i = 0; i < _long_alph; i++) {
					if (table[i] == str[k1])
						x1 = i;
					if (table[i] == x.str[k2])
						x2 = i;
				}
				r = x1 * x2 + a;
				//result[index - 2] += x1 * x2 + a;
				if (r > _long_alph) {
					a = r / _long_alph;
					result[index - 2] += table[r % _long_alph];//сделать нормальное суммирование, чтобы суммировались индексы таблицы и, в случае чего делался перенос на старший разряд
				}
				else {
					a = 0;
					result[index - 2] += table[r];
				}
			}
		}
		result[index - 1] = 0;
		if (index > 0) {
			result = move_left(result, index, max, index + 1);
		}
		return *(new key(result, len, num - x.num));
	}
#endif
	bool operator>(key& x) {
		int size = (len <= x.len) ? len : x.len;
		for (int i = 0; i <size; i++) {
			if (str[i] > x.str[i])
				return true;
			else {
				if (str[i] < x.str[i])
					return false;
			}
		}
		return false;
	}
	bool operator<(key& x) {
		int size = (len <= x.len) ? len : x.len;
		for (int i = 0; i <size; i++) {
			if (str[i] < x.str[i])
				return true;
			else {
				if (str[i] > x.str[i])
					return false;
			}
		}
		return false;
	}
	bool operator>=(key& x) {
		int size = (len <= x.len) ? len : x.len;
		for (int i = 0; i <size; i++) {
			if (str[i] >= x.str[i])
				return true;
			else {
				if (str[i] < x.str[i])
					return false;
			}
		}
		return true;
	}
	bool operator<=(key& x) {
		int size = (len <= x.len) ? len : x.len;
		for (int i = 0; i <size; i++) {
			if (str[i] <= x.str[i])
				return true;
			else {
				if (str[i] > x.str[i])
					return false;
			}
		}
		return true;
	}
	key& operator/(int x) {
		return convert(num / x);
	}
	key& operator/(key &x) {
		return convert(num / x.num);
	}
	char& operator[](int x) {
		if (x >= len || x < 0)
			throw out_of_range("Bad size");
		return str[x];
	}
	key& operator=(key& x) {
		delete[]str;
		len = x.len;
		str = new char[len];
		for (int i = 0; i < len; i++)
			str[i] = x.str[i];
		return *this;
	}
	key& operator=(char x[]) {
		delete[]str;
		int size = strlen(x) + 1;
		str = new char[size];
		len = size;
		for (int i = 0; i < size; i++)
			str[i] = x[i];
		return *this;
	}
	key& operator=(int x) {
		delete[]str;
		unsigned long long p = x;
		int n = 0;
		while (p) {
			p /= _long_alph;
			++n;
		}
		//int len = n;
		char *res = new char[n + 1];
		this->len = n + 1;
		res[n] = 0;
		unsigned long long _x = x;
		while (n > 0) {
			res[--n] = table[x % _long_alph];
			x /= _long_alph;
		}
		str = res;
		return *this; 
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
			res[i - 1] = 0;
			str = res;
			len = i;
		}
		else {
			if (i > len) {
				char *res = new char[i + 1];
				int l = 0;
				for (int k = 0; k < len; k++, l++)
					res[i - l] = str[len - k - 1];
				for (int k = 0; k <= i - l; k++)
					res[k] = 'a';
				if (str)
					delete[]str;
				//res[i - 1] = 0;
				str = res;
				len = i;
			}
		}
	}
	operator char*() {
		char* res = new char[len + 1];
		for (int i = 0; i <= len; i++)
			res[i] = str[i];
		return res;
	}
	operator int() {
		return convertInt(str, len);
	}
	char* get_index() {
		return str;
	}
	void refresh() {
		if (str)
			delete[]str;
		str = new char[len + 1];
		for (int i = 0; i < len; i++)
			str[i] = 'a';
		str[len] = 0;
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
	int n = 0;//кол-во записей
	int listSize = 0;//кол-во элементов
	int indexK = 0;//длина ключа
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
		indexK = n*listSize / alph + 1;// \0 Hail NULL! \0
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
	friend class key;
	T Search(key& x) {
		key& page = (*last_key) / listSize;
		int elem = *last_key % listSize;
		ReadPage(page.num);
		return arr[elem].second;
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
