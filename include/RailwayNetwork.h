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

class RailwayNetwork {
public:
    bool addStation(std::shared_ptr<Station> station);

    bool addTrack(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, std::string service, double w, int cost);
    bool addBidirectionalTrack(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, std::string service, double w, int cost);

    int getNumVertex() const;
    std::unordered_set<std::shared_ptr<Station>, StationHash, StationHashEquality> getStationSet() const;

    bool findAugmentingPathBFS(const std::shared_ptr<Station> &station_src, const std::shared_ptr<Station> &station_dest);
    bool findAugmentingPathDijkstra(const std::shared_ptr<Station> &station_src, const std::shared_ptr<Station> &station_dest);
    double edmondsKarp(const std::shared_ptr<Station> &station_src, const std::shared_ptr<Station> &station_dest);
    static void testAndVisit(std::queue<std::shared_ptr<Station>> &queue, std::shared_ptr<Track> track,
                      const std::shared_ptr<Station>& station, double residual);
    static double findMinResidual(const std::shared_ptr<Station> &station_src, std::shared_ptr<Station> station_dest);
    static int updatePath(const std::shared_ptr<Station> &station_src, std::shared_ptr<Station> station_dest, double minRes);

    double maxTrainsTo(const std::shared_ptr<Station> &dest);
    void connectSourceNodesTo(Station *mock_source);
    void connectSinkNodesTo(Station *mock_sink);

    void eraseEdgesFromMockSource(Station *mock_source);

    void deactivateTrack(const std::shared_ptr<Track> &track);
    void deactivateStation(const std::shared_ptr<Station> &station);
    void undoLastDeletion();
    void undoAllDeletions();

    void clearNetworkUtils();

    void stationsInConnectedPath(Station *station_src, Station *station_dest);
    int findMaxFlowMinCost(const std::shared_ptr<Station> &src, const std::shared_ptr<Station> &dest, int &flow_result);

    bool testAndVisitDijkstra(std::queue<Station*> &queue, std::shared_ptr<Track> track, Station* u, Station* v, double residual, bool isDest);

    void resetFlow();

    std::vector<std::shared_ptr<Station>> mostAffectedStations(int k);
    std::set<std::pair<std::shared_ptr<Station>, std::shared_ptr<Station>>> mostUsedPairsStations();
    std::vector<std::pair<std::string, double>> topRegionsByNeeds(int k, bool isDistrict);
protected:

    std::unordered_set<std::shared_ptr<Station>, StationHash, StationHashEquality> stationSet;

    std::stack<std::shared_ptr<Track>> inactiveTracks;
    std::stack<std::shared_ptr<Station>> inactiveStations;
    std::stack<char> deletionRecord;
};

#endif