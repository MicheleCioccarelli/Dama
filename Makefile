# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/sedano/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/212.5284.51/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/sedano/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/212.5284.51/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sedano/CLionProjects/Dama

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sedano/CLionProjects/Dama

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/home/linuxbrew/.linuxbrew/Homebrew/Cellar/cmake/3.19.6/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/home/sedano/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/212.5284.51/bin/cmake/linux/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/sedano/CLionProjects/Dama/CMakeFiles /home/sedano/CLionProjects/Dama//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/sedano/CLionProjects/Dama/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named MakeDamaGreatAgain

# Build rule for target.
MakeDamaGreatAgain: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 MakeDamaGreatAgain
.PHONY : MakeDamaGreatAgain

# fast build rule for target.
MakeDamaGreatAgain/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/build
.PHONY : MakeDamaGreatAgain/fast

apium/scoredmove/scoredMove.o: apium/scoredmove/scoredMove.cpp.o
.PHONY : apium/scoredmove/scoredMove.o

# target to build an object file
apium/scoredmove/scoredMove.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/apium/scoredmove/scoredMove.cpp.o
.PHONY : apium/scoredmove/scoredMove.cpp.o

apium/scoredmove/scoredMove.i: apium/scoredmove/scoredMove.cpp.i
.PHONY : apium/scoredmove/scoredMove.i

# target to preprocess a source file
apium/scoredmove/scoredMove.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/apium/scoredmove/scoredMove.cpp.i
.PHONY : apium/scoredmove/scoredMove.cpp.i

apium/scoredmove/scoredMove.s: apium/scoredmove/scoredMove.cpp.s
.PHONY : apium/scoredmove/scoredMove.s

# target to generate assembly for a file
apium/scoredmove/scoredMove.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/apium/scoredmove/scoredMove.cpp.s
.PHONY : apium/scoredmove/scoredMove.cpp.s

board/board.o: board/board.cpp.o
.PHONY : board/board.o

# target to build an object file
board/board.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/board/board.cpp.o
.PHONY : board/board.cpp.o

board/board.i: board/board.cpp.i
.PHONY : board/board.i

# target to preprocess a source file
board/board.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/board/board.cpp.i
.PHONY : board/board.cpp.i

board/board.s: board/board.cpp.s
.PHONY : board/board.s

# target to generate assembly for a file
board/board.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/board/board.cpp.s
.PHONY : board/board.cpp.s

color/color.o: color/color.cpp.o
.PHONY : color/color.o

# target to build an object file
color/color.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/color/color.cpp.o
.PHONY : color/color.cpp.o

color/color.i: color/color.cpp.i
.PHONY : color/color.i

# target to preprocess a source file
color/color.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/color/color.cpp.i
.PHONY : color/color.cpp.i

color/color.s: color/color.cpp.s
.PHONY : color/color.s

# target to generate assembly for a file
color/color.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/color/color.cpp.s
.PHONY : color/color.cpp.s

colorMatrix/colorMatrix.o: colorMatrix/colorMatrix.cpp.o
.PHONY : colorMatrix/colorMatrix.o

# target to build an object file
colorMatrix/colorMatrix.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/colorMatrix/colorMatrix.cpp.o
.PHONY : colorMatrix/colorMatrix.cpp.o

colorMatrix/colorMatrix.i: colorMatrix/colorMatrix.cpp.i
.PHONY : colorMatrix/colorMatrix.i

# target to preprocess a source file
colorMatrix/colorMatrix.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/colorMatrix/colorMatrix.cpp.i
.PHONY : colorMatrix/colorMatrix.cpp.i

colorMatrix/colorMatrix.s: colorMatrix/colorMatrix.cpp.s
.PHONY : colorMatrix/colorMatrix.s

# target to generate assembly for a file
colorMatrix/colorMatrix.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/colorMatrix/colorMatrix.cpp.s
.PHONY : colorMatrix/colorMatrix.cpp.s

command/command.o: command/command.cpp.o
.PHONY : command/command.o

# target to build an object file
command/command.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/command/command.cpp.o
.PHONY : command/command.cpp.o

command/command.i: command/command.cpp.i
.PHONY : command/command.i

# target to preprocess a source file
command/command.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/command/command.cpp.i
.PHONY : command/command.cpp.i

