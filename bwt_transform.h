#pragma once
#include <vector>
#include <string>

extern const char EOS;  // End of string

// Function template to sort a container
template <typename T>
static int partition(T& container, int low, int high) 
{
    auto pivot = container[high]; 
    int i = low - 1;              

    for (int j = low; j < high; j++) {
        if (container[j] < pivot) 
        {
            i++;
            std::swap(container[i], container[j]);
        }
    }
    std::swap(container[i + 1], container[high]);
    return i + 1;
}

// QuickSort
template <typename T>
static void quickSort(T& container, int low, int high) 
{
    if (low < high) 
    {
        int pi = partition(container, low, high); 

        quickSort(container, low, pi - 1); 
        quickSort(container, pi + 1, high); 
    }
}

// Sort
template <typename T>
void sort(T& container) {
    if (container.size() > 1) 
    { 
        quickSort(container, 0, container.size() - 1);
    }
}

// Create a vector of rotations of the input string
std::vector<std::string> generateRotations(const std::string& str);

// Sort the rotations in lexicographical order
std::vector<std::string> sortRotations(const std::vector<std::string>& rotations);

// Get the Burrows-Wheeler Transform of the input string
std::string getBWT(const std::vector<std::string>& sorted_rotations);

// Get the original string from the Burrows-Wheeler Transform
std::string BurrowsWheelerTransform(const std::string& input);

// Sort the BWT string and return the sorted BWT
std::vector<char> sortBWT(const std::string& bwt);

// Build the first and last tables for the BWT string
std::vector<int> buildFirstLastTables(const std::string& bwt, 
    const std::vector<char>& sorted_bwt);

// Recover the original string from the BWT string
std::string recoverBWT(const std::string& bwt);
