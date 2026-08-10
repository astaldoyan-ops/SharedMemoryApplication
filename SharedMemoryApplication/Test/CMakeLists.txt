add_executable(Test 
    Test.cpp
)



# Replaces "Preprocessor Definitions"
target_compile_definitions(Test PRIVATE LOG_LEVEL=3)