command/command.s: command/command.cpp.s
.PHONY : command/command.s

# target to generate assembly for a file
command/command.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/command/command.cpp.s
.PHONY : command/command.cpp.s

coords/coords.o: coords/coords.cpp.o
.PHONY : coords/coords.o

# target to build an object file
coords/coords.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/coords/coords.cpp.o
.PHONY : coords/coords.cpp.o

coords/coords.i: coords/coords.cpp.i
.PHONY : coords/coords.i

# target to preprocess a source file
coords/coords.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/coords/coords.cpp.i
.PHONY : coords/coords.cpp.i

coords/coords.s: coords/coords.cpp.s
.PHONY : coords/coords.s

# target to generate assembly for a file
coords/coords.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/coords/coords.cpp.s
.PHONY : coords/coords.cpp.s

engine/engine.o: engine/engine.cpp.o
.PHONY : engine/engine.o

# target to build an object file
engine/engine.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/engine/engine.cpp.o
.PHONY : engine/engine.cpp.o

engine/engine.i: engine/engine.cpp.i
.PHONY : engine/engine.i

# target to preprocess a source file
engine/engine.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/engine/engine.cpp.i
.PHONY : engine/engine.cpp.i

engine/engine.s: engine/engine.cpp.s
.PHONY : engine/engine.s

# target to generate assembly for a file
engine/engine.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/engine/engine.cpp.s
.PHONY : engine/engine.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/main.cpp.s
.PHONY : main.cpp.s

move/move.o: move/move.cpp.o
.PHONY : move/move.o

# target to build an object file
move/move.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/move/move.cpp.o
.PHONY : move/move.cpp.o

move/move.i: move/move.cpp.i
.PHONY : move/move.i

# target to preprocess a source file
move/move.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/move/move.cpp.i
.PHONY : move/move.cpp.i

move/move.s: move/move.cpp.s
.PHONY : move/move.s

# target to generate assembly for a file
move/move.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/move/move.cpp.s
.PHONY : move/move.cpp.s

movecase/movecase.o: movecase/movecase.cpp.o
.PHONY : movecase/movecase.o

# target to build an object file
movecase/movecase.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/movecase/movecase.cpp.o
.PHONY : movecase/movecase.cpp.o

movecase/movecase.i: movecase/movecase.cpp.i
.PHONY : movecase/movecase.i

# target to preprocess a source file
movecase/movecase.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/movecase/movecase.cpp.i
.PHONY : movecase/movecase.cpp.i

movecase/movecase.s: movecase/movecase.cpp.s
.PHONY : movecase/movecase.s

# target to generate assembly for a file
movecase/movecase.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/movecase/movecase.cpp.s
.PHONY : movecase/movecase.cpp.s

piece/Piece.o: piece/Piece.cpp.o
.PHONY : piece/Piece.o

# target to build an object file
piece/Piece.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/piece/Piece.cpp.o
.PHONY : piece/Piece.cpp.o

piece/Piece.i: piece/Piece.cpp.i
.PHONY : piece/Piece.i

# target to preprocess a source file
piece/Piece.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/piece/Piece.cpp.i
.PHONY : piece/Piece.cpp.i

piece/Piece.s: piece/Piece.cpp.s
.PHONY : piece/Piece.s

# target to generate assembly for a file
piece/Piece.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/piece/Piece.cpp.s
.PHONY : piece/Piece.cpp.s

player/player.o: player/player.cpp.o
.PHONY : player/player.o

# target to build an object file
player/player.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/player/player.cpp.o
.PHONY : player/player.cpp.o

player/player.i: player/player.cpp.i
.PHONY : player/player.i

# target to preprocess a source file
player/player.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/player/player.cpp.i
.PHONY : player/player.cpp.i

player/player.s: player/player.cpp.s
.PHONY : player/player.s

# target to generate assembly for a file
player/player.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/player/player.cpp.s
.PHONY : player/player.cpp.s

render/render.o: render/render.cpp.o
.PHONY : render/render.o

# target to build an object file
render/render.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/render/render.cpp.o
.PHONY : render/render.cpp.o

render/render.i: render/render.cpp.i
.PHONY : render/render.i

