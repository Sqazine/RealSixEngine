set(NAME RenderTest)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(${NAME} ${SRC})
target_include_directories(${NAME} PRIVATE ${REALSIX_INC_DIRS})
target_link_libraries(${NAME} PUBLIC ${REALSIX_EDITOR_LIB_NAME} ${THIRDPARTY_LIB} ${REALSIX_LIB_NAME})
target_compile_definitions(${NAME} PUBLIC ${COMPILE_DEFINITIONS})
target_compile_definitions(${NAME} PRIVATE TEST_SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/")
target_compile_definitions(${NAME} PRIVATE TEST_TEXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/")
target_compile_definitions(${NAME} PRIVATE TEST_MODEL_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/models/")
target_compile_definitions(${NAME} PRIVATE DESTINATION_DIR="${REALSIX_BINARY_DIR}/assets/")

if(MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set_property(TARGET ${NAME} PROPERTY FOLDER Test)
    target_compile_options(${NAME} PRIVATE "/wd4251;" "/wd4819" "/bigobj;")
endif()