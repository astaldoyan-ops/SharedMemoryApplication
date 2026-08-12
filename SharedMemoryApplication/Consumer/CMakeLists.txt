add_executable(Consumer 
    Consumer.cpp
    CConsumer.cpp
    ../Basics/Basics.cpp
)

target_include_directories(Consumer PUBLIC ${includes})

# Replaces "Preprocessor Definitions"
target_compile_definitions(Consumer PRIVATE LOG_LEVEL=3)

