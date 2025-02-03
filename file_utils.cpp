#include "file_utils.h"
#include "data_compression.h"
#include "bwt_transform.h"

extern const char EOS;  // End of string

std::vector<char> BWTZip::getAlphabet() const
{
	return this->alphabet;
}

std::vector<std::pair<int, int>> BWTZip::getPairs() const
{
	return this->bytesToPairs(this->bytes, this->header.pairSize);
}


bool BWTZip::read(std::ifstream& file)
{
	// Read the header
	file.read(reinterpret_cast<char*>(&this->header), sizeof(BWTZipHeader));

	// Check the magic number
	if (this->header.magic[0] != MAGIC[0] || this->header.magic[1] != MAGIC[1])
	{
		std::cout << "Error: Invalid file format.\n";
		return false;
	}

	// Check the version
	if (this->header.version != VERSION)
	{
		std::cout << "Error: Invalid file version.\n";
		return false;
	}

	// Read the alphabet
	this->alphabet.resize(this->header.alphabetSize);
	for (size_t i = 0; i < this->header.alphabetSize; i++)
	{
		file.read(reinterpret_cast<char*>(&this->alphabet[i]), sizeof(char));
	}

	// Read the compressed data
	this->bytes.resize(this->header.compressedSize);
	for (size_t i = 0; i < this->header.compressedSize; i++)
	{
		file.read(reinterpret_cast<char*>(&this->bytes[i]), sizeof(uint8_t));
	}

	return true;
}

void BWTZip::write(std::ofstream& file)
{
	// Write the header
	file.write(reinterpret_cast<const char*>(&this->header), sizeof(BWTZipHeader));

	// Write the alphabet
	for (const auto& c : this->alphabet)
	{
		file.write(reinterpret_cast<const char*>(&c), sizeof(char));
	}

	// Write the compressed data
	for (const auto& b : this->bytes)
	{
		file.write(reinterpret_cast<const char*>(&b), sizeof(uint8_t));
	}
}

std::vector<std::pair<int, int>> BWTZip::bytesToPairs(const std::vector<uint8_t>& bytes, uint8_t pairSize)
{
	std::vector<std::pair<int, int>> pairs;

	for (size_t i = 0; i < bytes.size(); i += pairSize)
	{
		// 1 pair = pairSize bytes
		// num of the byte array = number of pairs * pairSize
		// first (1 byte)
		int first = bytes[i];
		// second  (pairSize - 1 bytes)
		int second = 0;
		for (int j = 0; j < pairSize - 1; j++) 
		{
			second |= static_cast<int>(bytes[i + j + 1] << (j * 8));
		}
		pairs.push_back(std::make_pair(first, second));
	}

	return pairs;
}

std::vector<uint8_t> BWTZip::pairsToBytes(const std::vector<std::pair<int, int>>& pairs, uint8_t pairSize)
{
	std::vector<uint8_t> bytes;

	for (const auto& p : pairs)
	{
		// 1 pair = pairSize bytes
		// num of the byte array = number of pairs * pairSize
		// first (1 byte)
		// first (1 byte)
		bytes.push_back(static_cast<uint8_t>(p.first));
		// second (pairSize - 1 bytes)
		for (int i = 0; i < pairSize - 1; i++) {

			bytes.push_back(static_cast<uint8_t>((p.second >> (i * 8)) & 0xff));
		}
	}

	return bytes;
}


void checkExistence(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cout << "Error: Unable to open " << filename << ".\n";
		exit(1);
	}
	file.close();
}

std::vector<std::string> readInputFile(const std::string& filename)
{
	std::vector<std::string> input;
	std::ifstream file(filename);

	std::string line;
	while (getline(file, line))
	{
		input.push_back(line);
	}

	file.close();

	return input;
}

void writeOutputFile(const std::string& filename, const std::vector<std::string>& output)
{
	std::ofstream file(filename);
	if (!file.is_open())
	{
		std::cout << "Error: Unable to open " << filename << ".\n";
		exit(1);
	}

	for (const auto& str : output)
	{
		file << str << std::endl;
	}

	file.close();
}

std::string readParagraph(const std::string& filename)
{
	std::ifstream file(filename);

	std::string paragraph;
	getline(file, paragraph);

	file.close();

	return paragraph;
}

std::vector<std::string> readPatterns(const std::string& filename)
{
	std::vector<std::string> patterns;
	std::ifstream file(filename);

	std::string line;
	while (getline(file, line))
	{
		patterns.push_back(line);
	}

	file.close();

	return patterns;
}

void writeResults(const std::string& filename, const std::vector<std::string>& patterns, const std::vector<std::vector<int>>& results)
{
	std::ofstream file(filename);
	if (!file.is_open())
	{
		std::cout << "Error: Unable to open " << filename << ".\n";
		exit(1);
	}

	for (size_t i = 0; i < patterns.size(); i++)
	{
		file << patterns[i] << ": ";

		if (results[i].empty())
		{
			file << "Not found!";
		}

		for (size_t j = 0; j < results[i].size(); ++j)
		{
			file << results[i][j];
			if (j != results[i].size() - 1)
			{
				file << ", ";
			}
		}
		file << std::endl;
	}

	file.close();
}

std::string readAll(const std::string& filename)
{
	std::ifstream file(filename);

	std::string content;
	std::string line;
	while (getline(file, line))
	{
		content += line + '\n';
	}

	content.pop_back();  // Remove the last newline character in last line

	file.close();

	return content;
}

void writeCompressedFile(const std::string& srcFile, const std::string& desFile)
{
	std::string paragraph = readAll(srcFile);

	if (paragraph[paragraph.size() - 1] != EOS)
	{
		paragraph.push_back(EOS);
	}

	std::ofstream file(desFile, std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "Error: Unable to open " << desFile << ".\n";
		exit(1);
	}
	
	BWTZip zip;
	std::vector<std::pair<int, int>> toRLE = RLE(MTF(paragraph));
	int pairSize = countBytes(toRLE) + 1;

	zip.setter(initAlphabet(paragraph), BWTZip::pairsToBytes(toRLE, pairSize), paragraph.size(), pairSize);

	zip.write(file);
}

void writeDecompressedFile(const std::string& srcFile, const std::string& desFile)
{
	std::ifstream file(srcFile, std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "Error: Unable to open " << srcFile << ".\n";
		exit(1);
	}

	BWTZip zip;
	if (!zip.read(file))
	{
		std::cout << "Error: Unable to read " << srcFile << ".\n";
		exit(1);
	}

	std::string paragraph = MTFDecode(zip.getAlphabet(), RLEDecode(zip.getPairs()));

	if (paragraph[paragraph.size() - 1] == EOS)
	{
		paragraph.pop_back();
	}

	writeOutputFile(desFile, { paragraph });

	file.close();
}