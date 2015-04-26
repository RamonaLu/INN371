#include <iostream>
#include <string>
#include <sstream>
#include "CityMap.h"


using namespace std;

bool addCityTest() {
    stringstream out;
    stringstream err;
    CityMap<double> citymap = CityMap<double>(out, err);
    citymap.AddCity("A", 5, 6);
    string res = out.str();
    return "Added City: A\n" == res;
}

bool addExistingCityTest() {
    stringstream out;
    stringstream err;
    CityMap<double> citymap = CityMap<double>(out, err);
    citymap.AddCity("A", 5, 6);
    citymap.AddCity("A", 5, 6);
    string res = err.str();
    return "Error: already exists\nCity: A\n" == res;
}

bool removeNonExistantCityTest() {
    stringstream out;
    stringstream err;
    CityMap<double> citymap = CityMap<double>(out, err);
    citymap.RemoveCity("A");
    string res = err.str();
    return "Error: doesn't exist\nCity: A\n" == res;
}

bool addingRoadTest() {
    stringstream out;
    stringstream err;
    CityMap<double> citymap = CityMap<double>(out, err);
    citymap.AddCity("A",6,5);
    citymap.AddCity("B",7,8);
    out.str("");
    citymap.AddRoad("A","B");
    string res = out.str();
    return "Added Road: A-B\n" == res;
}

bool addingRoadWithNonExistantCityTest() {
    stringstream out;
    stringstream err;
    CityMap<double> citymap = CityMap<double>(out, err);
    citymap.AddCity("B", 0,0);
    out.str("");
    citymap.AddRoad("A","B");
    string res = err.str();
    return "Error: doesn't exist\nCity: A\n" == res;
}


bool singleCityTest() {
    stringstream out;
    stringstream err;
    CityMap<double> citymap = CityMap<double>(out, err);
    citymap.AddCity("A",5,6);
    vector<string> path = citymap.ShortestPath("A","A");
    bool isItself = (path.size()==1) &&(path[0] == "A");
    bool distanceIsZero = citymap.FindDistance("A","A")== 0;
    return isItself && distanceIsZero;
}

bool twoCitiesTest() {
    stringstream out;
    stringstream err;
    CityMap<double> citymap = CityMap<double>(out, err);
    citymap.AddCity("A",0,0);
    citymap.AddCity("B",10, 0);
    citymap.AddRoad("A","B");
    vector<string> path = citymap.ShortestPath("A","B");
    vector<string> shortestPath;
    shortestPath.push_back("A");
    shortestPath.push_back("B");
    bool isPath = path == shortestPath;
    bool distanceIsTen = citymap.FindDistance("A","B")== 10;
    return isPath&&distanceIsTen;
}

bool straightLine() {
    stringstream out;
    stringstream err;
    CityMap<double> citymap = CityMap<double>(out, err);
    //Straight-line
    citymap.AddCity("A", 0, 0);
    citymap.AddCity("B", 10, 0);
    citymap.AddCity("C", 20, 0);
    citymap.AddCity("D", 30, 0);
    citymap.AddCity("E", 40, 0);
    citymap.AddRoad("A", "B");
    citymap.AddRoad("B", "C");
    citymap.AddRoad("C", "D");
    citymap.AddRoad("D", "E");

    vector<string> path = citymap.ShortestPath("A","E");
    vector<string> shortestPath;
    shortestPath.push_back("A");
    shortestPath.push_back("B");
    shortestPath.push_back("C");
    shortestPath.push_back("D");
    shortestPath.push_back("E");
    bool isPath = path == shortestPath;
    bool distanceIsForty = citymap.FindDistance("A", "E") == 40;
    return isPath && distanceIsForty;
}

