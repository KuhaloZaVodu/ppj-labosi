#include <iostream>
#include <string>
#include "./analizator.h"

int main() {
    std::string input;

    std::getline(std::cin, input, '\0'); // Procitaj citav stdin u varijablu input

    // ??

    SourceReader* reader = new SourceReader(&input);
    reader->analyze();

    return 0;
}