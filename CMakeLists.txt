# On the shoulders of AliceO2's CMake

# Minimum version of CMake
CMAKE_MINIMUM_REQUIRED(VERSION 3.15.0 FATAL_ERROR)

project(RootDictMacroReproducer)
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake)

# Install directories
SET(INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
SET(INSTALL_INC_DIR ${CMAKE_INSTALL_PREFIX}/include)
SET(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)

if(APPLE)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-undefined,error") # avoid undefined in our libs
elseif(UNIX)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined") # avoid undefined in our libs
endif()

include(GNUInstallDirs)

# Set CMAKE_INSTALL_LIBDIR explicitly to lib (to avoid lib64 on CC7)
set(CMAKE_INSTALL_LIBDIR lib)

include(GNUInstallDirs)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_BINDIR})
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_LIBDIR})
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_LIBDIR})
endif()

if(APPLE)
  set(basePoint @loader_path)
else()
  set(basePoint $ORIGIN)
endif()

# use, i.e. do not skip, the full RPATH in the _build_ tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)
# when building, do not use the install RPATH already (will only be used when
# actually installing), unless we are on a Mac (where the install is otherwise
# pretty slow)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
if(APPLE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif()

# add to the install RPATH the (automatically determined) parts of the RPATH
# that point to directories outside the build tree
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# specify libraries directory relative to binaries one.
file(RELATIVE_PATH relDir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(CMAKE_INSTALL_RPATH ${basePoint} ${basePoint}/${relDir})

#################
# Find packages #
#################

########
# ROOT #
########
# Find ROOT and get useful functions from ROOT_USE_FILE,
# e.g. ROOT_GENERATE_DICTIONARY
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS Core)
include(${ROOT_USE_FILE})

#######################
# Do the installation #
#######################
# set(CMAKE_INSTALL_LIBDIR ${INSTALL_LIB_DIR})
# set(CMAKE_INSTALL_INCLUDEDIR ${INSTALL_INC_DIR})


add_subdirectory(sub_dir)

