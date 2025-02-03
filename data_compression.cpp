#include "data_compression.h"
#include "bwt_transform.h"
#include <cstdint>

extern const char EOS;  // End of string
const int ERROR = -1;  // Error value

std::vector<char> initAlphabet(const std::string& data)
{
	std::vector<char> alphabet;  // Store the alphabet
	std::string sortedData = data;  // Store the sorted data
	sort(sortedData);  // Sort the data

	// Initialize the alphabet
	for (size_t i = 0; i < sortedData.size(); i++)
	{
		alphabet.push_back(sortedData[i]);  // Add the character to the alphabet
		while (i + 1 < sortedData.size() && sortedData[i] == sortedData[i + 1])
		{
			i++;  // Skip the duplicate characters
		}
	}

	return alphabet;
}

std::vector<int> MTF(const std::string& data)
{
	std::string bwt = BurrowsWheelerTransform(data);  // Convert the string to BWT form

	std::vector<int> result;  // Store the result
	std::vector<char> alphabet = initAlphabet(data);  // Store the alphabet

	// Move-to-Front encoding
	for (const auto& c : bwt)
	{
		for (size_t i = 0; i < alphabet.size(); i++)
		{
			if (alphabet[i] == c)
			{
				result.push_back(i);  // Add the index of the character to the result
				alphabet.erase(alphabet.begin() + i);  // Remove the character from the alphabet
				alphabet.insert(alphabet.begin(), c);  // Add the character to the front of the alphabet
				break;
			}
		}
	}

	return result;
}

std::vector<std::pair<int, int>> RLE(const std::vector<int>& toMTF)
{
	std::vector<std::pair<int, int>> result;  // Store the result


	int count = 1;  // Initialize the count
	int prev = toMTF[0];  // Initialize the previous character

	// Run-Length Encoding
	for (size_t i = 1; i < toMTF.size(); i++)
	{
		if (toMTF[i] == prev)
		{
			++count;  // Increment the count
		}
		else
		{
			result.push_back(std::make_pair(prev, count));  // Add the character and count to the result
			count = 1;  // Reset the count
			prev = toMTF[i];  // Update the previous character
		}
	}

	result.push_back(std::make_pair(prev, count));  // Add the last character and count to the result

	return result;
}


std::vector<int> RLEDecode(const std::vector<std::pair<int, int>>& data)
{
	std::vector<int> result;  // Store the result

	// Run-Length Encoding decoding
	for (const auto& p : data)
	{
		for (int i = 0; i < p.second; i++)
		{
			result.push_back(p.first);  // Add the character to the result
		}
	}

	return result;
}


std::string MTFDecode(const std::vector<char>& alphabet, const std::vector<int>& data)
{
	std::string bwt;  // Store the bwt
	std::vector<char> temp = alphabet;  // Store the alphabet

	// Move-to-Front decoding
	for (const auto& i : data)
	{
		bwt.push_back(temp[i]);  // Add the character to the bwt
		char c = temp[i];  // Store the character
		temp.erase(temp.begin() + i);  // Remove the character from the alphabet
		temp.insert(temp.begin(), c);  // Add the character to the front of the alphabet
	}

	std::string result = recoverBWT(bwt);  // Recover the original string from the BWT string

	return result;
}

int countBytes(const std::vector<std::pair<int, int>>& data)
{
	int maValue = data[0].second;  // Store the maximum value
	for (const auto& p : data)
	{
		if (p.second > maValue)
		{
			maValue = p.second;  // Update the maximum value
		}
	}

	if (maValue < (1 << 8))
	{
		return sizeof(uint8_t);  // Return 1 byte
	}
	else if (maValue < (1 << 16))
	{
		return sizeof(uint16_t);  // Return 2 bytes
	}
	else
	{
		return sizeof(uint32_t);  // Return 4 bytes
	}
}

