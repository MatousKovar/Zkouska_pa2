/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/13/23
 */
/**
*  Ukládáme jednotlivé balíčky pomocí třídy CPkg… drží své unikátní jméno (string) a seznam závislostí (libovolný kontejner stringů, např. set).
 *  Požaduje se implementace metody void addDep(string depName), která přidá jméno balíčku do závislostí. Pokud dostaneme
 *  již obsažený balíček, metoda nic neudělá. Do třídy můžeme přidat libovolné vlastní pomocné metody.

Třída CPkgSys pak eviduje všechny balíčky, a to v možných stavech 'dostupný' a 'nainstalovaný'.
 Metoda void addPkg(CPkg input) přidá balíček do dostupných (tedy balíček se ještě nenainstaluje).
 Metoda set<string> install(list<string> input) nainstaluje balíčky ve vstupním listu a všechny jejich závislosti
 (pokud ještě nejsou nainstalované). Je potřeba rozpoznat nainstalované balíčky (nic se nestane), balíčky dostupné k
 nainstalování (přidají se do výstupního setu a nainstalují se) a balíčky které vůbec nejsou evidované
 (metoda nic nenainstaluje a vyhodí vyjímku). V této metodě je potřeba vhodně prohledávat dostupné balíčky pomocí BFS.
 Třída také musí umožňovat výpis všech nainstalovaných balíčků pomocí operator «, balíčky jsou seřazené lexikograficky
 (ideálně ukládat v nějakém setu/mapě, jsou rovnou seřazené).

Po základních testovacích datech také Progtest testoval rychlost programu, bez tohoto testu student dostal 70 % bodů (10.5 / 15).
*/

//doneeeee



#include<iostream>
#include<cassert>
#include<vector>
#include<set>
#include<map>
#include<string>
#include<unordered_map>
#include<memory>
#include<unordered_set>
#include<queue>
#include<list>
#include<sstream>
#include<cstring>

using namespace std;

class CPkg{
public:
    CPkg(const string & name) : m_Name(move(name)) {}
    CPkg & addDep(const string & name){
        m_Deps.insert(name);
        return *this;
    }
    bool operator < (const CPkg & other) const
    {
        return m_Name < other.m_Name;
    }
    friend ostream & operator << ( ostream & out, const CPkg & a){
        out << a.m_Name;
        return out;
    }

    set<string> GetDeps() const
    {
        return m_Deps;
    }
    string GetName () const {return m_Name;}

private:
    string m_Name;
    set<string> m_Deps;
};


class CPkgSys{
public:
    CPkgSys & addPkg (const CPkg & package)
    {
        m_Available.insert(package);
        return *this;
    }
    set<string> install (list<string> toInstall) {
        queue<string> DepsToAdd;
        set<string> visited;
        set<string> res;
        //nasazet do fronty vsechny zacatkovy
        auto iter = toInstall.begin();
        //dokud neprojedeme vsechny zdrojovy
        while(iter != toInstall.end()) {
            if(visited.find(*iter) == visited.end())
                DepsToAdd.push(*iter);

            while(! DepsToAdd.empty()){
                visited.insert(DepsToAdd.front());
                //add all the dependencies that werent already visited
                for(const auto & a : m_Available.find(DepsToAdd.front())->GetDeps()) {
                    if(m_Available.find(CPkg(a)) == m_Available.end())
                        throw invalid_argument("Package not found.");
                    if (visited.find(a) == visited.end())
                        DepsToAdd.push(a);
                }
                //pokud jeste neni nainstalovany, tak ho pridame do nainstalovanych a do vysledku
                if(m_Installed.find(CPkg(DepsToAdd.front())) == m_Installed.end())
                {
                    CPkg toInsert = *m_Available.find(CPkg(DepsToAdd.front()));
                    res.insert(DepsToAdd.front());
                }
                DepsToAdd.pop();
            }
            iter++;
        }
        for ( const auto & a : res)
        {
            auto iter = m_Available.find(CPkg(a));
            if(iter != m_Available.end())
                m_Installed.insert(*iter);
        }
        return res;
    }

    friend ostream & operator<< ( ostream & out, const CPkgSys & a) {
        bool first = true;
        for (const auto & a : a.m_Installed)
        {
            if (!first)
                out << ", ";
            out<< a.GetName();
            first = false;
        }
        return out;
    }
private:
    set<CPkg> m_Installed;
    set<CPkg> m_Available;
};

