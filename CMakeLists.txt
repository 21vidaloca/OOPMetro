cmake_minimum_required(VERSION 3.26)
set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")


project(${MAIN_PROJECT_NAME})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)
add_executable(${MAIN_EXECUTABLE_NAME}
    main.cpp
    generated/src/Helper.cpp
        sources/ReteaMetrou.cpp
        sources/Statia.cpp
        sources/Traseu.cpp
        sources/Tren.cpp
        sources/TrenElectric.cpp
        sources/TrenNoapte.cpp
        sources/TrenRapid.cpp
        sources/TrenFactory.cpp
        sources/TraseuBuilder.cpp
)

set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE generated/include)
install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()
copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})