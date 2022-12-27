cmake_minimum_required(VERSION 3.11)
project(_BookStore)

set(CMAKE_CXX_STANDARD 17)

add_executable(code bookstore.cpp unrolllinklist.cpp book.cpp log_status.cpp account.cpp token_scanner.cpp transaction_log.cpp main.cpp)
