# src/CMakeLists.txt

# Library that implements volatilityFormula
add_library(volatility
    volatilityFormula.cpp
)

# Only expose the include/ directory so the header is found
target_include_directories(volatility
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# Main executable
add_executable(volatility_app
    main.cpp
)

# Link executable to the library
target_link_libraries(volatility_app
    PRIVATE
        volatility
)

