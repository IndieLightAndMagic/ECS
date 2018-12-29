#CMAKE TO MAKE A STATIC LIBRARY
#HEADER
cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(TARGET_NAME ECS)
project(${TARGET_NAME})
set(CMAKE_CXX_STANDARD 17)

#Installation directory 
get_filename_component(GTECH_DIR $ENV{GTECH_ROOT} ABSOLUTE)
set(GTECH_DIR_LIB ${GTECH_DIR}/3rd/lib/)
set(GTECH_DIR_INC ${GTECH_DIR}/3rd/include/)

set(GTECH_DIR_INC_ECS ${GTECH_DIR}/3rd/include/ECS)

#DIRECTORIES
get_filename_component(ROOT_DIR "." ABSOLUTE)
message(STATUS "-----------------------------")
message(STATUS "SYSTEM NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "ROOT_DIR: ${ROOT_DIR}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "-----------------------------")

#GLM
set(GLM_DIR "${GTECH_DIR}/GLM")


add_library(${TARGET_NAME} STATIC

	${CMAKE_SOURCE_DIR}/Component/anchorpointcomponent.cpp
	${CMAKE_SOURCE_DIR}/Component/componentfactory.cpp
	${CMAKE_SOURCE_DIR}/Component/componentmanager.cpp
	${CMAKE_SOURCE_DIR}/Component/entityinformationcomponent.cpp
	${CMAKE_SOURCE_DIR}/Component/texturecomponent.cpp
	${CMAKE_SOURCE_DIR}/Entity/entityfactory.cpp
	${CMAKE_SOURCE_DIR}/Entity/entitymanager.cpp
	${CMAKE_SOURCE_DIR}/System/glRendering.cpp
	${CMAKE_SOURCE_DIR}/System/glRendering_loader.cpp
	${CMAKE_SOURCE_DIR}/System/glRendering_material.cpp
	${CMAKE_SOURCE_DIR}/System/kinematics.cpp
	${CMAKE_SOURCE_DIR}/System/rendering.cpp
	${CMAKE_SOURCE_DIR}/System/sceneresourcer.cpp 

)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_BUILD_TYPE debug)

target_include_directories(${TARGET_NAME} PUBLIC 
	${CMAKE_SOURCE_DIR}
	${GLM_DIR}
	${GTECH_DIR_INC}
)
message(STATUS "-----------------------------")
message(STATUS "INCLUDE_DIRECTORIES: ${INC_DIR_SET}")
message(STATUS "-----------------------------")



file(MAKE_DIRECTORY ${GTECH_DIR_LIB})
file(MAKE_DIRECTORY ${GTECH_DIR_INC_ECS})
file(MAKE_DIRECTORY ${GTECH_DIR_INC_ECS}/Component)
file(MAKE_DIRECTORY ${GTECH_DIR_INC_ECS}/System)
file(MAKE_DIRECTORY ${GTECH_DIR_INC_ECS}/Entity)



#POST BUILD COMMANDS 
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy lib${TARGET_NAME}.a ${GTECH_DIR_LIB}
	COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DIR}/ECS/*.h ${GTECH_DIR_INC_ECS}/
	COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DIR}/ECS/Component/*.h ${GTECH_DIR_INC_ECS}/Component/
	COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DIR}/ECS/System/*.h ${GTECH_DIR_INC_ECS}/System/
	COMMAND ${CMAKE_COMMAND} -E copy ${ROOT_DIR}/ECS/Entity/*.h ${GTECH_DIR_INC_ECS}/Entity/
	
	) 
	
