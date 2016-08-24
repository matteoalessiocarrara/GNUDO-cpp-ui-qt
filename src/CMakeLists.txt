cmake_minimum_required(VERSION 3.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)

set(SOURCES  delete_priority_dialog.cpp edit_priority_dialog.cpp main.cpp main_window.cpp priority_levels_dialog.cpp task_info_dialog.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${Qt5Widgets_LIBRARIES} sqlite3pp-static gnudo-backend-static)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)