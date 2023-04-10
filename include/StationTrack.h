#ifndef STATION_TRACK
#define STATION_TRACK

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <memory>

class Track;

#define INF std::numeric_limits<double>::max()

// ********************************* Station *********************************

class Station : public std::enable_shared_from_this<Station> {
public:
    Station(std::string name, std::string district, std::string municipality, std::string township, std::string line);

    Station() = default;

    bool operator<(const Station &s2) const {
        return this->dist < s2.getDist();
    }

    std::vector<std::shared_ptr<Track>> getAdj() const;

    bool isVisited() const;

    double getDist() const;

    std::shared_ptr<Track> getPath() const;

    std::vector<std::shared_ptr<Track>> getIncoming() const;

    void setVisited(bool visited);

    void setDist(double dist);

    void setPath(std::shared_ptr<Track> path);

    /**
     * Adiciona uma linha com destino @dest ao vetor adj e, se o @noIncoming estiver falso,
     * adiciona também essa linha ao vetor incoming do @dest
     * @param dest estação de destino
     * @param service "ALFA PENDULAR" ou "STANDARD"
     * @param w capacidade da linha
     * @param cost
     * @param noIncoming
     */
    void
    addTrack(const std::shared_ptr<Station> &dest, const std::string &service, double w, int cost, bool noIncoming);

    const std::string &getName() const;

    const std::string &getDistrict() const;

    const std::string &getMunicipality() const;

    bool isActive() const;

    void setActive(bool active);

    [[nodiscard]] std::vector<Track *> getMultipleParentsPath() const;

    /**
     * Dá clear ao vetor multiple_parents_path
     * Complexidade temporal: O(n), em que \i n é o tamanho do vetor multiple_parents_path
     */
    void clearMultipleParentsPath();

    double getLostRatio() const;

    void setLostRatio(double lostRatio);

    /**
     * Remove a linha @track do vetor adj
     * Complexidade temporal: O(n), em que \i n é o tamanho do vetor adj
     * @param track
     */
    void removeTrackAdj(Track *track);

    /**
     * Remove a linha @track do vetor incoming
     * Complexidade temporal: O(n), em que \i n é o tamanho do vetor incoming
     * @param track
     */
    void removeTrackIncoming(Track *track);

    /**
     * Adiciona a linha @t ao vetor multiple_parents_path
     * Complexidade temporal: O(n), em que n é o tamanho do vetor multiple_parents_path,
     * se precisar de alocar nova memória, O(1) se não for necessário
     * @param t
     */
    void addToMultipleParents(Track *t);

    bool isInPath() const;

    void setIsInPath(bool isInPath);

    bool isMock1() const;

    void setIsMock(bool isMock);

    double getPreviousFlow() const;

    void setPreviousFlow(double previousFlow);

    /**
     * Index da MutablePriorityQueue
     */
    unsigned int queueIndex = 0;

protected:
    std::string name;

    std::string district;

    std::string municipality;

    std::string township;

    std::string line;

    std::vector<std::shared_ptr<Track>> adj;

    std::vector<std::shared_ptr<Track>> incoming;

    double dist = 0;

    bool isMock = false;

    double lostRatio = 0;

    std::shared_ptr<Track> parent_path = nullptr;

    std::vector<Track *> multiple_parents_path;

    bool inPath = false;

    bool visited = false;

    bool active = true;

    double previousFlow = 0;

};

// ********************************* Track *********************************
/**
 * Classe representante de uma linha de comboios. Une duas estações e tem um certo serviço com custo associado e
 * uma certa capacidade de comboios ao mesmo tempo
 */
class Track {
public:
    Track(std::shared_ptr<Station> orig, std::shared_ptr<Station> dest, std::string service, double capacity, int cost);

    [[nodiscard]] std::shared_ptr<Station> getDest() const;

    [[nodiscard]] double getCapacity() const;

    [[nodiscard]] std::shared_ptr<Station> getOrig() const;

    [[nodiscard]] double getFlow() const;

    void setFlow(double flow);

    [[nodiscard]] int getCost() const;

    [[nodiscard]] bool isActive() const;

    void setActive(bool active);

    [[nodiscard]] bool isVisited() const;

    void setVisited(bool visited);

protected:
    /**
     * "STANDARD" ou "ALFA PENDULAR"
     */
    std::string service;

    std::weak_ptr<Station> dest;

    double capacity;

    /**
     * 2 para serviço "STANDARD" e 4 para serviço "ALFA PENDULAR"
     */
    int cost;

    bool active = true;

    bool visited = false;

    std::weak_ptr<Station> orig;

    std::shared_ptr<Track> reverse = nullptr;

    double flow = 0;
};

#endif