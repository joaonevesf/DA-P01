#ifndef P1_BASICSERVICESMENU_H
#define P1_BASICSERVICESMENU_H

#include "Menu.h"

/**
 * Menu onde o utilizador pode determinar o fluxo entre duas estações por ele especificadas, os pares de estações com mais fluxo,
 * os concelhos ou distritos com maior razão fluxo por capacidade, ou o máximo número de comboios que podem chegar a uma estação especifica
 */
class BasicServicesMenu : public Menu {
public:
    BasicServicesMenu(std::weak_ptr<RailwayManager> railwayManager);

    /**
     * Mostra as opções do menu e aceita. Volta para o MainMenu caso seja selecionada a opção go back
     * @return false caso ocorra um erro, true caso contrário
     */
    bool execute() override;
};

#endif
