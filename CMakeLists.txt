cmake_minimum_required(VERSION 3.12)
project(cppbitflag)

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
	add_executable(cppbitflag_tests)
	set_property(TARGET cppbitflag_tests PROPERTY CXX_STANDARD 17)
	if (MSVC)
		target_compile_options(cppbitflag_tests PRIVATE /W4 /permissive-)
	else ()
		target_compile_options(cppbitflag_tests PRIVATE -Wall -Wextra -Wshadow -Wunused -Wpedantic)
	endif ()

	target_sources(cppbitflag_tests PRIVATE
		include/bitflag/bitflag.hpp
		test/bitflag.cpp)
	target_link_libraries(cppbitflag_tests PRIVATE em::cppbitflag)
endif ()
