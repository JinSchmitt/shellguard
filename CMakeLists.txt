cmake_minimum_required(VERSION 3.20)
project(shellguard)
set(CMAKE_CXX_STANDARD 17)

add_executable(shellguard
        src/main.cpp
        src/corrector.cpp
        src/danger.cpp
        src/trie.cpp
)
install(TARGETS shellguard DESTINATION /usr/local/bin)
