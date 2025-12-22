
cmake_minimum_required(VERSION 3.10)
project(azi_project C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(tets azi.c)

# Add the GNU extension macro for glibc features like tm_gmtoff
target_compile_definitions(tets PRIVATE _GNU_SOURCE)

# Optional: warnings and extra flags
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(tets PRIVATE -Wall -Wextra -pedantic)
endif()

# Link libraries if needed
target_link_libraries(tets PRIVATE m myazi)

