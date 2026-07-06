cmake_minimum_required(VERSION 3.10)

project(tokenvalidatorlinux)

add_executable(tokenvalidatorlinux main.cpp)

install(TARGETS tokenvalidatorlinux RUNTIME DESTINATION bin)
