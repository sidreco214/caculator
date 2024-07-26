add_executable(test_core
    test_core.cpp
    core/calc_tree.hpp
    core/calc_tree.cpp
)

add_executable(caculator
    main.cpp
    core/calc_tree.hpp
    core/calc_tree.cpp
)