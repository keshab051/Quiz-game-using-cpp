#include <iostream>
#include <fstream>
#include <vector>

int main() {
    std::string filename;
    std::cout << "Enter the name of the binary file: ";
    std::cin >> filename;

    // Open the binary file for reading
    std::ifstream inputFile(filename, std::ios::in | std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    // Read the contents of the binary file into a vector
    std::vector<char> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Print the content to the console
    std::cout << "File content:" << std::endl;
    for (char c : buffer) {
        std::cout << c;
    }
    std::cout << std::endl;

    return 0;
}
