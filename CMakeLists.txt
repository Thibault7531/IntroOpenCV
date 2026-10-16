cmake_minimum_required(VERSION 4.3)
project(IntroOpenCV)

set(CMAKE_CXX_STANDARD 26)

add_custom_target(OpenCVCopyImages
                  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/images" "$<TARGET_FILE_DIR:IntroOpenCV>/images"
                  COMMENT "Copying images to build directory..."
)

add_executable(IntroOpenCV main.cpp Histogram.h Histogram.cpp
               ImageComparator.cpp
               ColorHistogram.h
               ColorHistogram.cpp
               WatershedSegmenter.h
               WatershedSegmenter.cpp
               SobelEdgeDetection.h
               SobelEdgeDetection.cpp
               EdgeDetectionStats.h
               EdgeDetectionStats.cpp
               LaplaceEdgeDetection.h
               LaplaceEdgeDetection.cpp
               CannyEdgeDetector.cpp
)
target_link_libraries(IntroOpenCV opencv_core opencv_highgui opencv_imgcodecs opencv_imgproc opencv_video)
target_include_directories(IntroOpenCV PUBLIC
                           /usr/include/opencv5
)
add_dependencies(IntroOpenCV OpenCVCopyImages)
