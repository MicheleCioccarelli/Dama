#pragma once

#include "../render/render.h"

namespace HelpPages {
    // Called when the executable receives wrong command line arguments
    void cli_help_page();

    // Called when in a game someone types "aiuto", shows general syntax
    void help_page();

    // Called when someone types "comandi", shows commands utilities
    void commands_help_page();
}
