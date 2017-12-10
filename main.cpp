#include <iostream>
#include <fstream>

#include "Dense.h"

using namespace std;

void main() {
	bd::Dense<int> bd1(100, 10);
	bd::Dense<int> bd2(100, 10);
	for (int i = 0, j = 1; i < 100; j++) {
		try{
			for (; i < 10 * j; i++)
				bd1.Add(i);
			bd1.SetPath("D:\\sandcube\\data1.bin");
			bd1.WritePage();
		}
		catch (char x[]) {
			cout << x << endl;
		}
	}
	bd2.SetPath("D:\\sandcube\\data1.bin");
	bd2.ReadPage(2);
	bd2.test();
	getchar();
	getchar();
}