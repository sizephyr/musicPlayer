cmake_minimum_required(VERSION 3.16)

find_package(QCoro6 QUIET COMPONENTS Core Network)
if(NOT QCoro6_FOUND)
  message(STATUS "QCoro6 not found, downloading via FetchContent...")
  FetchContent_Declare(
    QCoro
    GIT_REPOSITORY https://github.com/qcoro/qcoro.git
    GIT_TAG v0.13.0)

  block()
  set(QCORO_BUILD_EXAMPLES
      OFF
      CACHE BOOL "Disable QCoro examples" FORCE)
  set(QCORO_ENABLE_WARNINGS_AS_ERRORS
      OFF
      CACHE BOOL "Disable warnings as errors for QCoro" FORCE)
  set(QCORO_WITH_QTDBUS
      OFF
      CACHE BOOL "Disable QCoro QtDBus support" FORCE)
  set(QCORO_WITH_QTNETWORK
      ON
      CACHE BOOL "Enable QCoro QtNetwork support" FORCE)
  set(QCORO_WITH_QTSQL
      OFF
      CACHE BOOL "Disable QCoro QtSql support" FORCE)
  set(QCORO_WITH_QTWIDGETS
      OFF
      CACHE BOOL "Disable QCoro QtWidgets support" FORCE)
  set(QCORO_WITH_QTQUICK
      OFF
      CACHE BOOL "Disable QCoro QtQuick support" FORCE)
  set(QCORO_WITH_QTWEBSOCKETS
      OFF
      CACHE BOOL "Disable QCoro QtWebSockets support" FORCE)
  set(QCORO_WITH_QML
      OFF
      CACHE BOOL "Disable QCoro QML support" FORCE)
  set(BUILD_SHARED_LIBS
      ON
      CACHE BOOL "Build QCoro as shared" FORCE)
  set(QCORO_BUILD_TESTING
      OFF
      CACHE BOOL "Disable QCoro tests" FORCE)
  set(BUILD_TESTING OFF)
  FetchContent_MakeAvailable(QCoro)
  endblock()

  foreach(_qcoro_target IN ITEMS QCoro6Core QCoro6Network)
    if(TARGET ${_qcoro_target})
      target_compile_options(${_qcoro_target} PRIVATE -fexceptions)
      target_compile_options(${_qcoro_target} PRIVATE -Wno-error)
    endif()
  endforeach()
endif()
