#ifndef P1_LINEFAILURESMENU_H
#define P1_LINEFAILURESMENU_H

#include "Menu.h"

/**
 * Este menu serve para ativar/desativar linhas ou estações, representando falhas na rede, assim como mostrar as estações mais afetadas por essas falhas
 */
class LineFailuresMenu: public Menu {
public:
    LineFailuresMenu(std::weak_ptr<RailwayManager> railwayManager);
    /**
     * Cria uma interface de texto no terminal por onde o utilizador pode escolher várias opçoes,
     * e muda de menu caso estas sejam 1,2 ou 3
     * @return false se acontecer algum erro, true caso contrário
     */
    bool execute() override;
    /**
     * Cria uma interface de texto para o utilizador poder desativar uma linha ou estação, opção 1 para linha e opção 2 para estação
     * @return falso se a opção for "Go back", true caso contrário
     */
    bool deleteTrackStationMenu();
    /**
     * Recebe inputs do utilizador para desativar uma linha e, se os inputs forem válidos, desativa a mesma
     */
    void deleteTrack();
    /**
     * Recebe inputs do utilizador para desativar uma estação e, se os inputs forem válidos, desativa a mesma
     */
    void deleteStation();
    /**
     * Recebe um input k do utilizador e depois imprime as k estações mais afetadas pelas falhas no terminal
     */
    void listAffectedStations();
    /**
     * Cria uma interface de texto onde o utilizador pode desfazer as suas ações anteriores, de modo a ativar linhas e
     * estações que se encontravam desativadas: 1 para a última ação e 2 para todas as ações
     * @return false se a opção escolhida for "Go back", true caso contrário
     */
    bool undoActionsMenu();
};


#endif //P1_LINEFAILURESMENU_H
