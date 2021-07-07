cmake_minimum_required(VERSION 3.19)
project(MakeDamaGreatAgain)

set(CMAKE_CXX_STANDARD 14)

add_executable(MakeDamaGreatAgain main.cpp board/board.cpp board/board.h square/square.h square/square.cpp render/render.h render/render.cpp engine/engine.cpp engine/engine.h enums/Piece.h enums/ColumnNotatiton.h enums/SquareColor.h move/move.cpp move/move.h player/player.cpp player/player.h enums/PlayerColor.h enums/BoardStyles.h coords/coords.cpp coords/coords.h enums/MoveReturn.h enums/GameStyle.h enums/MoveType.h ui/ui.cpp ui/ui.h enums/UiReturn.h command/command.cpp command/command.h command/command.cpp command/command.h)