#pragma once
#include <string>
#include <vector>


// Suffix array
std::vector<int> suffixArray(const std::string& str);

// Create the CTable for the BWT string
std::vector<int> createCTable(const std::string& bwt);

// Create the Occurrence Table for the BWT string
std::vector<std::vector<int>> createOccurrenceTable(const std::string& bwt);

// Calculate the Last-to-First mapping
int calculateLF(int i, const std::vector<int>& C, const std::vector<std::vector<int>>& Occ, const std::string& L);

// Find the range of the pattern in the paragraph
std::pair<int, int> findRange(const std::string& paragraph, const std::string& pattern);

// Find the pattern in the paragraph and return the indices
std::vector<int> findPattern(const std::string& paragraph, const std::string& pattern);