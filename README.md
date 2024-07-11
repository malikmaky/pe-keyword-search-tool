
## Features :

- #### __PE File Verification :__ 
Checks if a file is a valid PE file by verifying its headers.

- #### __Executable Section Validation :__ 
Validates executable sections within PE files to ensure accurate keyword search results.
 
- #### __Keyword Search :__

Searches for a specified keyword within the __executable sections__ of PE files.

- #### __Directory Scanning :__
Iterates through files in a specified directory and applies the PE verification and keyword search.

## Applications :
- __Application 1 :__ Prints the first two bytes of each file.

- __Application 2 :__ Searches for a specified keyword within the entire content of a single file.

- __Application 3 :__ Iterates through files in a specified directory and searches for a keyword in each file.

- __Application 4 :__ Validates each file as a PE file and searches for a keyword within executable sections.

## Dependencies :

- #### Standard C++ Library

- #### Windows API

## PE File Validation : 
- What is a PE File?
A Portable Executable (PE) file is a file format used in Windows operating systems for executables, object code, and DLLs. It is a data structure that encapsulates the information necessary for the Windows OS loader to manage the wrapped executable code.

![Logo](https://images2.imgbox.com/ee/5d/jWz6DhCf_o.png)

##

- How Do We Validate a PE File?

__1- Check the DOS Header:__ The PE file begins with a DOS header, which includes the magic number "MZ." This header is used to ensure compatibility with DOS-based systems.

__2- Verify the NT Headers:__ The DOS header contains a pointer to the PE header, known as the NT headers. The NT headers start with the "PE\0\0" signature.

__3- Check Machine Type:__ The file header within the NT headers specifies the architecture type. We check if it matches known values for x86 or x64 architectures.

##

- Validating Executable Sections :

Executable sections within a PE file are crucial for code execution. We validate these sections to ensure accurate keyword search results:

-__Identify Executable Sections:__ Using the NT headers, we iterate through each section header to identify sections marked as executable (IMAGE_SCN_MEM_EXECUTE).

-__Section Size and Offset:__ We calculate the size and offset of each executable section to ensure that our search remains within valid boundaries.

-__Search within Sections:__ We load the section data into memory and perform our keyword search within these validated executable sections.

## Running the Application : 

The application runs through the command line (CMD) and requires two parameters: the directory path containing the files to search and the keyword to search for. ( You can use the executable files)

- Example Usage:

C:>/ApplicationName.exe <directory_path> <keyword>

## Example Of Running Application 3 & 4 : 
![Logo](https://images2.imgbox.com/89/7f/qPbKCh8D_o.png)


## Lessons Learned :

- __Understanding PE File Structure :__ Working with PE files provided insights into their structure, including DOS headers, NT headers, and section headers, which are critical for validating and analyzing executable files.

- __Error Handling :__ Implementing robust error handling mechanisms was crucial due to potential issues with file access, invalid headers, and unexpected file formats. This improved the reliability and stability of the application.

- __Executable Section Validation :__ Validating executable sections within PE files ensured accurate search results, enhancing the application's functionality for cybersecurity and forensic analysis purposes.

- __Command-Line Interface (CLI) Design :__ Designing applications to accept command-line arguments efficiently allowed for flexible and streamlined usage, catering to various use cases from single file analysis to directory-wide searches.

- __Industry Application:__ Developing this project during my internship at __C-Prot__, a cybersecurity company specializing in antivirus software, provided practical experience in applying C++ for analyzing and securing executable files, which aligns with the company's mission to enhance digital security.

- __Future Enhancements :__ Identified areas for future enhancements include implementing recursive directory scanning, optimizing search algorithms for large files, and integrating multi-threading for improved performance.

## Conclusion :
This project demonstrates a practical application of C++ in the field of cybersecurity, specifically in the analysis of PE files. It provides a foundation for further enhancements, such as recursive directory scanning, multi-threaded search, and advanced keyword matching techniques.
