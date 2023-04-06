#ifndef P1_MAINMENU_H
#define P1_MAINMENU_H

#include "ReadFileMenu.h"

/**
 * Menu onde o utilizador poderá escolher que funcionalidade quer usar
 */
class MainMenu : public ReadFileMenu {
public:
    MainMenu(std::weak_ptr<RailwayManager> railwayManager);

    /**
     * Mostra as opções e aceita input. Muda para outro menu caso sejam selecionadas as opções 2, 3 ou 4
     * @return false se for selecionada a opção exit, true caso contrário
     */
    bool execute() override;
};

#endif