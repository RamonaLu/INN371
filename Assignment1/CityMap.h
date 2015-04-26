#ifndef CITYMAP_H_INCLUDED
#define CITYMAP_H_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <set>
#include <queue>
#include <math.h>
#include <algorithm>
using namespace std;

template <typename T>
struct Pos {
    Pos() {};
    Pos(T x, T y) : x(x), y(y){}
    T x;
    T y;
};

template <typename T>
class CityMap
{
    public:
        CityMap();
        CityMap(ostream &out, ostream &err);
        ~CityMap();
        void AddCity(string cityName, T x, T y);
        void AddRoad(string firstCity, string secondCity); // connect cities
        void RemoveCity(string cityName);
        void RemoveRoad(string firstCity, string secondCity); // disconnect cities
        T FindDistance(string firstCity, string secondCity); // Dijkstra
        vector<string> ShortestPath(string firstCity, string secondCity); // cities on the path
        void PrintPath(string firstCity, string secondCity); // display cities on the path and distances
    private:
        void Error(string subjectType, string subject, string reason);
        T CartesianDistance(Pos<T> pos1, Pos<T> pos2);
        T CartesianDistance(string firstCity, string secondCity);
        vector<string> ReconstructPath(map<string, string> paths, string current);
        pair<vector<string>, T> ShortestPathCore(string firstCity, string secondCity);
        map<string, Pos<T> > cities;
        map<string, map<string, T> > adjacentRoads;
        ostream &out;
        ostream &err;


};
//Constants
const string ALREADY_EXISTS = "already exists";
const string DOESNT_EXIST = "doesn't exist";
const string CITY = "City";
const string ROAD = "Road";
const string PATH = "Path";

//Private
template <typename T>
void CityMap<T>::Error(string subjectType, string subject, string reason) {
    err << "Error: " << reason << endl;
    err << subjectType << ": " << subject << endl;
}
template <typename T>
T CityMap<T>::CartesianDistance(string firstCity, string secondCity)
{
    return CartesianDistance(cities[firstCity], cities[secondCity]);
}
template <typename T>
T CityMap<T>::CartesianDistance(Pos<T> pos1, Pos<T> pos2)
{
    T deltaX = pos2.x - pos1.x;
    T deltaY = pos2.y - pos1.y;
    return sqrt(deltaX * deltaX + deltaY*deltaY);
}


template <typename T>
struct Lesser {
    bool operator()(const pair<string, T >& a, const pair<string, T >& b) const{
        return a.second >  b.second;
    }
};
template <typename T>
vector<string> CityMap<T>::ReconstructPath(map<string, string> paths, string current) {
    vector<string> path;
	path.push_back(current);
	while (paths.find(current) != paths.end()) {
		current = paths[current];
		path.push_back(current);
	}

	return vector<string>(path.rbegin(), path.rend());
}

template <typename T>
pair<vector<string>, T> CityMap<T>::ShortestPathCore(string firstCity, string secondCity) {
    priority_queue <pair<string, T>, vector<pair<string, T> >, Lesser<T> > fQueue;
    set<string> open;
    set<string> closed;
    map<string, T> gScores;
    map<string,string> paths;
    T g = 0;
    T h = CartesianDistance(firstCity, secondCity);
    gScores[firstCity] = g;
    pair<string, T> current = pair<string, T>(firstCity, g+h);
    fQueue.push(current);
    open.insert(firstCity);

    while(!open.empty()) {
        current = fQueue.top();
        fQueue.pop()
        string currentCityName = current.first;
        open.erase(currentCityName);
        closed.insert(currentCityName);
        if (currentCityName == secondCity) {
            vector<string> shortestPath = ReconstructPath(paths, currentCityName);
            T shortestDist = gScores[currentCityName];
            return pair<vector<string>, T>(shortestPath, shortestDist);
        }
        else {
            map<string, T> neighbours = adjacentRoads[currentCityName];
            for(typename map<string, T> :: iterator neighbourIt = neighbours.begin(); neighbourIt != neighbours.end(); neighbourIt++) {
                string neigbourName = neighbourIt->first;
                T distanceToNeighbour = neighbourIt->second;
                if (closed.count(neigbourName) == 0) {
                    pair<string, T> neighbour = (*neighbourIt);
                    if (open.count(neigbourName) == 0) {
                         paths[neigbourName] = currentCityName;
                         T neighbourG = gScores[currentCityName] + distanceToNeighbour;
                         gScores[neigbourName] = neighbourG;

                        pair<string, T> neighbour = pair<string, T>(neigbourName, neighbourG+CartesianDistance(neigbourName, secondCity));
                        fQueue.push(neighbour);
                        open.insert(neigbourName);
                    }
                }
            }
        }
    }
    Error(PATH, firstCity + " - " + secondCity, DOESNT_EXIST);
    return pair<vector<string>, T>();
}



