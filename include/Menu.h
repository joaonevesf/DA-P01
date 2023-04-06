#ifndef P1_MENU_H
#define P1_MENU_H

#include <string>
#include <memory>
#include <vector>
#include <queue>

#include "RailwayManager.h"
#include "StationTrack.h"

class RailwayManager;
class Station;

class Menu {
public:
    Menu(std::weak_ptr<RailwayManager> railwayManager);

    virtual bool execute() = 0;
protected:
    /**
     * Este atributo é utilizado uma vez que o RailwayManager vai ter uma RailwayNetwork válida que terá de ser
     * manipulada ou simplesmente visitada pelos menus à medida que o utiliziador pede informações ou altera a
     * rede
     */
    std::weak_ptr<RailwayManager> railwayManager_;

    /**
     * Imprime length traços.
     * Não imprime nenhum tipo de caratér de quebra de linha
     * @param length number of dashes to print
     */
    void printDashes(int length);

    /**
     * Serve como uma função para pedir ao utilizador para inserir um valor numérico.
     * É por isso um wrapper para o std::cin
     * @param selectedOption A variável para onde vai ficar guardado o número inserido pelo utilizador. Só deve ser
     * utilizado se a função retornar verdadeiro
     * @return Verdadeiro se o valor inserido é valido numericamente e falso caso contrário
     */
    static bool getNumericalInput(int &selectedOption);

    /**
     * Obtém um input do utilizador utilizando o std::getline
     * @param s A string que se deve passar para ficar com o valor inserido pelo utilizador
     * @return Só retornará falso em ocasiões muito especiais quando por algum motivo ler uma string do utilizador
     * invoca um erro no std::getline
     */
    static bool getStringInput(std::string &s);

    /**
     * Serve para pedir ao utilizador para inserir um número e não termina até o utilizador inserir um número
     * entre 1 e numberOfOptions
     * @param numberOfOptions
     * @return O número inserido pelo utilizador que deve ser inferior a numberOfOptions
     */
    static int readOption(int numberOfOptions);

    /**
     * A partir de uma estação de destino vai percorrer os pais dele e das arestas adjacentes recursivamente
     * imprimindo cada uma das arestas
     * Complexidade temporal: O(|V| + |E|), sendo V o nº de estações e E o nº de linhas. No pior dos casos é um caminho
     * em que liga todos os nós
     * @param dest
     */
    void printPaths(Station *dest);

    std::shared_ptr<Station> getStation(const std::string& name);
};

#endif
