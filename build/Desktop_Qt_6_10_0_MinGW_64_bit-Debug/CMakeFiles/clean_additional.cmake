# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\idokeno_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\idokeno_autogen.dir\\ParseCache.txt"
  "idokeno_autogen"
  )
endif()
