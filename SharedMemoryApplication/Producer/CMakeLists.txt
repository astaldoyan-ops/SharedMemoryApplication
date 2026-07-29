add_executable(Producer 
    Producer.cpp
    CProducer.cpp
)

target_include_directories(Producer PUBLIC ${includes})

# Replaces "Preprocessor Definitions"
target_compile_definitions(Producer PRIVATE LOG_LEVEL=3)

