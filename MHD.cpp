/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/13/23
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>

using namespace std;

/**
 * Jsi frajer
 *
 */



static unsigned NumberOfLine = 0;

class CPublicTransport {
public:
    CPublicTransport(){}


    //potrebuju linka, sousedni linky tudiz pro vsechny zastavky, ktere jsou v aktualni lince, chci pridat jeste jejich ostatni linky pomoci m_StopLines
    CPublicTransport& addLine(unsigned cost, const vector<string>& stops) {
        set<string> stopset;
        for (const auto & a : stops)
            stopset.insert(a);
        m_Lines[NumberOfLine] = Line({cost,stopset});
        //projdu vsechny linky a pokusim se v nich najit nejaky prunik
        for (const auto & b : m_Lines) {
            for (const auto & a: m_Lines[b.first].stops)
                if (stopset.find(a) != stopset.end()) {
                    m_Neighbors[b.first].insert(NumberOfLine);
                    m_Neighbors[NumberOfLine].insert(b.first);
                    break;
                }
        }
        NumberOfLine++;
        return *this;
    }

    CPublicTransport& optimize() {
        return *this;
    }

    unsigned findCheapest(const string& from, const string& to) {
        queue<pair<unsigned, unsigned>> q; //line, price
        map<unsigned, unsigned> visited; // stop, minimal price
        set<unsigned> targetLines;

        if (from == to )
            return 0;

        //linky, na ktere se chceme dostat
        for ( const auto & a : m_Lines)
            if(a.second.stops.count(to) != 0)
                targetLines.insert(a.first);

        //potrebuju najit linku, ve ktere je from...
        for ( const auto & a : m_Lines)
            if(a.second.stops.count(from) != 0) {
                q.push({a.first, a.second.cost});
                visited[q.front().first] = a.second.cost;
            }

        while(!q.empty())
        {
            auto currentVisited = q.front();
            q.pop();

            //udelat to nejak na dve vrstvy, aby se porad nezvedala current price
            for (const auto & a : m_Neighbors[currentVisited.first])
            {
                if(visited.count(a) == 0 || visited[a] > currentVisited.second + m_Lines[a].cost){
                    q.push({a,currentVisited.second + m_Lines[a].cost});
                    visited[a] = currentVisited.second + m_Lines[a].cost;
                }
            }
        }
        unsigned min = 9999;
        for ( const auto &  a : targetLines)
            if (visited[a] < min)
                min = visited[a];
        return min;

    }

private:
    struct Line {
        unsigned cost;
        set<string> stops;
    };

    map<unsigned, Line> m_Lines;
    map<unsigned, set<unsigned> > m_Neighbors;
};

int main() {

    CPublicTransport t;

    t.addLine(1, {"Staromestska", "Muzeum", "Mustek", "Namesti Miru", "Jiriho z Podebrad"}).
            addLine(3, {"Hlavni Nadrazi", "Orionka", "Baterie", "Petriny", "Jiriho z Podebrad"}).
            addLine(2, {"Kamenicka", "Letenske namesti", "Baterie", "Petriny", "Namesti Miru", "Malovanka"}).optimize();


    assert(t.findCheapest("Staromestska", "Baterie") == 3);
    assert(t.findCheapest("Staromestska", "Staromestska") == 0);
    assert(t.findCheapest("Staromestska", "Namesti Miru") == 1);
    assert(t.findCheapest("Staromestska", "Hlavni Nadrazi") == 4);
    assert(t.findCheapest("Orionka", "Kamenicka") == 5);

    t.addLine(6, {"Vitezne namesti", "I P Pavlova", "Andel"});

    try {t.findCheapest("Orionka", "Andel"); }
    catch ( const logic_error & e ) { assert(e.what() != "");}

}