cmake_minimum_required(VERSION 3.11)
project(_BookStore)

set(CMAKE_CXX_STANDARD 17)

add_executable(code
        src/bookstore.cpp
        src/urll/urll.cpp
        src/book/book.cpp
        src/account/account.cpp
        src/status/status.cpp
        src/utils/token_scanner.cpp
        src/log/log.cpp
        src/main.cpp)
