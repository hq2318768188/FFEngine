# 使用 target_link_libraries() 链接库
function(configure_target TARGET_NAME)
	set(SRC_FILE "examples/${TARGET_NAME}.cpp")
    add_executable(${TARGET_NAME} ${SRC_FILE})
    target_link_libraries(${TARGET_NAME} PRIVATE ff_lib glad::glad glfw assimp::assimp)
    target_compile_definitions(${TARGET_NAME} PRIVATE -DUNICODE D_UNICODE)
	
	set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "examples")
endfunction()