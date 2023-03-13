#include "RailwayNetwork.h"

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

bool RailwayNetwork::addTrack(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, std::string service, double w) {
    if (station_src == nullptr || station_dest == nullptr)
        return false;

    station_src->addTrack(station_dest, service, w);
    return true;
}

bool RailwayNetwork::addBidirectionalTrack(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, std::string service, double w) {
    if (station_src == nullptr || station_dest == nullptr)
        return false;

    auto e1 = station_src->addTrack(station_dest, service, w);
    auto e2 = station_dest->addTrack(station_src, service, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

bool RailwayNetwork::bfs(const std::shared_ptr<Station>& station_src, const std::shared_ptr<Station>& station_dest) {
    for(const auto& station : stationSet) {
        station->setVisited(false);
    }
    std::queue<std::shared_ptr<Station>> queue;
    queue.push(station_src);
    station_src->setVisited(true);
    while(!queue.empty()) {
        std::shared_ptr<Station> currStation = queue.front();
        queue.pop();
        for(const auto& track : currStation->getAdj()) {
            std::shared_ptr<Station> dest = track->getDest();
            if(!dest->isVisited() && (track->getCapacity() - track->getFlow() != 0)) {
                queue.push(dest);
                dest->setVisited(true);
                dest->setPath(track);
                if(dest==station_dest) {
                    return true;
                }
            }
        }
        for(const auto& edge : currStation->getIncoming()){
            std::shared_ptr<Station> rdest = edge->getOrig();
            std::shared_ptr<Track> reverse = edge->getReverse();
            if(!rdest->isVisited() && edge->getFlow()!=0){
                rdest->setVisited(true);
                rdest->setPath(reverse);
                queue.push(rdest);
                reverse->setFlow(0);
            }
        }
    }
    return false;
}

double RailwayNetwork::edmondsKarp(const std::shared_ptr<Station>& station_src, const std::shared_ptr<Station>& station_dest) {
    for(auto station : stationSet) {
        for(auto track : station->getAdj()) {
            track->setFlow(0);
            std::shared_ptr<Track> reverse = std::make_shared<Track>(track->getDest().get(),track->getOrig(),track->getService(),0.0);
            reverse->setFlow(0);
            track->setReverse(reverse);
            reverse->setReverse(track);
            reverse->setSelected(true);
        }
    }

    while(bfs(station_src,station_dest)) {
        double minRes = INF;
        std::shared_ptr<Station> currStation = station_dest;
        while(currStation != station_src) {
            std::shared_ptr<Track> path = currStation->getPath();
            minRes = std::min(minRes, path->getCapacity() - path->getFlow());
            currStation = path->getOrig();
        }
        currStation = station_dest;
        while(currStation != station_src) {
            std::shared_ptr<Track> path = currStation->getPath();
            if(path->isSelected()) {
                auto originalEdge = path->getReverse();
                originalEdge->setFlow(originalEdge->getFlow() - minRes);
                path->setCapacity(path->getCapacity() - minRes);
                path->setFlow(0);
            }
            else {
                auto reverse = path->getReverse();
                path->setFlow((path->getFlow() + minRes));
                reverse->setCapacity(reverse->getCapacity() + minRes);
            }
            currStation = path->getOrig();
        }
    }

    double result = 0;

    for(const std::shared_ptr<Track> &t: station_dest->getIncoming()) {
        result += t->getFlow();
    }

    return result;
}