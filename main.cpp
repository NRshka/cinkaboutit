#include <iostream>
#include <fstream>
#include <ctime>
#define _EXTENDED_ALPHABET_KEY_BD_
#define _USERS_ALPHABET_BD_KEY_
#include "Dense.h"

#define cin_fail if(cin.fail()){cin.clear();cin.ignore();}
#define cin_clear cin.clear(); cin.ignore();

using namespace std;
using namespace bd;


enum { CLS, SET, WRITE, READ, ADD, SEARCH, DELETE, DELETE_BIG, ADD_BIG, GET, VIEW, NEW_PAGE, WRITE_PAGE, INSERT, SEARCH_ELEM, VISUAL, VISUAL_VERY_ALL, REMOVE_FILE, NEW_FILE, DELETE_MOVE, SEARCH_BIG
};

char menu[] = "0. Clear console and show menu\n1. Set path to data file\n2. Write page\n3. Read page\n4. Add to the page\n5. Search entry with key\n6. Delete with key\n 7.Delete and move\n8. Test to add\n9. Put the entry in page with index at the console\n10. Put to console a page\n11. New page\n12. Write page with a number\n13. Insert with a key\n14. Find a element\n15. Visualize page\n16. Visual all\n\n\n";


void main() {
	srand(time(NULL));
	int choose;
	cout << "Please, input the size of page:\n";
	//cin >> choose;
	choose = 10;
	Dense<int> bd1(choose, 200);
	system("cls");
	/*char path[100];
	if (cin.fail()) {
		cin.clear();
		cin.ignore();
	}
	cin.ignore();
	//cin.ignore();
	cout << "Input the path to the data file:\n";
	cin.getline(path, 99);*/
	char path[] = "D:\\sandcube\\data.bin";
	try {
		bd1.SetPath(path);
	}
	catch (...) {
		cout << "Can not open file\n";
		bd1.Remove();
		try {
			bd1.SetPath(path);
		}
		catch (...) {
			cout << "Critical error: Can not open file\n Program will be closed.\n";
			getchar();
			exit(-1);
		}
		cout << "New file was created\n";
		getchar();
		system("cls");
	}
	system("cls");
	cout << menu;
	while (1) {
		cin >> choose;
		switch (choose)
		{
		case VISUAL: {
			int n = 0;
			cout << "Input a number of page:\n";
			cin >> n;
			bd1.Visual(n);
			break;
		}
		case VISUAL_VERY_ALL: {
			bd1.VisualVeryAll();
			break;
		}
		case CLS: {
			system("cls");
			cout << menu;
			break;
		}
		case SET: {
			char path[100];
			cout << "Input the path to the data file:\n";
			cin.ignore();
			cin.getline(path, 99);
			bd1.SetPath(path);
		}
		case WRITE: {
			try {
				bd1.WritePage();
			}
			catch (char x[]) {
				cout << x;
			}
			break;
		}
		case READ: {
			try {
				int n;
				cout << "Please, input a number of a page:\n";
				cin >> n;
				int s = bd1.ReadPage(n);
				if (s == 1)
					cout << "It haven't path with this number\n";
				else
					cout << "Got it\n";
			}
			catch (char x[]) {
				cout << x;
			}
			break;
		}
		case ADD: {
			try {
				int n;
				cout << "Please, input a item:\n";
				cin >> n;
				key s = bd1.Add(n);
				cout << "Added with the key: " << s << endl;
			}
			catch (char x[]) {
				cout << x;
			}
			break;
		}
		case SEARCH: {
			try {
				cout << "Input a key: \n";
				char a[80];
				std::cin >> a;
				cout << "That is: " << bd1.Search(key(a)) << endl;
			}
			catch (char x[]) {
				cout << x;
			}
			break;
		}
		case DELETE: {
			try {
				cout << "Input a key: \n";
				char a[80];
				std::cin >> a;
				bd1.Delete(key(a));
				cout << "Got it\n";
			}
			catch (char x[]) {
				cout << x;
			}
			break;
		}
		case DELETE_MOVE: {
			try {
				cout << "Input a key: \n";
				int a = 0;
				std::cin >> a;
				bd1.DeleteMove(key(a));
			}
			catch (char x[]) {
				cout << x;
			}
			break;
		}
		/*case SEARCH_BIG: {
			int a, b;
			cout << "Input the min and max of range: \n";
			cin >> a >> b;
			try {
				unsigned int start = clock();
				for (int i = a; i < b; i++) {
					key u = i;
					bd1.Search(u);
				}
				unsigned int end = clock();
				cout << "Time is " << end - start << endl;
				cout << "Got it!\n";
			}
			catch (char x[]) {
				cout << x;
			}
			break;
		}
		case DELETE_BIG: {
			int a, b;
			cout << "Input the min and max of range: \n";
			cin >> a >> b;
			try {
				key u = a;
				unsigned int start = clock();
				for (int i = a; i < b; i++) {
					bd1.DeleteMove(u);
				}
				bd1.ReadPage(a);
				unsigned int end = clock();
				cout << "Got it\n";
				cout << "Time is " << end - start << endl;
			}
			catch (char x[]) {
				cout << x;
			}
			break;
		}*/
		case ADD_BIG: {
			int a, b;
			cout << "Input the min and max of range: \n";
			cin >> a >> b;
			try {
				unsigned int start = clock();
				int k = 8954;
				for (int i = a; i < b; i++) {
					/*bd1.Insert(k, i);
					k += rand() % 1587;*/
					bd1.Add(i, true);
				}
				bd1.WritePage();
				unsigned int end = clock();
				cout << "Time is " << end - start << endl;
			}
			catch (char x[]) {
				cout << x;
			}
			break;
		}
		case GET: {
			int n;
			cout << "Input number of entry at page:\n";
			try {
				cin >> n;
				cout << bd1[n] << endl;
			}
			catch (char x[]) {
				cout << x;
			}
			break;
		}
		case VIEW: {
			cout << "Count of pages: ";
			cout << bd1.GetPageCount() << endl;
			bd1.view();
			cout << "Got it\n";
			break;
		}
		case NEW_PAGE: {
			bd1.NewPage();
			cout << "Got it";
			break;
		}
		case WRITE_PAGE: {
			int c = 0;
			cout << "Input the number of page:\n";
			cin >> c;
			cout << "Key first now: " << bd1.WritePage(c) << endl;
			break;
		}
		case INSERT: {
			try {
				char c[80];
				cout << "Input the key:\n";
				cin >> c;
				cout << "Input a item:\n";
				int item = 0;
				cin >> item;
				bd1.Insert(c, item);
				cout << "Got it\n";
			}
			catch (char x[]) {
				cout << x << endl;
			}
			break;
		}
		case SEARCH_ELEM: {
			try {
				int a = 0;
				cout << "Input the element:\n";
				cin >> a;
				cout << bd1.SearchEntry(a)<<endl;
			}
			catch (const char x[]) {
				cout << x << endl;
			}
		}
		case REMOVE_FILE: {
			try {
				bd1.Remove();
			}
			catch (const char x[]) {
				cout << x << endl;
			}
			break;
		}
		default: {
			//bd1.Remove();
			exit(0);
			break;
		}
		}
	}

	getchar();
}