bool straightLineWithDeadendBranch() {
    stringstream out;
    stringstream err;
    CityMap<double> citymap = CityMap<double>(out, err);

    //Straight-line
    citymap.AddCity("A", 0, 0);
    citymap.AddCity("B", 10, 0);
    citymap.AddCity("C", 20, 0);
    citymap.AddCity("D", 30, 0);
    citymap.AddCity("E", 40, 0);
    citymap.AddRoad("A", "B");
    citymap.AddRoad("B", "C");
    citymap.AddRoad("C", "D");
    citymap.AddRoad("D", "E");

    //Dead-end
    citymap.AddCity("F", 40, 0);
    citymap.AddCity("G", 40, 0);
    citymap.AddCity("H", 40, 0);
    citymap.AddRoad("A", "F");
    citymap.AddRoad("F", "G");
    citymap.AddRoad("G", "H");

    vector<string> path = citymap.ShortestPath("A","E");
    vector<string> shortestPath;
    shortestPath.push_back("A");
    shortestPath.push_back("B");
    shortestPath.push_back("C");
    shortestPath.push_back("D");
    shortestPath.push_back("E");
    bool isPath = path == shortestPath;
    bool distanceIsForty = citymap.FindDistance("A", "E") == 40;
    return isPath && distanceIsForty;
}

bool straightLineWithDeadendBranch2() {
    stringstream out;
    stringstream err;
    CityMap<double> citymap = CityMap<double>(out, err);

    //Straight-line
    citymap.AddCity("B", 30, 0);
    citymap.AddCity("C", 20, 20);
    citymap.AddCity("E", 45, 10);
     //Dead-end
    citymap.AddCity("A", 0, 10);
    citymap.AddCity("D", 40, 10);

    citymap.AddRoad("A", "D");
    citymap.AddRoad("A", "B");
    citymap.AddRoad("A", "C");
    citymap.AddRoad("C", "E");
    citymap.AddRoad("B", "E");

    // two equally length paths
    vector<string> path = citymap.ShortestPath("A","E");
    vector<string> shortestPathOne;
    shortestPathOne.push_back("A");
    shortestPathOne.push_back("C");
    shortestPathOne.push_back("E");

    vector<string> shortestPathTwo;
    shortestPathTwo.push_back("A");
    shortestPathTwo.push_back("B");
    shortestPathTwo.push_back("E");

    bool isPath = (path == shortestPathOne) || (path == shortestPathTwo);
    return isPath;
}

bool square() {

    CityMap<double> citymap = CityMap<double>(cout, cerr);
    citymap.AddCity("A", 0, 0);
    citymap.AddCity("B", 10, 0);
    citymap.AddCity("C", 0, 10);
    citymap.AddCity("D", 10, 10);
    citymap.AddCity("E", 5, 5);
    citymap.AddCity("F", 9, 9);
    citymap.AddRoad("A", "B");
    citymap.AddRoad("A", "C");
    citymap.AddRoad("A", "E");
    citymap.AddRoad("B", "D");
    citymap.AddRoad("E", "B");
    citymap.AddRoad("E", "C");
    citymap.AddRoad("E", "F");
    double distAD = citymap.FindDistance("A", "D");
    double distAF = citymap.FindDistance("A", "F");
    return distAD == 20 && distAF == sqrt(81 + 81) ;

}

