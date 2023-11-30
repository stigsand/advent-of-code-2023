// https://adventofcode.com/2023/day/2

#include "common.hpp"
#include <algorithm>
#include <array>
#include <map>
#include <numeric>
#include <print>
#include <ranges>
#include <string_view>

using namespace aoc;
using namespace std::string_literals;
using namespace std::string_view_literals;

std::array const example_games {
    "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green"sv,
    "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue"sv,
    "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red"sv,
    "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red"sv,
    "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green"sv };

using cube_set = std::map<std::string, unsigned int, std::less<>>;

unsigned int lookup(cube_set const & cs, auto const & c)
{
    auto it = cs.find(c);
    return it != cs.end() ? it->second : 0U;
}

struct game {
    unsigned int id;
    std::vector<cube_set> observations;
};

auto parse_observation(std::ranges::view auto s)
{
    cube_set obs;
    for (auto o: s | std::views::split(',')) {
        std::cmatch matches;
        enforce(std::regex_search(o.begin(), o.end(), matches, std::regex(R"((\d+)\s(\w+))")));
        obs[matches[2]] = std::stoul(matches[1]);
    }
    return obs;
}

auto parse_observations(std::ranges::view auto s)
{
    return s | std::views::split(';') | std::views::transform([] (auto s) { return parse_observation(s); }) | std::ranges::to<std::vector>();
}

game parse_game(std::ranges::range auto const & line)
{
    std::cmatch matches;
    std::string_view const line_sv(line);
    enforce(std::regex_match(line_sv.begin(), line_sv.end(), matches, std::regex(R"(Game (\d+): (.*))")));

    unsigned int id = std::stoul(matches[1]);
    return { id, parse_observations(to_view(matches[2])) };
}

std::ranges::view auto parse_games(std::ranges::range auto const & lines)
{
    return lines | std::views::transform([] (auto const & s) { return parse_game(s); });
}

namespace part1 {

bool possible_observation(cube_set const & obs)
{
    return lookup(obs, "red") <= 12 && lookup(obs, "green") <= 13 && lookup(obs, "blue") <= 14;
}

bool possible(game const & g)
{
    return std::ranges::all_of(g.observations, possible_observation);
}

unsigned int possible_game_id_sum(std::ranges::view auto games)
{
    auto possible_game_ids = games | std::views::filter(possible) | std::views::transform(&game::id);
    return std::accumulate(possible_game_ids.begin(), possible_game_ids.end(), 0U);
}

void test()
{
    enforce(possible_game_id_sum(parse_games(example_games)) == 8);
}

}

namespace part2 {

cube_set minimum_set(game const & g)
{
    cube_set min;
    for (auto const & obs: g.observations)
        for (auto const & [c, n]: obs)
            min[c] = std::max(min[c], n);
    return min;
}

unsigned int power(cube_set const & cs)
{
    return lookup(cs, "red") * lookup(cs, "green") * lookup(cs, "blue");
}

unsigned int sum_power_minimum_set(std::ranges::view auto games)
{
    auto powers = games | std::views::transform(minimum_set) | std::views::transform(power);
    return std::accumulate(powers.begin(), powers.end(), 0U);
}

void test()
{
    enforce(sum_power_minimum_set(parse_games(example_games)) == 2286);
}

}

int main()
{
    part1::test();
    part2::test();

    auto const lines = read_lines("input/day-02");
    auto const games = parse_games(lines);

    std::println("Possible game ID sum: {}", part1::possible_game_id_sum(games));
    std::println("Sum of powers of minimum sets: {}", part2::sum_power_minimum_set(games));
}
