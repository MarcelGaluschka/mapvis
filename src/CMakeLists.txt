
add_executable(${PROJECT_NAME}
    main.cpp
    wav_reader/reader.cpp
    doa_compute/doa_estimator.cpp
    doa_compute/sod.cpp
    doa_compute/sod_3d.cpp
    output_writer/csv_writer.cpp
    
    iir1/iir/Biquad.cpp
    iir1/iir/Butterworth.cpp
    iir1/iir/Cascade.cpp
    iir1/iir/ChebyshevI.cpp
    iir1/iir/ChebyshevII.cpp
    iir1/iir/Custom.cpp
    iir1/iir/PoleFilter.cpp
    iir1/iir/RBJ.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/build/)


# Add additonial compile options to enable more warnings
add_compile_options(-Wall -Wextra -O3)

# Set the version of the C++ standard to use, use C++14, published in 2014
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")