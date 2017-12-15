#include <iostream>
#include <fstream>
#define _LONG_ALPH_
#include "Dense.h"

using namespace std;
using namespace bd;


void main() {
	bd::Dense<int> bd1(1000, 10);
	bd::Dense<int> bd2(1000, 10);
	key a1("abc");
	key a2("aac");
	key &b = a1 / a2;
	b.Resize(6);
	bd1.SetPath("D:\\sandcube\\data1.bin");
	for (int i = 0, j = 1; i < 1000; j++) {
		try{
			for (; i < 10 * j; i++)
				bd1.Add(i);
			bd1.WritePage();
		}
		catch (char x[]) {
			cout << x << endl;
		}
	}
	bd2.SetPath("D:\\sandcube\\data1.bin");
	bd2.ReadPage(25);
	bd2.test();
	getchar();
	getchar();
}