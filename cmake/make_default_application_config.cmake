cmake_minimum_required(VERSION 3.16)

set(CONFIG_DIR "${CMAKE_BINARY_DIR}/config")
set(CONFIG_FILEPATH "${CONFIG_DIR}/ApplicationConfig.json")

if(MAKE_DEFAULT_CONFIG AND (NOT DEFINED FORCED_APPLICATION_CONFIG_PATH
                            OR NOT EXISTS "${CONFIG_FILEPATH}"))
  message(STATUS "Создание конфигурации по-умолчанию")
  if(NOT EXISTS "${CONFIG_FILEPATH}")
    set(CONFIG_CONTENT
        [[
{
"window": {
"width": 1000,
"height": 600,
"title": "player",
"background": ":/qt/qml/Main/img/base_background.jpg"
},
"animaltion": {
"duration_ms": 250,
"notification_hide_after_ms": 4000
},
"network": {
"base_transfer_timeout_ms": 1000
}
}
]])

    file(WRITE "${CONFIG_FILEPATH}" "${CONFIG_CONTENT}")
    message(STATUS "Создан конфигурационный файл: ${CONFIG_FILEPATH}")
  else()
    message(
      STATUS
        "Файл конфигурации уже существует: ${CONFIG_FILEPATH} (пропуск записи)")
  endif()

  set(FORCED_APPLICATION_CONFIG_PATH
      "${CONFIG_FILEPATH}"
      CACHE PATH "Path to application config file" FORCE)
endif()

if(DEFINED FORCED_APPLICATION_CONFIG_PATH)
  message(
    STATUS
      "Конфигурация приложения переопределена на ${FORCED_APPLICATION_CONFIG_PATH}"
  )
  target_compile_definitions(
    configuration
    PRIVATE FORCED_APPLICATION_CONFIG_PATH="${FORCED_APPLICATION_CONFIG_PATH}")
endif()
