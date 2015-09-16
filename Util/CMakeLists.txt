# Source files for the library.
include_directories(../)
add_library(Util SHARED
  Util.h strtoc.c strtosc.c strtosc.c strtouc.c strtos.c strtous.c
  strtoi.c strtoui.c

  strtoi8.c strtoui8.c strtoil8.c strtouil8.c strtoif8.c strtouif8.c
  strtoi16.c strtoui16.c strtoil16.c strtouil16.c strtoif16.c strtouif16.c
  strtoi32.c strtoui32.c strtoil32.c strtouil32.c strtoif32.c strtouif32.c
  strtoi64.c strtoui64.c strtoil64.c strtouil64.c strtoif64.c strtouif64.c

  StrToChar.c StrToSChar.c StrToUChar.c StrToShort.c StrToUShort.c
  StrToInt.c StrToUInt.c StrToLong.c StrToULong.c
  StrToLongLong.c StrToULongLong.c
  StrToFloat.c StrToDouble.c StrToLongDouble.c

  StrToInt8.c StrToUInt8.c StrToLInt8.c StrToULInt8.c
  StrToFInt8.c StrToUFInt8.c
  StrToInt16.c StrToUInt16.c StrToLInt16.c StrToULInt16.c
  StrToFInt16.c StrToUFInt16.c
  StrToInt32.c StrToUInt32.c StrToLInt32.c StrToULInt32.c
  StrToFInt32.c StrToUFInt32.c
  StrToInt64.c StrToUInt64.c StrToLInt64.c StrToULInt64.c
  StrToFInt64.c StrToUFInt64.c
)
set_target_properties(Util PROPERTIES VERSION 0.0.0 SONAME 0.0.0)

# Add global compile options.
target_compile_options(Util PUBLIC -Wall)

# Set up the install stuff.
install(TARGETS Util LIBRARY DESTINATION lib)
install(FILES
  Util.h
  DESTINATION include
)
