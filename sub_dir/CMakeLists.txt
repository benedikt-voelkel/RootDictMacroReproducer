set(MODULE_NAME "RootDictMacro")

# Source directories
SET(IMP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(INC_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/)

include(CMakeUtils)

####################
# Add core library #
####################
  rwd_add_library(
    TARGETNAME MacroHelper
    DEPENDENCIES ROOT::Core
    SOURCES ${IMP_SRC_DIR}/MacroHelper.cxx
    INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include)

  rwd_add_library(
    TARGETNAME NamespaceStuff
    DEPENDENCIES ROOT::Core
    SOURCES ${IMP_SRC_DIR}/NamespaceStuff.cxx
    INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include
    LINKDEFDIR ${CMAKE_CURRENT_LIST_DIR}/src
    ROOT_DICTIONARY_HEADERS include/NamespaceStuff.h)

  rwd_add_library(
    TARGETNAME Base
    DEPENDENCIES ROOT::Core
    SOURCES ${IMP_SRC_DIR}/Base.cxx
    INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include
    LINKDEFDIR ${CMAKE_CURRENT_LIST_DIR}/src
    ROOT_DICTIONARY_HEADERS include/Base.h)

# Install executables (and add to export such that we can easily find its dependencies later)
set(EXECUTABLE_NAME executable)
add_executable(${EXECUTABLE_NAME} ${IMP_SRC_DIR}/executable.cxx)
target_link_libraries(${EXECUTABLE_NAME} MacroHelper Base)
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
install(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION ${INSTALL_BIN_DIR})
