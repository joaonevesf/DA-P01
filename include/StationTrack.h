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

class Station {
public:
    Station(std::string name, std::string district, std::string municipality, std::string township, std::string line);
    Station() = default;

    bool operator<(const Station &s2) const {
        return this->dist < s2.getDist();
    }

    std::vector<std::shared_ptr<Track>> getAdj() const;
    bool isVisited() const;
    double getDist() const;
    std::shared_ptr<Track>getPath() const;
    std::vector<std::shared_ptr<Track>> getIncoming() const;

    unsigned int queueIndex;

    void setVisited(bool visited);
    void setDist(double dist);
    void setPath(std::shared_ptr<Track>path);

    std::shared_ptr<Track> addTrack(const std::shared_ptr<Station>& dest, const std::string& service, double w, int cost);
    bool removeTrack(const std::shared_ptr<Station>& station_dest);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::string &getDistrict() const;

    void setDistrict(const std::string &district);

    const std::string &getMunicipality() const;

    void setMunicipality(const std::string &municipality);

    const std::string &getTownship() const;

    void setTownship(const std::string &township);

    const std::string &getLine() const;

    void setLine(const std::string &line);

    bool isActive() const;

    void setActive(bool active);

    [[nodiscard]] std::vector<Track *> getMultipleParentsPath() const;
    void clearMultipleParentsPath();

    int getTotalCapacity() const;
    void setTotalCapacity(int totalCapacity);

    double getLostRatio() const;
    void setLostRatio(double lostRatio);

    void addToMultipleParents(Track *t);
protected:
    std::string name;
    std::string district;
    std::string municipality;
public:
    bool isInPath() const;

    void setIsInPath(bool isInPath);

protected:
    std::string township;

    std::string line;
    std::vector<std::shared_ptr<Track>> adj;

    std::vector<std::shared_ptr<Track>> incoming;
    double dist = 0;


    double lostRatio;

    std::shared_ptr<Track>parent_path = nullptr;

    std::vector<Track *> multiple_parents_path;

    bool inPath = false;
    bool visited = false;
    bool active = true;
};

// ********************************* Track *********************************

class Track {
public:
    Track(Station* orig, std::shared_ptr<Station> dest, std::string service, double capacity, int cost);

    std::shared_ptr<Station> getDest() const;
    double getCapacity() const;
    bool isSelected() const;
    std::shared_ptr<Station> getOrig() const;
    std::shared_ptr<Track> getReverse() const;
    double getFlow() const;
    std::string getService() const;

    void setSelected(bool selected);
    void setReverse(std::shared_ptr<Track> reverse);
    void setFlow(double flow);
    void setCapacity(double capacity);

    int getCost() const;
    void setCost(int cost);

    bool isActive() const;

    void setActive(bool active);

protected:
    std::string service;
    std::shared_ptr<Station> dest;
    double capacity;
    int cost;

    bool selected = false;
    bool active = true;

    bool visited = false;
public:
    bool isVisited() const;

    void setVisited(bool visited);

protected:

    // used for bidirectional edges
    std::shared_ptr<Station> orig;
    std::shared_ptr<Track> reverse = nullptr;

    double flow;
};

#endif