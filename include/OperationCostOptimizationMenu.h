#ifndef P1_OPERATIONCOSTOPTIMIZATIONMENU_H
#define P1_OPERATIONCOSTOPTIMIZATIONMENU_H

#include "Menu.h"

/**
 * Menu onde o utilizador poderá escolher uma estação de partida e de chegada e ver o caminho com o menor custo
 * possível que resulte no maior número possível de comboios
 */
class OperationCostOptimizationMenu : public Menu {
public:
    OperationCostOptimizationMenu(std::weak_ptr<RailwayManager> railwayManager);

    /**
     * Vai criar uma interface de texto no terminal onde o utilizador vai ter a opção de escolher uma estação
     * de partida e uma de destino e ver qual é o caminho com o menor custo que dê o máximo fluxo possível.
     * É mais no contexto de transporte de mercadorias sendo que não tem em consideração que o máximo nº de arestas
     * deve estar ocupado se fosse no contexto de passangeiros
     * @return true
     */
    bool execute() override;
};


#endif //P1_OPERATIONCOSTOPTIMIZATIONMENU_H
