
cmake_minimum_required(VERSION 3.1)

project(cvTools)

set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )

#list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )

message("looking for opencv:")
find_package(OpenCV 3.0 QUIET)
if(NOT OpenCV_FOUND)
	message("not found opencv3.0")
   find_package(OpenCV 2.4.3 QUIET)
   if(NOT OpenCV_FOUND)
		message("not found opencv2.4")
      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
   endif()
else()
   message("found opencv3.0:",${OpenCV_LIBS})
endif()

#find_package( G2O REQUIRED )
#find_package( CSparse REQUIRED )

include_directories( 
    ${OpenCV_INCLUDE_DIRS} 
#   ${G2O_INCLUDE_DIRS}
#   ${CSPARSE_INCLUDE_DIR}
#   "/usr/include/eigen3/"
)

add_executable(yuv2png src/yuv2png.cpp)
target_link_libraries(yuv2png  ${OpenCV_LIBS})

