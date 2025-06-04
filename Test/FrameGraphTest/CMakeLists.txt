add_executable(FrameGraphTest FrameGraphTest.cpp)
target_include_directories(FrameGraphTest PRIVATE ${REALSIX_INC_DIRS})

if(MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set_property(TARGET FrameGraphTest PROPERTY FOLDER Test)
    target_compile_options(FrameGraphTest PRIVATE "/wd4251;" "/wd4819" "/bigobj;")
endif()