#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>

struct Race
{
    uint64_t time{};
    uint64_t distance{};
};

void ReadInput(const std::string& path, std::vector<Race>& result)
{
    std::ifstream file{ path };
    std::string line{};

    std::vector<uint64_t> times{};
    std::vector<uint64_t> distances{};

    while (std::getline(file, line))
    {
        auto& vecRef = line.starts_with("Time") ? times : distances;
        std::regex numbReg{ "(\\d+)" };
        std::sregex_iterator start{ line.begin(), line.end(), numbReg };
        std::sregex_iterator end = std::sregex_iterator();
        for (auto it = start; it != end; ++it)
        {
            vecRef.push_back(std::stoull(it->str()));
        }
    }

    for (size_t i = 0; i < times.size(); ++i)
    {
        result.push_back({ times[i], distances[i] });
    }
}

Race ReadInputP2(const std::string& path)
{
    std::ifstream file{ path };
    std::string line{};
    std::string totalTime;
    std::string totalDist;

    while (std::getline(file, line))
    {
        auto& strRef = line.starts_with("Time") ? totalTime : totalDist;
        std::regex numbReg{ "(\\d+)" };
        std::sregex_iterator start{ line.begin(), line.end(), numbReg };
        std::sregex_iterator end = std::sregex_iterator();
        for (auto it = start; it != end; ++it)
        {
            strRef += it->str();
        }
    }

    return { std::stoull(totalTime), std::stoull(totalDist) };
}

uint64_t GetNumberOfWaysForRace(const Race& race)
{
    uint64_t tot{};
    for (uint64_t i = 1; i < race.time; ++i)
    {
        uint64_t dist = i * (race.time - i);
        if (dist > race.distance)
        {
            tot++;
        }
    }

    return tot;
}

uint64_t Part1(const std::vector<Race>& races)
{
    uint64_t total{ 1 };
    for (auto& race : races)
    {
        total *= GetNumberOfWaysForRace(race);
    }

    return total;
}

int main()
{
    std::vector<Race> input{};
    ReadInput("./input.txt", input);
    std::cout << "Part 1: " << Part1(input) << std::endl;
    std::cout << "Part 2: " << GetNumberOfWaysForRace(ReadInputP2("./input.txt")) << std::endl;
}