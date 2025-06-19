# HeroDungeonSim 
## How to Build
Create + enter build directory:
```
cd <RepoRoot>
mkdir build
cd build/
```
Run CMake and Make
```
# 1. CMake (Cross-Platform)
cmake ../

# 2A. Compile (Mac/Linux)
make

# 2B. Compile (Windows)
msbuild HeroDungeonSim.sln
```
## How to Run
Default execution
```
# Run (Mac/Linux, from build directory)
./Game

# Run (Windows, from build directory)
Debug\Game.exe
```
Additional execution flags
```
# Ex. Mac/Linux with 100 Dungeon attempts
./Game 100

# Ex. Windows with 700 Dungeon attempts
Debug\Game.exe 700
```
## Log Output
TBD