cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME BinaryRenderer)
project(${PROJECT_NAME} VERSION 1.0)


set(CMAKE_CXX_STANDARD 17)

add_executable(BinaryRenderer
		Source/main.cpp

		Source/GP_ONE.h
		Source/GP_ONE.cpp

		Source/Sprite.h
		Source/Sprite.cpp
		Source/FrameBuffer.h
		Source/FrameBuffer.cpp
		Source/SpriteInstance.h
		Source/Scene.h

		External/Clock.h
		External/Clock.cpp
		External/ImageManager.h
		External/ImageManager.cpp
		External/SceneLoader.h
		External/SceneLoader.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}/External
	${PROJECT_SOURCE_DIR}/Source
)

file(COPY ${PROJECT_SOURCE_DIR}/Sprites DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/Scenes  DESTINATION ${PROJECT_BINARY_DIR})
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Render)