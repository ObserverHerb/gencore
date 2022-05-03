cmake_minimum_required(VERSION 3.13)

project(Gencore LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
option(USE_SYSTEM_LIBS "Link against system SDL2 and JsonCpp" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(USE_SYSTEM_LIBS)
	find_package(PkgConfig REQUIRED)
	find_package(SDL2 REQUIRED)
	pkg_check_modules(SDL2_IMAGE SDL2_image)
	link_libraries(${SDL2_IMAGE_LIBRARIES})
	pkg_check_modules(JSONCPP jsoncpp)
	link_libraries(${JSONCPP_LIBRARIES})
else()
	add_subdirectory("SDL")
	add_subdirectory("SDL_image")
	add_subdirectory("jsoncpp")
	if(NOT WIN32)
		target_compile_options(SDL2_image PRIVATE -fPIC)
	endif()
endif()

add_executable(Gencore
	globals.h
	texture.h
	texture.cpp
	actor.h
	actor.cpp
	state.h
	state.cpp
	main.cpp
)

if(USE_SYSTEM_LIBS)
	include_directories(${SDL2_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})
	target_link_libraries(Gencore SDL2 SDL2main ${SDL2_IMAGE_LIBRARIES} ${JSONCPP_LIBRARIES})
else()
	target_include_directories(Gencore PRIVATE $<TARGET_PROPERTY:SDL2,INTERFACE_INCLUDE_DIRECTORIES> $<TARGET_PROPERTY:SDL2_image,INTERFACE_INCLUDE_DIRECTORIES> $<TARGET_PROPERTY:jsoncpp_lib,INTERFACE_INCLUDE_DIRECTORIES>)
	set_target_properties(SDL2 PROPERTIES LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_BINARY_DIR})
	target_link_directories(Gencore PRIVATE $<TARGET_PROPERTY:SDL2,LIBRARY_OUTPUT_DIRECTORY> $<TARGET_PROPERTY:SDL2_image,LIBRARY_OUTPUT_DIRECTORY> $<TARGET_PROPERTY:jsoncpp_lib,LIBRARY_OUTPUT_DIRECTORY>)
	target_link_libraries(Gencore SDL2 SDL2main SDL2_image jsoncpp_lib)
endif()
