#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <numeric>

// This behave well enough as a grid
using grid_t = std::vector<std::string>;
constexpr uint32_t MAX_OVERLAPS = 2;

struct PartNumberCoord
{
    int32_t i{};
    int32_t j{};
    int32_t length{};
    int32_t value{};

    PartNumberCoord(int32_t i, int32_t j, int32_t length, int32_t value) : i{ i }, j{ j }, length{ length }, value{ value } {}
    bool operator==(const PartNumberCoord&) const = default;
};

static void ReadInput(const std::string& path, grid_t& result)
{
    std::ifstream file{ path };
    std::string line{};

    while (std::getline(file, line))
    {
        result.push_back(line);
    }
}

bool IsGear(const std::vector<PartNumberCoord>& partNumbers, int32_t posI, int32_t posJ, uint32_t& gearPower)
{
    uint32_t numOverlaps{};
    gearPower = 1;

    for (auto& coord : partNumbers)
    {
        if ((posI == coord.i || posI == (coord.i - 1) || posI == (coord.i + 1)) &&
            (posJ >= (coord.j - 1) && posJ <= (coord.j + coord.length)))
        {
            numOverlaps++;
            gearPower *= coord.value;
        }

        if (numOverlaps > MAX_OVERLAPS)
        {
            return false;
        }
    }

    return numOverlaps == MAX_OVERLAPS;
}

void GetPartNumbers(const grid_t& schema, std::vector<PartNumberCoord>& parts)
{
    std::vector<PartNumberCoord> allNumbers{};
    const std::regex numberRegex{ "(\\d+)" };
    for (int32_t i = 0; i < schema.size(); ++i)
    {
        const auto& line = schema[i];
        auto words_begin = std::sregex_iterator(line.begin(), line.end(), numberRegex);
        auto words_end = std::sregex_iterator();
        for (auto it = words_begin; it != words_end; ++it)
        {
            std::smatch match = *it;
            std::string match_str = match.str();
            allNumbers.push_back(PartNumberCoord{ i, static_cast<int32_t>(match.position()), static_cast<int32_t>(match_str.size()), std::stoi(match_str)});
        }
    }

    for (auto& coord : allNumbers)
    {
        if ((coord.j > 0 && schema[coord.i][coord.j - 1] != '.') ||
            (coord.j + coord.length < schema[coord.i].size() && schema[coord.i][coord.j + coord.length] != '.'))
        {
            parts.push_back(coord);
            continue;
        }

        if (coord.i > 0)
        {
            const auto& str = schema[coord.i - 1];
            auto substr = str.substr(std::clamp(coord.j - 1, 0, static_cast<int32_t>(str.size())), coord.length + 2);
            auto it = std::find_if(substr.begin(), substr.end(), [](const auto& ch) { return !std::isdigit(ch) && ch != '.'; });
            if (it != substr.end())
            {
                parts.push_back(coord);
                continue;
            }
        }

        if (coord.i < schema.size() - 1)
        {
            const auto& str = schema[coord.i + 1];
            auto substr = str.substr(std::clamp(coord.j - 1, 0, static_cast<int32_t>(str.size())), coord.length + 2);
            auto it = std::find_if(substr.begin(), substr.end(), [](const auto& ch) { return !std::isdigit(ch) && ch != '.'; });
            if (it != substr.end())
            {
                parts.push_back(coord);
                continue;
            }
        }
    }
}

uint32_t Part2(const grid_t& schema)
{
    std::vector<PartNumberCoord> partNumbers{};
    GetPartNumbers(schema, partNumbers);

    uint64_t totalGearRatio{};

    for (int32_t i = 0; i < schema.size(); ++i)
    {
        const auto& str = schema[i];
        size_t pos = str.find('*');
        while (pos != std::string::npos)
        {
            uint32_t gearRatio{};
            if (IsGear(partNumbers, i, pos, gearRatio))
            {
                totalGearRatio += gearRatio;
            }
            pos = str.find('*', pos + 1);
        }
    }

    return totalGearRatio;
}

uint32_t Part1(const grid_t& schema)
{
    std::vector<PartNumberCoord> partNumbers{};
    GetPartNumbers(schema, partNumbers);

    auto accumulateLambda = [](const int32_t& a, const PartNumberCoord& b)
    {
        return a + b.value;
    };

    return std::accumulate(partNumbers.begin(), partNumbers.end(), 0, accumulateLambda);
}

int main()
{
    std::vector<std::string> input{};
    ReadInput("./input.txt", input);
    std::cout << Part1(input) << std::endl;
    std::cout << Part2(input) << std::endl;
}