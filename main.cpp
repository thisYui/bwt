#include "file_utils.h"
#include "pattern_search.h"
#include "bwt_transform.h"
#include "data_compression.h"

int main(int argc, char** argv)
{
	// Check if the user has entered the correct number of arguments
	if (argc < 4 || argc > 5)
	{
		std::cout << "Invalid number of arguments. Please enter the correct number of arguments." << std::endl;
		return 1;
	}

	/*
	* Check if the user has entered the correct command
	* 
	* Command 1: <program.exe>​ - c input.txt output.txt [--bwt]
	* Where:
	*	 • input.txt: This text file contains strings that will be converted to the BWT form.
	*	 Each string is on a separate line in the file.
	*	 • output.txt: This text file will contain the converted form of each string from input.txt,
	*	 with each converted string on a separate line.
	*	 • --bwt: If this parameter is included, the program will convert strings from normal form to
	*	 BWT form. If it is not included, the program will convert from BWT form to normal form.
	*/
	if (std::string(argv[1]) == "-c")
	{
		checkExistence(argv[2]);  // Check if the input file exists
		
		std::vector<std::string> input = readInputFile(argv[2]);  // Read the input file
		std::vector<std::string> output;  // Store the output strings

		for (const auto& str : input)
		{
			if (argc == 5 && std::string(argv[4]) == "--bwt")
			{
				output.push_back(BurrowsWheelerTransform(str));  // Convert the string to BWT form
			}
			else
			{
				output.push_back(recoverBWT(str));  // Convert the string to normal form
			}
		}

		writeOutputFile(argv[3], output);  // Write the output to a file

		if (argc == 5 && std::string(argv[4]) == "--bwt")
		{
			std::cout << "The strings have been converted to BWT form and written to " << argv[3] << ".\n";
		}
		else
		{
			std::cout << "The strings have been converted to normal form and written to " << argv[3] << ".\n";
		}
	}
	/*
	* Command 2: <program.exe>​ -p paragraph.txt patterns.txt output.txt
	* Where:
	*	• paragraph.txt: This text file contains the paragraph, all on a single line.
	*	• patterns.txt: This text file contains the patterns to search for, with each pattern on a
	*	separate line.
	*	• output.txt: This text file will contain the results of the pattern search, in the following
	*	format:
	*		pattern1: 0, 3, ...
	* 		pattern2: 4, 9, ...
	* 		...
	*	Here, pattern1, pattern2, .. are the patterns from patterns.txt. After each pattern,
	*	there is a list of the starting positions (indices) where the pattern appears in the paragraph.
	*	The indices start from 0, where 0 is the position of the first character
	*/
	else if (std::string(argv[1]) == "-p")
	{
		checkExistence(argv[2]);  // Check if the paragraph file exists
		checkExistence(argv[3]);  // Check if the patterns file exists

		std::string paragraph = readParagraph(argv[2]);  // Read the paragraph from a file
		std::vector<std::string> patterns = readPatterns(argv[3]);  // Read the patterns from a file
		std::vector<std::vector<int>> results;  // Store the results of the pattern search

		for (const auto& pattern : patterns)
		{
			results.push_back(findPattern(paragraph, pattern));  // Find the pattern in the paragraph
		}

		writeResults(argv[4], patterns, results);  // Write the results to a file

		std::cout << "The results of the pattern search have been written to " << argv[4] << ".\n";
	}
	/*
	* Command 3: <program.exe>​ -z src.txt des.txt [--u]
	* Where:
	*	• src.txt: This text file contains the data to be compressed or decompressed.
	*	• des.txt: This text file will contain the compressed data or the decompressed data.
	*	• --u: If this parameter is included, the program will decompress the data. If it is not included, the program will compress the data.
	*/
	else if (std::string(argv[1]) == "-z")
	{
		checkExistence(argv[2]);  // Check if the input file exists

		if (argc == 5 && std::string(argv[4]) == "--u")
		{
			writeDecompressedFile(argv[2], argv[3]);  // Write the decompressed paragraph to a file
			std::cout << "The paragraph has been decompressed and written to " << argv[3] << ".\n";		
		}
		else
		{
			writeCompressedFile(argv[2], argv[3]);  // Write the compressed paragraph to a file
			std::cout << "The paragraph has been compressed and written to " << argv[3] << ".\n";
		}
	}
	/*
	* Invalid command
	*/
	else
	{
		std::cout << "Invalid command. Please enter a valid command.\n";
		return 1;
	}
	
    return 0;
}
