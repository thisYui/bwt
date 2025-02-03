#include "pattern_search.h"
#include "bwt_transform.h"

extern const char EOS;  // End of string
const int NF = -1;  // Not found

std::vector<int> suffixArray(const std::string& str)
{
	std::vector<std::pair<std::string, int>> suffixes;  // Store the suffixes

	// Generate the suffixes
	for (size_t i = 0; i < str.size(); i++)
	{
		suffixes.push_back(std::make_pair(str.substr(i), i));
	}

	// Sort the suffixes
	sort(suffixes);

	std::vector<int> SA;  // Store the suffix array

	// Get the indices of the suffixes
	for (const auto& suffix : suffixes)
	{
		SA.push_back(suffix.second);
	}

	return SA;
}

std::vector<int> createCTable(const std::string& bwt)
{
	std::vector<int> C(256, 0);  // Initialize the C table

	// Count the occurrences of each character in the BWT string
	for (const auto& c : bwt)
	{
		C[c]++;
	}

	// Calculate the cumulative sum of the occurrences
	for (size_t i = 1; i < C.size(); i++)
	{
		C[i] += C[i - 1];
	}

	// Adjust the cumulative sum
	for (const auto& c : bwt)
	{
		C[c]--;
	}

	return C;
}

std::vector<std::vector<int>> createOccurrenceTable(const std::string& bwt)
{
	std::vector<std::vector<int>> Occ(256, std::vector<int>(bwt.size(), 0));

	// Calculate the occurrences of each character in the BWT string
	for (size_t i = 0; i < bwt.size(); i++)
	{
		for (size_t j = 0; j < Occ.size(); j++)
		{
			Occ[j][i] = (i > 0 ? Occ[j][i - 1] : 0) + (bwt[i] == j ? 1 : 0);
		}
	}

	return Occ;
}

int calculateLF(int i, const std::vector<int>& C, const std::vector<std::vector<int>>& Occ, const std::string& L)
{
	if (i == NF)
	{
		return NF;
	}

	// Adjust the cumulative sum
	return C[L[i]] + Occ[L[i]][i] - 1;
}

std::pair<int, int> findRange(const std::string& paragraph, const std::string& pattern)
{
	std::string bwt = BurrowsWheelerTransform(paragraph);  // Convert the paragraph to BWT form
	std::vector<int> C = createCTable(bwt);  // Create the C table
	std::vector<std::vector<int>> Occ = createOccurrenceTable(bwt);  // Create the Occurrence table

	int top = 0;  // Initialize the top
	int bottom = bwt.size() - 1;  // Initialize the bottom

	// Search for the pattern in the BWT string
	for (int i = pattern.size() - 1; i >= 0; i--)
	{
		char c = pattern[i];  // Get the character
		int first = NF;  // Initialize the first
		int last = NF;  // Initialize the last

		// Find the range of occurrences of the character
		for (int j = top; j <= bottom; j++)
		{
			if (bwt[j] == c)
			{
				if (first == NF)
				{
					first = j;
				}
				last = j;
			}
		}

		// Update the top and bottom
		top = calculateLF(first, C, Occ, bwt);
		bottom = calculateLF(last, C, Occ, bwt);
	}

	return std::make_pair(top, bottom);
}

std::vector<int> findPattern(const std::string& paragraph, const std::string& pattern)
{
	std::vector<int> indices;  // Store the indices
	std::vector<int> SA = suffixArray(paragraph);  // Get the suffix array
	std::pair<int, int> range = findRange(paragraph, pattern);  // Find the range of the pattern

	// Check if the pattern is found
	if (range.first == NF || range.second == NF)
	{
		return indices;
	}

	// Get the indices of the pattern
	for (int i = range.first; i <= range.second; i++)
	{
		indices.push_back(SA[i]);
	}

	// Sort the indices
	sort(indices);

	return indices;
}
