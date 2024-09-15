/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/13/23
 */

//Tohle je fakt jednoduchy to musis dat vole...

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>
using namespace std;

class CTrain{
public:
    //default constructor

    //default destructor

    //chceme jenom ke kazde zastavce pridat nasledujici zastavku, na kteru se lze z ni dostat
    void Add(istringstream & is)
    {
        string station;
        string nextStation;
        getline(is,station);
        while ( getline (is, nextStation))
        {
            m_Route[station].insert(nextStation);
            m_Route[nextStation].insert(station);
            station = nextStation;
        }

    }
    set<string> Dest (const string & from, int maxCost) {
        if (m_Route.find(from) == m_Route.end())
            throw invalid_argument("");

        int repeats = 0;
        set < string > res;
        queue<string> ToVisit;
        set < string > visited;
        ToVisit.push(from);



        while (repeats <= maxCost) {
            repeats++;
            int InLayer = ToVisit.size();
            for (int i = 0 ; i < InLayer ; i++){
            for (const auto & a: m_Route[ToVisit.front()])
                if(visited.find(a) == visited.end())
                    ToVisit.push(a);
            res.insert(ToVisit.front());
            visited.insert(ToVisit.front());
            ToVisit.pop();
            }
        }
        return res;
    }

private:
    map < string, set<string> > m_Route;
};

int main ()
{
    CTrain t0;
    istringstream iss;
    iss . clear ();
    iss . str ("Forest Hill\nNational Museum\nWhite City\nChina Town\nWembley\nNew Cross\n");
    t0 . Add ( iss );
    iss . clear ();
    iss . str ("Lady Palace\nKings Cross\nOldTown\nNew Cross\nNew Aley\n");
    t0 . Add ( iss );
    iss . clear ();
    iss . str ("ZOO\nChina Town\nKings Cross\nFantom Bridge\n");
    t0 . Add ( iss );


    set<string> s = (t0.Dest("National Museum", 1));
/*Forest Hill
  National Museum
  White City*/

    set<string> t = (t0.Dest("China Town", 0));
/*China Town*/
    assert (t == set<string> {"China Town"});
    set<string> u = (t0.Dest("China Town", 2));
/*China Town
  Fantom Bridge
  Kings Cross
  Lady Palace
  National Museum
  New Cross
  Old Town
  Wembley
  White City
  ZOO*/

    set<string> v = (t0.Dest("ZOO", 2));
/*China Town
  Kings Cross
  Wembley
  White City
  ZOO*/

    return 0;
}