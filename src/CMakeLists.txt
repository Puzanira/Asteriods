# cmake_minimum_required(VERSION 3.9)

if(APPLE)
	# brew version of SFML keeps FindSFML.cmake in extra Modules/ dir, so it is not found during conventional lookup...
	list(APPEND CMAKE_MODULE_PATH "/usr/local/share/SFML/cmake/Modules")
endif()

set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package(OpenCV REQUIRED)
find_package(SFML REQUIRED system window graphics network audio)

add_executable(Asteroids
	Asteroid.cpp
	Bullet.cpp
	Entity.cpp
	Game.cpp
	GameOverState.cpp
	GameState.cpp
	GameplayState.cpp
	HighscoresState.cpp
	Hud.cpp
	MainMenuState.cpp
	Ship.cpp
	World.cpp
	Controller.cpp
	Audio.cpp
	main.cpp

# Adding headers just for consistency/IDE.
	Asteroid.h
	Bullet.h
	Entity.h
	Game.h
	GameOverState.h
	GameState.h
	GameplayState.h
	HighscoresState.h
	Hud.h
	MainMenuState.h
	Ship.h
	World.h
	Controller.h
	Audio.h
)

target_include_directories(Asteroids PRIVATE ${OpenCV_INCLUDE_DIR})
target_link_libraries(Asteroids ${OpenCV_LIBS})

TARGET_LINK_LIBRARIES(Asteroids -pthread ${LIBS})

target_include_directories(Asteroids PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(Asteroids ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