int main(void){
    CPkgSys s;
    stringstream ss;
    s.addPkg(CPkg("ssh").addDep("sudo").addDep("apt"))
            .addPkg(CPkg("sudo").addDep("git").addDep("c++"));
    s.addPkg(CPkg("apt"))
            .addPkg(CPkg("c++").addDep("c").addDep("asm").addDep("fortran"));
    s.addPkg(CPkg("git"))
            .addPkg(CPkg("c").addDep("kekw"))
            .addPkg(CPkg("kekw"))
            .addPkg(CPkg("asm"))
            .addPkg(CPkg("fortran"));
    s.addPkg(CPkg("python").addDep("bash").addDep("sadge"))
            .addPkg(CPkg("karel").addDep("python"))
            .addPkg(CPkg("bash").addDep("sadge"))
            .addPkg(CPkg("sadge"))
            .addPkg(CPkg("cython").addDep("dev"));
    s.addPkg(CPkg("perl"));

    ss << s;
    assert(ss.str() == "");
    ss.clear();
    ss.str("");

    set<string> t1 = s.install(list<string> {"sudo"});
    assert(t1 == (set<string> {"asm", "c", "c++", "fortran", "git", "kekw", "sudo"}));
    set<string> t2 = s.install(list<string> {"ssh", "c++"});

    assert(t2 == (set<string> {"apt", "ssh"}));

    ss << s;
    assert(ss.str() == "apt, asm, c, c++, fortran, git, kekw, ssh, sudo");
    ss.clear();
    ss.str("");

    //Nevyhazuje vyjimku
    try{
        set<string> e = s.install(list<string> {"karel", "cython"});
        assert("Sem ses nemel dostat debilku" == nullptr);
    }
    catch(const invalid_argument & e){
        assert(strcmp("Package not found.", e.what()) == 0);
    }
    set<string> t3 = s.install(list<string> {"karel", "fortran", "git"});
    for ( const auto & a : t3)
        cout << a << endl;
    assert(t3 == (set<string> {"bash", "karel", "python", "sadge"}));

    s.addPkg(CPkg("java").addDep("utils"))
            .addPkg(CPkg("utils").addDep("VB"))
            .addPkg(CPkg("VB").addDep("java"));

    set<string> t4 = s.install(list<string> {"java", "perl"});
    assert(t4 == (set<string> {"VB", "java", "perl", "utils"}));

    ss << s;
    assert(ss.str() == "VB, apt, asm, bash, c, c++, fortran, git, java, karel, kekw, perl, python, sadge, ssh, sudo, utils");
    ss.clear();
    ss.str("");

    CPkgSys k;
    k.addPkg(CPkg("ssh").addDep("sudo").addDep("apt"))
            .addPkg(CPkg("sudo").addDep("git"));
    k.addPkg(CPkg("apt"));
    k.addPkg(CPkg("git"))
            .addPkg(CPkg("c").addDep("kekw"))
            .addPkg(CPkg("kekw"))
            .addPkg(CPkg("asm"))
            .addPkg(CPkg("fortran"));
    k.addPkg(CPkg("python").addDep("bash").addDep("sadge"))
            .addPkg(CPkg("karel").addDep("python"))
            .addPkg(CPkg("bash").addDep("sadge"))
            .addPkg(CPkg("sadge"));
    k.addPkg(CPkg("perl").addDep("no"));

    set<string> t5 = k.install(list<string> {"asm"});
    assert(t5 == (set<string> {"asm"}));
    set<string> t6 = k.install(list<string> {"python", "ssh"});
    assert(t6 == (set<string> {"apt", "bash", "git", "python", "sadge", "ssh", "sudo"}));

    try{
        set<string> t7 = k.install(list<string> {"perl", "c"});
        assert("Sem ses nemel dostat debilku" == nullptr);
    }
    catch(const invalid_argument & e){
        assert(strcmp("Package not found.", e.what()) == 0);
    }
    set<string> t8 = k.install(list<string> {"c", "ssh", "karel"});

    assert(t8 == (set<string> {"c", "karel", "kekw"}));

    ss << k;
    assert(ss.str() == "apt, asm, bash, c, git, karel, kekw, python, sadge, ssh, sudo");
    ss.clear();
    ss.str("");

}