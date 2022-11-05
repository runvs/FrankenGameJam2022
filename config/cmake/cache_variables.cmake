set(JT_ENABLE_DEMOS OFF CACHE BOOL "enable stresstests")
set(JT_ENABLE_WEB OFF CACHE BOOL "enable web build using SDL and em++")
set(JT_ENABLE_CLANG_TIDY OFF CACHE BOOL "enable clang tidy checks")
set(JT_ENABLE_DEBUG ON CACHE BOOL "enable debug options")

# if JT_ENABLE_WEB is ON, it is required to use SDL
if (JT_ENABLE_WEB)
    set(USE_SFML OFF CACHE BOOL "Used SFML as Library")
    add_definitions(-DJT_ENABLE_WEB)
else ()
    set(USE_SFML ON CACHE BOOL "Used SFML as Library")
endif ()

if (JT_ENABLE_DEBUG)
    add_definitions(-DJT_ENABLE_DEBUG)
endif ()

if (USE_SFML)
    add_definitions(-DUSE_SFML)
else ()
    add_definitions(-DUSE_SDL)
endif ()