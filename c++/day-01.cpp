// https://adventofcode.com/2023/day/1

#include "common.hpp"
#include <array>
#include <map>
#include <numeric>
#include <print>
#include <ranges>
#include <string_view>

using namespace aoc;
using namespace std::string_literals;
using namespace std::string_view_literals;

namespace part1 {

char const digits[] = "0123456789";

unsigned int calibration_value(std::string_view line)
{
    auto first_idx = line.find_first_of(digits);
    auto last_idx = line.find_last_of(digits);
    enforce(first_idx != std::string_view::npos && last_idx != std::string_view::npos);
    return std::stoul(std::string(1, line[first_idx]) + line[last_idx]);
}

unsigned int calibration_value_sum(std::ranges::range auto const & lines)
{
    return std::accumulate(lines.begin(), lines.end(), 0U, [] (auto acc, auto const & line) {
        return acc + calibration_value(line);
    });
}

void test()
{
    std::array const lines {
        "1abc2"sv,
        "pqr3stu8vwx"sv,
        "a1b2c3d4e5f"sv,
        "treb7uchet"sv };
    enforce(calibration_value_sum(lines) == 142);
}

}

namespace part2 {

std::map<std::string_view, unsigned int> const digits {
    { "0"sv, 0 }, { "1"sv, 1 }, { "2"sv, 2 }, { "3"sv, 3 }, { "4"sv, 4 },
    { "5"sv, 5 }, { "6"sv, 6 }, { "7"sv, 7 }, { "8"sv, 8 }, { "9"sv, 9 },
    { "zero"sv, 0 }, { "one"sv, 1 }, { "two"sv, 2 }, { "three"sv, 3 }, { "four"sv, 4 },
    { "five"sv, 5 }, { "six"sv, 6 }, { "seven"sv, 7 }, { "eight"sv, 8 }, { "nine"sv, 9} };

unsigned int calibration_value(std::string_view line)
{
    auto first_digit = [&] {
        for (auto line_view = line; !line_view.empty(); line_view.remove_prefix(1))
            for (auto const & [word, digit] : digits)
                if (line_view.starts_with(word))
                    return digit;
        fail();
    }();
    auto last_digit = [&] {
        for (auto line_view = line; !line_view.empty(); line_view.remove_suffix(1))
            for (auto const & [word, digit] : digits)
                if (line_view.ends_with(word))
                    return digit;
        fail();
    }();
    return 10 * first_digit + last_digit;
}

unsigned int calibration_value_sum(std::ranges::range auto const & lines)
{
    return std::accumulate(lines.begin(), lines.end(), 0U, [] (auto acc, auto const & line) {
        return acc + calibration_value(line);
    });
}

void test()
{
    std::array const lines {
        "two1nine"sv,
        "eightwothree"sv,
        "abcone2threexyz"sv,
        "xtwone3four"sv,
        "4nineeightseven2"sv,
        "zoneight234"sv,
        "7pqrstsixteen"sv };
    enforce(calibration_value_sum(lines) == 281);
}

}

int main()
{
    part1::test();
    part2::test();

    auto const lines = read_lines("input/day-01");

    std::println("Sum of calibration values - part one: {}", part1::calibration_value_sum(lines));
    std::println("Sum of calibration values - part two: {}", part2::calibration_value_sum(lines));
}
