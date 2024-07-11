#include <iostream>
#include <fstream>
#include <filesystem> 
#include <string>
#include <Windows.h>
#include <sstream>
using namespace std;
using namespace std::filesystem;

string FindFirstTwoBytes(const string& path);
bool IsPE(string path);
string FindKeyword(string path, string keyword);
string SearchFiles(string path, string keyword);

int main(int argc, char* argv[])
{
	string path;
	string keyword;
	if (argc >= 3) {
		path = argv[1];		// Assign file path
		keyword = argv[2];  // Assign chosen search keyword
		string print = SearchFiles(path, keyword);
		cout << print;
	}
	else {
		cerr << "Verilen argumanda bir hata vardir.\nKontrol edin ve yeniden calistirin." << endl;
		return 1;	//Unclear arguments
	}
}

string FindFirstTwoBytes(const string& path) {
	try {
		fstream file;
		char readBytes[2]; // 1 char = 1 Byte
		file.open(path, ios::in | ios::binary);

		if (!file) {
			throw runtime_error("Dosya acarken bir hata olustu! FindFirstTwoBytes");
		}

		file.read(readBytes, 2);
		if (file.gcount() >= 2) { // Check if at least 2 byte were read
			file.close();
			return string(readBytes, 2);
		}
		else {
			throw runtime_error("Dosya 2 bayttan daha kucuktur.");
		}
		file.close();
	}
	catch (exception& e) {
		cerr << e.what() << endl;
		return nullptr;
	}

}

char* GetBuffer(const string& path, int& fileSize) {
	try {
		fstream file;
		file.open(path, ios::in | ios::binary);

		if (!file) {
			throw runtime_error("Dosya acarken bir hata olustu! GetBuffer");
		}

		file.seekg(0, ios::end);        // Go to the end
		fileSize = file.tellg();        // Get file size
		file.seekg(0, ios::beg);        // Back to the beginning

		if (fileSize < sizeof(IMAGE_DOS_HEADER)) { 
			return nullptr; // Not a valid PE File
		}

		char* buffer = new char[fileSize];
		file.read(buffer, fileSize);

		if (!file) {
			throw runtime_error("Dosya okuma hatasi!");
		}

		file.close();
		return buffer;
	}
	catch (exception& e) {
		cerr << e.what() << endl;
		return nullptr;
	}
}

bool IsPE(string path) {
	int fileSize = 0;
	char* buffer = GetBuffer(path, fileSize);

	if (!buffer) {
		delete[] buffer; // Not a valid PE File
		return false;
	}

	PIMAGE_DOS_HEADER DOSHeader = (PIMAGE_DOS_HEADER) buffer; // DOS Header to get to NT Header
	PIMAGE_NT_HEADERS NTHeaders = (PIMAGE_NT_HEADERS) (buffer + DOSHeader->e_lfanew); // Get to NT Header using offset

	if (DOSHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		delete[] buffer;
		return false;
	}

	if (NTHeaders->Signature != IMAGE_NT_SIGNATURE) { // Verify that the Signature field is equal to IMAGE_NT_SIGNATURE
		delete[] buffer;
		return false;
	}

	if (NTHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_I386 && NTHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64) {
		delete[] buffer;	// Verify that the Machine field is equal to IMAGE_FILE_MACHINE_I386 or IMAGE_FILE_MACHINE_AMD64
		return false;
	}

	delete[] buffer;
	return true;
}

string FindKeyword(string path, string keyword) {

	try {
		int fileSize = 0;
		char* buffer = GetBuffer(path, fileSize);
		string print;


		if (!buffer) {
			delete[] buffer;
			abort();
		};

		PIMAGE_DOS_HEADER DOSHeader = (PIMAGE_DOS_HEADER)buffer; // DOS Header to get to NT Header
		PIMAGE_NT_HEADERS NTHeaders = (PIMAGE_NT_HEADERS)(buffer + DOSHeader->e_lfanew); // Get to NT Header using offset
		PIMAGE_SECTION_HEADER sectionHeader = IMAGE_FIRST_SECTION(NTHeaders); // Get to sectionHeader ( first section ) 

		// Printing sections
		for (int i = 0; i < NTHeaders->FileHeader.NumberOfSections; ++i, ++sectionHeader) {
			if(sectionHeader->Characteristics & IMAGE_SCN_MEM_EXECUTE){

				DWORD sectionSize = sectionHeader->SizeOfRawData;       // Get size of section and offset of section
				DWORD sectionOffset = sectionHeader->PointerToRawData;	// to iterate inside sections

				if (sectionOffset + sectionSize > fileSize) {
					throw runtime_error("Dosya acarken bir hata olustu!");
				}

				char* sectionData = buffer + sectionOffset;      // Get begining of the section data
				string sectionContent(sectionData, sectionSize); // Load section data into sectionContent

				// Search for the keyword in the section content
				size_t pos = sectionContent.find(keyword);

				// If keyword is found
				if (pos != string::npos) {
					DWORD address = sectionOffset + static_cast<DWORD>(pos); // Calculate postion of found data
					ostringstream resultStream;
					resultStream << "BULUNDU (" << std::dec << address << ") adresinde ve \""
						<< sectionHeader->Name << "\" section'i icerisinde";
					print = resultStream.str();
					delete[] buffer;
					return print;
				}
			}
		}
		// If keyword is not found
		print = "BULUNAMADI";
		delete[] buffer;
		return print;
	}
	catch (exception& e) {
		cerr << e.what() << endl;
		return nullptr;
	}
}

string SearchFiles(string path, string keyword) {

	try {
		string print = "";
		if (exists(path) && is_directory(path)) { // Directory exists and is actually a directory
			for (const auto& file : directory_iterator(path)) {
				string firstTwoBytes = FindFirstTwoBytes(file.path().string());
				if (firstTwoBytes == "MZ" && IsPE(file.path().string())) { // PE File Verification
					string answer = FindKeyword(file.path().string(), keyword);
					print += file.path().string() + ": " + answer + "\n";
				}
				else {
					print += file.path().string() + ": Gecerli PE dosyasi degil\n";
				}
				
			}
		}
		else {
			print = "Girilen dosyanin yolu bir klasor degildir.";
		}
		return print; // Final report for all files
	}
	catch (exception& e) {
		cerr << e.what() << endl;
	}
}
