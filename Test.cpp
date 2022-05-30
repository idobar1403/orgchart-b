#include "doctest.h"
#include "OrgChart.hpp"
#include <iostream>
#include <string>
using namespace ariel;
TEST_CASE("sting chart")
{
    OrgChart<string> chart_int;
    CHECK_NOTHROW(chart_int.add_root("1"));
    CHECK_NOTHROW(chart_int.add_sub("1", "2"));
    CHECK_NOTHROW(chart_int.add_sub("1", "3"));
    CHECK_NOTHROW(chart_int.add_sub("2", "4"));
    CHECK_NOTHROW(chart_int.add_sub("2", "5"));
    CHECK_THROWS(chart_int.add_sub("0", "1"));
    CHECK_THROWS(chart_int.add_sub("6", "5"));
    CHECK_THROWS(chart_int.add_sub("-1", "2"));

    std::vector<string> level_order = {"1", "2", "3", "4", "5"};
    std::vector<string> reverse_order = {"4", "5", "2", "3", "1"};
    std::vector<string> pre_order = {"1", "2", "4", "5", "3"};
    size_t i = 0;
    for (auto it = chart_int.begin_level_order(); it != chart_int.end_level_order(); ++it)
    {
        CHECK_EQ((*it), level_order.at(i));
        i++;
    }
    i = 0;
    for (auto it = chart_int.begin_reverse_order(); it != chart_int.reverse_order(); ++it)
    {
        CHECK_EQ((*it), reverse_order.at(i));
        i++;
    }
    i = 0;
    for (auto it = chart_int.begin_preorder(); it != chart_int.end_preorder(); ++it)
    {
        CHECK_EQ((*it), pre_order.at(i));
        i++;
    }
}
TEST_CASE("bad chart")
{
    OrgChart<string> chart;
    CHECK_THROWS(chart.add_sub("root", "sub"));
    CHECK_NOTHROW(chart.add_root("hi"));
    CHECK_NOTHROW(chart.add_sub("hi", "there"));
    CHECK_NOTHROW(chart.add_sub("there", "was"));
    CHECK_THROWS(chart.add_sub("here", "was"));
    CHECK_NOTHROW(chart.add_sub("was", "I"));
    CHECK_THROWS(chart.add_root("\t"));
    CHECK_THROWS(chart.add_root("\n"));
    CHECK_THROWS(chart.add_root("\r"));
    CHECK_THROWS(chart.add_root(""));
    CHECK_THROWS(chart.add_root(" "));
    CHECK_THROWS(chart.add_sub("hi", ""));
    CHECK_THROWS(chart.add_sub("", "throw"));
    CHECK_THROWS(chart.add_sub("\t", "abc"));
    CHECK_THROWS(chart.add_sub("abc", "\t"));
    CHECK_THROWS(chart.add_sub("abc", "\n"));
    CHECK_THROWS(chart.add_sub("\n", "abc"));
    CHECK_THROWS(chart.add_sub("\r", "abc"));
    CHECK_THROWS(chart.add_sub("abc", "\r"));
    CHECK_THROWS(chart.add_sub("", "abc"));
    CHECK_THROWS(chart.add_sub("abc", ""));
    CHECK_THROWS(chart.add_sub(" ", "abc"));
    CHECK_THROWS(chart.add_sub("abc", " "));
}
TEST_CASE("empty chart")
{
    OrgChart<string> empty_chart;
    CHECK_THROWS(empty_chart.begin());
    CHECK_THROWS(empty_chart.end());
    CHECK_THROWS(empty_chart.begin_level_order());
    CHECK_THROWS(empty_chart.end_level_order());
    CHECK_THROWS(empty_chart.begin_preorder());
    CHECK_THROWS(empty_chart.end_preorder());
    CHECK_THROWS(empty_chart.begin_reverse_order());
    CHECK_THROWS(empty_chart.reverse_order());
}