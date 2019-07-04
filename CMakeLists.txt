cmake_minimum_required(VERSION 3.12)
project(cppbitflag)

include(FetchContent)
include(GNUInstallDirs)

add_library(cppbitflag INTERFACE)
add_library(em::cppbitflag ALIAS cppbitflag)

option(EM_CPPBITFLAG_BUILD_TESTS "Build the cppbitflag tests" OFF)

target_include_directories(cppbitflag INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>)

install(TARGETS cppbitflag EXPORT cppbitflagTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT cppbitflagTargets FILE cppbitflagTargets.cmake
	    NAMESPACE em
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppbitflag)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/cppbitflag/
	    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cppbitflag)

if (EM_CPPBITFLAG_BUILD_TESTS)
	FetchContent_Declare(Catch
			GIT_REPOSITORY https://github.com/catchorg/Catch2.git
			GIT_TAG v2.9.1)
	FetchContent_GetProperties(Catch)
	if (NOT Catch_populated)
		FetchContent_Populate(Catch)
		add_subdirectory(${catch_SOURCE_DIR} ${catch_BINARY_DIR})
	endif ()

	add_executable(cppbitflag_tests)
	set_property(TARGET cppbitflag_tests PROPERTY CXX_STANDARD 17)
	if (MSVC)
		target_compile_options(cppbitflag_tests PRIVATE /W4 /permissive-)
	else ()
		target_compile_options(cppbitflag_tests PRIVATE -Wall -Wextra -Wshadow -Wunused -Wpedantic)
	endif ()

	target_sources(cppbitflag_tests PRIVATE
		include/bitflag/bitflag.hpp
		test/bitflag.cpp
        test/main.cpp)
	target_link_libraries(cppbitflag_tests PRIVATE em::cppbitflag Catch2::Catch2)
endif ()
