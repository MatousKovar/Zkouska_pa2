/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/13/23
 */

#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <cassert>
using namespace std;

//Jses trida vole, na prvni dobrou, ted jeste aby to jelo rychle...


static unsigned NOL = 0;
class CMHD
{
public:
    void Add ( istringstream & is )
    {
        m_LineStops[NOL];
        string stop;
        while(getline(is,stop))
        {
            m_LineStops[NOL].insert(stop);
            m_StopsLines[stop].insert(NOL);

            //je potreba prubezne plnit i prestupovou mapu
            for ( const auto & a: m_StopsLines[stop])
            {
                m_Transfers[NOL].insert(a);
                m_Transfers[a].insert(NOL);
            }
        }
        NOL++;
    }
    //===============================================================================

    set < string > Dest ( const string & from, int maxCost ) const
    {
        set<string> result;
        if( m_StopsLines.find(from) == m_StopsLines.end()){
            result.insert(from);
            return result;
        }
        int currentCost = 0;

        //bude to fronta linek, protoze prestupy jsou merna jednotka
        queue<unsigned> ToVisit;
        set<unsigned> visited;

        for ( const auto & a : m_StopsLines.at(from))
            ToVisit.push(a);

        int QueueSize = ToVisit.size();

        while ( currentCost <= maxCost && !ToVisit.empty())
        {
            currentCost++;
            QueueSize = ToVisit.size();
            for (size_t i = 0 ; i < QueueSize ; i ++ )
            {
                //potrebuju vytahnout prvni ve fronte - nejaka linka
                // u te linky potrebuju kouknout na vsechny stanice
                // potrebuju pridat sousedni linky do fronty
                for (const auto & a : m_LineStops.at(ToVisit.front()))
                {
                    result.insert(a);
                }
                visited.insert(ToVisit.front());
                for ( const auto & a : m_Transfers.at(ToVisit.front()))
                    if ( visited.find(a) == visited.end())
                        ToVisit.push(a);
                ToVisit.pop();
            }
        }
        return result;

    }
    //===============================================================================
private:
    //chceme pomoci prestupu, takze potrebujeme vsechny informace o tom, kde se prestupuje
    map < size_t, set<string> > m_LineStops; // pro kazdou linku vsechny jeji zastavky
    map < string, set<int> > m_StopsLines; //pro kazdou zastavku, linky, kterymi prochazi
    map < size_t , set<size_t> > m_Transfers; // pro kazdou linku, linky, ktere protina
};

int main ( void )
{
    CMHD city;
    istringstream iss;
    iss.clear();

    iss . str ( "A\nB\nC\nD\nE\n" );
    city . Add ( iss );
    iss . clear();

    iss . str ( "B\nC\nF\nH\n" );
    city . Add ( iss );
    iss . clear();

    iss . str ( "F\nG\nI\nJ\nK\nN\n" );
    city . Add ( iss );
    iss . clear();

    iss . str ( "H\nL\n" );
    city . Add ( iss );
    iss . clear();

    iss . str ( "L\nM\nN\nO\n" );
    city . Add ( iss );
    iss . clear();

    iss . str ( "P\nQ\nR\nN\nS" );
    city . Add ( iss );
    iss . clear();

    assert ( city . Dest ( "S", 0 ) == set < string > ( {"S", "N", "R", "Q", "P"} ) );

    assert ( city . Dest ( "S", 1 ) == set < string > ( { "S", "N", "R", "Q", "P",
                "O", "M", "L",
                "K", "J", "I", "G", "F" } ) );

    assert ( city . Dest ( "N", 0 ) == set < string > ( { "S", "N", "R", "Q", "P",
                "O", "M", "L",
                "K", "J", "I", "G", "F" } ) );

    assert ( city . Dest ( "N", 1 ) == set < string > ( { "S", "N", "R", "Q", "P",
                "O", "M", "L",
                "K", "J", "I", "G", "F",
                "H", "F", "C", "B" } ) );

    assert ( city . Dest ( "N", 2 ) == set < string > ( { "S", "N", "R", "Q", "P",
                "O", "M", "L",
                "K", "J", "I", "G", "F",
                "H", "F", "C", "B",
                "A", "D", "E" } ) );

    assert ( city . Dest ( "unknown", 0 ) == set < string > ( { "unknown" } ) );
    assert ( city . Dest ( "unknown", 1 ) == set < string > ( { "unknown" } ) );
    assert ( city . Dest ( "unknown", 2 ) == set < string > ( { "unknown" }) );

    return 0;
}

