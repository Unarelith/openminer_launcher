#------------------------------------------------------------------------------
# CMakeLists.txt
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.8)

project(openminer_launcher)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#------------------------------------------------------------------------------
# Get source files
#------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES source/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.hpp)
file(GLOB_RECURSE QRC_FILES res/*.qrc)

foreach(HEADER_FILE ${HEADER_FILES})
	get_filename_component(HEADER_DIRECTORY ${HEADER_FILE} DIRECTORY)
	include_directories(${HEADER_DIRECTORY})
endforeach(HEADER_FILE)

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wfatal-errors")

#------------------------------------------------------------------------------
# Packages
#------------------------------------------------------------------------------
# - Qt5
#------------------------------------------------------------------------------
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Network Sql Widgets REQUIRED)

#------------------------------------------------------------------------------
# Submodules
# from https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
#------------------------------------------------------------------------------
find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if(GIT_SUBMODULE)
		message(STATUS "Submodule update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/quazip/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

#------------------------------------------------------------------------------
# - QuaZip
#------------------------------------------------------------------------------
# set(QUAZIP_STATIC ON)

add_subdirectory(external/quazip)
include_directories(external/quazip)

#------------------------------------------------------------------------------
# Add executable
#------------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(${CMAKE_PROJECT_NAME}
               ${SOURCE_FILES}
               ${HEADER_FILES}
               ${QRC_FILES})

add_dependencies(${CMAKE_PROJECT_NAME} Quazip)

#------------------------------------------------------------------------------
# Link options
#------------------------------------------------------------------------------
target_link_libraries(${CMAKE_PROJECT_NAME} Quazip)

QT5_USE_MODULES(${CMAKE_PROJECT_NAME} Core Network Sql Widgets)

#------------------------------------------------------------------------------
# Install paths
#------------------------------------------------------------------------------
if(NOT DEFINED CMAKE_INSTALL_BINDIR)
  set(CMAKE_INSTALL_BINDIR bin)
endif()

install(TARGETS ${CMAKE_PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

