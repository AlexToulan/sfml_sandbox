cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# sfml
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# utils
file(GLOB_RECURSE util_files
    src/thirdparty/*.hpp
    src/thirdparty/*.cpp
    src/Utils/*.hpp
    src/Utils/*.cpp
    src/Utils/EventSystem/*.hpp
    src/Utils/EventSystem/*.cpp
    src/Utils/GameMode/*.hpp
    src/Utils/GameMode/*.cpp
    src/Utils/UI/*.hpp
    src/Utils/UI/*.cpp
    )

### game modes
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

# if you'd like to add another gamemode create another directory "src/Example/"
# glob the files in the directory and reference the source in the add_executable command

# file(GLOB_RECURSE example_game_mode_files
#     src/Example/*.cpp
#     src/Example/*.hpp
#     )

# add_executable(SFMLSandbox src/main.cpp ${example_game_mode_files})

# executable
add_executable(SFMLSandbox src/main.cpp ${util_files} ${quad_test_files} ${game_of_life_files} ${ants_files})
target_include_directories(SFMLSandbox
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/
)

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
