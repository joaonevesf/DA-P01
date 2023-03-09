#include "StationTrack.h"
#include <utility>

// ********************************* Station *********************************

Station::Station(std::string name, std::string district, std::string municipality, std::string township,
                 std::string line)
        : name(std::move(name)), district(std::move(district)), municipality(std::move(municipality)),
          township(std::move(township)), line(std::move(line)) {}

std::shared_ptr<Track> Station::addTrack(std::shared_ptr<Station> dest, double w) {
    auto newTrack = std::make_shared<Track>(this, dest, w);
    adj.push_back(newTrack);
    dest->incoming.push_back(newTrack);
    return newTrack;
}

bool Station::removeTrack(std::shared_ptr<Station> station_dest) {
    bool removeTrack = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        std::shared_ptr<Track> edge = *it;
        std::shared_ptr<Station>dest = edge->getDest();
        if (dest.get() == station_dest.get()) {
            it = adj.erase(it);
            // Also remove the corresponding edge from the incoming list
            auto it2 = dest->incoming.begin();
            while (it2 != dest->incoming.end()) {
                if ((*it2)->getOrig().get() == this) {
                    it2 = dest->incoming.erase(it2);
                }
                else {
                    it2++;
                }
            }
            removeTrack = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removeTrack;
}

std::vector<std::shared_ptr<Track>> Station::getAdj() const {
    return this->adj;
}

bool Station::isVisited() const {
    return this->visited;
}

double Station::getDist() const {
    return this->dist;
}

std::shared_ptr<Track> Station::getPath() const {
    return this->parent_path;
}

std::vector<std::shared_ptr<Track>> Station::getIncoming() const {
    return this->incoming;
}

void Station::setVisited(bool visited) {
    this->visited = visited;
}

void Station::setDist(double dist) {
    this->dist = dist;
}

void Station::setPath(std::shared_ptr<Track> path) {
    this->parent_path = path;
}

const std::string &Station::getName() const {
    return name;
}

void Station::setName(const std::string &name) {
    Station::name = name;
}

const std::string &Station::getDistrict() const {
    return district;
}

void Station::setDistrict(const std::string &district) {
    Station::district = district;
}

const std::string &Station::getMunicipality() const {
    return municipality;
}

void Station::setMunicipality(const std::string &municipality) {
    Station::municipality = municipality;
}

const std::string &Station::getTownship() const {
    return township;
}

void Station::setTownship(const std::string &township) {
    Station::township = township;
}

const std::string &Station::getLine() const {
    return line;
}

void Station::setLine(const std::string &line) {
    Station::line = line;
}

// ********************************* Track *********************************

Track::Track(Station* orig, std::shared_ptr<Station> dest, int c): orig(std::shared_ptr<Station>(orig)), dest(dest), capacity(c) {}

std::shared_ptr<Station> Track::getDest() const {
    return this->dest;
}

int Track::getCapacity() const {
    return this->capacity;
}

std::shared_ptr<Station> Track::getOrig() const {
    return this->orig;
}

std::shared_ptr<Track> Track::getReverse() const {
    return this->reverse;
}

bool Track::isSelected() const {
    return this->selected;
}

double Track::getFlow() const {
    return flow;
}

void Track::setSelected(bool selected) {
    this->selected = selected;
}

void Track::setReverse(std::shared_ptr<Track> reverse) {
    this->reverse = reverse;
}

void Track::setFlow(double flow) {
    this->flow = flow;
}

void Track::setCapacity(int c) {
    this->capacity = capacity;
}