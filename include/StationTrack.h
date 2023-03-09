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

    std::vector<std::shared_ptr<Track>> getAdj() const;
    bool isVisited() const;
    double getDist() const;
    std::shared_ptr<Track>getPath() const;
    std::vector<std::shared_ptr<Track>> getIncoming() const;

    void setVisited(bool visited);
    void setDist(double dist);
    void setPath(std::shared_ptr<Track>path);
    std::shared_ptr<Track> addTrack(std::shared_ptr<Station> dest, double w);
    bool removeTrack(std::shared_ptr<Station> station_dest);

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

protected:
    std::string name;
    std::string district;
    std::string municipality;
    std::string township;
    std::string line;

    std::vector<std::shared_ptr<Track>> adj;

    bool visited = false;
    double dist = 0;
    std::shared_ptr<Track>parent_path = nullptr;

    std::vector<std::shared_ptr<Track>> incoming;
};

// ********************************* Track *********************************

class Track {
public:
    Track(Station* orig, std::shared_ptr<Station> dest, int c);

    std::shared_ptr<Station> getDest() const;
    int getCapacity() const;
    bool isSelected() const;
    std::shared_ptr<Station> getOrig() const;
    std::shared_ptr<Track> getReverse() const;
    double getFlow() const;

    void setSelected(bool selected);
    void setReverse(std::shared_ptr<Track> reverse);
    void setFlow(double flow);
    void setCapacity(int capacity);
protected:
    std::string service;
    std::shared_ptr<Station> dest;
    int capacity;

    bool selected = false;

    // used for bidirectional edges
    std::shared_ptr<Station> orig;
    std::shared_ptr<Track> reverse = nullptr;

    double flow;
};

#endif