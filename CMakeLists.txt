cmake_minimum_required(VERSION 3.5.0)
project(Leviathan VERSION 0.1)

file (GLOB_RECURSE Leviathan_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file (GLOB_RECURSE Leviathan_HEADERS CONFIGURE_DEPENDS "src/*.h")

add_executable(Leviathan.out $Leviathan_SOURCES)
