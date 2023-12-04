#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <unordered_map>

static void ReadInput(const std::string& path, std::vector<std::string>& result)
{
    std::ifstream file{ path };
    std::string line{};

    while (std::getline(file, line))
    {
        result.push_back(line);
    }
}

uint32_t GetCardNumber(const std::string& line)
{
    std::regex numRegex("(\\d+)");
    std::smatch numMatch{};
    std::regex_search(line, numMatch, numRegex);
    return std::stoi(numMatch.str());
}

void GetMyNumbers(const std::string& line, std::vector<int32_t>& numbers)
{
    size_t columnIndex = line.find(':');
    size_t barIndex = line.find('|');
    size_t length = barIndex - columnIndex;

    auto substr = line.substr(columnIndex + 1, length);

    std::regex numRegex("(\\d+)");
    auto numStart = std::sregex_iterator(substr.begin(), substr.end(), numRegex);
    auto numEnd = std::sregex_iterator();

    for (auto it = numStart; it != numEnd; ++it)
    {
        numbers.push_back(std::stoi(it->str()));
    }
}

void GetWinningNumbers(const std::string& line, std::vector<int32_t>& numbers)
{
    size_t barIndex = line.find('|');

    auto substr = line.substr(barIndex);

    std::regex numRegex("(\\d+)");
    auto numStart = std::sregex_iterator(substr.begin(), substr.end(), numRegex);
    auto numEnd = std::sregex_iterator();

    for (auto it = numStart; it != numEnd; ++it)
    {
        numbers.push_back(std::stoi(it->str()));
    }
}

uint32_t Part2(const std::vector<std::string>& lines)
{
    std::unordered_map<size_t, uint32_t> copiesDic{};
    uint32_t total{};

    for (size_t i = 0; i < lines.size(); ++i)
    {
        copiesDic[i]++;

        const auto& line = lines[i];
        std::vector<int32_t> myNums{};
        std::vector<int32_t> winningNums{};

        GetMyNumbers(line, myNums);
        GetWinningNumbers(line, winningNums);
        uint32_t cardNumber = GetCardNumber(line);

        std::sort(myNums.begin(), myNums.end());
        std::sort(winningNums.begin(), winningNums.end());

        std::vector<int32_t> intersections{};
        std::set_intersection(myNums.begin(), myNums.end(), winningNums.begin(), winningNums.end(), std::back_inserter(intersections));

        size_t numIntersections = intersections.size();

        for (size_t j = 0; j < numIntersections; ++j)
        {
            copiesDic[cardNumber + j] += copiesDic[i];
        }

        total += (copiesDic[i]);
    }

    return total;
}

uint32_t Part1(const std::vector<std::string>& lines)
{
    uint32_t total{};

    for (auto& line : lines)
    {
        std::vector<int32_t> myNums{};
        std::vector<int32_t> winningNums{};

        GetMyNumbers(line, myNums);
        GetWinningNumbers(line, winningNums);

        std::sort(myNums.begin(), myNums.end());
        std::sort(winningNums.begin(), winningNums.end());

        std::vector<int32_t> intersections{};
        std::set_intersection(myNums.begin(), myNums.end(), winningNums.begin(), winningNums.end(), std::back_inserter(intersections));

        uint32_t score = intersections.empty() ? 0 : (1 << (intersections.size() - 1));
        total += score;
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