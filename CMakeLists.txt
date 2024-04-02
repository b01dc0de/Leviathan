cmake_minimum_required(VERSION 3.5.0)
project(Leviathan VERSION 0.1)

set(SRC_DIR src)

set(SRC_FILES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/Common.cpp
    ${SRC_DIR}/Sudoku.cpp
    ${SRC_DIR}/Pseudoku.cpp
)

add_executable(Leviathan ${SRC_FILES})

# set_source_files_properties(<files> ...
#                            [DIRECTORY <dirs> ...]
#                            [TARGET_DIRECTORY <targets> ...]
#                            PROPERTIES <prop1> <value1>
#                            [<prop2> <value2>] ...)