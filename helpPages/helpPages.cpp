#include "helpPages.h"


void HelpPages::cli_help_page() {
    std::cout << "\nComandi:" << "\n";
    std::cout << "-apium";
    RenderV2::padding(10);
    std::cout << "| Gioca contro Apium" << "\n";
    std::cout << "Nessun comando";
    RenderV2::padding(2);
    std::cout << "| Gioca contro un'altra persona" << "\n";
}

void HelpPages::help_page() {
    std::cout << "\n";
    for (int i = 1; i < 22; i++) {
        std::cout << "════";
    }
    std::cout << "\n" << CYN <<"Pagina di aiuto" << RESET << "\n\n";
    std::cout << "Sintassi: " << MAG << "[colonna]" << CYN <<"[riga]" << RESET << "[operatore]" << MAG <<
              "[colonna]" << CYN << "[riga]" << RESET << "\n\n";
    std::cout << "Per esempio per muoversi da A3 a B4: " << MAG <<"A" << CYN "3" << RESET "-" << MAG << "B" << CYN << "4" << RESET << std::endl;

    std::cout << "\n" << "Colonne: " << MAG << "A, B, C, D, E, F, G, H" << RESET << "\n";
    std::cout << "Righe:   " << CYN << "1, 2, 3, 4, 5, 6, 7, 8" << RESET << "\n\n";

    std::cout << "Operatori:\n";

    RenderV2::padding(3);
    std::cout << MOVE_COLOR << "-" << RESET;
    RenderV2::padding(3);
    std::cout << "Movimento, cambia casella di 1 alla volta";
    RenderV2::padding(8);
    std::cout << "|  Sintassi: A3" << MOVE_COLOR << "-" << RESET << "B4" << "\n";

    RenderV2::padding(3);
    std::cout << EAT_COLOR << "x" << RESET;
    RenderV2::padding(3);
    std::cout << "Mangiare";
    RenderV2::padding(41);
    std::cout << "|  Sintassi: A3" << EAT_COLOR "x" << RESET << "B4" << "\n";

    RenderV2::padding(3);
    std::cout << BLOW_COLOR << "*" << RESET;
    RenderV2::padding(3);
    std::cout << "Soffio, deve essere separato da altri comandi";
    RenderV2::padding(4);
    std::cout << "|  Sintassi: A3" << BLOW_COLOR << "*" << RESET << "B4_C3" << MOVE_COLOR << "-" << RESET << "D4" << "\n";
    RenderV2::padding(7);
    std::cout << "con '_'. Per usarlo bisogna scrivere la mossa";
    RenderV2::padding(4);
    std::cout << "|  Esempio: il nemico non ha fatto A3" << EAT_COLOR << "x" << RESET << "B4," << "\n";
    RenderV2::padding(7);
    std::cout << "che il nemico non ha fatto con un " << BLOW_COLOR << "*" << RESET << " invece di " << EAT_COLOR << "x" << RESET;
    RenderV2::padding(2);
    std::cout << "|  quindi si soffia con A3" << BLOW_COLOR << "*" << RESET << "B4" << "\n";
    RenderV2::padding(7);
    std::cout << "\n";

    std::cout << "Per informazioni sui comandi scrivi \"" << HGRN << "comandi" << RESET << "\"\n";

    for (int i = 1; i < 22; i++) {
        std::cout << "════";
    } std::cout << "\n";
}

void HelpPages::commands_help_page() {
    // Cool padding
    std::cout << "\n";
    for (int i = 1; i < 22; i++) {
        std::cout << "════";
    }

    std::cout << HGRN << "\nPagina di aiuto dei comandi\n\n" << RESET;

    RenderV2::padding(3);
    std::cout << COMMAND_COLOR << "Resign" << RESET;
    RenderV2::padding(21);
    std::cout << "| I"
                 ""
                 "l giocatore che scrive \"resign\" si arrende\n";

    RenderV2::padding(3);
    std::cout << COMMAND_COLOR << "Summary" << RESET;
    RenderV2::padding(20);
    std::cout << "| Si stampano i dettagli della partita\n";

    RenderV2::padding(3);
    std::cout << COMMAND_COLOR << "Draw" << RESET;
    RenderV2::padding(23);
    std::cout << "| Offerta di pareggio che andra accettata o rifiutata\n";

    RenderV2::padding(3);
    std::cout << COMMAND_COLOR << "Quit" << RESET;
    RenderV2::padding(23);
    std::cout << "| Il programma termina l'esecuzione\n";

    RenderV2::padding(3);
    std::cout << COMMAND_COLOR << "MoveOrder" << RESET;
    RenderV2::padding(18);
    std::cout << "| Stampa la sequenza di mosse che hanno originato la posizione corrente\n";

    RenderV2::padding(3);
    std::cout << COMMAND_COLOR << "Save" << RESET;
    RenderV2::padding(23);
    std::cout <<  "| Salvare i contenuti della partita in un file .dama\n";

    RenderV2::padding(3);
    std::cout << COMMAND_COLOR << "LoadBoardPos" << RESET;
    RenderV2::padding(19);
    std::cout <<  "| Sovrascrive la disposizione della posizione corrente con una fornita\n";
    RenderV2::padding(30);
    std::cout << "| dall'utente usando la notazione BoardPos (vedi wiki)\n";

    RenderV2::padding(3);
    std::cout << COMMAND_COLOR << "GenerateBoardPos" << RESET;
    RenderV2::padding(11);
    std::cout <<  "| Stampa la notazione BoardPos per la posizione corrente\n";


    // More cool padding
    std::cout << "\n";
    for (int i = 1; i < 22; i++) {
        std::cout << "════";
    }
    std::cout << "\n";
}

void HelpPages::playback_help_page() {
    std::cout << "\n";
    for (int i = 1; i < 22; i++) {
        std::cout << "════";
    }
    std::cout << "\n" << HGRN <<"Pagina di aiuto del PlayBack" << RESET << "\n\n";

    std::cout << "Comandi:\n";
    std::cout << COMMAND_COLOR << "Switch";
    RenderV2::padding(15);
    std::cout << "| Cambia il punto di vista della scacchiera (bianco o nero)\n";
    std::cout << COMMAND_COLOR << "q" << RESET << " o " << COMMAND_COLOR << "quit";
    RenderV2::padding(6);
    std::cout << "| Termina l'esecuzione del programma\n";

    std::cout << "\n";
    for (int i = 1; i < 22; i++) {
        std::cout << "════";
    }
    std::cout << "\n";
}