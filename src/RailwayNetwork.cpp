#include "RailwayNetwork.h"
#include "MutablePriorityQueue.h"

#include <iostream>

bool operator<(const std::shared_ptr<Station> &s1, const std::shared_ptr<Station> &s2) {
    return s1->getDist() < s2->getDist();
}


int RailwayNetwork::getNumVertex() const {
    return stationSet.size();
}

std::unordered_set<std::shared_ptr<Station>,StationHash,StationHashEquality> RailwayNetwork::getStationSet() const {
    return stationSet;
}

bool RailwayNetwork::addStation(std::shared_ptr<Station> station) {
    stationSet.insert(station);
    return true;
}

bool RailwayNetwork::addTrack(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, std::string service, double w, int cost) {
    if (station_src == nullptr || station_dest == nullptr)
        return false;

    station_src->addTrack(station_dest, service, w, cost);
    return true;
}

bool RailwayNetwork::addBidirectionalTrack(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, std::string service, double w, int cost) {
    if (station_src == nullptr || station_dest == nullptr)
        return false;

    auto e1 = station_src->addTrack(station_dest, service, w, cost);
    auto e2 = station_dest->addTrack(station_src, service, w, cost);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

void RailwayNetwork::testAndVisit(std::queue<std::shared_ptr<Station>> &queue,std::shared_ptr<Track> track, const std::shared_ptr<Station>& station, double residual) {
    if(!station->isVisited() && residual > 0 && track->isActive() && station->isActive()) {
        station->setVisited(true);
        station->setPath(std::move(track));
        queue.push(station);
    }
}

bool RailwayNetwork::findAugmentingPathBFS(const std::shared_ptr<Station>& station_src, const std::shared_ptr<Station>& station_dest) {
    for(const auto& station : stationSet) {
        station->setVisited(false);
    }

    std::queue<std::shared_ptr<Station>> queue;
    queue.push(station_src);
    station_src->setVisited(true);
    while(!queue.empty() && !station_dest->isVisited()) {
        std::shared_ptr<Station> currStation = queue.front();
        queue.pop();
        for(const auto& track : currStation->getAdj()) {
            testAndVisit(queue, track, track->getDest(), track->getCapacity() - track->getFlow());
        }
        for(const auto& track : currStation->getIncoming()){
            testAndVisit(queue, track, track->getOrig(), track->getFlow());
        }
    }
    return station_dest->isVisited();
}

void RailwayNetwork::clearNetworkUtils() {
    for(const auto &station: this->stationSet) {
        station->setVisited(false);
        station->setDist(INF);
        
        if(station->getMultipleParentsPath().size() > 0) {
            station->clearMultipleParentsPath();
        }
    }
}



void RailwayNetwork::stationsInConnectedPath(Station *station_src, Station *station_dest) {
    this->clearNetworkUtils();

    std::queue<Station *> stations;
    stations.push(station_src);
    while(!stations.empty()) {
        Station *u = stations.front();
        stations.pop();

        for(std::shared_ptr<Track> &t: u->getAdj()) {
            Station *v = t->getDest().get();
            if(!v->isVisited()) {
                stations.push(v);
                
                v->setVisited(true);
                v->addToMultipleParents(u);
            }
        }
    }
}

bool RailwayNetwork::findAugmentingPathDijkstra(const std::shared_ptr<Station> &station_src, const std::shared_ptr<Station> &station_dest) {
    MutablePriorityQueue<Station> stations_per_dist;

    this->stationsInConnectedPath(station_src.get(), station_dest.get());

    // Encontrar os nós do componente ligado entre source e sink
    std::queue<Station *> stations;
    stations.push(station_dest.get());

    while(!stations.empty()) {
        Station *s = stations.front();

        if(s != station_src.get()) {
            s->setDist(INF);
        } else {
            s->setDist(0);
        }
        stations_per_dist.insert(s);

        for(Station *v: s->getMultipleParentsPath()) {
            if(!v->isVisited()) {
                stations.push(v);
            } 
        }
        
        stations.pop();
    }

    while(!stations_per_dist.empty()) {
        Station *u = stations_per_dist.extractMin();
        u->setVisited(true);

        if(u == station_dest.get()) {
            return true;
        }

        for(std::shared_ptr<Track> &t: u->getAdj()) {
            Station *v = t->getDest().get();
            if(!v->isVisited() && v->getDist() > u->getDist() + t->getCost()) {
                v->setDist(u->getDist() + t->getCost());
                v->setPath(t);
                stations_per_dist.decreaseKey(t->getDest().get());
            }
        }

    }

    return false;
}

double RailwayNetwork::findMinResidual(const std::shared_ptr<Station>& station_src, std::shared_ptr<Station> station_dest) {
    double minRes = INF;
    std::shared_ptr<Station> currStation = std::move(station_dest);
    while(currStation != station_src) {
        std::shared_ptr<Track> path = currStation->getPath();
        if(path->getDest() == currStation) {
            minRes = std::min(minRes, path->getCapacity() - path->getFlow());
            currStation = path->getOrig();
        }
        else {
            minRes = std::min(minRes, path->getFlow());
            currStation = path->getDest();
        }
    }
    return minRes;
}

int RailwayNetwork::updatePath(const std::shared_ptr<Station>& station_src, std::shared_ptr<Station> station_dest, double minRes) {
    int cost = 0;

    std::shared_ptr<Station> currStation = std::move(station_dest);
    while(currStation != station_src) {
        std::shared_ptr<Track> path = currStation->getPath();
        double flow = path->getFlow();
        cost += (path->getCost()) * minRes;

        if(path->getDest() == currStation) {
            path->setFlow(flow + minRes);
            currStation = path->getOrig();
        }
        else {
            path->setFlow(flow - minRes);
            currStation = path->getDest();
        }
    }

    return cost;
}

void RailwayNetwork::resetFlow() {
    for(const auto& station : stationSet) {
        for(const auto& track : station->getAdj()) {
            track->setFlow(0);
        }
    }
}

double RailwayNetwork::edmondsKarp(const std::shared_ptr<Station>& station_src, const std::shared_ptr<Station>& station_dest) {
    if(station_src == nullptr || station_dest == nullptr || station_src == station_dest || !station_src->isActive() || !station_dest->isActive()) {
        throw std::logic_error("Invalid source and/or target station");
    }

    this->resetFlow();

    while(findAugmentingPathBFS(station_src, station_dest)) {
        double minRes = findMinResidual(station_src, station_dest);
        updatePath(station_src, station_dest, minRes);
    }

    double result = 0;
    for(const std::shared_ptr<Track> &t: station_dest->getIncoming()) {
        result += t->getFlow();
    }

    return result;
}

int RailwayNetwork::findMaxFlowMinCost(const std::shared_ptr<Station> &src, const std::shared_ptr<Station> &dest) {
    int cost = 0;

    this->resetFlow();

    while(findAugmentingPathDijkstra(src, dest)) {
        double minFlow = findMinResidual(src, dest);
        int cost = updatePath(src, dest, minFlow);
    }

    return cost;
}

void RailwayNetwork::deactivateTrack(const std::shared_ptr<Track>& track) {
    if(track == nullptr) {
        throw std::logic_error("Invalid track");
    }

    track->setActive(false);
    inactiveTracks.push(track);
    deletionRecord.push(0);
}

double RailwayNetwork::maxTrainsTo(const std::shared_ptr<Station> &dest) {
    double result = 0; 
    std::shared_ptr<Station> mockSource = std::make_shared<Station>();

    connectSourceNodesTo(mockSource.get());
    result = edmondsKarp(mockSource, dest);

    mockSource->getAdj().clear();

    return result;
}

void RailwayNetwork::connectSourceNodesTo(Station *mock_source) {
    for(auto station: this->stationSet) {
        if(station->getIncoming().size() == 0) {
            mock_source->addTrack(station, "", std::numeric_limits<double>::max(), 0);
            continue;
        }

        if(station->getIncoming().size() == 1) {
            Track *reverse_track = station->getIncoming().at(0).get();
            for(auto track: station->getAdj()) {
                if(track->getDest().get() == reverse_track->getOrig().get() && track->getOrig().get() == reverse_track->getDest().get()) {
                    mock_source->addTrack(station, "", std::numeric_limits<double>::max(), 0);
                }
            }
        }
    }
}

void RailwayNetwork::deactivateStation(const std::shared_ptr<Station>& station) {
    if(station == nullptr) {
        throw std::logic_error("Invalid station");
    }

    station->setActive(false);
    inactiveStations.push(station);
    deletionRecord.push(1);
}

void RailwayNetwork::undoLastDeletion() {
    if(deletionRecord.empty()) return;

    if(deletionRecord.top()) {
        inactiveStations.pop();
        inactiveStations.top()->setActive(true);
    }
    else {
        inactiveTracks.top()->setActive(true);
        inactiveTracks.pop();
    }

    deletionRecord.pop();
}

void RailwayNetwork::undoAllDeletions() {
    while(!inactiveTracks.empty()) {
        inactiveTracks.top()->setActive(true);
        inactiveTracks.pop();
    }

    while(!inactiveStations.empty()) {
        inactiveStations.pop();
        inactiveStations.top()->setActive(true);
    }

    while(!deletionRecord.empty())
        deletionRecord.pop();
}

class Compare {
public:
    bool operator()(const std::shared_ptr<Station>& a, const std::shared_ptr<Station>& b)
    {
        return a->getLostRatio() > b->getLostRatio();
    }
};



std::vector<std::shared_ptr<Station>> RailwayNetwork::mostAffectedStations(int k) {
    std::priority_queue<std::shared_ptr<Station>,std::vector<std::shared_ptr<Station>>,Compare> queue;
    for (const auto& station : stationSet) {
        if(station->isActive()) {
            double lostCapacity = 0;
            for (const auto& edge : station->getAdj()) {
                if(!edge->isActive() || !edge->getDest()->isActive())
                    lostCapacity += edge->getCapacity();
            }
            for (const auto& edge : station->getIncoming()) {
                if(!edge->isActive() || !edge->getOrig()->isActive())
                    lostCapacity += edge->getCapacity();
            }
            station->setLostRatio(((double)lostCapacity)/station->getTotalCapacity());
            if(queue.size() < k) {
                queue.push(station);
            }
            else if (queue.top()->getLostRatio() < station->getLostRatio()) {
                queue.pop();
                queue.push(station);
            }
        }
    }
    std::vector<std::shared_ptr<Station>> res(k);
    for(int i = 0; i < k; i++) {
        res[k - i - 1] = queue.top();
        queue.pop();
    }
    return res;
}

std::set<std::pair<std::shared_ptr<Station>, std::shared_ptr<Station>>> RailwayNetwork::mostUsedPairsStations() {
    std::set<std::pair<std::shared_ptr<Station>, std::shared_ptr<Station>>> res;
    double maxFlow = -1;
    for (const auto & stationA : stationSet) {
        if (!stationA->isActive())
            continue;

        for (const auto& stationB : stationSet) {
            if (!stationB->isActive() || stationA == stationB)
                continue;

            std::pair<std::shared_ptr<Station>, std::shared_ptr<Station>> stationPair {stationA, stationB};
            double currentFlow = edmondsKarp(stationA, stationB);
            if (currentFlow > maxFlow) {
                maxFlow = currentFlow;
                res.clear();
                res.insert(stationPair);
            }
            else if (currentFlow == maxFlow && res.find(stationPair) == res.end())
                res.insert(stationPair);
        }
    }
    return res;
}

