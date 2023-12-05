#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ranges>
#include <sstream>
#include <algorithm>

struct SeedRange
{
    uint64_t start{};
    uint64_t range{};
};

class SeedMap
{
public:
    SeedMap(uint64_t dStart, uint64_t sStart, uint64_t r) : destinationStart{ dStart }, sourceStart{ sStart }, range{ r } {}

    bool TryGetMapping(uint64_t val, uint64_t& result) const
    {
        if (val < sourceStart || val >= sourceStart + range)
        {
            return false;
        }

        int64_t diff = val - sourceStart;
        result = destinationStart + diff;
        return true;
    }

    void GetMappingForRange(std::vector<SeedRange>& ranges, std::vector<SeedRange>& result) const
    {
        std::vector<SeedRange> leftovers{};

        for (auto& r : ranges)
        {
            uint64_t rangeStart = r.start;
            uint64_t rangeEnd = r.start + r.range - 1;
            uint64_t mapStart = sourceStart;
            uint64_t mapEnd = sourceStart + range - 1;

            // No overlaps
            if (rangeStart > mapEnd || rangeEnd < mapStart)
            {
                leftovers.push_back(r);
                continue;
            }

            if (rangeStart < mapStart)
            {
                leftovers.push_back({ rangeStart, mapStart - rangeStart });
                rangeStart = mapStart;
            }

            if (rangeEnd > mapEnd)
            {
                leftovers.push_back({ mapEnd + 1, rangeEnd - mapEnd });
                rangeEnd = mapEnd;
            }

            result.push_back({ destinationStart + (rangeStart - sourceStart), (rangeEnd - rangeStart + 1)});
        }

        ranges = leftovers;
    }

private:
    uint64_t destinationStart{};
    uint64_t sourceStart{};
    uint64_t range{};
};

class SeedMapGroup
{
public:
    void PushMap(const SeedMap& map) { maps.push_back(map); }

    void GetMapping(uint64_t val, uint64_t& result) const
    {
        for (auto& i : maps)
        {
            if (i.TryGetMapping(val, result))
            {
                return;
            }
        }

        result = val;
    }

    void GetMappingForRange(const std::vector<SeedRange>& ranges, std::vector<SeedRange>& result) const
    {
        std::vector<SeedRange> localRanges = ranges;
        result.clear();

        for (auto& sm : maps)
        {
            sm.GetMappingForRange(localRanges, result);
        }

        if (!localRanges.empty())
        {
            result.insert(result.end(), localRanges.begin(), localRanges.end());
        }
    }

private:
    std::vector<SeedMap> maps{};
};

using MapsContainer = std::vector<SeedMapGroup>;

void ReadInput(const std::string& path, std::vector<std::string>& result)
{
    std::ifstream file{ path };
    std::string line{};

    while (std::getline(file, line))
    {
        result.push_back(line);
    }
}

SeedMapGroup GenerateMap(const std::vector<std::string>& lines, uint64_t offset)
{
    SeedMapGroup result{};

    for (size_t i = offset; i < lines.size(); ++i)
    {
        if (!std::isdigit(lines[i][0]))
        {
            break;
        }

        std::stringstream stream{ lines[i] };
        uint64_t values[3]{};
        stream >> values[0];
        stream >> values[1];
        stream >> values[2];
        result.PushMap(SeedMap(values[0], values[1], values[2]));
    }

    return result;
}

void GenerateMapContainer(const std::vector<std::string>& lines, MapsContainer& container)
{
    uint64_t seedToSoilOffset =
        std::distance(lines.begin(), std::find_if(lines.begin(), lines.end(), [](const auto& s) { return s.find("seed-to-soil") != std::string::npos; }));
    container.push_back(GenerateMap(lines, seedToSoilOffset + 1));

    uint64_t soilToFertOffset =
        std::distance(lines.begin(), std::find_if(lines.begin(), lines.end(), [](const auto& s) { return s.find("soil-to-fertilizer") != std::string::npos; }));
    container.push_back(GenerateMap(lines, soilToFertOffset + 1));

    uint64_t fertToWaterOffset =
        std::distance(lines.begin(), std::find_if(lines.begin(), lines.end(), [](const auto& s) { return s.find("fertilizer-to-water") != std::string::npos; }));
    container.push_back(GenerateMap(lines, fertToWaterOffset + 1));

    uint64_t waterToLightOffset =
        std::distance(lines.begin(), std::find_if(lines.begin(), lines.end(), [](const auto& s) { return s.find("water-to-light") != std::string::npos; }));
    container.push_back(GenerateMap(lines, waterToLightOffset + 1));

    uint64_t lightToTempOffset =
        std::distance(lines.begin(), std::find_if(lines.begin(), lines.end(), [](const auto& s) { return s.find("light-to-temperature") != std::string::npos; }));
    container.push_back(GenerateMap(lines, lightToTempOffset + 1));

    uint64_t tempToHumOffset =
        std::distance(lines.begin(), std::find_if(lines.begin(), lines.end(), [](const auto& s) { return s.find("temperature-to-humidity") != std::string::npos; }));
    container.push_back(GenerateMap(lines, tempToHumOffset + 1));

    uint64_t humToLocationOffset =
        std::distance(lines.begin(), std::find_if(lines.begin(), lines.end(), [](const auto& s) { return s.find("humidity-to-location") != std::string::npos; }));
    container.push_back(GenerateMap(lines, humToLocationOffset + 1));
}

uint64_t GetMinValueForRanges(const std::vector<SeedRange>& ranges)
{
    auto it = std::min_element(ranges.begin(), ranges.end(), [](const auto& lhs, const auto& rhs) { return lhs.start < rhs.start; });

    return it->start;
}

void GetInitialSeeds(const std::string& line, std::vector<uint64_t>& seeds)
{
    std::string substr = line.substr(line.find(':') + 2);

    std::stringstream stream{ substr };
    uint64_t temp{};
    while (stream >> temp)
    {
        seeds.push_back(temp);
    }
}

void GetInitialRanges(const std::string& line, std::vector<SeedRange>& ranges)
{
    std::string substr = line.substr(line.find(':') + 2);

    std::stringstream stream{ substr };
    uint64_t temp{};
    while (stream >> temp)
    {
        uint64_t next{};
        stream >> next;
        ranges.push_back({ temp, next });
    }
}

uint64_t Part2(const std::vector<std::string>& lines, const MapsContainer& container)
{
    std::vector<SeedRange> ranges{};
    GetInitialRanges(lines[0], ranges);

    std::vector<SeedRange> res = ranges;

    for (auto& group : container)
    {
        group.GetMappingForRange(res, res);
    }

    return GetMinValueForRanges(res);
}


uint64_t Part1(const std::vector<std::string>& lines, const MapsContainer& container)
{
    std::vector<uint64_t> seeds{};
    GetInitialSeeds(lines[0], seeds);
    uint64_t lowestLocation{ UINT64_MAX };

    for (auto& m : seeds)
    {
        uint64_t res = m;
        for (auto& group : container)
        {
            group.GetMapping(res, res);
        }

        if (res < lowestLocation)
        {
            lowestLocation = res;
        }
    }

    return lowestLocation;
}

int main()
{
    std::vector<std::string> input{};
    MapsContainer container{};
    ReadInput("./input.txt", input);
    GenerateMapContainer(input, container);
    std::cout << Part1(input, container) << std::endl;
    std::cout << Part2(input, container) << std::endl;
}