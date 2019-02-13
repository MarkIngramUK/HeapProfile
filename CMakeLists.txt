cmake_minimum_required(
	VERSION
		3.9.0
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(HeapProfile
	LANGUAGES
		CXX
	VERSION
		1.0.0.0
)

add_executable(HeapProfile
	src/Allocators.hpp
	src/HeapProfile.cpp
)
