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

// represents the coordinates of a city
struct Pos {
    Pos() {};
    Pos(T x, T y) : x(x), y(y){}
    T x;
    T y;
};

// represents a collection of cities and roads between them;
// provides an interface of calculating the shortest distance between any two cities, and construct a path of cities along the shortest path;
// for each city, the roads connecting to it are stored along with the road length,
// the road length is assumed to be the distance between two cities on a cartesian plane
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
        T FindDistance(string firstCity, string secondCity); // find shortest distance between two cities
        vector<string> ShortestPath(string firstCity, string secondCity); // cities on the path
        void PrintPath(string firstCity, string secondCity); // display cities on the path and distances
    private:
        void Error(string subjectType, string subject, string reason); // template for displaying error message
        T CartesianDistance(Pos<T> pos1, Pos<T> pos2); // get distance between points on a cartesian plane
        T CartesianDistance(string firstCity, string secondCity); // get distance between two cities
        vector<string> ReconstructPath(map<string, string> paths, string current); // get the cities visited on the shortest path
        pair<vector<string>, T> ShortestPathCore(string firstCity, string secondCity); // A* algorithm
        map<string, Pos<T> > cities; // map of city name and its coordinates
        map<string, map<string, T> > adjacentRoads; // map of cities and a map containing their neighbors and distances to their neighbors
        ostream &out;
        ostream &err;


};
//Constants
const string ALREADY_EXISTS = "already exists";
const string DOESNT_EXIST = "doesn't exist";
const string MUST_BE_DIFFERENT = "must be different";
const string CITY = "City";
const string ROAD = "Road";
const string PATH = "Path";

//Private
// template for displaying error message
template <typename T>
void CityMap<T>::Error(string subjectType, string subject, string reason) {
    err << "Error: " << reason << endl;
    err << subjectType << ": " << subject << endl;
}

// takes two cities and get distance between them
// precondition: the cities are in the cities map
template <typename T>
T CityMap<T>::CartesianDistance(string firstCity, string secondCity)
{
    return CartesianDistance(cities[firstCity], cities[secondCity]);
}

// takes two points and gets distance between points on a cartesian plane
// precondition: two exsiting points of struct Pos
template <typename T>
T CityMap<T>::CartesianDistance(Pos<T> pos1, Pos<T> pos2)
{
    T deltaX = pos2.x - pos1.x;
    T deltaY = pos2.y - pos1.y;
    return sqrt(deltaX * deltaX + deltaY*deltaY);
}

// comparer for priority queue
template <typename T>
struct Lesser {
    bool operator()(const pair<string, T >& a, const pair<string, T >& b) const{
        return a.second >  b.second;
    }
};

// takes a map which contains a city and its previous city on the path, and a city; gets all the cities visited on the shortest path
// precondition: the map exists and the city is included in the map as a key
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

