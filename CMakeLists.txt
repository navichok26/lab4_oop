cmake_minimum_required(VERSION 3.10)
project(lab4_oop)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab4_oop
    main.cpp
    Encryptor.cpp
)

if (WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()