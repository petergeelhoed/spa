
cmake_minimum_required(VERSION 3.10)
project(azi_project C)

# Use C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Option to build shared libraries (ON -> .so/.dylib, OFF -> .a)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------- Library: myazi ----------
add_library(myparse
    src/parseutil.c
)
 target_include_directories(myparse
     PUBLIC
         ${CMAKE_SOURCE_DIR}/inc
 )
add_library(myazi
    src/libmyazi.c
)

 target_include_directories(myazi
     PUBLIC
         ${CMAKE_SOURCE_DIR}/inc
 )

# Define GNU extensions for glibc (e.g., tm_gmtoff)
target_compile_definitions(myazi PRIVATE _GNU_SOURCE)

# Warnings (GNU/Clang)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(myazi PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(myparse PRIVATE -Wall -Wextra -pedantic)
endif()

# ---------- Executable: azi ----------
add_executable(azi
    src/azi.c
)
add_executable(noon
    src/noon.c
)

# Define GNU extensions for the executable as well
target_compile_definitions(azi PRIVATE _GNU_SOURCE)
target_compile_definitions(noon PRIVATE _GNU_SOURCE)

# Warnings (GNU/Clang)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(azi PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(noon PRIVATE -Wall -Wextra -pedantic)
endif()

# Link against libmyazi and libm (math)
target_link_libraries(azi PRIVATE myazi myparse m)
target_link_libraries(noon PRIVATE myazi myparse m)

# Library & executable
install(TARGETS myazi myparse azi noon
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# ---------- Optional: output directories ----------
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include(CTest)
enable_testing()
add_subdirectory(tests)

