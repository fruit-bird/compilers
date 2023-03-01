#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

std::string const NUMBERS { "0123456789" };
std::string const LETTERS { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" };
std::string const SYMBOLS { ",.!?;:" };

std::vector<std::string> split(std::string_view str, char delim = ' ')
{
    std::vector<std::string> tokens;
    std::string owned_str { str }; // no overload from stringstream to string
    std::stringstream ss(owned_str);
    std::string token;

    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }

    return tokens;
}

// never mind closing the file for now
std::string read_to_string(std::string_view fileName)
{
    // Open the file for reading
    std::ifstream inputFile(fileName);

    // Read the file contents into the buffer then convert it to a string
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string contents = buffer.str();
    return contents;
}

bool is_word(std::string_view input)
{
    for (auto&& c : input) {
        if (LETTERS.find(c) == std::string::npos) {
            return false;
        }
    }
    return true;
}

// floating point numbers to do
bool is_number(std::string_view input)
{
    for (auto&& c : input) {
        if (NUMBERS.find(c) == std::string::npos) {
            return false;
        }
    }
    return true;
}

bool is_symbol(std::string_view input)
{
    for (auto&& c : input) {
        if (SYMBOLS.find(c) == std::string::npos) {
            return false;
        }
    }
    return true;
}

struct Stats {
    int word_count = 0;
    int number_count = 0;
    int symbol_count = 0;

    static Stats from_string(std::string_view input)
    {
        auto stats = Stats {};
        auto const word_tokens = split(input, ' ');

        // std::string owned_input { input };
        // std::replace(input.begin(), input.end(), "\n", " ");

        for (auto&& e : word_tokens) {
            std::cout << "[" << e << "]\n";
        }

        for (auto&& str_token : word_tokens) {
            if (is_word(str_token)) {
                stats.word_count += 1;
            } else if (is_number(str_token)) {
                stats.number_count += 1;
            } else if (is_symbol(str_token)) {
                stats.symbol_count += 1;
            }
        }
        return stats;
    }
};

int main()
{
    auto const file_contents = read_to_string("text_stats/input.txt");

    auto const stats = Stats::from_string(file_contents);           // should be
    std::cout << "\nWord Count:\t" << stats.word_count              // 14
              << "\nNumber Count:\t" << stats.number_count          // 4
              << "\nSymbol Count:\t" << stats.symbol_count << '\n'; // 3

    return 0;
}
