cmake_minimum_required(VERSION 3.19)
project(MakeDamaGreatAgain)

set(CMAKE_CXX_STANDARD 14)

add_executable(MakeDamaGreatAgain main.cpp board/board.cpp
        board/board.h square/square.h square/square.cpp render/render.h
        render/render.cpp engine/engine.cpp engine/engine.h enums/ColumnNotatiton.h
        enums/SquareColor.h move/move.cpp move/move.h player/player.cpp player/player.h
        enums/PlayerColor.h coords/coords.cpp coords/coords.h enums/MoveData.h
        enums/GameStyle.h enums/MoveType.h ui/ui.cpp ui/ui.h
        piece/Piece.cpp piece/Piece.h color/color.cpp color/color.h rendersquare/rendersquare.cpp
        rendersquare/rendersquare.h constants/board_assets.h constants/number_constants.h colorMatrix/colorMatrix.cpp
        colorMatrix/colorMatrix.h gamehandler/gamehandler.cpp gamehandler/gamehandler.h enums/cliCases.h
        apium/apium/apium.cpp apium/apium/apium.h enums/MoveIssue.h apium/a_piece/a_piece.h apium/playstyle/playstyle.h fileHandler/fileHandler.cpp
        fileHandler/fileHandler.h enums/fileIssue.h commandHandler/commandHandler.cpp commandHandler/commandHandler.h
        helpPages/helpPages.h helpPages/helpPages.cpp boardPos/boardPos.cpp boardPos/boardPos.h
        enums/playstyle.h apium/apiumline/apiumline.cpp apium/apiumline/apiumline.h)