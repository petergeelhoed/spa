
# tests/CMakeLists.txt

# 1) Create input at configure time (exact content, including trailing newline)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/input_testnoon.txt " 1766401980 \n1766461980\n")

# 2) Single test: run noon and compare outputs (no newlines in -c string)
add_test(
  NAME testnoon
  COMMAND /bin/sh -c
          "set -e; \
           \"$<TARGET_FILE:noon>\" < \"${CMAKE_CURRENT_BINARY_DIR}/input_testnoon.txt\" > \"${CMAKE_CURRENT_BINARY_DIR}/actual_testnoon.txt\"; \
           \"${CMAKE_COMMAND}\" -E compare_files \"${CMAKE_CURRENT_BINARY_DIR}/actual_testnoon.txt\" \"${CMAKE_CURRENT_SOURCE_DIR}/expected_testnoon.txt\""
)

