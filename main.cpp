#include <iostream>
#include <fstream>
#define _EXTENDED_ALPHABET_KEY_BD_
#include "Dense.h"

using namespace std;
using namespace bd;


void main() {
	key a = 3843;
	key b = 1;
	key c = a + b;
	bd::Dense<int> bd1(100, 10);
	bd::Dense<int> bd2(10, 10);
	bd1.SetPath("D:\\sandcube\\data1.bin");
	int size_key = bd1.GetKeySize();
	for (int i = 0, j = 1; i < 100; j++) {
		try{
			for (; i < 10 * j; i++)
				bd1.Add(i);
			bd1.WritePage();
		}
		catch (char x[]) {
			cout << x << endl;
		}
	}
	key i(size_key);
	key len(0);
	len = 20;
	len.Resize(size_key);
	for (; i < len; i++) {
		cout << bd1.Search(i);
	}
	bd2.SetPath("D:\\sandcube\\data1.bin");
	bd2.ReadPage(25);
	bd2.test();
	getchar();
	getchar();
}