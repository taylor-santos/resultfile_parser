#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void main(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "No filename specified!" << endl;
		return;
	}
	ifstream resultFile(argv[1]);
	if (!resultFile.is_open()) {
		cout << "Could not open file!" << endl;
		return;
	}
	ofstream outFile("resultfile_parsed.txt");
	if (!outFile.is_open()) {
		cout << "Could not create output file!" << endl;
		return;
	}
	char c;
	int indentLevel = 0;
	bool escape = false;
	bool newLine = false;
	while (resultFile.get(c)) {
		if (newLine) {
			newLine = false;
			for (int i = 0; i < indentLevel; ++i) {
				outFile << "    ";
			}
		}
		if (escape) {
			escape = false;
			if (c == 'n') {
				outFile << endl;
				newLine = true;
				continue;
			}
		}
		if (c == '\\') {
			escape = true;
			continue;
		}
		if (c == '}') {
			outFile << endl;
			indentLevel--;
			for (int i = 0; i < indentLevel; ++i) {
				outFile << "    ";
			}
			outFile << "}" << endl;
			newLine = true;
			continue;
		}
		if (c == '{') {
			outFile << endl;
			for (int i = 0; i < indentLevel; ++i) {
				outFile << "    ";
			}
			outFile << "{" << endl;
			newLine = true;
			indentLevel++;
			continue;
		}
		/*
		if (c == ']') {
			outFile << endl;
			indentLevel--;
			for (int i = 0; i < indentLevel; ++i) {
				outFile << "    ";
			}
			outFile << "]" << endl;
			newLine = true;
			continue;
		}
		if (c == '[') {
			outFile << endl;
			for (int i = 0; i < indentLevel; ++i) {
				outFile << "    ";
			}
			outFile << "[" << endl;
			newLine = true;
			indentLevel++;
			continue;
		}
		*/
		if (c == 'u') {
			string text = "pdate game field ";
			bool isField = true;
			for (int i = 0; i < text.length(); ++i) {
				char newC;
				resultFile.get(newC);
				if (newC != text[i]) {
					resultFile.seekg(-(i + 1), ios::cur);
					isField = false;
					break;
				}
			}
			if (isField) {
				string fieldString = "update game field ";
				outFile << endl;
				for (int i = 0; i < indentLevel + 1; ++i)
					outFile << "    ";
				outFile << "\xE2\x94\x8C";
				for (int x = 0; x < 20; ++x)
					outFile << "\xE2\x94\x80" << "\xE2\x94\x80";
				outFile << "\xE2\x94\x90" << endl;
				for (int y = 0; y < 14; ++y) {
					for (int i = 0; i < indentLevel + 1; ++i) {
						outFile << "    ";
					}
					outFile << "\xE2\x94\x82";
					for (int x = 0; x < 20; ++x) {
						char newC;
						resultFile.get(newC);
						fieldString += newC;
						string curr;
						while (newC != ',' && newC != '\\') {
							curr += newC;
							/*
							if (newC == 'x')
								outFile << "\xE2\x96\x88" << "\xE2\x96\x88";
							else if (newC == '.')
								outFile << "  ";
							else
								outFile << newC << newC;
							
							*/
							resultFile.get(newC);
							fieldString += newC;
						}
						if (curr.length() == 1) {
							if (curr[0] == 'x')
								outFile << "\xE2\x96\x88" << "\xE2\x96\x88";
							else if (curr[0] == '.')
								outFile << "  ";
							else
								outFile << " " << curr[0];
						}
						else {
							for (int i = 0; i < curr.length(); ++i) {
								if (curr[i] == 'x')
									outFile << "\xE2\x96\x88";
								else if (curr[i] == '.')
									outFile << " ";
								else
									outFile << curr[i];
							}
						}
						if (newC == '\\') {
							resultFile.seekg(-1, ios::cur);
							fieldString = fieldString.substr(0, fieldString.size() - 1);
						}
					}
					outFile << "\xE2\x94\x82" << endl;
				}
				for (int i = 0; i < indentLevel + 1; ++i)
					outFile << "    ";
				outFile << "\xE2\x94\x94";
				for (int x = 0; x < 20; ++x)
					outFile << "\xE2\x94\x80" << "\xE2\x94\x80";
				outFile << "\xE2\x94\x98" << endl;
				for (int i = 0; i < indentLevel; ++i)
					outFile << "    ";
				outFile << fieldString;
				continue;
			}
		}
		outFile << c;
	}
	cout << "Done." << endl;
}