#include "bwt_transform.h"

const char EOS = '$';  // End of string
const int UNDEFINED = -1;  // Undefined value


// Create a vector of rotations of the input string
std::vector<std::string> generateRotations(const std::string& str)
{
	std::vector<std::string> rotations;
	std::string temp = str;

	// Generate all the rotations of the input string
	for (size_t i = 0; i < str.size(); ++i)
	{
		rotations.push_back(temp);
		temp = temp.substr(1) + temp[0];
	}

	return rotations;
}

// Sort the rotations in lexicographical order
std::vector<std::string> sortRotations(const std::vector<std::string>& rotations)
{
	std::vector<std::string> sortedRotations = rotations;

	// Sort the rotations in lexicographical order
	sort(sortedRotations);

	return sortedRotations;
}

// Get the Burrows-Wheeler Transform of the input string
std::string getBWT(const std::vector<std::string>&sortedRotations)
{
	std::string bwt;

	// Get the last characters of the sorted rotations
	for (const auto& str : sortedRotations)
	{
		bwt += str.back();
	}

	return bwt;

}

// Get the original string from the Burrows-Wheeler Transform
std::string BurrowsWheelerTransform(const std::string& input)
{
	std::vector<std::string> rotations = generateRotations(input);
	std::vector<std::string> sortedRotations = sortRotations(rotations);
	std::string bwt = getBWT(sortedRotations);

	return bwt;
}

// Sort the BWT string and return the sorted BWT
std::vector<char> sortBWT(const std::string& bwt)
{
	// Copy the BWT string to a vector
	std::vector<char> sorted_bwt(bwt.begin(), bwt.end());

	// Sort the BWT string
	sort(sorted_bwt);

	return sorted_bwt;
}

// Build the first and last tables for the BWT string
std::vector<int> buildFirstLastTables(const std::string& bwt, const std::vector<char>& sorted_bwt)
{
	// Initialize the first and last table with UNDEFINED (indicating no value assigned)
	std::vector<int> first(bwt.size(), UNDEFINED);

	// Build the first table
	for (size_t i = 0; i < sorted_bwt.size(); i++)
	{
		for (size_t j = 0; j < bwt.size(); j++)
		{
			if (sorted_bwt[i] == bwt[j] && first[j] == UNDEFINED)
			{
				first[j] = i;
				break;
			}
		}
	}

	return first;
}

// Recover the original string from the BWT string
std::string recoverBWT(const std::string& bwt)
{
	std::vector<char> sorted_bwt = sortBWT(bwt);
	std::vector<int> first = buildFirstLastTables(bwt, sorted_bwt);

	// Find the index of the EOS the BWT string
	// If in the string there is a character smaller than the ascii code of $ 
	// then the index of $ is not 0 
	int index = 0;
	while (bwt[index] != EOS)
	{
		index++;
	}

	std::string original;
	while (original.size() < bwt.size())
	{
		original = bwt[index] + original;
		index = first[index];
	}

	return original;
}