//Public
template <typename T>
CityMap<T>::CityMap() : out(cout), err(cerr) {}

template <typename T>
CityMap<T>::CityMap(ostream &out, ostream &err) : out(out), err(err) {}


template <typename T>
CityMap<T>::~CityMap() {}

template <typename T>
void CityMap<T>::AddCity(string cityName, T x, T y) {
    bool cityAdded = cities.insert(pair<string, Pos<T> >(cityName, Pos<T>(x,y))).second;
    if (!cityAdded) {
        Error(CITY, cityName, ALREADY_EXISTS);
    }
    else {

        adjacentRoads.insert(pair<string, map<string, T> >(cityName, map<string,T>() ));
        out << "Added "+ CITY+ ": "+cityName << endl;
    }
};

template <typename T>
void CityMap<T>::AddRoad(string firstCity, string secondCity)
{
    typename map<string, map<string, T> >::iterator firstAdjacencies  = adjacentRoads.find(firstCity);
    typename map<string, map<string, T> >::iterator secondAdjacencies  = adjacentRoads.find(secondCity);
    typename map<string, map<string, T> >::const_iterator endIt = adjacentRoads.end();
    if (firstAdjacencies == endIt) {
        Error(CITY, firstCity, DOESNT_EXIST);
    } else if (secondAdjacencies == endIt) {
        Error(CITY, secondCity, DOESNT_EXIST);
    }
    else {
        T roadLength = CartesianDistance(firstCity, secondCity);
        bool roadAlreadyExisted = !(firstAdjacencies->second).insert(pair<string, T>(secondCity, roadLength )).second
            || !(secondAdjacencies->second).insert(pair<string, T>(firstCity, roadLength )).second;
        if(roadAlreadyExisted) {
            Error(ROAD, firstCity + " - "+secondCity, ALREADY_EXISTS);
        }
        else {
            out << "Added "+ROAD+ ": "+firstCity+"-"+secondCity << endl;
        }
    }


}

template <typename T>
void CityMap<T>::RemoveCity(string cityName)
{

}


template <typename T>
void CityMap<T>::RemoveRoad(string firstCity, string secondCity)
{

}

template <typename T>
T CityMap<T>::FindDistance(string firstCity, string secondCity)
{
    pair<vector<string>, T> res = ShortestPathCore(firstCity, secondCity);
    return res.second;
}


template <typename T>
vector<string> CityMap<T>::ShortestPath(string firstCity, string secondCity)
{
    vector<string> cityPath;
    vector<string> roads = ShortestPathCore(firstCity, secondCity).first;
    for(vector<string>::iterator it = roads.begin(); it != roads.end(); it++) {
        cityPath.push_back(*it);
    }
    return cityPath;
}

template <typename T>
void CityMap<T>::PrintPath(string firstCity, string secondCity)
{
    vector<string> roads = ShortestPath(firstCity, secondCity);
    for(vector<string>::iterator it = roads.begin(); it != roads.end(); it++) {
        out << (*it) << endl;
    }
}

#endif // CITYMAP_H_INCLUDED
