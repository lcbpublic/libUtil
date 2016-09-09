# Source files for the library.
include_directories(../ ./)
add_library(Util SHARED
  Util.h


  StrToX.h
  strtochar.c strtoschar.c strtouchar.c
  
  strtosc.c strtouc.c strtos.c strtous.c strtoi.c strtoui.c

  strtoi8.c strtoui8.c strtoil8.c strtouil8.c
  strtoif8.c strtouif8.c strtoi16.c strtoui16.c strtoil16.c
  strtouil16.c strtoif16.c strtouif16.c strtoi32.c strtoui32.c
  strtoil32.c strtouil32.c strtoif32.c strtouif32.c strtoi64.c
  strtoui64.c strtoil64.c strtouil64.c strtoif64.c strtouif64.c

  StrToChar.c StrToSChar.c StrToUChar.c
  
  StrToSC.c StrToUC.c StrToS.c StrToUS.c
  StrToI.c StrToUI.c StrToL.c StrToUL.c
  StrToLL.c StrToULL.c StrToIMax.c StrToUMax.c
  StrToF.c StrToD.c StrToLD.c

  StrToI8.c StrToUI8.c StrToIL8.c StrToUIL8.c
  StrToIF8.c StrToUIF8.c
  StrToI16.c StrToUI16.c StrToIL16.c StrToUIL16.c
  StrToIF16.c StrToUIF16.c
  StrToI32.c StrToUI32.c StrToIL32.c StrToUIL32.c
  StrToIF32.c StrToUIF32.c
  StrToI64.c StrToUI64.c StrToIL64.c StrToUIL64.c
  StrToIF64.c StrToUIF64.c

  String.h String.c

  Errors.h Errors.c
)
set_target_properties(Util PROPERTIES VERSION 0.0.3 SONAME 0.0.3)

# Add global compile options.
target_compile_options(Util PUBLIC -Wall)

# Set up the install stuff.
install(TARGETS Util LIBRARY DESTINATION lib)
install(FILES
  Util.h Errors.h StrToX.h String.h
  DESTINATION include
)
