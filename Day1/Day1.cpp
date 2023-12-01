#include <iostream>
#include <vector>
#include <fstream>
#include <string>

struct DigitPair
{
    std::string word{};
    uint8_t digit{};
};

static void ReadInput(const std::string& path, std::vector<std::string>& result)
{
    std::ifstream file{ path };
    std::string line{};

    while (std::getline(file, line))
    {
        result.push_back(line);
    }
}

uint32_t FindAndAddDigits(const std::string& str)
{
    std::string digits{};

    auto firstDigit = std::find_if(std::begin(str), std::end(str), [](auto& ch) { return std::isdigit(ch); });
    auto lastDigit = std::find_if(std::rbegin(str), std::rend(str), [](auto& ch) { return std::isdigit(ch); });

    digits += *firstDigit;
    digits += *lastDigit;

    return std::stoi(digits);
}

uint32_t FindAndAddDigitsWithWords(const std::string& str, const std::vector<DigitPair>& words)
{
    std::string digits{};

    auto firstProperDigit = std::find_if(std::begin(str), std::end(str), [](auto& ch) { return std::isdigit(ch); });
    auto lastProperDigit = std::find_if(std::rbegin(str), std::rend(str), [](auto& ch) { return std::isdigit(ch); });

    int32_t distanceToFirst = std::distance(std::begin(str), firstProperDigit);
    int32_t distanceToLast = std::distance(std::rbegin(str), lastProperDigit);

    uint8_t firstNumber = *firstProperDigit;
    uint8_t secondNumber = *lastProperDigit;

    for (auto& wrd : words)
    {
        // Check front
        if (auto pos = str.find(wrd.word); pos != std::string::npos && pos < distanceToFirst)
        {
            distanceToFirst = pos;
            firstNumber = wrd.digit;
        }

        // Check back
        if (auto pos = str.rfind(wrd.word); pos != std::string::npos && (str.length() - pos - 1) < distanceToLast)
        {
            distanceToLast = (str.length() - pos - 1);
            secondNumber = wrd.digit;
        }
    }

    digits += firstNumber;
    digits += secondNumber;

    return std::stoi(digits);
}

uint32_t Part1(const std::vector<std::string>& lines)
{
    uint32_t total{};
    for (auto& line : lines)
    {
        total += FindAndAddDigits(line);
    }

    return total;
}

uint32_t Part2(const std::vector<std::string>& lines)
{
    std::vector<DigitPair> words =
    {
        {"one", '1'},
        {"two", '2'},
        {"three", '3'},
        {"four", '4'},
        {"five", '5'},
        {"six", '6'},
        {"seven", '7'},
        {"eight", '8'},
        {"nine", '9'}
    };

    uint32_t total{};
    for (auto& line : lines)
    {
        total += FindAndAddDigitsWithWords(line, words);
    }

    return total;
}

int main()
{
    std::vector<std::string> input{};
    ReadInput("./input.txt", input);
    std::cout << Part1(input) << std::endl;
    std::cout << Part2(input) << std::endl;
}