#ifndef RAILWAY_NETWORK
#define RAILWAY_NETWORK

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <memory>
#include <tuple>
#include <unordered_set>
#include <stack>
#include <set>
#include <map>

#include "StationTrack.h"

struct StationHash {
    std::size_t operator()(std::shared_ptr<Station> const& station) const noexcept {
        return std::hash<std::string>()(station->getName());
    }
};

struct StationHashEquality {
    bool operator() (std::shared_ptr<Station> const& s1, std::shared_ptr<Station> const& s2) const noexcept {
        return s1->getName() == s2->getName();
    }
};

/**
 * Classe que irá guardar todas as informações e métodos para manipular o grafo que representa a rede rodoviária
 * carregada pelo utilizador
 */
class RailwayNetwork {
public:
    /**
     * Simplesmente insere um apontador para uma estação na tabela de dispersão que tem as estações
     * @param station Apontador para a estação a adicionar
     * @return Retorna sempre verdadeiro a não ser que a inserção falhe
     */
    bool addStation(std::shared_ptr<Station> station);

    /**
     * Adiciona uma linha no vetor adj do station_src, assim como também adiciona essa mesma linha ao vetor incoming
     * do station_dest
     * @param station_src
     * @param station_dest
     * @param service "ALFA PENDULAR" ou "STANDARD"
     * @param w A capacidade dessa linha
     * @param cost
     * @return Falso caso os apontadores sejam NULL. Caso contrário, retorna verdadeiro.
     */
    bool addTrack(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, std::string service, double w, int cost);

    /**
     * Esta função não tem nada de especial, é um getter normal
     */
    std::unordered_set<std::shared_ptr<Station>, StationHash, StationHashEquality> getStationSet() const;

    /**
     * Utilizado para percorrer o caminho entre uma partida e uma chegada e colocar todos os pais de cada aresta
     * no atributo multiple_parents_path de cada nó.
     * Esta função é util quando se pretende reconstruir o caminho pelos pais de cada nó
     * @param src
     * @param dest
     * @param flow_min_limit O número que serve como limite numérico para determinar se o BFS ignora ou não uma linha
     * mediante o seu fluxo
     */
    void setPathBFS(Station *src, Station *dest, double flow_min_limit);

    /**
     * Uma variação do BFS para encontrar o caminho mais curto de peso 1 entre um source e um destino
     * É maioritariamente utilizada como função de pathfinding no algoritmo do edmonds karp
     * Complexidade Temporal: O(|V| + |E|) (pior dos casos)
     * @param station_src
     * @param station_dest
     * @return Verdadeiro se encontrar um caminho por onde é possível aumentar fluxo entre uma partida e uma chegada.
     * Falso, caso contrário
     */
    bool findAugmentingPathBFS(const std::shared_ptr<Station> &station_src, const std::shared_ptr<Station> &station_dest, bool ignoreIsActive);

    /**
     * Uma variação do Dijkstra Single Source Shortest Path para um determinado destino.
     * É utilizado para encontrar um caminho por onde se pode aumentar fluxo no contexto de contrar o fluxo máximo
     * pelo custo mínimo
     * @param station_src
     * @param station_dest
     * @return Verdadeiro se encontrar um caminho por onde é possível aumentar fluxo entre uma partida e uma chegada. Falso, caso contrário
     */
    bool findAugmentingPathDijkstra(const std::shared_ptr<Station> &station_src, const std::shared_ptr<Station> &station_dest);

    /**
     * Aplicação sem muitas alterações do algoritmo do edmonds karp para encontrar o fluxo máximo
     * entre uma estação de chegada e outra de partida
     * Complexidade temporal: O(VE^2)
     * @param station_src
     * @param station_dest
     * @return O resultado do fluxo máximo
     */
    double edmondsKarp(const std::shared_ptr<Station> &station_src, const std::shared_ptr<Station> &station_dest, bool isIgnoreIsActive);

    /**
     *
     * @param queue
     * @param track
     * @param station
     * @param residual
     */
    static void testAndVisit(std::queue<std::shared_ptr<Station>> &queue, const std::shared_ptr<Track>& track,
                      const std::shared_ptr<Station>& station, double residual, bool ignoreIsActive);

    /**
     *
     * @param station_src
     * @param station_dest
     * @return
     */
    static double findMinResidual(const std::shared_ptr<Station> &station_src, std::shared_ptr<Station> station_dest);

    /**
     *
     * @param station_src
     * @param station_dest
     * @param minRes
     * @return
     */
    static int updatePath(const std::shared_ptr<Station> &station_src, std::shared_ptr<Station> station_dest, double minRes);

    /**
     *
     * @param dest
     * @return
     */
    double maxTrainsTo(const std::shared_ptr<Station> &dest);

    /**
     * Serve para conectar uma mock source a todos os nós que sejam sources individuais.
     * Isto é usado para os algoritmos de cálculo de fluxo máximo onde queremos considerar as várias sources
     * ao mesmo tempo
     * Complexidade temporal: O(|V| + |E|), no pior dos casos
     * @param mock_source
     */
    void connectSourceNodesTo(Station *mock_source);

    /**
     * Serve para conectar todos os nós que sejam destinos individualmente a um destino geral que será um mock sink
     * Isto é usado para os algoritmos de cálculo de fluxo máximo onde queremos
     * @param mock_sink
     */
    void connectSinkNodesTo(std::shared_ptr<Station> mock_sink);

    void eraseEdgesFromMockSource(Station *mock_source);

    void deactivateTrack(const std::shared_ptr<Track> &track);
    void deactivateStation(const std::shared_ptr<Station> &station);
    void undoLastDeletion();
    void undoAllDeletions();

    void clearNetworkUtils();

    void stationsInConnectedPath(Station *station_src, Station *station_dest);
    int findMaxFlowMinCost(const std::shared_ptr<Station> &src, const std::shared_ptr<Station> &dest, int &flow_result);

    bool testAndVisitDijkstra(std::queue<Station*> &queue, std::shared_ptr<Track> track, Station* u, Station* v, double residual, bool isDest);

    void resetFlow();

    std::vector<std::shared_ptr<Station>> mostAffectedStations(int k);
    std::set<std::pair<std::shared_ptr<Station>, std::shared_ptr<Station>>> mostUsedPairsStations();
    std::vector<std::pair<std::string, std::pair<double,double>>> topRegionsByNeeds(int k, bool isDistrict);
protected:

    std::unordered_set<std::shared_ptr<Station>, StationHash, StationHashEquality> stationSet;

    std::stack<std::shared_ptr<Track>> inactiveTracks;
    std::stack<std::shared_ptr<Station>> inactiveStations;
    std::stack<char> deletionRecord;
};

#endif