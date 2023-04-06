#ifndef P1_READFILEMENU_H
#define P1_READFILEMENU_H

#define FILES_FOLDER "../Project1Data/"

#include <fstream>

#include "Menu.h"
#include "FileManager.h"

/**
 * Este menu é para servir como interface para o utilizador conseguir escolher os ficheiros de onde el@ pretende
 * carregar a informação sobre a rede
 */
class ReadFileMenu : public Menu {
public:
    ReadFileMenu(std::weak_ptr<RailwayManager> railwayManager);

    /**
     * Vai criar uma interface de texto no terminal por onde o utilizador poderá escolher os ficheiros que poderá
     * carregar.
     * @return Falso quando o menu fechar. O railwayManager tem um menu e mostra o menu até o menu retornar falso,
     * depois mostra outro que este menu irá carregar para o RailwayManager
     */
    bool execute() override;
protected:
    /**
     * Serve para dar informação ao utilizador sobre os ficheiros que pode carregar e invoca os métodos
     * para efetivamentne ler esses ficheiros
     * @param fm O file manager será a classe que irá manipular diretamente os ficheiros e carregar na rede o que
     * for necessário
     * @return Verdadeiro se a escolha dos ficheiros para ler as estações e as linhas forem válidas
     */
    bool showFileReadOptions(FileManager &fm);

    /**
     * Vai agir como middleman entre o showFileReadOptions e o chooseFiles e chooseEdgesFile, chamando estes
     * dois últimos métodos
     * @param fm O file manager será a classe que irá manipular diretamente os ficheiros e carregar na rede o que
     * @return Verdadeiro se as leituras foram feitas com sucesso
     */
    bool chooseFiles(FileManager &fm);

    /**
     * Pede ao utilizador para ler o ficheiro por onde iremos carregar as linhas e chama as respetivas funções do
     * FileManager que interagem com a RailwayNetwork para adicionar essas arestas
     * @param fm O file manager será a classe que irá manipular diretamente os ficheiros e carregar na rede o que
     * @return Verdadeiro se conseguir ler o ficheiro com sucesso. Caso não consiga, estará sempre a repetir o pedido
     */
    bool chooseEdgesFile(FileManager &fm);

    /**
     * Pede ao utilizador para ler o ficheiro de onde iremos carregar as estaçõe ee chama as respetivas funções do
     * FileManager que interagem com a RailwayNetwork para adicionar essas estaçãos ao stationSet.
     * @param fm O file manager será a classe que irá manipular diretamente os ficheiros e carregar na rede o que
     * @return Verdadeiro se conseguir ler o ficheiro com sucesso. Caso não consiga, estará sempre a repetir o pedido
     */
    bool chooseStationsFile(FileManager &fm);
};

#endif
