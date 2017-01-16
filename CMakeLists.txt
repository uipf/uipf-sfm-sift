cmake_minimum_required(VERSION 3.0.0)
project(uipf-sfm-sift)

#c++11 support
if(${CMAKE_VERSION} VERSION_LESS 3.1.0)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else()
	set(CMAKE_CXX_STANDARD 11)
endif()

if(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

	# sanitize memory issues https://gcc.gnu.org/gcc-4.8/changes.html
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
endif()

message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

# download sift binary
include(ExternalProject)
ExternalProject_Add(SIFT
	URL http://www.cs.ubc.ca/~lowe/keypoints/siftDemoV4.zip
	SOURCE_DIR ${uipf-sfm-sift_BINARY_DIR}/sift
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	TEST_COMMAND ""
	INSTALL_COMMAND ""
)

# find libuipf-module
# search in normal system paths as well as installation prefix for installation as unprivileged user.
find_library(
	ModuleBase uipf-module
	PATHS ${CMAKE_INSTALL_PREFIX}/lib
)
message("ModuleBase: ${ModuleBase}")
include_directories(${ModuleBase_INCLUDE_DIRS})


add_library(SfMLoeweSiftModule SHARED LoeweSiftModule.cpp)
target_link_libraries(SfMLoeweSiftModule ${ModuleBase})
add_dependencies(SfMLoeweSiftModule SIFT)

# installation targets
install(TARGETS
	SfMLoeweSiftModule

	LIBRARY DESTINATION lib/uipf
	ARCHIVE DESTINATION lib/uipf
)

# install headers
#install(DIRECTORY ${uipf-sfm-sift_SOURCE_DIR}/include
#	DESTINATION include/uipf-sfm/sift
#	FILES_MATCHING PATTERN "*.hpp"
#)
install(FILES ${uipf-sfm-sift_BINARY_DIR}/sift/sift
	DESTINATION bin/
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	RENAME uipf-sfm-sift
)