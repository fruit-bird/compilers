#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
bool is_int(std::string_view input)
{
    for (auto&& c : input) {
        if (NUMBERS.find(c) == std::string::npos) {
            return false;
        }
    }
    return true;
}

bool is_float(std::string_view input) {
    return false;
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
    int int_count = 0;
    int float_count = 0;
    int symbol_count = 0;

    static Stats from_string(std::string_view input)
    {
        // fix for an issue with return carriage
        // collect everything to a string
        std::string input_in_one_line {};
        auto const lines = split(input, '\n');
        for (auto&& line : lines) {
            input_in_one_line += line;
        }

        auto const word_tokens = split(input_in_one_line, ' ');
        auto stats = Stats {};
        // iterate over "words"
        for (auto&& str_token : word_tokens) {
            if (is_word(str_token)) {
                stats.word_count += 1;
            } else if (is_int(str_token)) {
                stats.int_count += 1;
            } else if (is_float(str_token)) {
                stats.float_count += 1;
            } else if (is_symbol(str_token)) {
                stats.symbol_count += 1;
            }
        }
        return stats;
    }
};

// TODO: is_int has to validate floating points too
int main(int argc, char const* argv[])
{
    auto const file_name = argv[1];
    auto const file_contents = read_to_string(file_name);

    auto const stats = Stats::from_string(file_contents);
    std::cout << "\nWord Count:\t" << stats.word_count
              << "\nInteger Count:\t" << stats.int_count
              << "\nFloat Count:\t" << stats.float_count
              << "\nSymbol Count:\t" << stats.symbol_count << '\n';

    return 0;
}
