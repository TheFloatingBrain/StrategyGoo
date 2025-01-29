cmake_minimum_required(VERSION 3.29)
project(Bubbles CXX)
set(CMAKE_CXX_STANDARD 23)
include(FetchContent)
# Need libvorbis oggvorbis libflac and probably opengl
FetchContent_Declare(
	sfml
	GIT_REPOSITORY "https://github.com/SFML/SFML.git"
	GIT_TAG "2.6.2"
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(sfml)

#add_compile_options(-fconcepts-diagnostics-depth=10)
file(GLOB SOURCES Source/*.cpp)
include_directories(Include Include/External)
add_executable(BioGooContainmentSquadron ${SOURCES})
target_link_libraries(BioGooContainmentSquadron PRIVATE 
	sfml-window
	sfml-graphics
	sfml-audio
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

