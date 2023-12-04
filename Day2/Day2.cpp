#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

constexpr uint32_t MAX_RED = 12;
constexpr uint32_t MAX_GREEN = 13;
constexpr uint32_t MAX_BLUE = 14;

const std::regex REGEX_GAME_ID{ "(\\d+)" };
const std::regex REGEX_GAME{ "(\\d+ [\\w ,]+;?)" };
const std::regex REGEX_BLUE{ "(\\d+) (blue)" };
const std::regex REGEX_RED{ "(\\d+) (red)" };
const std::regex REGEX_GREEN{ "(\\d+) (green)" };

static void ReadInput(const std::string& path, std::vector<std::string>& result)
{
    std::ifstream file{ path };
    std::string line{};

    while (std::getline(file, line))
    {
        result.push_back(line);
    }
}

uint32_t GetPowerOfGame(const std::string& game)
{
    uint32_t maxRed = 0;
    uint32_t maxGreen = 0;
    uint32_t maxBlue = 0;
    
    auto regStart = std::sregex_iterator(game.begin(), game.end(), REGEX_GAME);
    auto regEnd = std::sregex_iterator();

    for (auto it = regStart; it != regEnd; ++it)
    {
        std::string match = it->str();
        std::smatch colorMatch{};
        if (std::regex_search(match, colorMatch, REGEX_RED))
        {
            uint32_t red = std::stoi(colorMatch[1]);
            if (red > maxRed)
            {
                maxRed = red;
            }
        }

        if (std::regex_search(match, colorMatch, REGEX_GREEN))
        {
            uint32_t green = std::stoi(colorMatch[1]);
            if (green > maxGreen)
            {
                maxGreen = green;
            }
        }

        if (std::regex_search(match, colorMatch, REGEX_BLUE))
        {
            uint32_t blue = std::stoi(colorMatch[1]);
            if (blue > maxBlue)
            {
                maxBlue = blue;
            }
        }
    }

    return maxRed * maxGreen * maxBlue;
}

bool IsGamePossible(const std::string& game, uint32_t& gameNumber)
{
    std::smatch match{};

    std::regex_search(game, match, REGEX_GAME_ID);
    gameNumber = std::stoi(match[1]);

    auto regStart = std::sregex_iterator(game.begin(), game.end(), REGEX_GAME);
    auto regEnd = std::sregex_iterator();

    for (auto it = regStart; it != regEnd; ++it)
    {
        uint32_t totRed{};
        uint32_t totGreen{};
        uint32_t totBlue{};
        std::string match = it->str();
        std::smatch colorMatch{};

        if (std::regex_search(match, colorMatch, REGEX_RED))
        {
            totRed += std::stoi(colorMatch[1]);
        }

        if (std::regex_search(match, colorMatch, REGEX_GREEN))
        {
            totGreen += std::stoi(colorMatch[1]);
        }

        if (std::regex_search(match, colorMatch, REGEX_BLUE))
        {
            totBlue += std::stoi(colorMatch[1]);
        }

        if (totRed > MAX_RED || totGreen > MAX_GREEN || totBlue > MAX_BLUE)
        {
            return false;
        }
    }

    return true;
}

uint32_t Part2(const std::vector<std::string>& lines)
{
    uint32_t total{};
    for (auto& line : lines)
    {
        total += GetPowerOfGame(line);
    }

    return total;
}

uint32_t Part1(const std::vector<std::string>& lines)
{
    uint32_t total{};
    for (auto& line : lines)
    {
        uint32_t currentGame{};
        if (IsGamePossible(line, currentGame))
        {
            total += currentGame;
        }
    }

    return total;
}

int main()
{
    std::vector<std::string> input{};
    ReadInput("input.txt", input);
    std::cout << Part1(input) << std::endl;
    std::cout << Part2(input) << std::endl;
}