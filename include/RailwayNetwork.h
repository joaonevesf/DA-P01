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
     * Complexidade temporal: O(|V| + |E|), no pior dos casos o src e o dest estão entre os nós todos
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
     * Complexidade temporal: O(Elog(V))
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
     * Função utilizada no contexto de encontrar um agumenting path no problema do fluxo máximo para verificar
     * se se pode adicionar fluxo por uma aresta ou pelo reverso dela ou não
     * Complexidade Temporal: O(1)
     * @param queue Será uma fila para onde o testAndVisit irá carregar as estações que estarão no caminho
     * @param track
     * @param station
     * @param residual
     */
    static void testAndVisit(std::queue<std::shared_ptr<Station>> &queue, const std::shared_ptr<Track>& track,
                      const std::shared_ptr<Station>& station, double residual, bool ignoreIsActive);

    /**
     * Percorre um caminho encontrado pelo @augmentingPathBFS por onde foi atualizado e encontra o mínimo de capacidade
     * restante entre todas as arestas entre o destino e partida
     * Complexidade Temporal: O(|A|), sendo |A| o nº de arestas do caminho entre um destino e uma partida
     * @param station_src
     * @param station_dest
     * @return
     */
    static double findMinResidual(const std::shared_ptr<Station> &station_src, std::shared_ptr<Station> station_dest);

    /**
     * Deve ser corrida apenas depois do @findMinResidual e do @findAugmentingPathBFS
     * Serve para pegar no resultado do findMinResidual e atualizar o fluxo em cada aresta
     * Complexidade temporal: O(|E|) no pior dos casos
     * @param station_src
     * @param station_dest
     * @param minRes
     * @return
     */
    static int updatePath(const std::shared_ptr<Station> &station_src, std::shared_ptr<Station> station_dest, double minRes);

    /**
     * Cria uma mock source e liga-as a todas as outras sources individuais e corre o edmonds karp começando no mock source
     * para encontrar o número máximo de comboios que pode chegar ao dest
     * Complexidade temporal: O(EV^2), no pior dos casos
     * @param dest
     * @return O resultado do fluxo máximo entre o mock source e o destino
     */
    double maxTrainsTo(const std::shared_ptr<Station> &dest);

    /**
     * Conecta o mock_source a todas as estações que não têm arestas incoming ou às que têm apenas uma aresta incoming que é reverse de uma das arestas outgoing dessa estação
     * Isto é usado para os algoritmos de cálculo de fluxo máximo onde queremos considerar as várias sources
     * ao mesmo tempo
     * Complexidade temporal: O(|V| + |E|), no pior dos casos
     * @param mock_source
     */
    void connectSourceNodesTo(Station *mock_source);
    /**
     * Conecta o mock_sink a todas as estações que não têm arestas outgoing
     * Isto é usado para os algoritmos de cálculo de fluxo máximo onde queremos
     * Complexidade temporal: O(V), onde V é o número de estações
     * @param mock_sink
     */
    void connectSinkNodesTo(std::shared_ptr<Station> mock_sink);

    /**
     * Desativa uma track, adiciona essa track à stack inactiveTracks e adiciona ??
     * Dá throw a uma exceção std::logic_error caso track seja nullptr
     * Complexidade temporal: O(1)
     * @param track
     */
    void deactivateTrack(const std::shared_ptr<Track> &track);
    /**
     * Desativa uma track, adiciona essa track à stack inactiveStations e adiciona ??
     * Dá throw a uma exceção std::logic_error caso station seja nullptr
     * Complexidade temporal: O(1)
     * @param station
     */
    void deactivateStation(const std::shared_ptr<Station> &station);
    /**
     * Reativa o último elemento desativado se este existir
     * Complexidade temporal: O(1)
     */
    void undoLastDeletion();
    /**
     * Reativa todos os elementos desativados
     * Complexidade temporal: O(V + E), onde V é o número de estações desativadas e E é o número de tracks desativadas
     */
    void undoAllDeletions();

    /**
     * Repõe os valores padrão dos boleanos utilitários de todas as estações e tracks no grafo
     * Complexidade temporal: O(V * N + E), onde V é o número de estações, N é o tamanho do vetor e E é o número de arestas
     */
    void clearNetworkUtils();

    /**
     * Encontra o fluxo máximo com o menor custo possível com uma variante do Edmonds-Karp que em vez de usar BFS, usa uma variante do Dijkstra
     * Complexidade temporal: O((Elog(V)) * E), onde V é o número de estações e E é o número de arestas
     * @param src
     * @param dest
     * @param flow_result
     * @return cost
     */
    int findMaxFlowMinCost(const std::shared_ptr<Station> &src, const std::shared_ptr<Station> &dest, int &flow_result);

    /**
     * Serve para ser utilizado no findAugmentingDijkstraPath para verificar se um determinado caminho é possível de ser adicionado á priority queue utilizada pelo dijkstra
     * @param queue
     * @param track
     * @param v
     * @param residual
     * @param isDest
     * @return false quando não podemos passar fluxo pela estação, true caso contrário
     */
    bool testAndVisitDijkstra(std::queue<Station*> &queue, std::shared_ptr<Track> track, Station* v, double residual, bool isDest);

    /**
     * Atribui o valor zero ao flow de todas as arestas do grafo
     * Complexidade temporal: O(V + E), onde V é o número de estações e E é o número de arestas
     */
    void resetFlow();

    /**
     * Determina quais estações têm mais fluxo perdido
     * Complexidade temporal: O(VE^2), onde V é o número de estações e E é o número de arestas
     * @param k
     * @return vetor com as k estações mais afetadas
     */
    std::vector<std::shared_ptr<Station>> mostAffectedStations(int k);

    /**
     * Determina os pares de estações pelos quais passam mais fluxo
     * Complexidade temporal: O(2^V * VE^2), onde V é o número de estações e E é o número de arestas
     * @return set com os pares de estações pelos quais passam mais fluxo
     */
    std::set<std::pair<std::shared_ptr<Station>, std::shared_ptr<Station>>> mostUsedPairsStations();

    /**
     * Determina os concelhos ou distritos (dependendo do valor do parâmetro isDistrict) com a maior razão fluxo por capacidade, em que o fluxo
     * é a soma dos fluxos das arestas nessa região e a capacidade é a soma das capacidades das arestas nessa região
     * Complexidade temporal: O(VE^2), onde V é o número de estações e E é o número de arestas
     * @param k
     * @param isDistrict
     * @return vetor com o nome das regiões com maior razão fluxo por capacidade e os respetivos fluxos e capacidades
     */
    std::vector<std::pair<std::string, std::pair<double,double>>> topRegionsByNeeds(int k, bool isDistrict);

protected:
    std::unordered_set<std::shared_ptr<Station>, StationHash, StationHashEquality> stationSet;

    std::stack<std::shared_ptr<Track>> inactiveTracks;

    std::stack<std::shared_ptr<Station>> inactiveStations;

    std::stack<char> deletionRecord;
};

#endif