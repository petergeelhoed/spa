# Define the test
add_test(
  NAME testnoon
  COMMAND bash -c "echo -e ' 1766401980 \n1766461980' | \"$<TARGET_FILE:noon>\""
)

# Use bracket literals to avoid escaping long multi-line regex
set_tests_properties(testnoon PROPERTIES
  PASS_REGULAR_EXPRESSION [[
2025-12-22T12:39:56 1766403596\.428729 45596\.428729 0\.000000 180\.000000
2025-12-23T12:40:26 1766490026\.197102 45626\.197102 86429\.768373 179\.999999
]]
)

