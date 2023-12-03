// https://adventofcode.com/2023/day/3

#include "common.hpp"
#include "generator.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <numeric>
#include <optional>
#include <print>
#include <ranges>
#include <utility>

using namespace aoc;
using namespace std::string_literals;

std::vector const example_schema {
    "467..114.."s,
    "...*......"s,
    "..35..633."s,
    "......#..."s,
    "617*......"s,
    ".....+.58."s,
    "..592....."s,
    "......755."s,
    "...$.*...."s,
    ".664.598.."s };

struct pos {
    std::size_t x;
    std::size_t y;
};

struct engine_schema {
    std::vector<std::string> lines;
    pos dims;
};

engine_schema create_schema(std::vector<std::string> lines)
{
    auto dims = pos { !lines.empty() ? lines[0].size() : 0uz, lines.size() };
    return { std::move(lines), dims };
}

struct offset {
    int x;
    int y;
};

std::optional<pos> add(pos p, offset o, pos dims)
{
    auto x = static_cast<int>(p.x) + o.x;
    auto y = static_cast<int>(p.y) + o.y;
    if (x < 0 || x >= static_cast<int>(dims.x) || y < 0 || y >= static_cast<int>(dims.y))
        return {};
    return pos { static_cast<std::size_t>(x), static_cast<std::size_t>(y) };
}

struct range {
    pos start_pos;
    std::size_t n;
};

generator<range> number_ranges(engine_schema const & schema)
{
    for (auto y = 0uz; y < schema.dims.y; ++y)
        for (auto x = 0uz; x < schema.dims.x; ++x)
            if (std::isdigit(schema.lines[y][x])) {
                auto x_max = x;
                while (x_max < schema.dims.x - 1 && std::isdigit(schema.lines[y][x_max + 1]))
                    ++x_max;
                co_yield range { { x, y }, x_max - x + 1 };
                x = x_max;
            }
}

generator<std::optional<pos>> surrounding_candidate_positions(range r, pos dims)
{
    co_yield add(r.start_pos, { -1, -1 }, dims);
    co_yield add(r.start_pos, { -1,  0 }, dims);
    co_yield add(r.start_pos, { -1, +1 }, dims);
    for (auto i = 0uz; i < r.n; ++i) {
        pos p { r.start_pos.x + i, r.start_pos.y };
        co_yield add(p, { 0, -1 }, dims);
        co_yield add(p, { 0, +1 }, dims);
    }
    pos beyond_last { r.start_pos.x + r.n, r.start_pos.y };
    co_yield add(beyond_last, { 0, -1 }, dims);
    co_yield add(beyond_last, { 0,  0 }, dims);
    co_yield add(beyond_last, { 0, +1 }, dims);
}

std::ranges::view auto surrounding_positions(range r, pos dims)
{
    return surrounding_candidate_positions(r, dims)
        | std::views::filter([] (auto p) { return p.has_value(); })
        | std::views::transform([] (auto p) { return *p; });
}

unsigned int part_number(range r, engine_schema const & schema)
{
    auto & [p, n] = r;
    return std::stoul(schema.lines[p.y].substr(p.x, n));
}

namespace part1 {

bool is_symbol(pos p, engine_schema const & schema)
{
    auto c = schema.lines[p.y][p.x];
    return !(c == '.' || std::isdigit(c));
}

bool has_adjacent_symbol(range r, engine_schema const & schema)
{
    return std::ranges::any_of(
        surrounding_positions(r, schema.dims),
        [&] (auto p) { return is_symbol(p, schema); });
}

unsigned int part_number_sum(engine_schema const & schema)
{
    auto part_numbers = number_ranges(schema)
        | std::views::filter([&] (auto r) { return has_adjacent_symbol(r, schema); })
        | std::views::transform([&] (auto r) { return part_number(r, schema); })
        | std::ranges::to<std::vector>();  // because std::accumulate doesn't work with ranges
    return std::accumulate(part_numbers.begin(), part_numbers.end(), 0U);
}

void test()
{
    enforce(part_number_sum(create_schema(example_schema)) == 4361);
}

}

namespace part2 {

bool is_inside(pos p, range r)
{
    return p.y == r.start_pos.y && p.x >= r.start_pos.x && p.x < r.start_pos.x + r.n;
}

std::vector<range> adjacent_number_ranges(pos p, engine_schema const & schema, std::vector<range> const & nums)
{
    auto positions = surrounding_positions({ p, 1 }, schema.dims) | std::ranges::to<std::vector>();
    return nums
        | std::views::filter([&] (auto r) { return std::ranges::any_of(positions, [&] (auto p) { return is_inside(p, r); }); })
        | std::ranges::to<std::vector>();
}

unsigned int calculate_gear_ratio(std::vector<range> const & adj_nums, engine_schema const & schema)
{
    return part_number(adj_nums[0], schema) * part_number(adj_nums[1], schema);
}

generator<pos> star_symbols(engine_schema const & schema)
{
    for (auto y = 0uz; y < schema.dims.y; ++y)
        for (auto x = 0uz; x < schema.dims.x; ++x)
            if (auto c = schema.lines[y][x]; c == '*')
                co_yield pos { x, y };
}

unsigned int gear_ratio_sum(engine_schema const & schema)
{
    auto nums = number_ranges(schema) | std::ranges::to<std::vector>();
    auto gear_ratios = star_symbols(schema)
        | std::views::transform([&] (auto p) { return adjacent_number_ranges(p, schema, nums); })
        | std::views::filter([] (auto const & adj_nums) { return adj_nums.size() == 2; })
        | std::views::transform([&] (auto const & adj_nums) { return calculate_gear_ratio(adj_nums, schema); })
        | std::ranges::to<std::vector>();  // because std::accumulate doesn't work with ranges
    return std::accumulate(gear_ratios.begin(), gear_ratios.end(), 0U);
}

void test()
{
    enforce(gear_ratio_sum(create_schema(example_schema)) == 467835);
}

}

int main()
{
    part1::test();
    part2::test();

    auto const schema = create_schema(read_lines("input/day-03"));

    std::println("Part number sum: {}", part1::part_number_sum(schema));
    std::println("Gear ratio sum: {}", part2::gear_ratio_sum(schema));
}
