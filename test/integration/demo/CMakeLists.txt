file(GLOB_RECURSE DEMOFILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*)
FetchContent_GetProperties(imgui)
add_executable(jt_demos ${DEMOFILES} ${imgui_SOURCE_DIR}/imgui_demo.cpp)
if (JT_ENABLE_WEB)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

target_include_directories(jt_demos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(jt_demos JamTemplateLib)
jt_use_assets(jt_demos)

