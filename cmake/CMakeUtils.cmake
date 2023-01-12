# On the shoulders of AliceO2's CMake

include_guard()

# Common CMake functionality

function(rwd_add_library)
  cmake_parse_arguments(
    PARSE_ARGV
    0
    A
    ""
    "TARGETNAME"
    "DEPENDENCIES;SOURCES;INCLUDE_DIRECTORIES;ROOT_DICTIONARY_HEADERS;LINKDEFDIR"
  )
  if(A_UNPARSED_ARGUMENTS)
    message(
      FATAL_ERROR "Unexpected unparsed arguments: ${A_UNPARSED_ARGUMENTS}")
  endif()

  set(target "${A_TARGETNAME}")

  add_library(${target} SHARED ${A_SOURCES})
  target_link_libraries(${target} ${DEPENDENCIES})
    #set(includeDirs $<TARGET_PROPERTY:${target},INCLUDE_DIRECTORIES>)
  if(A_INCLUDE_DIRECTORIES)
    foreach(d IN LISTS A_INCLUDE_DIRECTORIES)
      get_filename_component(adir ${d} ABSOLUTE)
      if(NOT IS_DIRECTORY ${adir})
        message(
          FATAL_ERROR "Trying to append non existing include directory ${d}")
      endif()
      target_include_directories(${target} PUBLIC $<BUILD_INTERFACE:${adir}>)
    endforeach()
  endif()


  if(A_ROOT_DICTIONARY_HEADERS)

    # Build the LD_LIBRARY_PATH required to get rootcling running fine
    #
    # Need at least root core library
    get_filename_component(LD_LIBRARY_PATH ${ROOT_Core_LIBRARY} DIRECTORY)
    # and possibly toolchain libs if we are using a toolchain
    if(DEFINED ENV{GCC_TOOLCHAIN_ROOT})
      set(LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:$ENV{GCC_TOOLCHAIN_ROOT}/lib")
      set(LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:$ENV{GCC_TOOLCHAIN_ROOT}/lib64")
    endif()

    set(dictionary G__${target})
    set(pcmFile "${CMAKE_CURRENT_BINARY_DIR}/${dictionary}_rdict.pcm")
    set(rootmapFile "${CMAKE_CURRENT_BINARY_DIR}/lib${target}.rootmap")
    set(dictionaryFile "G__${target}.cxx")
    set(linkdef "${A_LINKDEFDIR}/${target}LinkDef.h")
    message("LINKDEF for target ${target} assumed at ${linkdef}")
    # Since this is actually generated before libraries are built, we have the dictionary files available even though this is inside a CMake function
    add_custom_command(
      OUTPUT ${dictionaryFile} ${pcmFile} ${rootmapFile}
      VERBATIM
      COMMAND
      ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${LD_LIBRARY_PATH} ${ROOT_rootcling_CMD}
      -f ${dictionaryFile}
      -rmf ${rootmapFile}
      -rml ${target}
      -noGlobalUsingStd
      -inlineInputHeader
      -I$<JOIN:$<REMOVE_DUPLICATES:$<TARGET_PROPERTY:${target},INCLUDE_DIRECTORIES>>,$<SEMICOLON>-I>
      ${A_ROOT_DICTIONARY_HEADERS} ${linkdef}
      DEPENDS
      ${A_ROOT_DICTIONARY_HEADERS} ${linkdef}
      COMMAND_EXPAND_LISTS
    )
    # Now add to target
    target_sources(${target} PRIVATE ${dictionaryFile})
    if(NOT ROOT::RIO IN_LIST A_DEPENDENCIES)
      # add ROOT::IO if not already there as a target that has a Root dictionary
      # has to depend on ... Root
      message(STATUS "Link against ROOT::RIO")
      target_link_libraries(${target} PUBLIC ROOT::RIO)
    endif()
  endif()

  if(A_DEPENDENCIES)
    target_link_libraries(${target} PUBLIC ${A_DEPENDENCIES})
    set_target_properties(${target} PROPERTIES INTERFACE_LINK_LIBRARIES "${A_DEPENDENCIES}")
  endif()

  install(TARGETS ${target}
          INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  if(A_ROOT_DICTIONARY_HEADERS)
    install(FILES ${rootmapFile} ${pcmFile} DESTINATION ${CMAKE_INSTALL_LIBDIR})
  endif()
endfunction()
