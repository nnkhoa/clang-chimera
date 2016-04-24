# Create library
add_library(operators INTERFACE)

# Groups of operators
set(operator_dirs)
set(operator_libs)
## FLAP
list(APPEND operator_dirs FLAP)
list(APPEND operator_libs flap_operators)
## Ward-Armstrong
list(APPEND operator_dirs Ward-Armstrong)
list(APPEND operator_libs ward_armstrong_operators)

# Check mode
list(LENGTH operator_dirs operator_dirs_total)
math(EXPR operator_dirs_total ${operator_dirs_total}-1)
foreach(index RANGE 0 ${operator_dirs_total})
  list(GET operator_dirs ${index} operator_dir)
  list(GET operator_libs ${index} operator_lib_name)
  
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${operator_dir})
    #message(STATUS "Directory ${operator_dir}: FOUND")
    
    # Devel mode: add subdirectory
    add_subdirectory(${operator_dir})
    set(operator_lib_name ${operator_lib_name})
  else (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${operator_dir})
    #message(STATUS "Directory ${operator_dir}: NOT FOUND")
    message(STATUS "Searching for library: ${operator_lib_name}")
    
    # Release mode: search for a pre-linked library
    find_library(operator_lib ${operator_lib_name} PATHS ${CMAKE_SOURCE_DIR}/lib NO_DEFAULT_PATH)
    
    # Check if it has been found
    if (${operator_lib} STREQUAL operator_lib-NOTFOUND)
      message(FATAL_ERROR "${operator_lib_name} NOT FOUND in ${CMAKE_SOURCE_DIR}/lib")
    endif(${operator_lib} STREQUAL operator_lib-NOTFOUND)
    
    set(operator_lib_name ${operator_lib})
    unset(operator_lib CACHE)
  endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${operator_dir})
  
  message(STATUS "Adding library: ${operator_lib_name}")
  target_link_libraries(operators INTERFACE
                       ${operator_lib_name}
                       )
endforeach(index)
