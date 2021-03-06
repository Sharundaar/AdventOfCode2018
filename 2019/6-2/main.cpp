#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <functional>

#include "../helpers/types.h"
#include "../helpers/graph.hpp"

using namespace std;

struct orbit
{
    char source[4];
    char orbiter[4];
};



int get_id( const vector<char*>& id_map, const char* obj_name )
{
    for( int i=0; i<id_map.size(); ++i )
    {
        if( strcmp( id_map[i], obj_name ) == 0 )
            return i;
    }

    assert( false );
    return -1;
}

int main()
{
    ifstream file( "input.txt" );
    if( file )
    {
        string line;
        vector<orbit> orbits;
        
        while( getline( file, line ) )
        {
            orbit o;
            memcpy( o.source, line.c_str(), 3 * sizeof( char ) );
            o.source[3] = '\0';
            memcpy( o.orbiter, line.c_str() + 4, 3 * sizeof( char ) );
            o.orbiter[3] = '\0';
            orbits.emplace_back( o );
        }

/*
        vector<char*> object_to_id_map;
        for( auto& o: orbits )
        {
            bool found_orbiter = false;
            bool found_source = false;
            for( char* id: object_to_id_map )
            {
                if( strcmp( id, o.orbiter ) == 0 )
                    found_orbiter = true;
                if( strcmp( id, o.source ) == 0 )
                    found_source = true;

                if( found_orbiter && found_source )
                    break;
            }

            if( !found_source )
                object_to_id_map.push_back( o.source );
            if( !found_orbiter )
                object_to_id_map.push_back( o.orbiter );
        }
*/

        graph<char*, 10000> g;
        char* COM = "COM";
        g.add_node( nullptr, COM );
        g.iterate_all_nodes( [&]( graph_node<char*>& node ) -> void {
            for( auto& o: orbits )
            {
                if( strcmp( o.source, node.data ) == 0 )
                    g.add_node( &node, o.orbiter );
            }
        } );

        graph_node<char*> *YOU;
        g.iterate_all_nodes_with_stops( [&]( graph_node<char*>& node ) -> bool {
            if( strcmp( node.data, "YOU" ) == 0 )
            {
                YOU = &node;
                return true;
            }

            return false;
        } );

        char* target = "SAN";
        auto find_target = [&]( char* data ) {
            return strcmp( data, target ) == 0;
        };

        graph_node<char*> *current_node = YOU->parent;
        int link_count = 0;
        while( true )
        {
            graph_node<char*>* san_node = current_node->find_child( find_target );
            if( san_node )
            {
                graph_node<char*> *iter_node = san_node->parent;
                while( iter_node != current_node )
                {
                    link_count++;
                    iter_node = iter_node->parent;
                }
                break;
            }
            else
            {
                link_count++;
                current_node = current_node->parent;
            }
        }
        cout << link_count << endl;

    }

    return 0;
}