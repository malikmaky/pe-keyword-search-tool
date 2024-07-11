#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string findFirstTwoBytes(const string& path);

int main(int argc, char* argv[])
{
	string path;
	if (argc == 2) {
		path = argv[1];		// Assign file path
		string bytes = findFirstTwoBytes(path);
		if(bytes.length() == 2 )
			cout << "Dosyanin ilk iki bayti : " << bytes << endl;
	}
	else {
		cout << "Verilen argumanda bir hata vardir.\nKontrol edin ve yeniden calistirin." << endl;
		return 1;	//Unclear arguments
	}
}

string findFirstTwoBytes(const string& path) {

	try {
		fstream file;
		char readBytes[2]; // 1 char = 1 Byte
		file.open(path, ios::in | ios::binary);

		if (!file.is_open()) {
			throw runtime_error("Dosya acarken bir hata olustu!"); 
			abort();
		}

		file.read(readBytes, 2);
		if (file.gcount() >= 2) { // Check if at least 2 byte were read
			file.close();
			return string(readBytes, 2);
		}
		else {
			throw runtime_error("Dosya 2 bayttan daha kucuktur.");
			abort();
		}


		file.close();
	}
	catch (exception& e) {
		cerr << e.what() << endl;
	}

}

