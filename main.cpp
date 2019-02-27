#pragma warning (disable : 4267)

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <windows.h>
#include <Lmcons.h>
#include <conio.h>

using namespace std;

void clear(char fill = ' ') {
	COORD tl = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO s;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	FillConsoleOutputCharacter(console, fill, cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);
}

string GetName() {
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserNameA(username, &username_len);
	return (string)username;
}

void WriteNewKey(string name, string first, string second, string third) {
	ofstream Keys;
	string path = "C:/Users/" + GetName() + "/AppData/Roaming/Decryptor/keys.txt";
	CreateDirectoryA(((string)getenv("appdata") + "\\Decryptor").c_str(), NULL);
	Keys.open(path, ios::app);
	Keys << name << endl << first << endl << second << endl << third << endl << endl;
	Keys.close();
}

int correctValue(int value, int length) { // correct value into frames of size
	if (value < 0) {
		return length + value;
	}
	else if (value >= length) {
		return value%length;
	}
	else {
		return value;
	}
}

int encodeValue(int rotate, int size, int col, int row) { // encode function
	int back, forward;
	if (rotate > col) {
		back = abs(rotate - col);
		forward = size - back;
	}
	else {
		forward = abs(rotate - col);
		back = size - forward;
	}
	if (back < forward) { // if go back will be faster go on back track
		return (row * 10 + back) * -1;
	}
	else {
		return row * 10 + forward;
	}
}

