cmake_minimum_required (VERSION 2.6)
project (SystemIndicatorTest)

set (PROJECT_VERSION_MAJOR 0)
set (PROJECT_VERSION_MINOR 1)

set (SHORT_NAME test)
set (DBUS_NAME org.sneetsher.indicator.${SHORT_NAME})
set (DBUS_PATH /org/sneetsher/indicator/${SHORT_NAME})

include(GNUInstallDirs)
include_directories("${PROJECT_BINARY_DIR}")

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

configure_file (
  "${PROJECT_SOURCE_DIR}/data/com.canonical.indicator.test.in"
  "${PROJECT_BINARY_DIR}/data/${DBUS_NAME}"
  )
  
configure_file (
  "${PROJECT_SOURCE_DIR}/data/com.canonical.indicator.test.service.in"
  "${PROJECT_BINARY_DIR}/data/${DBUS_NAME}.service"
  )
  
configure_file (
  "${PROJECT_SOURCE_DIR}/data/90_unity-greeter.gschema.override.in"
  "${PROJECT_BINARY_DIR}/data/90_unity-greeter.gschema.override"
  )

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK3 REQUIRED gtk+-3.0)
include_directories (${GTK3_INCLUDE_DIRS})
link_directories (${GTK3_LIBRARY_DIRS})
add_definitions (${GTK3_CFLAGS_OTHER})

add_executable (indicator-${SHORT_NAME}-service src/indicator-test-service.c)
target_link_libraries (indicator-${SHORT_NAME}-service ${GTK3_LIBRARIES})

install (TARGETS indicator-${SHORT_NAME}-service DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE}/indicator-${SHORT_NAME})
install (DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/data/ DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons
          FILES_MATCHING PATTERN "*.svg"  PATTERN "*.png")
install (FILES ${PROJECT_BINARY_DIR}/data/${DBUS_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/unity/indicators)
install (FILES ${PROJECT_BINARY_DIR}/data/${DBUS_NAME}.service DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dbus-1/services)
install (FILES ${PROJECT_BINARY_DIR}/data/90_unity-greeter.gschema.override DESTINATION ${CMAKE_INSTALL_PREFIX}/share/glib-2.0/schemas)

install(SCRIPT post_install.cmake)
