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
    std::string owned_str { str }; // no overload from stringstream to string_view
    std::stringstream ss { owned_str };
    std::string token;

    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

// never mind closing the file
std::string read_to_string(std::string_view file_name)
{
    // Open the file for reading
    std::ifstream input_file(file_name);

    // Read the file contents into the buffer then convert it to a string
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    std::string contents = buffer.str();
    return contents;
}

bool is_word(std::string_view input) noexcept
{
    for (auto&& c : input) {
        if (LETTERS.find(c) == std::string::npos) {
            return false;
        }
    }
    return true;
}

bool is_int(std::string_view input) noexcept
{
    for (auto&& c : input) {
        if (NUMBERS.find(c) == std::string::npos) {
            return false;
        }
    }
    return true;
}

bool is_float(std::string_view input) noexcept
{
    bool input_has_exp = false;
    bool input_has_comma = false;

    for (auto&& c : input) {
        // Floating point comma guard
        if (c == '.' && !input_has_comma) {
            input_has_comma = true;
        } else if (c == '.' && input_has_comma) {
            return false; // input contains more than one comma
        }

        // Exponential e guard
        if (c == 'e' && !input_has_exp) {
            input_has_exp = true;
        } else if (c == 'e' && input_has_exp) {
            return false; // input contains more than one exp
        }

        if (NUMBERS.find(c) == std::string::npos) {
            return false;
        }
    }
    return true;
}

bool is_symbol(std::string_view input) noexcept
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

    static Stats from_string(std::string_view input) noexcept
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

int main(int argc, char const* argv[])
{
    // auto const test_input = "hello 3.45.2 little world 3.2 , i4m\n 5 I 2 am writing 3 fires\n lines . I shou5ld stop everywhere . 21\n lmao 3.2 lmao";
    auto const file_name = argv[1];
    auto const file_contents = read_to_string(file_name);

    auto const stats = Stats::from_string(file_contents);
    std::cout << "Word Count:\t" << stats.word_count
              << "\nInteger Count:\t" << stats.int_count
              << "\nFloat Count:\t" << stats.float_count
              << "\nSymbol Count:\t" << stats.symbol_count << '\n';

    return 0;
}
