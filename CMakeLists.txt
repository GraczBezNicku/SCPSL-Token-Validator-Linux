cmake_minimum_required(VERSION 3.10)

project(tokenvalidatorlinux)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp
        src/*.hpp
        src/*.h
)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
find_package(nlohmann_json REQUIRED)
find_package(ZXing REQUIRED)

if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

add_executable(tokenvalidatorlinux ${SOURCES})

target_link_libraries(tokenvalidatorlinux
    ${wxWidgets_LIBRARIES}
    nlohmann_json::nlohmann_json
    ZXing::ZXing
)

install(TARGETS tokenvalidatorlinux RUNTIME DESTINATION bin)
