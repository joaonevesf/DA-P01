#ifndef P1_FILEMANAGER_H
#define P1_FILEMANAGER_H

#include <string>
#include <memory>

#include "RailwayNetwork.h"

/**
 * Esta classe é usada como middleman para acedermos a ficheiros e manipular a @railwayNetwork a partir dos dados
 * desses mesmos ficheiros
 */
class FileManager {
public:
    FileManager(std::shared_ptr<RailwayNetwork> railwayNetwork);

    /**
     * Vai ao ficheiro indicado por @filepath para o tentar abrir e depois popula o stationSet do RailwayNetwork que lhe foi atribuído.
     * Para usar corretamente esta função é necessário que as estações estejam organizadas por linhas com o seguinte formato
     * Name,District,Municipality,Township,Line
     * @param filepath Ficheiro para abrir
     * @return Falso caso a abertura do ficheiro falhe de alguma forma
     */
    bool readStations(std::string filepath);

    /**
     * Vai ao ficheiro indicado por @filepath para o tentar abrir e depois popula os vetores de adjacências e de incomings
     * das estações do RailwayNetwork que lhe foi atribuído.
     * Para usar corretamente esta função é necessário que as conexões entre as estações estejam organizadas por linhas com o seguinte formato
     * Station_A,Station_B,Capacity,Service
     * @param filepath Ficheiro para abrir
     * @return Falso caso a abertura do ficheiro falhe de alguma forma
     */
    bool readNetworkEdges(std::string filepath);

private:
    /**
     * Apontador para uma RailwayNetwork que esta classe vai poder alterar
     */
    std::shared_ptr<RailwayNetwork> railwayNetwork;
};

#endif
