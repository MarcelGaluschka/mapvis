

add_executable(${PROJECT_NAME}
    main.cpp
    wav_reader/reader.cpp
    doa_compute/sod.cpp
    output_writer/csv_writer.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/build/)


#find_package(iir)
#target_link_libraries(... iir::iir)
find_package(iir)
target_link_libraries(${PROJECT_NAME} iir::iir_static)
# Add additonial compile options to enable more warnings
add_compile_options(-Wall -Wextra -O3)
add_compile_options(-lsndfile -liir)

#add_compile_options(-lvtk -L/software/vtk/lib -I/software/vtk/include)
# Set the version of the C++ standard to use, we use C++14, published in 2014
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")