bool is_number(const std::string& s)
{
	return !s.empty() && find_if(s.begin(),
		s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

int main() {
	menu:
	clear();
	cout << "Welcome to Wheel Decryptor v1.0!" << endl;
	cout << "Please select an option: " << endl;
	cout << "1. Write new cryptor key." << endl;
	cout << "2. View cryptor keys." << endl;
	cout << "3. Delete cryptor key." << endl;
	cout << "4. Decrypt message with key." << endl;
	cout << "5. Encrypt message with key." << endl;
	cout << "6. Help and changelog." << endl;
	cout << "7. Exit from Decryptor." << endl;
	int a, b, c;
	cout << "Select an option: ";
	cin >> a;
	clear();
	switch (a) {
		case 1: {
		key_start:
			clear();
			string name, first, second, third;
			cout << "Enter a name for key: ";
			cin >> name;
			cout << "WARNING: Three rows should contain all small letters a-z, \nnumbers 0-9 and they shouldn't repeat!\n All rows should be from 5 to 15 symbols!" << endl;
			cout << "Enter first row: ";
			cin >> first;
			cout << "Enter second row: ";
			cin >> second;
			cout << "Enter third row: ";
			cin >> third;
			cout << endl;
			string connected, symbols;
			symbols = "1234567890qwertyuiopasdfghjklzxcvbnm";
			connected = first + second + third;
			if (connected.length() != 36) {
				cout << "You don't have all the symbols or one or more symbols encounters twice or more!" << endl;
				cout << "Please select an option: " << endl;
				cout << "1. Try again." << endl;
				cout << "2. Exit to menu." << endl;
				length_mistake:
				cout << "Select an option: ";
				cin >> c;
				switch (c) {
					case 1: {
						goto key_start;
						break;
					}
					case 2: {
						goto menu;
						break;
					}
					default: {
						cout << "Please enter valid number." << endl;
						goto length_mistake;
						break;
					}
				}
			}
			bool curFound = false;
			for (int c = 0; c < 36; c++) {
				curFound = false;
				for (int d = 0; d < 36; d++) {
					if (symbols[c] == connected[d]) {
						curFound = true;
						break;
					}
				}
				if (!curFound) {
					cout << "Symbol " << symbols[c] << " not found!" << endl;
					cout << "Please select an option: " << endl;
					cout << "1. Try again." << endl;
					cout << "2. Exit to menu." << endl;
					symbol_mistake:
					cout << "Select an option: ";
					cin >> c;
					switch (c) {
						case 1: {
							goto key_start;
							break;
						}
						case 2: {
							goto menu;
							break;
						}
						default: {
							cout << "Please enter valid number." << endl;
							goto symbol_mistake;
							break;
						}
					}
				}
			}
			if (first.size() > 15 || first.size() < 5 ||
				second.size() > 15 || second.size() < 5 ||
				third.size() > 15 || third.size() < 5) {
				cout << "One of rows is lower than 5 or higher than 15 symbols!" << endl;
				cout << "Please select an option: " << endl;
				cout << "1. Try again." << endl;
				cout << "2. Exit to menu." << endl;
				row_mistake:
				cout << "Select an option: ";
				cin >> c;
				switch (c) {
					case 1: {
						goto key_start;
						break;
					}
					case 2: {
						goto menu;
						break;
					}
					default: {
						cout << "Please enter valid number." << endl;
						goto row_mistake;
						break;
					}
				}
			}
			string path = "C:/Users/" + GetName() + "/AppData/Roaming/Decryptor/";
			fstream keys;
			keys.open(path + "keys.txt");
			string line;
			while (getline(keys, line)) {
				if (line == name) {
					cout << "Key with that name already exists!" << endl;
					cout << "Please select an option: " << endl;
					cout << "1. Try again." << endl;
					cout << "2. Exit to menu." << endl;
					name_mistake:
					cout << "Select an option: ";
					cin >> c;
					switch (c) {
						case 1: {
							goto key_start;
							break;
						}
						case 2: {
							goto menu;
							break;
						}
						default: {
							cout << "Please enter valid number." << endl;
							goto name_mistake;
							break;
						}
					}
				}
			}
			WriteNewKey(name, first, second, third);
			cout << "Succesfully created!" << endl;
			write:
			cout << endl << "Please select an option: " << endl;
			cout << "1. Exit to menu." << endl;
			cout << "2. Exit from Decryptor." << endl;
			cout << "Select an option: ";
			cin >> b;
			clear();
			switch (b) {
				case 1: {
					goto menu;
					break;
				}
				case 2: {
					return 0;
					break;
				}
				default: {
					cout << "Please enter valid number." << endl;
					goto write;
					break;
				}
			}
			break;
		}
		case 2: {
			fstream keys;
			string line;
			string path = "C:/Users/" + GetName() + "/AppData/Roaming/Decryptor/";
			keys.open(path + "keys.txt");
			int count = 0;
			while (getline(keys, line)) {
				count++;
			}
			count = (count) / 5;
			if (count == 0) {
				cout << "You don't have any keys yet!" << endl;
				cout << "Please select an option: " << endl;
				cout << "1. Exit to menu." << endl;
				cout << "2. Exit from Decryptor" << endl;
				keys_view:
				cout << "Select an option: ";
				cin >> c;
				switch (c) {
					case 1: {
						goto menu;
						break;
					}
					case 2: {
						return 0;
						break;
					}
					default: {
						cout << "Please enter valid number." << endl;
						goto keys_view;
						break;
					}
				}
			}
			view_start:
			keys.close();
			keys.open(path + "keys.txt");
			string *names = new string[count];
			cout << "Select a key: " << endl;
			for (int co = 0; co < count; co++) {
				string line;
				getline(keys, line);
				names[co] = line;
				cout << co + 1 << ". " << line << endl;
				if (co + 1 != count) {
					getline(keys, line);
					getline(keys, line);
					getline(keys, line);
					getline(keys, line);
				}
			}
			int cho;
			cout << "Enter a value: ";
			cin >> cho;
			if (cho > count || cho < 1) {
				cout << "Please enter a valid number." << endl;
				goto view_start;
			}
			keys.close();
			keys.open(path + "keys.txt");
			while (getline(keys, line)) {
				if (line == names[cho - 1]) {
					cout << endl << "Name: " << names[cho - 1] << endl;
					getline(keys, line);
					cout << "First row: " << line << endl;
					getline(keys, line);
					cout << "Second row: " << line << endl;
					getline(keys, line);
					cout << "Third row: " << line << endl;
					break;
				}
			}
			cout << endl << "Please select an option: " << endl;
			cout << "1. Exit to menu." << endl;
			cout << "2. Exit from Decryptor" << endl;
			keys_view_end:
			cout << "Select an option: ";
			cin >> c;
			switch (c) {
				case 1: {
					goto menu;
					break;
				}
				case 2: {
					return 0;
					break;
				}
				default: {
					cout << "Please enter valid number." << endl;
					goto keys_view_end;
					break;
				}
			}
			break;
		}
		case 3: {
			fstream keys;
			string line;
			string path = "C:/Users/" + GetName() + "/AppData/Roaming/Decryptor/";
			keys.open(path + "keys.txt");
			int count = 0;
			while (getline(keys, line)) {
				count++;
			}
			count = (count) / 5;
			if (count == 0) {
				cout << "You don't have any keys yet!" << endl;
				cout << "Please select an option: " << endl;
				cout << "1. Exit to menu." << endl;
				cout << "2. Exit from Decryptor" << endl;
				keys_select:
				cout << "Select an option: ";
				cin >> c;
				switch (c) {
					case 1: {
						goto menu;
						break;
					}
					case 2: {
						return 0;
						break;
					}
					default: {
						cout << "Please enter valid number." << endl;
						goto keys_select;
						break;
					}
				}
			}
		choose_start:
			keys.close();
			keys.open(path + "keys.txt");
			string *names = new string[count];
			cout << "Select a key: " << endl;
			for (int co = 0; co < count; co++) {
				string line;
				getline(keys, line);
				names[co] = line;
				cout << co + 1 << ". " << line << endl;
				if (co + 1 != count) {
					getline(keys, line);
					getline(keys, line);
					getline(keys, line);
					getline(keys, line);
				}
			}
			int cho;
			cout << "Enter a value: ";
			cin >> cho;
			if (cho > count || cho < 1) {
				cout << "Please enter a valid number." << endl;
				goto choose_start;
			}
			keys.close();
			keys.open(path + "keys.txt");
			ofstream newKeys;
			newKeys.open(path + "newKeys.txt");
			string buffer;
			for (int co = 0; co < count; co++) {
				getline(keys, buffer);
				if (buffer != names[cho - 1]) {
					newKeys << buffer << endl;
					getline(keys, buffer);
					newKeys << buffer << endl;
					getline(keys, buffer);
					newKeys << buffer << endl;
					getline(keys, buffer);
					newKeys << buffer << endl;
					getline(keys, buffer);
					newKeys << buffer << endl;
				}
				else {
					getline(keys, buffer);
					getline(keys, buffer);
					getline(keys, buffer);
					getline(keys, buffer);
				}
			}
			newKeys.close();
			keys.close();
			remove((path + "keys.txt").c_str());
			rename((path + "newKeys.txt").c_str(), (path + "keys.txt").c_str());
			cout << endl << "Succesfully deleted!" << endl;
			cout << endl << "Please select an option: " << endl;
			cout << "1. Exit to menu." << endl;
			cout << "2. Exit from Decryptor" << endl;
			keys_delete:
			cout << "Select an option: ";
			cin >> c;
			switch (c) {
				case 1: {
					goto menu;
					break;
				}
				case 2: {
					return 0;
					break;
				}
				default: {
					cout << "Please enter valid number." << endl;
					goto keys_delete;
					break;
				}
			}
			break;
		}
		case 4: {
			fstream keys;
			string line;
			string path = "C:/Users/" + GetName() + "/AppData/Roaming/Decryptor/";
			keys.open(path + "keys.txt");
			int count = 0;
			while (getline(keys, line)) {
				count++;
			}
			count = (count) / 5;
			if (count == 0) {
				clear();
				cout << "You don't have any keys yet!" << endl;
				cout << "Please select an option: " << endl;
				cout << "1. Exit to menu." << endl;
				cout << "2. Exit from Decryptor" << endl;
				keys_decode:
				cout << "Select an option: ";
				cin >> c;
				switch (c) {
					case 1: {
						goto menu;
						break;
					}
					case 2: {
						return 0;
						break;
					}
					default: {
						cout << "Please enter valid number." << endl;
						goto keys_decode;
						break;
					}
				}
			}
			decode_start:
			keys.close();
			keys.open(path + "keys.txt");
			string *names = new string[count];
			cout << "Select a key: " << endl;
			for (int co = 0; co < count; co++) {
				string line;
				getline(keys, line);
				names[co] = line;
				cout << co + 1 << ". " << line << endl;
				if (co + 1 != count) {
					getline(keys, line);
					getline(keys, line);
					getline(keys, line);
					getline(keys, line);
				}
			}
			int cho;
			cout << "Enter a value: ";
			cin >> cho;
			if (cho > count || cho < 1) {
				cout << "Please enter a valid number." << endl;
				goto decode_start;
			}
			keys.close();
			keys.open(path + "keys.txt");
			string first, second, third;
			while (getline(keys, line)) {
				if (line == names[cho - 1]) {
					getline(keys, first);
					getline(keys, second);
					getline(keys, third);
					break;
				}
			}
			cout << "Create file \"input.txt\" in folder where's your Decryptor is \n and enter phrase you want to decode in this file. Every letter should be on new line." << endl;
			cout << "Press any key to continue..." << endl << endl;
			_getch();
			fstream input;
			input.open("input.txt");
			count = 0;
			while (getline(input, line)) {
				count++;
				if (!is_number(line)) {
					cout << "You have letters in your \"input.txt\"!" << endl;
					cout << "Please select an option: " << endl;
					cout << "1. Exit to menu." << endl;
					cout << "2. Exit from Decryptor" << endl;
					letter_decode:
					cout << "Select an option: ";
					cin >> c;
					switch (c) {
						case 1: {
							goto menu;
						}
						case 2: {
							return 0;
						}
						default: {
							cout << "Please enter valid number." << endl;
							goto letter_decode;
						}
					}
				}
			}
			if (count == 0) {
				cout << "You didn't create file!" << endl;
				keys_decode_mistake:
				cout << "Please select an option: " << endl;
				cout << "1. Exit to menu." << endl;
				cout << "2. Exit from Decryptor" << endl;
				cout << "Select an option: ";
				cin >> c;
				switch (c) {
					case 1: {
						goto menu;
						break;
					}
					case 2: {
						return 0;
						break;
					}
					default: {
						cout << "Please enter valid number." << endl;
						goto keys_decode_mistake;
						break;
					}
				}
			}
			input.close();
			input.open("input.txt");
			while (getline(input, line)) {
				if (stoi(line) > 39 || stoi(line) < -39) {
					cout << "Check your \"digits\" in your \"input.txt\" file. Some of them are incorrect." << endl;
					cout << "Please select an option: " << endl;
					cout << "1. Exit to menu." << endl;
					cout << "2. Exit from Decryptor" << endl;
					number_decode:
					cout << "Select an option: ";
					cin >> c;
					switch (c) {
						case 1: {
							goto menu;
						}
						case 2: {
							return 0;
						}
						default: {
							clear();
							cout << "Please enter valid number." << endl;
							goto number_decode;
						}
					}
				}
			}
			input.close();
			input.open("input.txt");
			ofstream output;
			output.open("output.txt");
			cout << "Your decoded message: ";
			int firstRot = 0, secondRot = 0, thirdRot = 0;
			for (int c = 0; c < count; c++) {
				int currChar;
				string currCharStr;
				getline(input, currCharStr);
				if (currCharStr == "") {
					cout << " ";
					output << " ";
					continue;
				}
				currChar = stoi(currCharStr);
				int row, col;
				if (currChar < 0) {
					row = (-1 * currChar) / 10 % 10;
					col = (-1 * currChar) % 10;
					col *= -1;
				}
				else {
					row = currChar / 10 % 10;
					col = currChar % 10;
				}
				char reqChar;
				switch (row) {
					case 1: {
						firstRot += col;
						firstRot = correctValue(firstRot, first.size());
						reqChar = first[firstRot];
						break;
					}
					case 2: {
						secondRot += col;
						secondRot = correctValue(secondRot, second.size());
						reqChar = second[secondRot];
						break;
					}
					case 3: {
						thirdRot += col;
						thirdRot = correctValue(thirdRot, third.size());
						reqChar = third[thirdRot];
						break;
					}
				}
				cout << reqChar;
				output << reqChar;
			}
			input.close();
			output.close();
			cout << endl << "This message has been put in \"output.txt\" in folder where Decryptor is." << endl;
			keys_decode_end:
			cout << "Please select an option: " << endl;
			cout << "1. Exit to menu." << endl;
			cout << "2. Exit from Decryptor" << endl;
			cout << "Select an option: ";
			cin >> c;
			switch (c) {
				case 1: {
					goto menu;
					break;
				}
				case 2: {
					return 0;
					break;
				}
				default: {
					cout << "Please enter valid number." << endl;
					goto keys_decode_end;
					break;
				}
			}
			break;
		}
		case 5: {
			fstream keys;
			string line;
			string path = "C:/Users/" + GetName() + "/AppData/Roaming/Decryptor/";
			keys.open(path + "keys.txt");
			int count = 0;
			while (getline(keys, line)) {
				count++;
			}
			count = (count) / 5;
			if (count == 0) {
				clear();
				cout << "You don't have any keys yet!" << endl;
				cout << "Please select an option: " << endl;
				cout << "1. Exit to menu." << endl;
				cout << "2. Exit from Decryptor" << endl;
				keys_encode:
				cout << "Select an option: ";
				cin >> c;
				switch (c) {
					case 1: {
						goto menu;
						break;
					}
					case 2: {
						return 0;
						break;
					}
					default: {
						cout << "Please enter valid number." << endl;
						goto keys_encode;
						break;
					}
				}
			}
			encode_start:
			keys.close();
			keys.open(path + "keys.txt");
			string *names = new string[count];
			cout << "Select a key: " << endl;
			for (int co = 0; co < count; co++) {
				string line;
				getline(keys, line);
				names[co] = line;
				cout << co + 1 << ". " << line << endl;
				if (co + 1 != count) {
					getline(keys, line);
					getline(keys, line);
					getline(keys, line);
					getline(keys, line);
				}
			}
			int cho;
			cout << "Enter a value: ";
			cin >> cho;
			if (cho > count || cho < 1) {
				cout << "Please enter a valid number." << endl;
				goto encode_start;
			}
			keys.close();
			keys.open(path + "keys.txt");
			string first, second, third;
			while (getline(keys, line)) {
				if (line == names[cho - 1]) {
					getline(keys, first);
					getline(keys, second);
					getline(keys, third);
					break;
				}
			}
			cout << "Create file \"input.txt\" in folder where's your Decryptor is \n and enter phrase you want to decode in this file on one line." << endl;
			cout << "Press any key to continue..." << endl << endl;
			_getch();
			fstream input;
			input.open("input.txt");
			count = 0;
			while (getline(input, line)) {
				count++;
			}
			if (count == 0) {
				cout << "You didn't create file!" << endl;
				goto keys_encode_mistake;
			}
			else if (count > 1) {
				cout << "You didn't put phrase on one line!" << endl;
				keys_encode_mistake:
				cout << "Please select an option: " << endl;
				cout << "1. Exit to menu." << endl;
				cout << "2. Exit from Decryptor" << endl;
				cout << "Select an option: ";
				cin >> c;
				switch (c) {
					case 1: {
						goto menu;
						break;
					}
					case 2: {
						return 0;
						break;
					}
					default: {
						clear();
						cout << "Please enter valid number." << endl;
						goto keys_encode_mistake;
						break;
					}
				}
			}
			input.close();
			input.open("input.txt");
			ofstream output;
			output.open("output.txt");
			string encodeit;
			getline(input, encodeit);
			int firstRot = 0, secondRot = 0, thirdRot = 0;
			for (int c = 0; c < encodeit.size(); c++) {
				char currChar = encodeit[c];
				int row, col;
				for (int fc = 0; fc < first.size(); fc++) {
					if (currChar == first[fc]) {
						row = 1;
						col = fc;
					}
				}
				for (int sc = 0; sc < second.size(); sc++) {
					if (currChar == second[sc]) {
						row = 2;
						col = sc;
					}
				}
				for (int tc = 0; tc < third.size(); tc++) {
					if (currChar == third[tc]) {
						row = 3;
						col = tc;
					}
				}
				if (currChar == ' ') {
					row = 0;
					col = 0;
					cout << endl;
					output << endl;
					continue;
				}
				switch (row) {
					case 1: {
						cout << encodeValue(firstRot, first.size(), col, row) << endl;
						output << encodeValue(firstRot, first.size(), col, row) << endl;
						firstRot = col;
						break;
					}
					case 2: {
						cout << encodeValue(secondRot, second.size(), col, row) << endl;
						output << encodeValue(secondRot, second.size(), col, row) << endl;
						secondRot = col;
						break;
					}
					case 3: {
						cout << encodeValue(thirdRot, third.size(), col, row) << endl;
						output << encodeValue(thirdRot, third.size(), col, row) << endl;
						thirdRot = col;
						break;
					}
				}
			}
			input.close();
			output.close();
			cout << endl << "This message has been put in \"output.txt\" in folder where Decryptor is." << endl;
			keys_encode_end:
			cout << "Please select an option: " << endl;
			cout << "1. Exit to menu." << endl;
			cout << "2. Exit from Decryptor" << endl;
			cout << "Select an option: ";
			cin >> c;
			switch (c) {
				case 1: {
					goto menu;
					break;
				}
				case 2: {
					return 0;
					break;
				}
				default: {
					cout << "Please enter valid number." << endl;
					goto keys_encode_end;
					break;
				}
			}
			break;
			}
		case 6: {
			cout << "This program has been made by Spynder." << endl;
			cout << "Contact: spynderyt@gmail.com/MadKey" << endl;
			cout << "Version 1.0: Write, view, delete, decrypt, encrypt, exit functions." << endl;
			cout << "Version 1.1: Added \"Help and changelog\". Fixed bugs with reading files." << endl << "Changed \"Encryptor\" to \"Decryptor\"." << endl;
			cout << endl << "Please select an option: " << endl;
			cout << "1. Exit to menu" << endl;
			cout << "2. Exit from Decryptor." << endl;
			help_end:
			cout << "Select an option: ";
			cin >> c;
			switch (c) {
				case 1: {
					goto menu;
					break;
				}
				case 2: {
					return 0;
					break;
				}
				default: {
					cout << "Please enter valid number." << endl;
					goto help_end;
					break;
				}
			}
		}
		case 7: {
				return 0;
				break;
			}
		default: {
			cout << "Please enter valid number." << endl;
			goto menu;
			break;
		}
	}
	return 0;
}
