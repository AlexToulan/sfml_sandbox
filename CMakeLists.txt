cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# dependancies
# sfml
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# gtest - requires cmake 3.13 or later
include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# use gtest CMake
include(GoogleTest)

# source
# utils
file(GLOB_RECURSE util_files
    src/thirdparty/*.hpp
    src/thirdparty/*.cpp
    src/Utils/*.hpp
    src/Utils/*.cpp
    src/Utils/EventSystem/*.hpp
    src/Utils/EventSystem/*.cpp
    )

    file(GLOB_RECURSE sfml_helper_files
    src/GameMode/*.hpp
    src/GameMode/*.cpp
    src/UI/*.hpp
    src/UI/*.cpp
    )

# game modes
# quad test
file(GLOB_RECURSE quad_test_files
src/QuadTest/*.cpp
src/QuadTest/*.hpp
)

# game of life
file(GLOB_RECURSE game_of_life_files
    src/GameOfLife/*.cpp
    src/GameOfLife/*.hpp
    )

# ANTS!
file(GLOB_RECURSE ants_files
    src/Ants/*.cpp
    src/Ants/*.hpp
    )

file(GLOB_RECURSE test_files
    test/src/*.cpp
    test/src/*.hpp
    )


# If you'd like to add another gamemode create a directory such as "src/Example/".
# Glob the files in the directory and reference the source in the add_executable command.

# file(GLOB_RECURSE example_game_mode_files
#     src/Example/*.cpp
#     src/Example/*.hpp
#     )

# add_executable(SFMLSandbox src/main.cpp ${example_game_mode_files})


# main game executable
add_executable(SFMLSandbox src/main.cpp ${sfml_helper_files} ${util_files} ${game_of_life_files} ${quad_test_files})
target_include_directories(SFMLSandbox
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/
)

# gtest executable
add_executable(testAll ${CMAKE_SOURCE_DIR}/test/main_testall.cpp ${test_files} ${util_files})
target_include_directories(testAll
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/test/src/
)

target_link_libraries(testAll GTest::gtest_main)
target_compile_features(testAll PRIVATE cxx_std_20)
gtest_discover_tests(testAll)

# linker
target_link_libraries(SFMLSandbox PRIVATE sfml-graphics)
target_compile_features(SFMLSandbox PRIVATE cxx_std_20)

if(WIN32)
    add_custom_command(
        TARGET SFMLSandbox
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:SFMLSandbox>
        VERBATIM)
endif()

install(TARGETS SFMLSandbox)
