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

void RailwayNetwork::testAndVisit(std::queue<std::shared_ptr<Station>> &queue,std::shared_ptr<Track> track, const std::shared_ptr<Station>& station, double residual) {
    if(!station->isVisited() && residual > 0) {
        station->setVisited(true);
        station->setPath(std::move(track));
        queue.push(station);
    }
}

bool RailwayNetwork::findAugmentingPath(const std::shared_ptr<Station>& station_src, const std::shared_ptr<Station>& station_dest) {
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

void RailwayNetwork::updatePath(const std::shared_ptr<Station>& station_src, std::shared_ptr<Station> station_dest, double minRes) {
    std::shared_ptr<Station> currStation = std::move(station_dest);
    while(currStation != station_src) {
        std::shared_ptr<Track> path = currStation->getPath();
        double flow = path->getFlow();
        if(path->getDest() == currStation) {
            path->setFlow(flow + minRes);
            currStation = path->getOrig();
        }
        else {
            path->setFlow(flow - minRes);
            currStation = path->getDest();
        }
    }
}

double RailwayNetwork::edmondsKarp(const std::shared_ptr<Station>& station_src, const std::shared_ptr<Station>& station_dest) {
    for(const auto& station : stationSet) {
        for(const auto& track : station->getAdj()) {
            track->setFlow(0);
        }
    }
    while(findAugmentingPath(station_src, station_dest)) {
        double minRes = findMinResidual(station_src, station_dest);
        updatePath(station_src, station_dest, minRes);
    }

    double result = 0;
    for(const std::shared_ptr<Track> &t: station_dest->getIncoming()) {
        result += t->getFlow();
    }

    return result;
}