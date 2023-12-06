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

// Returns the race for part 2, fill the vector with the races for part 1
Race ReadInput(const std::string& path, std::vector<Race>& races)
{
    std::ifstream file{ path };
    std::string line{};

    std::vector<uint64_t> times{};
    std::vector<uint64_t> distances{};
    std::string totalTime;
    std::string totalDist;

    while (std::getline(file, line))
    {
        auto& vecRef = line.starts_with("Time") ? times : distances;
        auto& strRef = line.starts_with("Time") ? totalTime : totalDist;

        std::regex numbReg{ "(\\d+)" };
        std::sregex_iterator start{ line.begin(), line.end(), numbReg };
        std::sregex_iterator end = std::sregex_iterator();
        for (auto it = start; it != end; ++it)
        {
            vecRef.push_back(std::stoull(it->str()));
            strRef += it->str();
        }
    }

    for (size_t i = 0; i < times.size(); ++i)
    {
        races.push_back({ times[i], distances[i] });
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
    std::vector<Race> racesP1{};
    Race raceP2 = ReadInput("./input.txt", racesP1);
    std::cout << "Part 1: " << Part1(racesP1) << std::endl;
    std::cout << "Part 2: " << GetNumberOfWaysForRace(raceP2) << std::endl;
}