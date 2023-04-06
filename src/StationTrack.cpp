#include "StationTrack.h"
#include <utility>

// ********************************* Station *********************************

Station::Station(std::string name, std::string district, std::string municipality, std::string township,
                 std::string line)
        : name(std::move(name)), district(std::move(district)), municipality(std::move(municipality)),
          township(std::move(township)), line(std::move(line)) {}

void Station::addTrack(const std::shared_ptr<Station>& dest, const std::string& service, double w, int cost, bool noIncoming) {
    auto newTrack = std::make_shared<Track>(shared_from_this(), dest, service, w, cost);

    adj.push_back(newTrack);
    if(!noIncoming) dest->incoming.push_back(newTrack);
}


std::vector<Track *> Station::getMultipleParentsPath() const {
    return this->multiple_parents_path;
}

void Station::clearMultipleParentsPath() {
    this->multiple_parents_path.clear();
}

void Station::addToMultipleParents(Track *t) {
    this->multiple_parents_path.push_back(t);
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

const std::string &Station::getDistrict() const {
    return district;
}


const std::string &Station::getMunicipality() const {
    return municipality;
}

bool Station::isActive() const {
    return active;
}

void Station::setActive(bool active) {
    Station::active = active;
}

double Station::getLostRatio() const {
    return lostRatio;
}


void Station::setLostRatio(double lostRatio) {
    Station::lostRatio = lostRatio;
}

bool Station::isInPath() const {
    return inPath;
}

void Station::setIsInPath(bool isInPath) {
    Station::inPath = isInPath;
}

void Station::removeTrackAdj(Track *track) {
    for (auto iterator = adj.begin(); iterator != adj.end(); ++iterator) {
        if((*iterator).get() == track) {
            this->adj.erase(iterator);
            break;
        }
    }

    track->getDest()->removeTrackIncoming(track);
}

void Station::removeTrackIncoming(Track *track) {
    for (auto iterator = incoming.begin(); iterator != incoming.end(); ++iterator) {
        if((*iterator).get() == track) {
            this->incoming.erase(iterator);
            break;
        }
    }
}


bool Station::isMock1() const {
    return isMock;
}

void Station::setIsMock(bool isMock) {
    Station::isMock = isMock;
}

double Station::getPreviousFlow() const {
    return previousFlow;
}

void Station::setPreviousFlow(double previousFlow) {
    Station::previousFlow = previousFlow;
}

// ********************************* Track *********************************

Track::Track(std::shared_ptr<Station> orig, std::shared_ptr<Station> dest, std::string service, double capacity, int cost)
    : orig(orig), dest(dest), service(service), capacity(capacity), cost(cost)
{

}

std::shared_ptr<Station> Track::getDest() const {
    return this->dest.lock();
}

double Track::getCapacity() const {
    return this->capacity;
}

std::shared_ptr<Station> Track::getOrig() const {
    return this->orig.lock();
}

double Track::getFlow() const {
    return flow;
}

void Track::setFlow(double flow) {
    this->flow = flow;
}

bool Track::isActive() const {
    return active;
}

void Track::setActive(bool active) {
    Track::active = active;
}

int Track::getCost() const {
    return cost;
}

bool Track::isVisited() const {
    return visited;
}

void Track::setVisited(bool visited) {
    Track::visited = visited;
}
