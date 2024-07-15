cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

# cmake setup begin
project(ecosystem)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# defaulting to debug mode, if not specified
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()

# setup my vim ycm :D
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# more paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

option(SOLANACEAE_ECOSYSTEM_BUILD_TESTING "Build tests" ${BUILD_TESTING})
message("II SOLANACEAE_ECOSYSTEM_BUILD_TESTING " ${SOLANACEAE_ECOSYSTEM_BUILD_TESTING})

# uggly, but it needs to be defined for all dependencies too
# what if its always export?
add_compile_definitions(ENTT_API_EXPORT)

if (SOLANACEAE_ECOSYSTEM_BUILD_TESTING)
	include(CTest)
endif()

#add_compile_options(-fsanitize=undefined)
#link_libraries(-fsanitize=undefined)

# external libs
add_subdirectory(./external) # before increasing warn levels, sad :(

set(CMAKE_CXX_EXTENSIONS OFF)

# bump up warning levels appropriately for clang, gcc & msvc
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	add_compile_options(
		-Wall -Wextra # Reasonable and standard
		-Wpedantic # Warn if non-standard C++ is used
		-Wunused # Warn on anything being unused
		#-Wconversion # Warn on type conversions that may lose data
		#-Wsign-conversion # Warn on sign conversions
		-Wshadow # Warn if a variable declaration shadows one from a parent context
	)

	if (NOT WIN32)
		#link_libraries(-fsanitize=address,undefined)
		#link_libraries(-fsanitize=undefined)
	endif()
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()

	# remove unreferenced objects, reducing binary and debugsymbol sizes
	# does not work !!! (it actually slightly increased the file size by <1%)
	# add_link_options("/OPT:REF")
endif()

# cmake setup end

add_subdirectory(./plugins)
