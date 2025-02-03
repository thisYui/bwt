#pragma once
#include <string>
#include <vector>


// Initialize the alphabet
std::vector<char> initAlphabet(const std::string& data);

// Using MTF (Move-to-Front) encoding to compress the data
std::vector<int> MTF(const std::string& data);

// Using RLE (Run-Length Encoding) to compress the data
std::vector<std::pair<int, int>> RLE(const std::vector<int>& toMTF);

// Decompress the data using RLE (Run-Length Encoding) decoding
std::vector<int> RLEDecode(const std::vector<std::pair<int, int>>& data);

// Decompress the data using MTF (Move-to-Front) decoding
std::string MTFDecode(const std::vector<char>& alphabet, const std::vector<int>& data);

// Count bytes need to store the number
int countBytes(const std::vector<std::pair<int, int>>& data);
