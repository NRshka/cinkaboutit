#define _CRT_SECURE_NO_WARNINGS
#ifndef _DENSE_H
#define _DENSE_H
#define alph 26
#include <conio.h>
#include <random>
#define size_block (len_key+2*sizeof(int))

#define ulong unsigned long
#include <fstream>
//#include <utility>

using namespace std;

namespace bd {
char table[63] = { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" };
int _long_alph = 62;

char* move_left(char* x, int delta, int len, int fir) {
	char* res = new char[len - delta];
	for (int i = 0; i < len - delta; i++) {
		res[i] = x[fir + i];
	}
	delete[]x;
	return res;
}


class key {
	friend ostream& operator<<(ostream& f, key &x);
	friend istream& operator>>(istream& f, key &x);
public:
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
		for (int i = 0; i < n; i++) {
			for (k = 0; k < _long_alph; k++) {
				if (table[k] == x[n - i - 1])
					break;
			}
			num += pow(_long_alph, i) * k;
		}
		return num;
	}
private:
	char *str = nullptr;
	int len;
	key(char *x, int n, unsigned long long _num) :len(n + 1), num(_num) {
		str = new char[len];
		for (int i = 0; i < len - 1; i++)
			str[i] = x[i];
		str[len - 1] = 0;
	}
public:
	long long num = 0;//численное представление
	/*explicit key(int n) :len((n > 0) ? n : 0) {
		if (n > 0)
			refresh();
	}*/
	key() {
		str = new char[2];
		str[0] = table[0];
		str[1] = 0;
		len = 1;
	}
	key(int x) {
		unsigned long long p = x;
		num = x;
		if (x == 0) {
			str = new char[2];
			len = 1;
			str[0] = table[x];
			str[1] = 0;
			return;
		}
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
		//delete[]str;
		str = new char[len + 1];
		for (int i = 0; i < len; i++)
			str[i] = x.str[i];
		str[len] = 0;
	}
	void random(int _len) {
		delete[]str;
		len = _len + 1;
		str = new char[len];
		for (int i = 0; i < len; i++)
			str[i] = table[rand() % _long_alph];
		str[_len] = 0;
	}
	int length() {
		return len;
	}
	void operator++(int) {
		for (int i = len - 1; i >= 0; i--) {
			if (str[i] == table[_long_alph - 1]) {
				str[i] = table[0];
				continue;
			}
			else {
				int x = 0;
				for (int k = 0; k < _long_alph; k++) {
					if (str[i] == table[k]) {
						x = k;
						break;
					}
				}
				str[i] = table[++x];
				return;
			}
		}
		char *buf = new char[len + 2];
		buf[0] = table[1];
		for (int i = 0; i < len + 1; i++)
			buf[i + 1] = str[i];
		delete[]str;
		++len;
		str = buf;
	}
	void operator--(int){
		if (len == 0)
			return;
		for (int i = len - 1; i >= 0; i--) {
			if (str[i] == table[0]) {
				str[i] = table[_long_alph - 1];
				continue;
			}
			else {
				int x = 0;
				for (int k = 0; k < _long_alph; k++) {
					if (str[i] == table[k]) {
						x = k;
						break;
					}
				}
				if (x == 1 && i == 0)
					break;
				str[i] = table[--x];
				return;
			}
		}
		char *buf = new char[len];
		for (int i = 0; i < len; i++)
			buf[i] = str[i + 1];
		delete[]str;
		--len;
		str = buf;
	}
	int operator%(int x){
		return convertInt(str, len) % x;
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
		//int min_len = (len <= x.len) ? len : x.len;
		for (int i = 0; i < index; i++)
			result[i] = 'a';
		int max_index = index;
		int big_len = 0;
		int min_len = 0;
		char* big_str = nullptr;
		char* min_str = nullptr;
		if (len >= x.len) {
			big_len = len;
			min_len = x.len;
			big_str = str;
			min_str = x.str;
		}
		else {
			big_len = x.len;
			min_len = len;
			big_str = str;
			min_str = x.str;
		}
		for (int k1 = min_len - 1, i = 0; k1 >= 0; k1--, i++) {
			index = max_index - i;
			for (int k2 = big_len - 1; k2 >= 0; k2--, index--) {
				for (int i = 0; i < _long_alph; i++) {
					if (table[i] == min_str[k1])
						x1 = i;
					if (table[i] == big_str[k2])
						x2 = i;
				}
				r = x1 * x2 + a;
				//result[index - 2] += x1 * x2 + a;
				if (r > _long_alph) {
					a = r / _long_alph;
					for (int i = 0; i < _long_alph; i++) {
						if (table[i] == result[index - 2]) {
							x1 = i;
							break;
						}
					}
					if(x1 + r%_long_alph < _long_alph)
						result[index - 2] = table[x1 + r % _long_alph];
					else {
						result[index - 2] = table[_long_alph - (x1 + r % _long_alph)];
						a += (x1 + r % _long_alph) / _long_alph;
					}
				}
				else {
					a = 0;
					for (int i = 0; i < _long_alph; i++) {
						if (table[i] == result[index - 2]) {
							x1 = i;
							break;
						}
					}
					if(x1 + r<_long_alph)
						result[index - 2] = table[r + x1];
					else {
						a = (x1 + r) / _long_alph;
						result[index - 2] = table[_long_alph - r - x1];
					}
				}
			}
		}
		result[max_index - 1] = 0;
		if (index - 1 > 0) {
			result = move_left(result, index - 1, max, index - 1);
		}
		return *(new key(result, max_index - index + 1, num * x.num));
	}
	
	bool operator>(key& x) {
		int size = ((len <= x.len) ? len : x.len) - 1;
		if (len > x.len)
			return true;
		if (x.len > len)
			return false;
		for (int i = 0; i <size; i++) {
			int x1 = 0;
			int x2 = 0;
			for (int k = 0; k < _long_alph; k++) {
				if (str[i] == table[k])
					x1 = k;
				if (x.str[i] == table[k])
					x2 = k;
			}
			if (x1 > x2)
				return true;
			else {
				if (x1 < x2)
					return false;
			}
		}
		return false;
	}
	bool operator==(key& x) {
		if (len != x.len)
			return false;
		for (int i = 0; i <len; i++) {
			int x1 = 0;
			int x2 = 0;
			for (int k = 0; k < _long_alph; k++) {
				if (str[i] == table[k])
					x1 = k;
				if (x.str[i] == table[k])
					x2 = k;
			}
			if (x1 != x2)
				return false;
		}
		return true;
	}
	bool operator<(key& x) {
		int size = (len <= x.len) ? len : x.len;
		if (len > x.len)
			return false;
		if (x.len > len)
			return true;
		for (int i = 0; i <size; i++) {
			int x1 = 0;
			int x2 = 0;
			for (int k = 0; k < _long_alph; k++) {
				if (str[i] == table[k])
					x1 = k;
				if (x.str[i] == table[k])
					x2 = k;
			}
			if (x1 > x2)
				return false;
			else {
				if (x1 < x2)
					return true;
			}
		}
		return false;
	}
	bool operator>=(key& x) {
		int size = (len <= x.len) ? len : x.len;
		if (len > x.len)
			return true;
		if (x.len > len)
			return false;
		for (int i = 0; i <size; i++) {
			int x1 = 0;
			int x2 = 0;
			for (int k = 0; k < _long_alph; k++) {
				if (str[i] == table[k])
					x1 = k;
				if (x.str[i] == table[k])
					x2 = k;
			}
			if (x1 >= x2)
				return true;
			else {
				if (x1 <= x2)
					return false;
			}
		}
		return false;
	}
	bool operator<=(key& x) {
		int size = ((len <= x.len) ? len : x.len) - 1;
		if (len > x.len)
			return false;
		if (x.len > len)
			return true;
		for (int i = 0; i <size; i++) {
			int x1 = 0;
			int x2 = 0;
			for (int k = 0; k < _long_alph; k++) {
				if (str[i] == table[k])
					x1 = k;
				if (x.str[i] == table[k])
					x2 = k;
			}
			if (x1 > x2)
				return false;
			else {
				if (x1 < x2)
					return true;
			}
		}
		return true;
	}
	int operator/(int x) {
		return convertInt(str, len) / x;
	}
	int operator/(key &x) {
		if (x.len > len)
			return 0;
		key left = pow(10, len - x.len);
		key right = pow(10, len - x.len + 2) - 1;
		key middle = (left + right) / 2;
		while (1) {
			key* a = &(*this - x * middle);
			if (a->len < x.len) {
				delete a;
				return middle.convertInt(middle.str, middle.len);
			}
			delete a;
			key* res = &(middle * x);
			if (res->len == this->len) {
				if (*res > *this) {
					middle--;
					continue;
				}
				if (*res < *this) {
					middle++;
					continue;
				}
			}
			if (res->len > this->len) {
				right = middle;
				middle = (left + right) / 2;
				continue;
			}
			if (res->len < this->len) {
				left = middle;
				middle = (left + right) / 2;
				continue;
			}
		}
	}
	char& operator[](int x) {
		if (x >= len || x < 0)
			throw out_of_range("Bad size");
		return str[x];
	}
	key& operator=(key& x) {
		delete[]str;
		len = x.len;
		str = new char[len + 1];
		for (int i = 0; i < len + 1; i++)
			str[i] = x.str[i];
		return *this;
	}
	key& operator=(char x[]) {
		delete[]str;
		len = strlen(x) + 1;
		str = new char[len + 1];
		for (int i = 0; i < len ; i++)
			str[i] = x[i];
		return *this;
	}
	key& operator=(int x) {
		delete[]str;
		if (x == 0) {
			str = new char[2];
			str[0] = table[0];
			str[1] = 0;
			len = 1;
			return *this;
		}
		unsigned long long p = x;
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
		return *this; 
	}
	void Resize(int i) {
		if (i < 0)
			throw bad_array_new_length();
		if (i == len)
			return;
		if (i < len) {
			char *res = new char[i + 1];
			for (int k = i, l = 0; k >= 0; k--, l++)
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
				res[i] = 0;
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
	/*
	operator int() {
		return convertInt(str, len);
	}*/
	char* get_index() {
		return str;
	}
	~key() {
		delete[]str;
	}
};

ostream& operator<<(ostream& f, key &x) {
	f << x.str;
	return f;
}

istream& operator>>(istream& f, key &x) {
	char c = 0;
	delete[]x.str;
	int size = 5;
	x.str = new char[size];
	int l = 0;
	cin.ignore();
	c = _getch();
	while (c != '\n' && c != -1&& c!= '\r') {
		cin.clear();
		bool z = true;
		for (int i = 0; i < _long_alph; i++) {
			if (table[i] == c)
				z = false;
		}
		if (z)
			throw "Alphabet haven't a symbol";
		++l;
		if (l >= size) {
			char *buf = new char[size + 5];
			for (int i = 0; i < size; i++)
				buf[i] = x.str[i];
			delete[]x.str;
			x.str = buf;
			size += 5;
		}
		c = _getch();
	}
	x.len = l;
	if (l < size) {
		char *buf = new char[l];
		for (int i = 0; i < l; i++)
			buf[i] = x.str[i];
		delete[]x.str;
		x.str = buf;
	}
	return f;
}

template<class T>
class Dense {
	friend class key;
	friend ostream& operator<<(ostream& f, Dense<T>& x);
private://just for func
	template<class K, class T>
	struct pair {
		K first;
		T second;
		pair() {

		}
		pair(K one, T two):first(one),second(two) {

		}
	};
private:
	int len_key = 150;
	int listSize = 0;//кол-во элементов
	fstream dataFile, keysFile, tableFile;
	char *file_path = nullptr;//путь к файлу с хранимыми данными
	char *keys_path = nullptr;//файл с ключами
	char *table_path = nullptr;//файл оглавления
	pair<key, T> *arr = nullptr;//массив с записями одной страницы
	key last_key = 0;//тндекс последного ключа, используется при дополнении данных
	int ind = 0;//количество непустых записей на странице

	bool empty_page = true;//пуста ли страница в текущий момент
public:
	Dense(int _n, int _m) {
		if (_n < 0 || _m < 0)
			throw out_of_range("Number of entires can't be negative!");
		listSize = _n;
		int a = 1;
		int i = 0;
		for (; a < _m; i++)
			a *= _long_alph;
		len_key = i + 10;
		arr = new pair<key, T>[listSize];
	}
	Dense(Dense& x) {
		listSile = x.listSize;
		ind = x.ind;
		arr = new key[listSize];
		file_path = new char[strlen(x.file_path) + 1];
		strcpy(file_path, x.file_path);
		keys_path = new char[strlen(x.keys_path) + 1];
		strcpy(keys_path, x.keys_path);
		table_path = new char[strlen(x.table_path) + 1];
		strcpy(table_path, x.table_path);
	}
#ifdef _USERS_ALPHABET_BD_KEY_
	void SetAlphabet(char* x) {
		int len = strlen(x);
		if (len) {
			delete[]table;
			table = new char[len + 1];
			strcpy(table, x);
		}
	}
#endif
	T SearchEntry(T elem) {
		int pages = GetPageCount();
		int n = pages * listSize;
		T a;
		for (int i = 0; i < n; i++)
		{
			a = Search(key(i));
			if (a == elem)
				return a;
		}
		throw "Entry wasn\t found";
	}
	void SetPath(char x[]) {
		delete[]file_path;
		int lenstr = strlen(x);
		file_path = new char[lenstr + 1];
		strcpy_s(file_path, lenstr + 1, x);
		int j = lenstr;
		for (; j >= 0 && x[j] != '.'; j--);
		keys_path = new char[j + 5];//.kys\0
		table_path = new char[j + 5];
		for (int i = 0; x[i] != '.'; i++) {
			keys_path[i] = x[i];
			table_path[i] = x[i];
		}
		keys_path[j] = '.';
		keys_path[j + 1] = 'k';
		keys_path[j + 2] = 'y';
		keys_path[j + 3] = 's';
		keys_path[j + 4] = 0;
		table_path[j] = '.';
		table_path[j + 1] = 'p';
		table_path[j + 2] = 'g';
		table_path[j + 3] = 'e';
		table_path[j + 4] = 0;
		tableFile.open(table_path, ios::out | ios::in | ios::binary);
		if (!tableFile.is_open()) {
			ofstream a(table_path);
			a.close();
			tableFile.open(table_path, ios::out | ios::binary);
			if (!tableFile.is_open())
				throw "Can't open file";
			tableFile.seekp(0, ios::beg);
			tableFile.write((char*)&listSize, sizeof(int));//кол-во записей на одной странице
			int i = 0;
			tableFile.write((char*)&i, sizeof(int));//кол-во страниц
			keysFile.open(keys_path, ios::binary);
			keysFile.close();
			tableFile.close();
			return;
		}
		else {
			tableFile.seekg(0, ios::beg);
			tableFile.read((char*)&listSize, sizeof(int));
			if (listSize <= 0)
				throw "Count of entry in page is not positive";
		}
		int i = 0;
		tableFile.read((char*)&i, sizeof(int));
		if (i == 0) {
			tableFile.seekp(0, ios::beg);
			tableFile.write((char*)&i, sizeof(int));
		}
		else {
			cout << "Size: " << i << endl;
			char* k = new char[len_key];
			keysFile.open(keys_path, ios::in | ios::binary);
			keysFile.seekg(0, ios::end);
			int p = keysFile.tellg();
			p -= size_block - sizeof(int);
			keysFile.seekg(p, ios::beg);
			keysFile.read(k, len_key);
			last_key = k;
			delete[]k;
			keysFile.close();
		}
		keysFile.open(keys_path, ios::binary);
		keysFile.close();
		tableFile.close();
	}
	key& Add(T _thing, bool write = false) {
		if (ind >= listSize) {
			if (write) {
				WritePage();
				ind = 0;
				//return arr[0].first;
			}
			else {
				throw "Page is full, you shoul write it.";
			}
		}
		if (ind == 0) {
 			int min = last_key;
			for (int i = 0; i < listSize; i++) {
				int l = min + rand() % 911898752082 + 15;
				arr[i].first = l;
				min = l;
				last_key = min;
			}
		}
		empty_page = false;
		arr[ind].second = _thing;
		++ind;
		return arr[ind-1].first;
	}
	key& GetLastKey() {
		tableFile.open(table_path, ios::in | ios::binary);
		tableFile.read((char*)&listSize, sizeof(int));
		int pages = 0;
		tableFile.read((char*)&pages, sizeof(int));
		last_key = (listSize * pages);
		return last_key;
	}
	void NewPage() {
		ind = 0;
	}
	void WritePage() {
		dataFile.open(file_path, ios::out | ios::binary | ios::app);
		keysFile.open(keys_path, ios::out | ios::binary | ios::app);
		tableFile.open(table_path, ios::in | ios::out | ios::binary);
		if (!(dataFile.is_open() && keysFile.is_open() && tableFile.is_open()))
			throw "Can not open file";
		int num_page = 0;
		tableFile.seekg(sizeof(int), ios::beg);//skip sizePage
		tableFile.read((char*)&num_page, sizeof(int));
		//last_key = (num_page) * listSize;
		int t = 0;
		int pointer = 0;
		if (num_page > 0) {
			tableFile.seekg(0, ios::end);
			int q = tableFile.tellg();
			q -= 2 * sizeof(int);
			tableFile.seekg(q, ios::beg);
			tableFile.read((char*)&q, sizeof(int));
			q += listSize * size_block;
			keysFile.seekp(q, ios::beg);
		}
		else
			keysFile.seekp(0, ios::end);
		dataFile.seekp(0, ios::end);
		pointer = keysFile.tellp();//указатель на начало записи текущей страницы
		for (int i = 0; i < listSize && !empty_page; i++) {
			int s = dataFile.tellp();
			dataFile.write((char*)&arr[i].second, sizeof(T));
			int len = arr[i].first.length();
			//big_len += 2 * sizeof(int) + (len + 1) * sizeof(char);
			keysFile.write((char*)&len, sizeof(int));
			keysFile.write((char*)arr[i].first , (len_key) * sizeof(char));
			keysFile.write((char*)&s, sizeof(int));
			//last_key++;
			if (dataFile.fail() || keysFile.fail())
				throw "Writing error";
		}
		tableFile.seekp(0, ios::end);
		tableFile.write((char*)&pointer, sizeof(int));
		tableFile.write((char*)&ind, sizeof(int));
		tableFile.seekp(sizeof(int), ios::beg);//переходим на позицию с кол-вом страниц
		++num_page;
		tableFile.write((char*)&num_page, sizeof(int));//обновлённое кол-во страниц
		ind = 0;
		empty_page = true;
		dataFile.close();
		keysFile.close();
		tableFile.close();
	}

	void DeleteMove(key &x) {
		int page = x / listSize;
		int elem = x % listSize;
		keysFile.open(keys_path, ios::in | ios::out | ios::binary);
		tableFile.open(table_path, ios::in | ios::out | ios::binary);
		int pg = 0;
		int pointer = 0;
		int size_page = 0;
		int pointer_size = 0;
		tableFile.seekg(2 * sizeof(int), ios::beg);
		tableFile.seekg(2 * page * sizeof(int), ios::cur);
		tableFile.read((char*)&pointer, sizeof(int));
		//считываем размер последней страницы:
		tableFile.seekg(0, ios::end);
		pointer_size = tableFile.tellg();
		pointer_size -= sizeof(int);
		tableFile.seekg(pointer_size, ios::beg);
		tableFile.read((char*)&size_page, sizeof(int));//считали размер
		--size_page;
		tableFile.seekp(pointer_size, ios::beg);
		tableFile.write((char*)&size_page, sizeof(int));//записали уменьшую длину
		keysFile.seekg(pointer, ios::beg);
		int len = 0;
		for (int i = 0; i < elem; i++) {
			keysFile.read((char*)&len, sizeof(int));
			keysFile.seekg((len + 1) * sizeof(char) + sizeof(int), ios::cur);//len+4
		}
		int i = 0;
		while (!keysFile.eof()) {
			//указатель стоит на начале записи удаляемого ключа
			keysFile.read((char*)&len, sizeof(int));
			//пропускаем строку
			keysFile.seekg((len + 1) * sizeof(char), ios::cur);
			int p1 = 0;//указатель на значение в файле данных
			int pp1 = keysFile.tellg();//запоминаем, где лежит указатель, который нужно перезаписать
			keysFile.read((char*)&p1, sizeof(int));
			//указатель стоит на начале записи следующего ключа
			keysFile.read((char*)&len, sizeof(int));
			//пропускаем строку
			keysFile.seekg((len + 1) * sizeof(char), ios::cur);
			int p2 = 0;//указатель на значение в файле данных
			keysFile.read((char*)&p2, sizeof(int));
			keysFile.seekp(pp1, ios::beg);
			keysFile.seekg(pp1, ios::beg);
			keysFile.write((char*)&p2, sizeof(int));
			if (p1 == p2)
				break;
			i++;
		}
		keysFile.close();
		tableFile.close();
	}
	void Visual(int n) {
		ReadPage(n);
		for (int i = 0; i < ind; i++) {
			cout << "Key: " << arr[i].first;
			for (int i = 0; i < 16 - arr[i].first.length(); i++)
				cout << " ";
			cout << "Item: " << arr[i].second << endl;
		}
	}
	void VisualAll(int n) {
		ReadPage(n);
		for (int i = 0; i < ind; i++) {
			cout << "Key: " << arr[i].first;
			for (int i = 0; i < 16 - arr[i].first.length(); i++)
				cout << " ";
			cout << "Item: " << arr[i].second << endl;
		}
	}
	void VisualVeryAll() {
		int n = GetPageCount();
		for (int k = 0; k < n; k++) {
			ReadPage(k);
			for (int i = 0; i < ind; i++) {
				cout << "Key: " << arr[i].first;
				for (int i = 0; i < 16 - arr[i].first.length(); i++)
					cout << " ";
				cout << "Item: " << arr[i].second << endl;
			}
			cout << endl;
			}
	}
	key WritePage(int n) {//rewrite
		dataFile.open(file_path, ios::out | ios::in | ios::binary);
		keysFile.open(keys_path, ios::out | ios::in | ios::binary);
		tableFile.open(table_path, ios::out | ios::in | ios::binary);
		if (!(dataFile.is_open() && keysFile.is_open() && tableFile.is_open()))
			throw "Can not open file";
		int num_page = 0;
		tableFile.seekg(sizeof(int), ios::beg);
		tableFile.read((char*)&num_page, sizeof(int));
		if (n >= num_page)
			throw "Range missed. There is not a page with this number.";
		int t = 0;
		int big_len = 0;
		int pg = 0;
		//tableFile.seekg(sizeof(int), ios::beg);
		int pointer = 0;
		tableFile.seekg(2 * sizeof(int)*(n + 1), ios::beg);
		tableFile.read((char*)&pointer, sizeof(int));
		tableFile.seekp(tableFile.tellg(), ios::beg);
		keysFile.seekg(pointer, ios::beg);
		int len = 0;
		keysFile.read((char*)&len, sizeof(int));
		keysFile.seekg((len + 1) * sizeof(char), ios::cur);
		int dataP = 0;
		keysFile.read((char*)&dataP, sizeof(int));
		//dataFile.seekp(0, ios::end);
		for (int i = 0; i < listSize && !empty_page; i++) {
			/*int datap = dataFile.tellp();
			dataFile.write((char*)&arr[i].second, sizeof(T));
			int len = arr[i].first.length();
			big_len += 2 * sizeof(int) + (len + 1) * sizeof(char);
			keysFile.write((char*)&len, sizeof(int));
			keysFile.write((char*)arr[i].first, (len + 1) * sizeof(char));
			keysFile.write((char*)&datap, sizeof(int));*/
			dataFile.seekp(dataP);
			dataFile.write((char*)&arr[i].second, sizeof(T));
			keysFile.read((char*)&len, sizeof(int));
			keysFile.seekg(len + 1, ios::cur);
			keysFile.read((char*)&dataP, sizeof(int));
			if (dataFile.fail() || keysFile.fail())
				throw "Writing error";
		}
		ind = 0;
		empty_page = true;
		dataFile.close();
		keysFile.close();
		tableFile.close();
		return listSize * n;
	}
	void InsertMove(key k, T en) {
		dataFile.open(file_path, ios::in | ios::out | ios::binary);
		keysFile.open(keys_path, ios::in | ios::out | ios::binary);
		tableFile.open(table_path, ios::in | ios::out | ios::binary);
		int numEntry = k % listSize;
		int numPage = k / listSize;
		int countPages = 0;
		int sizePage = 0;//кол-во записей на каждой конкретной странице
		tableFile.seekg(sizeof(int), ios::beg);
		tableFile.read((char*)&countPages, sizeof(int));
		dataFile.seekp(0, ios::end);
		int entry = dataFile.tellp();
		dataFile.write((char*)&en, sizeof(T));
		tableFile.seekg((2 * (numPage + 1)) * sizeof(int), ios::beg);
		int pointer = 0;
		tableFile.read((char*)&pointer, sizeof(int));
		tableFile.read((char*)&sizePage, sizeof(int));
		keysFile.seekg(pointer, ios::beg);
		for (int i = 0; i < numEntry; i++) {
			keysFile.read((char*)&pointer, sizeof(int));
			keysFile.seekg((pointer + 1) * sizeof(char) + sizeof(int), ios::cur);//len+4
		}
		pointer = keysFile.tellg();
		keysFile.seekp(pointer);
		int len = 0;
		int t = 0;
		key last;
		while (1) {
			keysFile.read((char*)&len, sizeof(int));
			if (numEntry + 1 == sizePage) {
				char* k = new char[len + 1];
				int qwe = keysFile.tellg();
				keysFile.read(k, (len + 1)*sizeof(char));
				keysFile.seekg(qwe, ios::beg);
				keysFile.seekg(len + 1, ios::cur);
				last = k;
				delete[]k;
			}
			else
				keysFile.seekg(len + 1, ios::cur);
			pointer = keysFile.tellg();
			keysFile.read((char*)&t, sizeof(int));
			keysFile.seekp(pointer, ios::beg);
			keysFile.write((char*)&entry, sizeof(int));
			entry = t;
			++numEntry;//число пройденных записей на текущей странице
			if (numEntry >= sizePage){
				numEntry = 0;
				++numPage;
				if (sizePage < listSize) {
					pointer = tableFile.tellg();
					pointer -= sizeof(int);
					tableFile.seekp(pointer);
					++sizePage;
					tableFile.write((char*)&sizePage, sizeof(int));

					last++;
					int leng = last.length();
					keysFile.write((char*)&leng, sizeof(int));
					keysFile.write((char*)last, leng + 1);
					keysFile.write((char*)&entry, sizeof(int));
					break;
				}
				if (numPage < countPages) {
					tableFile.seekg(sizeof(int), ios::cur);
					tableFile.read((char*)&sizePage, sizeof(int));//читаем кол-во непустых записей на следующей странице
				}
				/*if (numPage < countPages) {
					tableFile.seekg(sizeof(int), ios::cur);
					tableFile.read((char*)&sizePage, sizeof(int));
					last++;
					int leng = last.length();
					keysFile.write((char*)&leng, sizeof(int));
					keysFile.write((char*)last, leng + 1);
					keysFile.write((char*)&entry, sizeof(int));
//					break;
				}*/
			}
			if (numPage >= countPages) {
				++countPages;
				tableFile.seekg(sizeof(int), ios::beg);
				int a = 0;
				tableFile.read((char*)&a, sizeof(int));
				tableFile.seekp(sizeof(int), ios::beg);
				tableFile.write((char*)&countPages, sizeof(int));
				tableFile.seekp(0, ios::end);
				pointer = keysFile.tellp();
				tableFile.write((char*)&pointer, sizeof(int));
				pointer = 1;//кол-во записей на странице
				tableFile.write((char*)&pointer, sizeof(int));

				last++;
				int leng = last.length()	;
				keysFile.write((char*)&leng, sizeof(int));
				keysFile.write((char*)last, leng + 1);
				keysFile.write((char*)&entry, sizeof(int));
				for (int i = 1; i < listSize; i++) {
					last++;
					int leng = last.length();
					keysFile.write((char*)&leng, sizeof(int));
					keysFile.write((char*)last, leng + 1);
					keysFile.write((char*)&entry, sizeof(int));
				}
				break;
			}
		}
		dataFile.close();
		tableFile.close();
		keysFile.close();
	}
	int ReadPage(int _n) {
		dataFile.open(file_path, ios::in | ios::binary);
		keysFile.open(keys_path, ios::in | ios::binary);
		tableFile.open(table_path, ios::in | ios::binary | ios::app);
		if (!(dataFile.is_open() && keysFile.is_open() && tableFile.is_open()))
			throw "Can not open file!";
		int i = 0;
		keysFile.seekg(0, ios::beg);
		int pg = 0;
		int pointer = 0;
		int size = 0;
		tableFile.seekg(sizeof(int), ios::beg);//первое число - sizePage
		//tableFile.seekp(sizeof(int), ios::beg);
		tableFile.read((char*)&size, sizeof(int));//прочитали кол-во страниц
		if (size <= _n)
			return 1;
		tableFile.seekg(_n * 2 * sizeof(int), ios::cur);//на каждую страницу по две записи типа int
		tableFile.read((char*)&pointer, sizeof(int));
		tableFile.read((char*)&ind, sizeof(int));
		keysFile.seekg(pointer, ios::beg);
		/*while (pg < _n) {
			ulong p_block = 0;
			tableFile.read((char*)&p_block, sizeof(int));
			pointer += p_block;
			tableFile.seekg(sizeof(int), ios::cur);
			++pg;
		}
		tableFile.seekg(sizeof(int), ios::cur);
		tableFile.read((char*)&size, sizeof(int));
		ind = size;
		keysFile.seekg(pointer, ios::beg);*/
		while (i < ind) {
			if (keysFile.eof())
				break;
			int len = 0;
			keysFile.read((char*)&len, sizeof(int));
			char* str = new char[len_key];
			keysFile.read(str, (len_key) * sizeof(char));
			arr[i].first = str;
			delete[]str;
			int k = 0;
			keysFile.read((char*)&k, sizeof(int));
			dataFile.seekg(k, ios_base::beg);
			dataFile.read((char*)&arr[i].second, sizeof(T));
			++i;
			//last_key++;
		}
		empty_page = false;
		dataFile.close();
		keysFile.close();
		tableFile.close();
		return 0;
	}
	T& operator[](int n) {
		if (n < 0 || n >= listSize)
			throw "Big input, range missed\n";
		return arr[n].second;
	}
	friend class key;
	T Search(key& x) {
		int pages = GetPageCount();
		dataFile.open(file_path, ios::in | ios::binary | ios::app);
		keysFile.open(keys_path, ios::in | ios::out | ios::binary);
		tableFile.open(table_path, ios::in | ios::out | ios::binary);
		if (!(tableFile.is_open() || keysFile.is_open() || dataFile.is_open()))
			throw "Can not open file";
		int p = pages * listSize / 2;        
		int left = 0;
		int right = pages * listSize - 1;
		bool z = false;
		char* k;
		k = new char[len_key];
		while (1) {
			p = left + (right - left) / 2;
			keysFile.seekg(p*size_block + sizeof(int), ios::beg);
			keysFile.read(k, len_key);
			key a = k;
			if (x == a) {
				int dataP = 0;
				keysFile.read((char*)&dataP, sizeof(int));
				dataFile.seekg(dataP, ios::beg);
				T elem;
				dataFile.read((char*)&elem, sizeof(T));
				tableFile.close();
				keysFile.close();
				dataFile.close();
				return elem;
			}
			if (right - left <= 0) {
				dataFile.close();
				tableFile.close();
				keysFile.close();
				throw "There is no this key\n";
			}
			if (x < a) {
				right = p;
				continue;
			}
			if (x > a) {
				left = p + 1;
				continue;
			}
		}
		dataFile.close();
		keysFile.close();
		tableFile.close();
		//return ret;
	}
	void test() {
		for (int i = 0; i < ind; i++)
			cout << arr[i].second << endl;
	}
	int GetPageCount() {
		tableFile.open(table_path, ios::in | ios::binary);
		tableFile.seekg(sizeof(int), ios::beg);
		int i = 0;
		tableFile.read((char*)&i, sizeof(int));
		tableFile.close();
		return i;
	}
	void Insert(key x, T _thing) {
		int pos = 0;
		int pages = GetPageCount();
		int fir = 0;
		dataFile.open(file_path, ios::in | ios::out | ios::binary);
		keysFile.open(keys_path, ios::in | ios::out | ios::binary);
		tableFile.open(table_path, ios::in | ios::out | ios::binary);
		if (!(tableFile.is_open() && dataFile.is_open() && keysFile.is_open()))
			throw "Could not open file!";
		int vis = pages / 2;
		int dvis = pages / 2 + 0.5;
		int pointer = 0;
		bool skip = false;
		char* k = new char[len_key];
		while (1) {
			tableFile.seekg((vis + 1) * 2 * sizeof(int), ios::beg);
			tableFile.read((char*)&pointer, sizeof(int));
			tableFile.read((char*)&ind, sizeof(int));

			dvis = dvis / 2.0 + 0.5;
			keysFile.seekg(pointer + sizeof(int), ios::beg);
			keysFile.read(k, len_key);
			key a = k;
			if (x < a) {
				if (vis == 0) {//записать самым первым
					fir = 0;
					pos = 0;
					skip = true;
			//		goto wr_lbl;
					break;
				}
				keysFile.seekg(pointer + sizeof(int) - size_block, ios::beg);
				keysFile.read(k, len_key);
				a = k;
				if (x > a) {
					fir = pos = pointer;
					break;
				}
				vis -= dvis;
				continue;
			}
			keysFile.seekg(pointer + size_block * (ind - 1) + sizeof(int), ios::beg);
			keysFile.read(k, len_key);
			a = k;
			if (x > a) {
				if (vis + 1 == pages) {//создать новую страницу в конце файла
					//fir = pointer;
					if (ind == listSize) {
						pos = pointer + size_block * listSize;
						int lenX = x.length();
						dataFile.seekp(0, ios::end);
						keysFile.seekp(pos, ios::beg);
						keysFile.write((char*)&lenX, sizeof(int));
						keysFile.write((char*)x, len_key);
						lenX = dataFile.tellp();
						dataFile.write((char*)&_thing, sizeof(T));
						keysFile.write((char*)&lenX, sizeof(int));
						++pages;
						tableFile.seekp(sizeof(int), ios::beg);
						tableFile.write((char*)&pages, sizeof(int));
						tableFile.seekp(pages * 2 * sizeof(int), ios::beg);
						tableFile.write((char*)&pos, sizeof(int));
						pos = 1;
						tableFile.write((char*)&pos, sizeof(int));
						dataFile.close();
						keysFile.close();
						tableFile.close();
						return;
					}
					else {
						fir = pointer;
						pos = pointer + size_block * ind;
						int lenX = x.length();
						keysFile.seekp(pos, ios::beg);
						dataFile.seekp(0, ios::end);
						fir = dataFile.tellp();
						keysFile.write((char*)&lenX, sizeof(int));
						keysFile.write((char*)x, len_key);
						keysFile.write((char*)&fir, sizeof(int));
						dataFile.write((char*)&_thing, sizeof(T));
						tableFile.seekp(((vis + 1) * 2 + 1) * sizeof(int), ios::beg);
						++ind;
						tableFile.write((char*)&ind, sizeof(int));
						keysFile.close();
						dataFile.close();
						tableFile.close();
						return;
					}
				}
				vis += dvis;
				continue;
			}
			fir = pointer;
			break;
		}
		int f = ind / 2.0;
		int df = ind / 2.0 + 0.5;
		key a = 0;
		key t = 0;
		if (skip)
			goto wr_lbl;
		while (1) {
			keysFile.seekg(fir + f * size_block + sizeof(int), ios::beg);
			keysFile.read(k, len_key);
			a = k;
			if (x == a) {
				int dataP = 0;
				keysFile.read((char*)&dataP, sizeof(int));
				dataFile.seekp(dataP, ios::beg);
				dataFile.write((char*)&_thing, sizeof(T));
				dataFile.close();
				keysFile.close();
				tableFile.close();
				return;
				pos = fir + f * size_block;
				break;
			}
			if (f < 0) {
				keysFile.seekg(fir + sizeof(int), ios::beg);
				keysFile.read(k, len_key);
				a = k;
				if (x == a) {
					int dataP = 0;
					keysFile.read((char*)&dataP, sizeof(int));
					dataFile.seekp(dataP, ios::beg);
					dataFile.write((char*)&_thing, sizeof(T));
					dataFile.close();
					keysFile.close();
					tableFile.close();
					return;
					pos = fir + f * size_block;
					break;
				}
				pos = fir;
				break;
			}
			if (x < a) {
				df = df / 2.0 + 0.5;
				f -= df;
				continue;
			}
			if (x > a) {
				keysFile.seekg(2 * sizeof(int), ios::cur);
				keysFile.read(k, len_key);
				t = k;
				if (x < t) {
					pos = fir + (f + 1) * size_block;
					break;
				}
				df = df / 2.0 + 0.5;
				f += df;
				continue;
			}
		}

		wr_lbl:
		int sdv = 0;
		while (ind == listSize) {
			tableFile.read((char*)&pointer, sizeof(int));
			tableFile.read((char*)&ind, sizeof(int));
			++vis;
			if (vis == pages) {
				++pages;
				tableFile.close();
				tableFile.open(table_path, ios::in | ios::out | ios::binary);
				tableFile.seekp(sizeof(int), ios::beg);
				int q = tableFile.tellp();
				tableFile.write((char*)&pages, sizeof(int));
				tableFile.seekp((vis + 1) * 2 * sizeof(int), ios::beg);
				int qwe = vis * listSize * size_block;
				tableFile.write((char*)&qwe, sizeof(int));
				qwe = 1;
				tableFile.write((char*)&qwe, sizeof(int));
				ind = 0;
				//--vis;
				break;
			}
		}
		//if (ind < listSize) {
			keysFile.seekg(vis * listSize * size_block + (ind - 1) * size_block, ios::beg);
			char *keyy = new char[len_key];
			int num_pos = ((ind + (vis)* listSize) * size_block - pos) / size_block;
			for (int i = 0; i < num_pos; i++) {
				int qwe = 0;
				int dataP = 0;
				keysFile.read((char*)&qwe, sizeof(int));
				keysFile.read(keyy, len_key);
				keysFile.read((char*)&dataP, sizeof(int));
				//keysFile.read(buf, size_block);
				int gpos = keysFile.tellg();
				keysFile.seekp(gpos, ios::beg);
				keysFile.write((char*)&qwe, sizeof(int));
				keysFile.write(keyy, len_key);
				keysFile.write((char*)&dataP, sizeof(int));
				//pos = gpos - 2 * size_block;
				keysFile.seekg(gpos - 2 * size_block);
			}
		//}
		
			keysFile.close();
			keysFile.open(keys_path, ios::in | ios::out | ios::binary);

		++ind;
		keysFile.seekp(pos, ios::beg);
		pointer = x.length();
		keysFile.write((char*)&pointer, sizeof(int));
		keysFile.write((char*)x, len_key);
		dataFile.seekp(0, ios::end);
		pointer = dataFile.tellp();
		dataFile.write((char*)&_thing, sizeof(T));
		keysFile.write((char*)&pointer, sizeof(int));

		tableFile.seekp(((vis + 1) * 2 + 1) * sizeof(int), ios::beg);
		tableFile.write((char*)&ind, sizeof(int));
		tableFile.seekp(sizeof(int), ios::end);
		tableFile.write((char*)&pages, sizeof(int));

		dataFile.close();
		keysFile.close();
		tableFile.close();
	}
	void Delete(key& x) {
		int pages = GetPageCount();
		dataFile.open(file_path, ios::out | ios::binary | ios::app);
		keysFile.open(keys_path, ios::in | ios::out | ios::binary);
		tableFile.open(table_path, ios::in | ios::out | ios::binary);
		if (!(tableFile.is_open() || keysFile.is_open() || dataFile.is_open()))
			throw "Can not open file";
		int p = pages * listSize / 2;
		int d = p / 2;
		int z = 0;
		char* k;
		k = new char[len_key];
		while (1) {
			keysFile.close();
			keysFile.open(keys_path, ios::in | ios::out | ios::binary);
			keysFile.seekg(p*size_block + sizeof(int), ios::beg);
			keysFile.read(k, len_key);
			key a = k;
			if (z > 3) {
				dataFile.close();
				tableFile.close();
				keysFile.close();
				throw "There is no this key";
			}
			if (x < a) {
				p -= d;
				d = d / 2.0 + 0.6;
				if (d == 1)
					++z;
				continue;
			}
			if (x > a) {
				p += d;
				d = d / 2.0 + 0.6;
				if (d == 1)
					++z;
				continue;
			}
			if (x == a) {
				//char* buf = new char[size_block];
				keysFile.seekg((p + 1) * size_block, ios::beg);
				tableFile.seekg((pages * 2 + 1)*sizeof(int), ios::beg);
				int q = tableFile.tellg();
				/*q -= sizeof(int);
				tableFile.seekg(q, ios::beg);*/
				int size = 0;//кол-во элементов на последней странице
				tableFile.read((char*)&size, sizeof(int));
				if (size > 1) {
					--size;
					tableFile.seekp(q, ios::beg);
					tableFile.write((char*)&size, sizeof(int));
				}
				else {
					--size;
					tableFile.seekp(q, ios::beg);
					tableFile.write((char*)&size, sizeof(int));
					tableFile.seekg(sizeof(int), ios::beg);
					tableFile.read((char*)&size, sizeof(int));//кол-во записанных страниц
					tableFile.seekp(sizeof(int), ios::beg);
					--size;
					tableFile.write((char*)&size, sizeof(int));
				}
				int wp = keysFile.tellg();
				int wp2 = wp - size_block;
				keysFile.seekp(wp2, ios::beg);
				int l = 0;
				int dataP = 0;
				while (1) {
					keysFile.seekg(wp, ios::beg);
					keysFile.read((char*)&l, sizeof(int));
					keysFile.read(k, len_key);
					keysFile.read((char*)&dataP, sizeof(int));
					wp += size_block;
					//keysFile.seekp(wp - size_block, ios::beg);
					keysFile.seekp(wp2, ios::beg);
					keysFile.write((char*)&l, sizeof(int));
					keysFile.write(k, len_key);
					keysFile.write((char*)&dataP, sizeof(int));
					wp2 += size_block;
					++p;
					if (p == pages * listSize)
						break;
				}
			}
			break;
		}
		dataFile.close();
		tableFile.close();
		keysFile.close();
	}
	~Dense() {
		delete[]file_path;
		delete[]keys_path;
		for (int i = 0; i < ind; i++) {
			delete[]arr[i].first;
		}
		delete[]arr;
		delete[]last_key;
	}
	void view() {
		for (int i = 0; i < ind; i++)
			cout << arr[i].second << endl;
	}
	int Remove() {
		if (keys_path && file_path)
			return std::remove(keys_path) + std::remove(file_path) + std::remove(table_path);
		return -1;
	}
};

template<class T>
ostream& operator<<(ostream& f, Dense<T>& x) {
	for (int i = 0; i < ind; i++)
		f << arr[i] << endl;
}

}
#endif // !_DENSE_H