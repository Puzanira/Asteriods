# asteroids-project
### About 

C/C++ course final project. A hands-free controller for playing Asteroids arcade game. :sparkles:

## Build instructions

### Requirements

* CMake 3.9+, can be downloaded from `https://cmake.org/download/`
* SFML 2.4.2, can be downloaded from `https://www.sfml-dev.org/download.php`
* OpenCV 3.3.0, can be downloaded from `https://opencv.org/releases.html`
* any modern compiler or IDE (gcc/clang + make, Visual Studio, Xcode)
  
All of these can be installed via brew/apt/yum (check out the formulas for brew at `http://formulae.brew.sh/search/`).

### Project generation

Assuming, you've cloned the repo to `<dev-dir>/Asteroids`.
In console:

    cd <dev-dir>/Asteroids
    mkdir build
    cd build
    cmake ..
    make

At this point you'll have Asteroids in `<dev-dir>/Asteroids/build/src` depending on your environment. Execute `Asteroids`. Remember, changes made to that project configurations are not saved, because those project files are regenerated every time you clone and/or rerun `cmake` generator. So to make changes permanent, you have to edit the corresponding `CMakeLists.txt` file.

### Directions to play
At first you need to adjust the bars to reach the highest possible way of contrast between your hand and your environment. We suggest that you use a pair of red or green gloves, they seem to work the best. After you have configured everything, go on and play the game!
