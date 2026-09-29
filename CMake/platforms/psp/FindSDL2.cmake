# The PSPDEV toolchain ships prebuilt SDL2 static libraries and headers,
# but no SDL2Config.cmake / sdl2-config.cmake and no SDL2 pkg-config file,
# so CMake's Config-mode find_package(SDL2) cannot locate it on its own.
# This Module-mode finder locates the toolchain's SDL2 build directly.

find_library(SDL2_LIBRARY NAMES SDL2)
find_library(SDL2MAIN_LIBRARY NAMES SDL2main)
find_path(SDL2_INCLUDE_DIR NAMES SDL.h PATH_SUFFIXES SDL2)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2
  REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR
)

if(SDL2_FOUND AND NOT TARGET SDL2::SDL2)
  add_library(SDL2::SDL2 STATIC IMPORTED GLOBAL)
  set_target_properties(SDL2::SDL2 PROPERTIES
    IMPORTED_LOCATION "${SDL2_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
  )
endif()

if(SDL2MAIN_LIBRARY AND NOT TARGET SDL2::SDL2main)
  add_library(SDL2::SDL2main STATIC IMPORTED GLOBAL)
  set_target_properties(SDL2::SDL2main PROPERTIES
    IMPORTED_LOCATION "${SDL2MAIN_LIBRARY}"
  )
endif()

mark_as_advanced(SDL2_LIBRARY SDL2MAIN_LIBRARY SDL2_INCLUDE_DIR)
