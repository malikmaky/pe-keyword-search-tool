#include <iostream>
#include <fstream>
#include <filesystem> 
#include <string>
using namespace std;
using namespace std::filesystem;

string findKeyword(string path, string keyword);
string searchFiles(string path, string keyword);


int main(int argc, char* argv[])
{
	string path;
	string keyword;
	if (argc == 3) {
		path = argv[1];		// Assign file path
		keyword = argv[2];  // Assign chosen search keyword
		string print = searchFiles(path, keyword);
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
				string print = "BULUNDU (" + to_string(offset + pos) + " adresinde)";
				return print;									//offset till current line + pos
			}
			offset += read.length() + 1; // Line length + 1 (To not forget the new line char [\n])
		}

		string print = "BULUNMADI";
		return print;

		file.close();
	}
	catch (exception& e) {
		cerr << e.what() << endl;
	}
}

string searchFiles(string path, string keyword) {

	try {
		string print = "";
		if (exists(path) && is_directory(path) ) { // Directory exists and is actually a directory
			for (const auto& file : directory_iterator(path)) {
				string answer = findKeyword(file.path().string(), keyword);
				print += file.path().string() + ":" + answer + "\n";
			}
		}
		else {
			print = "Girilen dosyanin yolu bir klasor degildir.";
		}

		return print;
	}
	catch (exception& e) {
		cerr << e.what() << endl;
	}
}
