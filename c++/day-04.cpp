// https://adventofcode.com/2023/day/4

#include "common.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <map>
#include <numeric>
#include <print>
#include <ranges>
#include <regex>
#include <sstream>
#include <utility>

using namespace aoc;
using namespace std::string_view_literals;

std::array const example_cards {
    "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53"sv,
    "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19"sv,
    "Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1"sv,
    "Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83"sv,
    "Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36"sv,
    "Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11"sv };

struct card {
    std::size_t id;
    std::vector<unsigned int> winning;
    std::vector<unsigned int> actual;
};

card parse_card(std::ranges::range auto const & line)
{
    std::cmatch matches;
    std::string_view line_sv(line);
    enforce(std::regex_match(line_sv.begin(), line_sv.end(), matches, std::regex(R"(Card (.*): (.*)\|(.*))")));

    std::istringstream iss_winning(matches[2]);
    std::istringstream iss_actual(matches[3]);

    card c {
        .id = std::stoul(matches[1]),
        .winning = std::views::istream<unsigned int>(iss_winning) | std::ranges::to<std::vector>(),
        .actual = std::views::istream<unsigned int>(iss_actual) | std::ranges::to<std::vector>()
    };

    std::ranges::sort(c.winning);
    std::ranges::sort(c.actual);
    return c;
}

auto parse_cards(std::ranges::range auto const & lines)
{
    return lines
        | std::views::transform([] (auto const & s) { return parse_card(s); })
        | std::ranges::to<std::vector>();
}

namespace part1 {

unsigned int score_card(card const & c)
{
    std::vector<unsigned int> correct_nums;
    std::ranges::set_intersection(c.winning, c.actual, std::back_inserter(correct_nums));
    return correct_nums.empty() ? 0U : std::pow(2, correct_nums.size() - 1);
}

unsigned int sum_card_points(std::vector<card> const & cards)
{
    return std::accumulate(cards.begin(), cards.end(), 0U, [] (auto acc, auto const & c) {
        return acc + score_card(c);
    });
}

void test()
{
    enforce(sum_card_points(parse_cards(example_cards)) == 13);
}

}

namespace part2 {

struct card_info {
    std::size_t score;
    std::size_t processed = 0uz;
    std::size_t unprocessed = 0uz;
};

using card_pile = std::map<std::size_t, card_info>;

std::size_t score_card(card const & c)
{
    std::vector<unsigned int> correct_nums;
    std::ranges::set_intersection(c.winning, c.actual, std::back_inserter(correct_nums));
    return correct_nums.size();
}

card_pile build_pile(std::vector<card> const & initial_cards)
{
    return initial_cards
        | std::views::transform([] (auto const & c) {
            return std::pair { c.id, card_info { score_card(c), 0uz, 1uz } };
          })
        | std::ranges::to<card_pile>();
}

bool process(card_pile & pile)
{
    auto change = false;
    for (auto & [id, c]: pile) {
        if (c.unprocessed > 0) {
            change = true;
            for (auto i = id + 1; i <= id + c.score; ++i)
                if (auto it = pile.find(i); it != pile.end())
                    it->second.unprocessed += c.unprocessed;
            c.processed += std::exchange(c.unprocessed, 0uz);
        }
    }
    return change;
}

std::size_t process_cards(std::vector<card> const & initial_cards)
{
    auto pile = build_pile(initial_cards);
    while (process(pile)) {}
    return std::accumulate(pile.begin(), pile.end(), 0uz, [] (auto acc, auto const & c) {
        return acc + c.second.processed;
    });
}
void test()
{
    enforce(process_cards(parse_cards(example_cards)) == 30uz);
}

}

int main()
{
    part1::test();
    part2::test();

    auto const lines = read_lines("input/day-04");
    auto const cards = parse_cards(lines);

    std::println("Card point sum: {}", part1::sum_card_points(cards));
    std::println("Card count: {}", part2::process_cards(cards));
}
