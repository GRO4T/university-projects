add_executable(unit-tests src/catch_main.cc src/unit_tests.cc)

add_executable(graph-test
    src/graph_test.cc)

add_executable(naive-test
    src/naive_test.cc)

add_executable(bfs-test
    src/bfs_test.cc)

add_executable(astar-test
    src/astar.cc)

target_link_libraries(graph-test cmyk_sorter)
target_link_libraries(naive-test cmyk_sorter)
target_link_libraries(bfs-test cmyk_sorter)
target_link_libraries(unit-tests cmyk_sorter)
target_link_libraries(astar-test cmyk_sorter)