// takes two cities and gets a pair which contains the cities on the shortest path and the shortest distance between the two cities
// precondition: the cities are in the cities map
template <typename T>
pair<vector<string>, T> CityMap<T>::ShortestPathCore(string firstCity, string secondCity) {
    priority_queue <pair<string, T>, vector<pair<string, T> >, Lesser<T> > fQueue; // priority queue of f scores (g score + h score)with the lowest f score on the top
    set<string> open; // the set of nodes to be evaluated, initially containing the start node
    set<string> closed; // the set of nodes already evaluated
    map<string, T> gScores; // map of a node and its distance from the start along the best known path
    map<string,string> paths; // map of a node and its previous node
    T g = 0;
    T h = CartesianDistance(firstCity, secondCity); // distance between a node and the destination node
    gScores[firstCity] = g;
    pair<string, T> current = pair<string, T>(firstCity, g+h);
    fQueue.push(current);
    open.insert(firstCity);

    while(!open.empty()) {
        current = fQueue.top();
        fQueue.pop();
        string currentCityName = current.first;
        open.erase(currentCityName);
        closed.insert(currentCityName);

        // if destination is reached, constructs shortest path and gets shortest distance
        bool goalReached = currentCityName == secondCity;
        if (goalReached) {
            vector<string> shortestPath = ReconstructPath(paths, currentCityName);
            T shortestDist = gScores[currentCityName];
            return pair<vector<string>, T>(shortestPath, shortestDist);
        }

        else {
            // iterates all the neighbors of current city and their distances from current city
            map<string, T> neighbours = adjacentRoads[currentCityName];
            for(typename map<string, T> :: iterator neighbourIt = neighbours.begin(); neighbourIt != neighbours.end(); neighbourIt++) {
                pair<string, T> neighbour = (*neighbourIt);
                string neighbourName = neighbour.first;
                T distanceToNeighbour = neighbour.second;

                // if neighbour has not been evaluated,
                bool neighbourNotClosed = closed.count(neighbourName) == 0;
                if (neighbourNotClosed) {
                    // if neighbour has not been added to the to-be-evaluated set,
                    // adds it to the to-be-evaluated set and calculates f score of the neighbour and adds to the f queue
                    bool neighbourNotOpen = open.count(neighbourName) == 0;
                    if (neighbourNotOpen) {
                         paths[neighbourName] = currentCityName;
                         T neighbourG = gScores[currentCityName] + distanceToNeighbour;
                         gScores[neighbourName] = neighbourG;

                        pair<string, T> neighbourF = pair<string, T>(neighbourName, neighbourG+CartesianDistance(neighbourName, secondCity));
                        fQueue.push(neighbourF);
                        open.insert(neighbourName);
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

// add city and its coordinate to the cities map
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

// add road to the adjacentRoads map with firstCity as key and with secondCity and distance between them as value
template <typename T>
void CityMap<T>::AddRoad(string firstCity, string secondCity)
{
   if (firstCity == secondCity) {
        Error(ROAD, firstCity + " - " + secondCity, MUST_BE_DIFFERENT);
    }
    else {
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
}

// remove city from the cites map and all the roads connecting the city to other cities
template <typename T>
void CityMap<T>::RemoveCity(string cityName)
{
    typename map<string, Pos<T> >::iterator cityIt = cities.find(cityName);
    if (cityIt == cities.end()) {
        Error(CITY, cityName, DOESNT_EXIST);
    }
    else {
        typename map<string, map<string, T> >::iterator adjacentRoadsIt = adjacentRoads.find(cityName);
        map<string, T> roadsToRemove = adjacentRoadsIt->second;

        for(typename map<string, T>::iterator roadsToRemoveIt = roadsToRemove.begin(); roadsToRemoveIt != roadsToRemove.end(); roadsToRemoveIt++) {
            string neighbour = roadsToRemoveIt->first;
            map<string, T> neighbourRoads = adjacentRoads.find(neighbour)->second;

            neighbourRoads.erase(cityName);
        }
        roadsToRemove.clear();
        adjacentRoads.erase(cityName);
        cities.erase(cityName);
    }
}


// remove road between two cities
template <typename T>
void CityMap<T>::RemoveRoad(string firstCity, string secondCity)
{
    typename map<string, map<string, T> >::iterator firstAdjacentRoadsIt = adjacentRoads.find(firstCity);
    typename map<string, map<string, T> >::iterator secondAdjacentRoadsIt = adjacentRoads.find(secondCity);

    if (firstAdjacentRoadsIt == adjacentRoads.end()) {
        Error(CITY, firstCity, DOESNT_EXIST);
    }
    else if (secondAdjacentRoadsIt == adjacentRoads.end()) {
        Error(CITY, secondCity, DOESNT_EXIST);
    }
    else {
        map<string, T> firstAdjacentRoads = (firstAdjacentRoadsIt->second);
        map<string, T> secondAdjacentRoads = (secondAdjacentRoadsIt->second);
        bool secondToFirstExists = secondAdjacentRoads.find(firstCity) != secondAdjacentRoads.end();
        bool firstToSecondExists =  firstAdjacentRoads.find(secondCity) != firstAdjacentRoads.end();
        if (!(secondToFirstExists && firstToSecondExists)) {
            Error(ROAD, firstCity + " - " + secondCity, DOESNT_EXIST);
        }
        else {
            firstAdjacentRoads.erase(secondCity);
            secondAdjacentRoads.erase(firstCity);
        }
    }
}

// takes two cities and find the shortest distance between them
// precondition: the cities are added in the cities map
template <typename T>
T CityMap<T>::FindDistance(string firstCity, string secondCity)
{
    pair<vector<string>, T> res = ShortestPathCore(firstCity, secondCity);
    return res.second;
}

// takes two cities and gets all the cities visited on the shortest path
// precondition: the cities are added in the cities map
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

// takes two cities as start and destination and prints all the cities visited on the shortest path and the distances between each
// precondition: the cities are in the cities map
template <typename T>
void CityMap<T>::PrintPath(string firstCity, string secondCity)
{
    vector<string> roads = ShortestPath(firstCity, secondCity);
    string previousCity;
    for(vector<string>::iterator it = roads.begin(); it != roads.end(); it++) {
        string currentCity = (*it);
        if (previousCity != "") {
            out << CartesianDistance(previousCity, currentCity) << endl;
        }
        out << currentCity << endl;
        previousCity = currentCity;
    }
}

#endif // CITYMAP_H_INCLUDED
