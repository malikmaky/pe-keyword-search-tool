#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string findKeyword(string path, string keyword);

int main(int argc, char* argv[])
{
	string path;
	string keyword;
	if (argc == 3) {
		path = argv[1];		// Assign file path
		keyword = argv[2];  // Assign chosen search keyword
		string print = findKeyword(path, keyword);
		cout << print << endl;
	}
	else {
		cout << "Verilen argumanda bir hata vardir.\nKontrol edin ve yeniden calistirin." << endl;
		return 1;	//Unclear arguments
	}
}

string findKeyword(string path, string keyword) {

	try {
		fstream file;
		file.open(path, ios::in | ios::binary);

		if (!file.is_open()) {
			throw runtime_error("Dosya acarken bir hata olustu!");
			abort();
		}

		size_t offset = 0; 
		string read;
		while (getline(file, read)) {
			size_t pos = read.find(keyword);
			if (pos != std::string::npos) {
				string print = "\""+keyword+"\" ifadesi "+to_string(offset+pos)+" adresinde bulundu.";
				return print;									//offset till current line + pos
			}
			offset += read.length() + 1; // Line length + 1 (To not forget the new line char [\n])
		}

		string print = "\"" + keyword + "\" ifadesi herhangi bir konumda bulunamadi.";
		return print;

		file.close();
	}
	catch (exception& e) {
		cerr << e.what() << endl;
	}
}