bool acceptanceTest() {

    CityMap<double> citymap;

 // City positions are only examples
    citymap.AddCity("Canberra", 1445, 849.8);
    citymap.AddCity("Sydney", 1649, 689.0);
    citymap.AddCity("Melbourne", 1035, 1128);
    citymap.AddCity("Brisbane", 1829, -21.78);
    citymap.AddCity("Perth", -1829, 476.9);
    citymap.AddCity("Adelaide", 409.3, 807.4);
    citymap.AddCity("Hobart", 1268, 1692);
    citymap.AddCity("Darwin", -355.6, -1692);

    citymap.AddRoad("Canberra", "Sydney");
    citymap.AddRoad("Canberra", "Melbourne");
    citymap.AddRoad("Sydney", "Brisbane");
    citymap.AddRoad("Brisbane", "Darwin");
    citymap.AddRoad("Darwin", "Perth");
    citymap.AddRoad("Perth", "Adelaide");
    citymap.AddRoad("Adelaide", "Melbourne");
    citymap.AddRoad("Melbourne", "Hobart");

    cout << "From Brisbane to Sydney: " << citymap.FindDistance("Brisbane", "Sydney") << "km" << endl;
    cout << "From Sydney to Canberra: " << citymap.FindDistance("Sydney", "Canberra") << "km" << endl;
    cout << "From Canberra to Melbourne: " << citymap.FindDistance("Canberra", "Melbourne") << "km" << endl;
    cout << "From Sydney to Melbourne: " << citymap.FindDistance("Sydney", "Melbourne") << "km" << endl;
    cout << "From Melbourne to Hobart: " << citymap.FindDistance("Melbourne", "Hobart") << "km" << endl;
    cout << "From Brisbane to Melbourne: " << citymap.FindDistance("Brisbane", "Melbourne") << "km" << endl;
    cout << "From Brisbane to Adelaide: " << citymap.FindDistance("Brisbane", "Adelaide") << "km" << endl;
    cout << "From Brisbane to Hobart: " << citymap.FindDistance("Brisbane", "Hobart") << "km" << endl;
    cout << "From Brisbane to Canberra: " << citymap.FindDistance("Brisbane", "Canberra") << "km" << endl;
    cout << "From Brisbane to Perth: " << citymap.FindDistance("Brisbane", "Perth") << "km" << endl;


    cout << "Removing Sydney..." << endl;
    citymap.RemoveCity("Sydney");

    cout << "From Brisbane to Melbourne: " << citymap.FindDistance("Brisbane", "Melbourne") << "km" << endl;

    cout << "Adding Alice Springs..." << endl;

    citymap.AddCity("Alice Springs", -56.56, -440.6);
    citymap.AddRoad("Alice Springs", "Darwin");
    citymap.AddRoad("Alice Springs", "Adelaide");

    cout << "From Brisbane to Alice Springs: " << citymap.FindDistance("Brisbane", "Alice Springs") << "km" << endl;
    cout << "From Brisbane to Melbourne: " << citymap.FindDistance("Brisbane", "Melbourne") << "km" << endl;

    cout << "Removing the road between Darwin and Perth..." << endl;
    citymap.RemoveRoad("Darwin", "Perth");
    citymap.PrintPath("Brisbane", "Perth");
    cout << "From Brisbane to Perth: " << citymap.FindDistance("Brisbane", "Perth") << "km" << endl;
    return true;
}

CityMap<double> genRandom(int nCities, int maxRoadsPerCity) {
    CityMap<double> m;
    int mapSize = nCities;
    for(int i=0; i<nCities; i++) {
        double x = rand() % mapSize;
        double y = rand() % mapSize;
        ostringstream convert;
        convert << i;
        m.AddCity("City "+convert.str(), x, y);
    }
    //Ensure all cities connected
    for(int i=0; i<nCities-1; i++) {
        ostringstream fromCity;
        fromCity << i;
        ostringstream toCity;
        toCity << i+1;
        m.AddRoad("City "+fromCity.str(), "City "+toCity.str());
    }

    //Add random roads
    for(int i=0; i<nCities-1; i++) {
        ostringstream rFromCity;
        rFromCity << i;

        for (int j=0; j<rand()%maxRoadsPerCity; j++) {
            int rToCityIndex = rand() % nCities;
            ostringstream rToCity;
            rToCity << rToCityIndex;
            m.AddRoad("City "+rFromCity.str(), "City "+rToCity.str());
        }
    }
    return m;
}

bool performanceTest() {
    CityMap<double> m = genRandom(10,3);
    cout << m.FindDistance("City 0", "City 9") << endl;
    return true;
}
int main()
{
    if(
       addCityTest()
        && addExistingCityTest()
 //       && removeNonExistantCityTest()
        && addingRoadTest()
        && addingRoadWithNonExistantCityTest()
        && singleCityTest()
        && twoCitiesTest()
        && straightLine()
        && straightLineWithDeadendBranch()
        && straightLineWithDeadendBranch2()
        && square()
        && acceptanceTest()
        && performanceTest()){
        cout << "PASS" << endl;
    }
    else {
        cout << "FAIL" << endl;
    }
    return 0;
}
