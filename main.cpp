#include <iostream>
#include <fstream>

#include "Dense.h"

using namespace std;

void main() {
	bd::Dense<int> bd1(10, 10);
	for (int i = 0; i < 10; i++)
		bd1.Add(i);
	bd1.SetPath("D:\\sandcube\\data1.bin");
	bd1.WritePage();
	bd1.test();
	getchar();
	getchar();
}