cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(QML_IMPORT_PATH
    "${CMAKE_BINARY_DIR}/src/qml"
    CACHE PATH "Path to application qml modules" FORCE)

if(CMAKE_BUILD_TYPE MATCHES "Release")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

  add_compile_definitions(
    QT_NO_DEBUG QT_NO_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060B00
    QT_USE_QSTRINGBUILDER)
  set(CPACK_COMPONENTS_ALL Unspecified)
endif()

if(LINUX)
  if(EXISTS "/etc/os-release")
    file(READ "/etc/os-release" OS_RELEASE_CONTENT)

    if(OS_RELEASE_CONTENT MATCHES "ID(_LIKE)?=[^\n]*debian")
      set(IS_DEBIAN_LIKE TRUE)
    endif()
  endif()
endif()
