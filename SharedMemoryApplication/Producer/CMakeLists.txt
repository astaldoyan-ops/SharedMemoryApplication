add_executable(Producer 
    Producer.cpp
    CProducer.cpp
)



# Replaces "Preprocessor Definitions"
target_compile_definitions(Producer PRIVATE LOG_LEVEL=3)

