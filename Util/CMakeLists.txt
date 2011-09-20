# Source files for the library.
include_directories(../)
add_library(Util SHARED
  Util.h strtoc.c strtosc.c strtouc.c strtob.c strtoub.c
)
set_target_properties(Util PROPERTIES VERSION 0.0.0 SONAME 0.0.0)

# Set up the install stuff.
install(TARGETS Util LIBRARY DESTINATION lib)
install(FILES
  Util.h
  DESTINATION include
)
