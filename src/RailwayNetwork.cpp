#include "RailwayNetwork.h"
#include "MutablePriorityQueue.h"

#include <iostream>
#include <map>

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
        station->setPath(track);
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
    for(auto &station: this->stationSet) {
        station->setVisited(false);
        station->setDist(INF);
        station->setIsInPath(false);

        for(auto &track: station->getAdj()) {
            track->setVisited(false);
        }

        if(station->getMultipleParentsPath().size() > 0) {
            station->clearMultipleParentsPath();
        }
    }
}

bool RailwayNetwork::findAugmentingPathDijkstra(const std::shared_ptr<Station> &station_src, const std::shared_ptr<Station> &station_dest) {
    MutablePriorityQueue<Station> stations_per_dist;

    this->clearNetworkUtils();

    station_src->setDist(0); station_src->setVisited(true); station_src->setIsInPath(true);

    std::queue<Station *> stations;
    stations.push(station_src.get());
    stations_per_dist.insert(station_src.get());

    // Encontrar os nós que vão poder ser encontrados no caminho
    while(!stations.empty()) {
        Station *currStation = stations.front();
        stations.pop();

        for (const auto &track: currStation->getAdj()) {
            if (testAndVisitDijkstra(stations, track, currStation, track->getDest().get(),
                                     track->getCapacity() - track->getFlow(), track->getDest().get() == station_dest.get())) {
                stations_per_dist.insert(track->getDest().get());
                track->getDest().get()->setIsInPath(true);
            }
        }

        for (const auto &track: currStation->getIncoming()) {
            if (testAndVisitDijkstra(stations, track, currStation, track->getOrig().get(), track->getFlow(), track->getOrig().get() == station_dest.get())) {
                stations_per_dist.insert(track->getOrig().get());
                track->getOrig()->setIsInPath(true);
            }
        }
    }

    if(!station_dest->isInPath()) return false;

    for(auto &s: this->stationSet) {
        s->setVisited(false);
    }

    // Encontrar o caminho mais curto
    while(!stations_per_dist.empty()) {
        Station *u = stations_per_dist.extractMin();
        u->setVisited(true);

        if(u == station_dest.get()) {
            return true;
        }

        for(auto &t: u->getAdj()) {
            if(t->getCapacity() - t->getFlow() == 0) continue;
            Station *v = t->getDest().get();
            if(!v->isInPath()) continue;

            if(!v->isVisited() && v->getDist() > u->getDist() + t->getCost()) {
                v->setDist(u->getDist() + t->getCost());
                v->setPath(t);
                stations_per_dist.decreaseKey(v);
            }
        }

        for(auto &t: u->getIncoming()) {
            Station *v = t->getOrig().get();
            if(v->isInPath() && t->getFlow() > 0 && v->getDist() > u->getDist() + t->getCost()) {
                v->setPath(t);
                v->setDist(u->getDist() + t->getCost());
                stations_per_dist.decreaseKey(v);
            }
        }
    }    

    return true;
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

int RailwayNetwork::findMaxFlowMinCost(const std::shared_ptr<Station> &src, const std::shared_ptr<Station> &dest, int &flow_result) {
    if(src->getName() == dest->getName()) return 0;

    int cost = 0;

    std::shared_ptr<Station> real_src = *(this->stationSet.find(src));
    std::shared_ptr<Station> real_dest = *(this->stationSet.find(dest));

    this->clearNetworkUtils();
    this->resetFlow();

    while(findAugmentingPathDijkstra(real_src, real_dest)) {
        double minFlow = findMinResidual(real_src, real_dest);
        updatePath(real_src, real_dest, minFlow);
    }

    for(auto &s: this->stationSet) {
        s->setVisited(false);
    }

    std::queue<Station *> stations;
    stations.push(real_src.get());

    while(!stations.empty()) {
        Station* currStation = stations.front();
        stations.pop();

        for(const auto& track : currStation->getAdj()) {
            cost += (track->getCost() * track->getFlow());
            if(!track->getDest()->isVisited()) {
                stations.push(track->getDest().get());
                track->getDest()->setVisited(true);
            }
        }
    }

    for(auto &t: real_dest->getIncoming()) {
        flow_result += t->getFlow();
    }

    this->setPathBFS(real_src.get(), real_dest.get(), 0);

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
    this->clearNetworkUtils();

    double result = 0; 
    std::shared_ptr<Station> mockSource = std::make_shared<Station>();

    std::shared_ptr<Station> real_dest = *(this->stationSet.find(dest));

    connectSourceNodesTo(mockSource.get());
    result = edmondsKarp(mockSource, real_dest);

    std::vector<std::shared_ptr<Track>> mock_source_tracks = mockSource->getAdj();
    for(auto &t: mock_source_tracks) {
        mockSource->removeTrackAdj(t.get());
    }

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

bool RailwayNetwork::testAndVisitDijkstra(std::queue<Station*> &queue, std::shared_ptr<Track> track, Station* u, Station* v, double residual, bool isDest) {

    if((!v->isVisited() && !v->isInPath() && residual > 0 && track->isActive())) {
        queue.push(v);
        v->setVisited(true);

        return true;
    } else if(isDest && residual > 0) {
        if(!v->isVisited()) queue.push(v);

        return true;
    }

    return false;
}

void RailwayNetwork::connectSinkNodesTo(Station *mock_sink) {
    for (auto station : this->stationSet) {
        if (station->getAdj().size() == 0)
            mock_sink->addTrack(station, "", std::numeric_limits<double>::max(), 0);
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

class CompareByLostRatio {
public:
    bool operator()(const std::shared_ptr<Station>& a, const std::shared_ptr<Station>& b)
    {
        return a->getLostRatio() > b->getLostRatio();
    }
};

void RailwayNetwork::setPathBFS(Station *src, Station *dest, double flow_min_limit) {
    this->clearNetworkUtils();

    std::queue<Station *> stations;
    stations.push(src); 
    src->setVisited(true);

    while(!stations.empty()) {
        Station *u = stations.front();
        stations.pop();

        for(auto &t: u->getAdj()) {
            if(t->getFlow() <= flow_min_limit || t->isVisited()) continue;

            Station *v = t->getDest().get();

            if(!t->isVisited()) {
                t->setVisited(true);
                v->addToMultipleParents(t.get());

                if(!v->isVisited())  {
                    stations.push(v);
                    v->setVisited(true);
                }
            }
        }

    }
}

void RailwayNetwork::constructPath(Track *finish_track, std::vector<std::deque<Track *>> &paths, std::deque<Track *> &current_path) {

    current_path.push_front(finish_track);

    if(finish_track->getOrig()->getMultipleParentsPath().empty()) {
        if (!current_path.empty()) {
            paths.push_back(current_path);
            current_path.clear();
        }

        return;
    }

    for (auto &t: finish_track->getOrig()->getMultipleParentsPath()) {
        constructPath(t, paths, current_path);
        current_path.pop_front();
    }

}

std::vector<std::shared_ptr<Station>> RailwayNetwork::mostAffectedStations(int k) {
    std::priority_queue<std::shared_ptr<Station>,std::vector<std::shared_ptr<Station>>,CompareByLostRatio> queue;
    for (const auto& station : stationSet) {
        if(station->isActive()) {
            double lostCapacity = 0;
            double totalCapacity = 0;
            for (const auto& edge : station->getAdj()) {
                totalCapacity += edge->getCapacity();
                if(!edge->isActive() || !edge->getDest()->isActive())
                    lostCapacity += edge->getCapacity();
            }
            for (const auto& edge : station->getIncoming()) {
                totalCapacity += edge->getCapacity();
                if(!edge->isActive() || !edge->getOrig()->isActive())
                    lostCapacity += edge->getCapacity();
            }
            station->setLostRatio(lostCapacity/totalCapacity);
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

class CompareByFlowCapacityRatio {
public:
    bool operator()(const std::pair<std::string,std::pair<double,double>>& a, const std::pair<std::string,std::pair<double,double>>& b)
    {
        return (a.second.first / a.second.second) > (b.second.first / b.second.first);
    }
};

std::vector<std::pair<std::string, double>> RailwayNetwork::topRegionsByNeeds(int k, bool isDistrict) {
    std::map<std::string, std::pair<double, double>> region_by_flow_and_weight;
    std::vector<std::pair<std::string, double>> result;

    std::shared_ptr<Station> mock_source = std::make_shared<Station>();
    std::shared_ptr<Station> mock_sink = std::make_shared<Station>();

    connectSourceNodesTo(mock_source.get());
    connectSinkNodesTo(mock_sink.get());

    edmondsKarp(mock_source, mock_sink);

    for (const auto &station : stationSet) {
        for(const auto &track : station->getAdj()) {
            Station *u = track->getDest().get();
            std::string r = isDistrict ? u->getDistrict() : u->getMunicipality();
            std::string s = isDistrict ? station->getDistrict() : station->getMunicipality();
            if(r != s) {
                if(region_by_flow_and_weight.find(s) == region_by_flow_and_weight.end()) {
                    region_by_flow_and_weight.insert({ s,  { track->getFlow(), track->getCapacity() } });
                } else {
                    region_by_flow_and_weight[s].first += track->getFlow();
                    region_by_flow_and_weight[s].second += track->getCapacity();
                }
            }

            if(region_by_flow_and_weight.find(r) == region_by_flow_and_weight.end()) {
                region_by_flow_and_weight.insert({ r, { track->getFlow(), track->getCapacity() } });
            } else {
                region_by_flow_and_weight[r].first += track->getFlow();
                region_by_flow_and_weight[r].second += track->getCapacity();
            }

        }
    }

    std::priority_queue<std::pair<std::string, std::pair<double, double>>, std::vector<std::pair<std::string, std::pair<double, double>>>, CompareByFlowCapacityRatio> pq;

    for(const auto & p: region_by_flow_and_weight) {
        double ratio = (p.second.first / p.second.second);

        if(pq.size() < k) {
            pq.emplace(p);
        } else if((pq.top().second.first / pq.top().second.second) < ratio) {
            pq.pop();
            pq.emplace(p);
        }
    }

    for(int i = 0; i < k; i++) {
        std::pair<std::string, std::pair<double, double>> p = pq.top();
        double ratio = (p.second.first / p.second.second);

        result.emplace_back(p.first, ratio);
    }

    return result;
}

void removeTrack(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest) {

}