# target to preprocess a source file
render/render.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/render/render.cpp.i
.PHONY : render/render.cpp.i

render/render.s: render/render.cpp.s
.PHONY : render/render.s

# target to generate assembly for a file
render/render.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/render/render.cpp.s
.PHONY : render/render.cpp.s

rendersquare/rendersquare.o: rendersquare/rendersquare.cpp.o
.PHONY : rendersquare/rendersquare.o

# target to build an object file
rendersquare/rendersquare.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/rendersquare/rendersquare.cpp.o
.PHONY : rendersquare/rendersquare.cpp.o

rendersquare/rendersquare.i: rendersquare/rendersquare.cpp.i
.PHONY : rendersquare/rendersquare.i

# target to preprocess a source file
rendersquare/rendersquare.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/rendersquare/rendersquare.cpp.i
.PHONY : rendersquare/rendersquare.cpp.i

rendersquare/rendersquare.s: rendersquare/rendersquare.cpp.s
.PHONY : rendersquare/rendersquare.s

# target to generate assembly for a file
rendersquare/rendersquare.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/rendersquare/rendersquare.cpp.s
.PHONY : rendersquare/rendersquare.cpp.s

square/square.o: square/square.cpp.o
.PHONY : square/square.o

# target to build an object file
square/square.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/square/square.cpp.o
.PHONY : square/square.cpp.o

square/square.i: square/square.cpp.i
.PHONY : square/square.i

# target to preprocess a source file
square/square.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/square/square.cpp.i
.PHONY : square/square.cpp.i

square/square.s: square/square.cpp.s
.PHONY : square/square.s

# target to generate assembly for a file
square/square.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/square/square.cpp.s
.PHONY : square/square.cpp.s

ui/ui.o: ui/ui.cpp.o
.PHONY : ui/ui.o

# target to build an object file
ui/ui.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/ui/ui.cpp.o
.PHONY : ui/ui.cpp.o

ui/ui.i: ui/ui.cpp.i
.PHONY : ui/ui.i

# target to preprocess a source file
ui/ui.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/ui/ui.cpp.i
.PHONY : ui/ui.cpp.i

ui/ui.s: ui/ui.cpp.s
.PHONY : ui/ui.s

# target to generate assembly for a file
ui/ui.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/MakeDamaGreatAgain.dir/build.make CMakeFiles/MakeDamaGreatAgain.dir/ui/ui.cpp.s
.PHONY : ui/ui.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... MakeDamaGreatAgain"
	@echo "... apium/scoredmove/scoredMove.o"
	@echo "... apium/scoredmove/scoredMove.i"
	@echo "... apium/scoredmove/scoredMove.s"
	@echo "... board/board.o"
	@echo "... board/board.i"
	@echo "... board/board.s"
	@echo "... color/color.o"
	@echo "... color/color.i"
	@echo "... color/color.s"
	@echo "... colorMatrix/colorMatrix.o"
	@echo "... colorMatrix/colorMatrix.i"
	@echo "... colorMatrix/colorMatrix.s"
	@echo "... command/command.o"
	@echo "... command/command.i"
	@echo "... command/command.s"
	@echo "... coords/coords.o"
	@echo "... coords/coords.i"
	@echo "... coords/coords.s"
	@echo "... engine/engine.o"
	@echo "... engine/engine.i"
	@echo "... engine/engine.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... move/move.o"
	@echo "... move/move.i"
	@echo "... move/move.s"
	@echo "... movecase/movecase.o"
	@echo "... movecase/movecase.i"
	@echo "... movecase/movecase.s"
	@echo "... piece/Piece.o"
	@echo "... piece/Piece.i"
	@echo "... piece/Piece.s"
	@echo "... player/player.o"
	@echo "... player/player.i"
	@echo "... player/player.s"
	@echo "... render/render.o"
	@echo "... render/render.i"
	@echo "... render/render.s"
	@echo "... rendersquare/rendersquare.o"
	@echo "... rendersquare/rendersquare.i"
	@echo "... rendersquare/rendersquare.s"
	@echo "... square/square.o"
	@echo "... square/square.i"
	@echo "... square/square.s"
	@echo "... ui/ui.o"
	@echo "... ui/ui.i"
	@echo "... ui/ui.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

