#include "algorithm/algorithm.hpp"
#include "algorithm/generator.hpp"
#include "catch_amalgamated.hpp"

#include <functional>

std::string n_cmyk(int n) {
    std::string cmyk = "";
    for (int i = 0; i < n; ++i) {
        cmyk += "CMYK";
    }
    return cmyk;
}

void basic_test(std::function<PropertyParser(const std::string&)> algorithm) {
    std::cout << "test: K" << std::endl;
    REQUIRE(algorithm("K").get_sorted_string() == "K");
    std::cout << "test: CMYK" << std::endl;
    REQUIRE(algorithm("CMYK").get_sorted_string() == "CMYK");
    std::cout << "test: KKKKKKKKKK" << std::endl;
    REQUIRE(algorithm("KKKKKKKKKK").get_sorted_string() == "KKKKKKKKKK");
    std::cout << "test: CMMYK" << std::endl;
    REQUIRE(algorithm("CMMYK").get_sorted_string() == "CMMYK"); // this is not sortable
    std::cout << "test: CMMMYK" << std::endl;
    REQUIRE(algorithm("CMMMYK").get_sorted_string() == "CMYKMM"); // but this is
    std::cout << "test: random 8" << std::endl;
    REQUIRE(algorithm(Generator::random_permutation({2, 2, 2, 2})).get_sorted_string() == "CMYKCMYK");
    std::cout << "test: random 8 + M" << std::endl;
    REQUIRE(algorithm(Generator::random_permutation({2, 2, 2, 2}) + "M").get_sorted_string() == "CMYKCMYKM");
}

void mid_level_test(std::function<PropertyParser(const std::string&)> algorithm) {
    std::cout << "test: random 12" << std::endl;
    REQUIRE(algorithm(Generator::random_permutation({3, 3, 3, 3})).get_sorted_string() == "CMYKCMYKCMYK");
    std::cout << "test: random 16" << std::endl;
    REQUIRE(algorithm(Generator::random_permutation({4, 4, 4, 4})).get_sorted_string() == "CMYKCMYKCMYKCMYK");
    std::cout << "test: random 20" << std::endl;
    REQUIRE(algorithm(Generator::random_permutation({5, 5, 5, 5})).get_sorted_string() == "CMYKCMYKCMYKCMYKCMYK");
}

void extended_test(std::function<PropertyParser(const std::string&)> algorithm) {
    std::cout << "test: random 12" << std::endl;
    REQUIRE(algorithm(Generator::random_permutation({3, 3, 3, 3})).get_sorted_string() == "CMYKCMYKCMYK");
    std::cout << "test: random 36" << std::endl;
    REQUIRE(algorithm(Generator::random_permutation({9, 9, 9, 9})).get_sorted_string() == n_cmyk(9));
    std::cout << "test: C + random 36" << std::endl;
    REQUIRE(algorithm("C" + Generator::random_permutation({9, 9, 9, 9})).get_sorted_string() == n_cmyk(9) + "C");
    std::cout << "test: random 99" << std::endl;
    REQUIRE(algorithm(Generator::random_permutation({25, 25, 25, 25})).get_sorted_string().find(n_cmyk(24)) == 0);
}

TEST_CASE("Test full BFS", "[full_bfs]") {
    std::cout << "Start of full BFS test" << std::endl;
    basic_test(full_bfs_unit_tests_wrapper());
}

TEST_CASE("Test partial BFS", "[partial_bfs]") {
    std::cout << "Start of partial BFS test" << std::endl;
    std::cout << "front = false" << std::endl;
    std::cout << "seq len = 1" << std::endl;
    basic_test(partial_bfs_unit_tests_wrapper(false, 1));
    extended_test(partial_bfs_unit_tests_wrapper(false, 1));
    std::cout << "seq len = 2" << std::endl;
    basic_test(partial_bfs_unit_tests_wrapper(false, 2));
    extended_test(partial_bfs_unit_tests_wrapper(false, 2));
    std::cout << "front = true" << std::endl;
    std::cout << "seq len = 1" << std::endl;
    basic_test(partial_bfs_unit_tests_wrapper(true, 1));
    mid_level_test(partial_bfs_unit_tests_wrapper(true, 1));
    std::cout << "seq len = 2" << std::endl;
    basic_test(partial_bfs_unit_tests_wrapper(true, 2));
    //mid_level_test(partial_bfs(true, 2)); this takes about a minute
}


TEST_CASE("Test naive", "[naive]") {
    std::cout << "Start of naive test" << std::endl;
    for (std::size_t i=0; i < 3; ++i) {
        std::cout << "testing optimization " << i << "\n";
        basic_test(naive_unit_tests_wrapper(i));
        extended_test(naive_unit_tests_wrapper(i));
    }
}


TEST_CASE("Test A*", "[astar]") {
    std::cout << "Start of astar test" << std::endl;
    for (std::size_t i=0; i < 3; ++i) {
        std::cout << "testing " << i << " heuristic function\n";
        basic_test(astar_unit_tests_wrapper(i));
    }
}
