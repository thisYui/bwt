#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

const uint8_t MAGIC[3] = { 0x42, 0x57 ,0x54};  // Magic number to identify the file format
const uint8_t VERSION = 0x01;  // Version of the file format

// Header size 14 bytes  
struct BWTZipHeader
{
	// Magic Number: 0x42 0x57 0x54 or BWT
	uint8_t magic[3] = { 0x42, 0x57, 0x54};  // Magic number to identify the file format
	uint8_t version = 0x01;  // Version of the file format
	uint8_t alphabetSize;  // Size of the alphabet
	uint8_t pairSize;  // Size of the pair
	uint32_t originalSize;  // Original size of the data
	uint32_t compressedSize;  // Compressed size of the data
	
	void setter(uint32_t originalSize, uint32_t compressedSize, uint8_t alphabetSize, uint8_t pairSize)
	{
		this->originalSize = originalSize;
		this->compressedSize = compressedSize;
		this->alphabetSize = alphabetSize;
		this->pairSize = pairSize;
	}
};

struct BWTZip
{
public:
	
	std::vector<char> getAlphabet() const;
	std::vector<std::pair<int, int>> getPairs() const; // Get the compressed data as a string
	void setter(const std::vector<char>& alphabet, const std::vector<uint8_t>& bytes, unsigned int originalSize, unsigned int pairSize)
	{
		this->alphabet = alphabet;
		this-> bytes = bytes;
		this->header.setter(static_cast<uint32_t>(originalSize), bytes.size(), alphabet.size(), static_cast<uint8_t>(pairSize));
	};

	static std::vector<std::pair<int, int>> bytesToPairs(const std::vector<uint8_t>& bytes, uint8_t pairSize); // Convert the compressed data to a string
	static std::vector<uint8_t> pairsToBytes(const std::vector<std::pair<int, int>>& pairs, uint8_t pairSize); // Convert the string to compressed data

	bool read(std::ifstream& file);
	void write(std::ofstream& file);

private:
	BWTZipHeader header;  // Header of the compressed file
	std::vector<char> alphabet;  // Alphabet used for encoding
	std::vector<uint8_t> bytes;  // Compressed data
};

// Check if the file exists
void checkExistence(const std::string& filename);

 // Read the input file
std::vector<std::string> readInputFile(const std::string& filename);

// Write the output to a file
void writeOutputFile(const std::string& filename, const std::vector<std::string>& output);

// Read the paragraph from a file
std::string readParagraph(const std::string& filename);

// Read the patterns from a file
std::vector<std::string> readPatterns(const std::string& filename);

// Write the results to a file
void writeResults(const std::string& filename, const std::vector<std::string>& patterns,
	const std::vector<std::vector<int>>& results);

// Read all file content
std::string readAll(const std::string& filename);

// Write the compressed paragraph to a file
void writeCompressedFile(const std::string& srcFile, const std::string& desFile);

// Write the decompressed paragraph to a file
void writeDecompressedFile(const std::string& srcFile, const std::string& desFile);
