#include <iostream> 
#include <exception>
#include <algorithm>
#include <random>
#include <vector>

class Graph {

    static int randomize( int min, int max )
    {
        std::random_device rd; std::mt19937 gen{ rd() };
        std::uniform_int_distribution<> distrib{ min, max };

        return distrib(gen);
    }

    class Edge { public:

        int from, to; double weight; bool wchecked = false, cchecked = false;

        Edge( int _f, int _t ) : from {_f}, to {_t}

        { weight = static_cast<double>(randomize(1,1000)) * 0.01; }

    };

    std::vector<Edge> edges; int esize, nsize;

    public:

    bool iscycle( std::vector<Edge> edges ) // searching for a cycle ->
    {
        bool cycle = false;

        for ( auto &e0 : edges ) if ( e0.from > e0.to ) std::swap( e0.from, e0.to );

        auto sortFr = [] ( Edge a, Edge b ) { return a.from < b.from; };
        std::sort( edges.begin(), edges.end(), sortFr );

        auto sortTo = [] ( Edge a, Edge b ) { return a.to < b.to; };
        std::sort( edges.begin(), edges.end(), sortTo );

        std::vector<int> visited;

        class iTedges { public:

            void operator()( std::vector<Edge> &edges, Edge &estart, std::vector<int> &visited )

            {
                estart.cchecked = true;

                visited.push_back( estart.to );

                for ( auto &e0 : edges ) { bool pathend = false;

                    if ( !e0.cchecked && estart.to == e0.from )

                    { this->operator()( edges, e0, visited ); pathend = true; }

                    if ( pathend ) break;
                }
            }
        };

        for ( auto& e0 : edges ) { iTedges itE{};

            if ( !e0.cchecked ) { bool isfrom = false, isto = false;

                for ( auto &v0 : visited ) {

                    if ( v0 == e0.from ) isfrom = true; if ( v0 == e0.to ) isto = true;

                } if ( isfrom && isto ) { cycle = true; break; } visited.clear();

                visited.push_back(e0.from); itE.operator()( edges, e0, visited ); }
        }

        if ( cycle ) std::cout << "CYCLE ! -> ";

        return cycle;
    }

    // creates a random graph with esize vertices and nsize edges ->

    Graph( int _esize, int _nsize ) : esize { _esize }, nsize { _nsize }
    {       
        if ( ( ( ( esize + 1 ) * esize ) / 2 ) - 1 < nsize ) throw std::runtime_error("Too few vertices / too many edges !");

        else if ( esize > nsize + 1 ) throw std::runtime_error("Too many vertices / too few edges !");

        /* for ( int i = 0; i <= nsize; i++ ) { bool yesno = true; int from2, to2;

            while ( yesno ) {  _re :

                from2 = ( i < esize ) ? i : randomize( 0, esize );
                to2 = ( i < esize ) ? randomize( 0, esize ) : randomize( 0, esize );

                if ( to2 == from2 ) goto _re; // ?

            if ( i > 0 ) { for ( auto& e : edges ) {

                yesno = ( ( to2 == e.from && from2 == e.to ) || ( to2 == e.to && from2 == e.from ) ) ?

                        true : false; if ( yesno ) break; } } else yesno = false;

            if ( i < esize && !yesno ) { edges.push_back( Edge{ from2, to2 } );

            yesno = iscycle( edges ); edges.pop_back();

            if ( yesno ) std::cout << from2 << " : " << to2 << std::endl; }

            } edges.push_back( Edge{ from2, to2 } );

            std::cout << from2 << " - " << to2 << " : " << edges.back().weight << std::endl;

        } */

        edges.push_back({0,2}); edges.push_back({2,1}); edges.push_back({1,4}); edges.push_back({5,6});
        std::cout << "\n ---> " << iscycle( edges ) << "\n";
    };

    std::vector<Edge> presult;

    void Prim()
    {
        std::cout << std::endl;

        if ( esize - 1 == nsize ) {

            std::cout << "The graph itself its own minimum spanning tree..." << std::endl; return; }

        int ecnt = 0;

        while ( ecnt < esize ) { int pos = 0, ind = 0; double minw = 99;

            for ( auto& e0 : edges ) {

                if ( e0.weight < minw && !e0.wchecked ) { minw = e0.weight; pos = ind; } ind++;

            }

            presult.push_back( edges.at(pos) ); edges.at(pos).wchecked = true;

            if ( iscycle( presult ) ) presult.pop_back(); else ecnt++;

        }

        for ( auto &p0 : presult ) std::cout << p0.weight << " - ";

        std::cout << std::endl;

    }

    void Dijkstra()
    {


    }

};

int main()
{
    try { Graph g { 4, 3 };

          g.Prim();

          g.Dijkstra();

    } catch ( std::runtime_error e ) { std::cout << e.what() << std::endl; }

    return 0;
}