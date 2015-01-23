/**
 * name: Graph class
 * authors: Jakub Olczyk (jakub.olczyk@openmailbox.com) and Marcin Natanek (natanek.marcin@gmail.com)
 * license : Beer-ware license (included below)
 *
 * description:
 *  Generyczna klasa grafu zorientowanego
 *  zaimplementowana w oparciu o listy sąsiedztwa
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Above authors wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy us a beer in return.
 * ----------------------------------------------------------------------------
 */

#ifndef ASD_GRAPH_HPP
#define ASD_GRAPH_HPP

#include <vector>                   //the base of the implementation
#include <algorithm>                //operations that deal with vector
#include <iostream>                 //printing the graph
#include <stack>
#include <utility>
#include <initializer_list>

template <typename T>
class Graph
{
	typedef std::vector<T> vt;
	typedef std::vector< std::vector<T> > vvt;

	public:
	Graph();                                //default constructor
	Graph(Graph&);                          //copying constructor
	Graph(std::initializer_list<T> list);   //initializer constructor
	~Graph();                               //default destructor

	//size operations
	size_t vert_num();                      //returns the number of vertices
	size_t neigh_num(T v);                  //returns the number of neighbours a vertix has

	//access operations
	vt operator [](T v);                    //returns reference to vector of neighbours of vertix v
	bool find_vert(T v);                    //returns true if vertix v is in graph false otherwise

	//graph manipulation operations
	void add_vert(T v);                     //adds vertix to V
	bool add_edge(T v, T u);                //creates edge from  v to u
	void zero_visited();                    //nullified all the visits in the nodes
	//returns true on success and false otherwise
	bool del_vert(T v);                     //deletes a vertex
	bool del_edge(T v, T u);                //deletes an edge

	//graph specific algorithms
	bool check_cycle(T v);              //returns true if there's a cycle from v to v
	bool check_directed();              //returns true if the graph is directed otherwise false
	bool vert_directed(T v);            //returns true if for every edge from v edge is bidirected

	//vt shortest_path(T v, T u);       //TODO

	vvt find_bridges();                 //returns all bridges in graph
	vvt connected_comp();               //returns list of connected components
	vvt biconnected_comp();               //returns list of connected components
	vt cut_vertices();                  //returns list of all cut vertices

	//graph display
	void print_verts();                 //prints a sequence of vertices
	void print_edges();                 //prints a sequence of of vertices and theirs neighbours after ":"

	private:
	struct Node
	{
		vt neighbours;                  // List of neighbours
		T value;                        // Value on vertex

		// used by functions:
		bool visited;                   // True if vertex was visited during the function execution
		Node* previous;                 // Pointer to previous node. Used in biconnected_comp().
		int low;
		int d;

		Node(T value_) :
		value(value_),
		      visited(false),
		      previous(nullptr),
		      low(0),
		d(0) {}
		Node(T value_, vt neighbours_) :
		value(value_),
		neighbours(neighbours_),
		visited(false),
		previous(nullptr),
		low(0),
		d(0) {}

		/*
		 *bool operator==(Node* other){
		 *    return this->value == other->value;
		 *}
		 */

	};

	typedef std::vector<Node> vn;

	vn node_list;                       // main list of vertices

	Node * access_vert(T v);            // allows to operate on nodes only knowing T value
	int get_index(T v);                 // returns the node_list vector index for v value
	std::vector<T> & out_arcs(int v);   // returns list of outgoing arcs from v

	void print_stack(std::stack<T> hue);
};

/** constructors and destructors **/
template<typename T>
Graph<T>::Graph() {}

template<typename T>
Graph<T>::Graph(Graph& other)
{
	this->node_list = other.node_list;
}

template<typename T>
Graph<T>::Graph(std::initializer_list<T> list)  //initializer constructor
{
	for ( auto x : list )
	{
		this->add_vert(x);
	}
}

template<typename T>
Graph<T>::~Graph() {}

/**size operations**/
template<typename T>
size_t Graph<T>::vert_num()
{
	return node_list.size();
}

template<typename T>
size_t Graph<T>::neigh_num(T v)
{
	auto neigh_it = find(node_list.begin(), node_list.end(), v);
	if (neigh_it == node_list.end()) return 0;
	else return neigh_it.size();
}

/**access**/
template<typename T>
std::vector<T> Graph<T>::operator[](T v)
{
	auto it = node_list.end();
	for (auto jt = node_list.begin(); jt != node_list.end(); ++jt)
	{
		if (jt->value == v)
		{
			it = jt;
			break;
		}
	}
	if (it != node_list.end())
		return it->neighbours;
	else
	{
		std::vector<T> tmp;
		return tmp;
	}
}

template<typename T>
bool Graph<T>::find_vert(T v)
{
	for (auto it = node_list.begin(); it != node_list.end(); ++it)
	{
		if (it->value == v) return true;
	}
	return false;
}

/**data manipulation**/
template<typename T>
void Graph<T>::add_vert(T v)
{
	Node v_vert(v);
	node_list.push_back(v_vert);
}

template<typename T>
bool Graph<T>::add_edge(T v, T u)
{
	Node v_vert(v);
	auto v_node = node_list.end();
	auto u_node = node_list.end();

	for (auto it = node_list.begin(); it != node_list.end(); ++it)
	{
		if (it->value == v)
		{
			v_node = it;
			break;
		}
	}
	for (auto it = node_list.begin(); it != node_list.end(); ++it)
	{
		if (it->value == u)
		{
			u_node = it;
			break;
		}
	}

	if (v_node != node_list.end() and u_node != node_list.end())
	{
		v_node->neighbours.push_back(u);
		return true;
	}
	return false;
}

template<typename T>
void Graph<T>::zero_visited()
{
	auto end = node_list.end();
	for (auto iter = node_list.begin(); iter != end; ++iter)
	{
		(*iter).visited = false;
		(*iter).previous = nullptr;
	}
}

template<typename T>
bool Graph<T>::del_vert(T v)
{
	auto it_v = node_list.end();
	for (auto it = node_list.begin(); it != node_list.end(); ++it)
	{
		if (it->value == v)
		{
			it_v = it;
			break;
		}
	}
	if (it_v != node_list.end())
		node_list.erase(it_v);
	return true;
}

template<typename T>
bool Graph<T>::del_edge(T v, T u)
{
	auto it_v = node_list.end();
	for (auto it = node_list.begin(); it != node_list.end(); ++it)
	{
		if (it->value == v)
		{
			it_v = it;
			break;
		}
	}
	if (it_v != node_list.end())
	{
		auto it_u = it_v->neighbours.end();
		for (auto jt = it_v->neighbours.begin(); jt != it_v->neighbours.end(); ++jt)
		{
			if (*jt == u)
			{
				it_u = jt;
				break;
			}
		}
		if (it_u != it_v->neighbours.end())
		{
			it_v->neighbours.erase(it_u);
			return true;
		}
	}
	return false;
}

template<typename T>
bool Graph<T>::check_directed()
{
	bool answer = false;
	auto end = node_list.end();
	for (auto iter = node_list.begin(); iter != end; ++iter)
	{
		answer |= check_cycle(iter->value);
	}
	return !answer; //FIXME
}

template<typename T>
bool Graph<T>::vert_directed(T v)  // not-working-properly
{
	bool outcome = false;
	auto v_vert = access_vert(v);

	if (v_vert->value != -1)
	{
		for (auto it = v_vert->neighbours.begin(); it != v_vert->neighbours.end(); ++it)
		{
			//TODO FIXME : theres a bug in here

			auto u_vert = access_vert(*it);

			bool found = false;

			for (auto jt = u_vert->neighbours.begin(); jt != u_vert->neighbours.end(); ++jt)
			{
				if (*jt == u_vert->value)
				{
					found = true;
					break;
				}
			}
			if (!found) break;
			outcome &= found;
		}
	}
	return outcome;
}

template<typename T>
bool Graph<T>::check_cycle(T v)
{
	this->zero_visited();
	std::stack< Graph<T>::Node * > stos;
	stos.push(access_vert(v));
	while (!stos.empty())
	{
		Node* u = stos.top();
		stos.pop();
		u->visited = true;
		auto end = u->neighbours.end();
		for (auto iter = u->neighbours.begin(); iter != end; ++iter)
		{
			if (!access_vert(*iter)->visited)
				stos.push(access_vert(*iter));
			else if (*iter == v)
			{
				return true;
			};
		}
	}
	return false;
}

/**graph printing**/
template<typename T>
void Graph<T>::print_verts()
{
	for (auto it : node_list)
		std::cout << it.value << " ";
	std::cout << std::endl;
}

template<typename T>
void Graph<T>::print_edges()
{
	for (auto it : node_list)
	{
		std::cout << it.value << " : ";
		for (auto jt : it.neighbours)
		{
			std::cout << jt << " ";
		}
		std::cout << std::endl;
	}
}

/**connected components**/
template<typename T>
typename Graph<T>::vvt Graph<T>::connected_comp()
{
	if (vert_num() == 0) return std::vector<std::vector<T>>();

	std::vector<bool> visited(vert_num(), false);
	std::vector<std::vector<T>> out;

	for ( auto node : node_list )
	{
		if (!visited[get_index(node.value)])
		{
			std::vector<T> tmp;
			std::stack<int> Stos;
			Stos.push(get_index(node.value));
			while (!Stos.empty())
			{
				int x = Stos.top();
				Stos.pop();
				if (!visited[x])
				{
					tmp.push_back(node_list[x].value);
					visited[x] = true;
					for (auto iter : node_list[x].neighbours)
					{
						int i = get_index(iter);
						if (!visited[i])
						{
							Stos.push(i);
						}
					}
				}
			}
			out.push_back(tmp);
		}
	}
	return out;
}

template<typename T>
void Graph<T>::print_stack(std::stack<T> hue)
{
	std::cout << "Stos: ";
	while (!hue.empty())
	{
		std::cout << node_list[hue.top()].value << " ";
		hue.pop();
	}
	std::cout << std::endl;
}

template<typename T>
typename Graph<T>::vvt Graph<T>::biconnected_comp()
{
	if (this->node_list.size() == 0)
	{
		return {};
	}

	typedef typename std::vector<T>::iterator Iter;

	std::vector<bool> visited(this->node_list.size(), false);
	std::vector<bool> is_articulation_point(this->node_list.size(), false);
	std::vector<int> parent(this->node_list.size(), -1);
	std::vector<int> depth(this->node_list.size(), std::numeric_limits<int>::max());
	std::vector<int> min_succ_depth(this->node_list.size(), std::numeric_limits<int>::max());
	std::vector<Iter> next_out_arc(this->node_list.size());

	const int root = 0;

	std::stack<int> Stos;

	Stos.push(root);
	depth[root] = 0;

	std::stack<int> kolejnosc_odwiedzin;
	vt current_bcc;
	vvt odpowiedz;



	while (!Stos.empty())
	{
		int x = Stos.top();
		Stos.pop();

		if (!visited[x])
		{
			visited[x] = true;
			next_out_arc[x] = out_arcs(x).begin();
			min_succ_depth[x] = depth[x];
			kolejnosc_odwiedzin.push(x);
			print_stack(kolejnosc_odwiedzin);
		}
		else
		{
			int y = get_index(*(next_out_arc[x]));
			if (min_succ_depth[y] >= depth[x] && !is_articulation_point[x] && x != root)
			{
				is_articulation_point[x] = true;

				//dziwki!
				int tmp = kolejnosc_odwiedzin.top();
				while (tmp != x)
				{
					tmp = kolejnosc_odwiedzin.top();
					kolejnosc_odwiedzin.pop();
					current_bcc.push_back(node_list[tmp].value);
				}
				odpowiedz.push_back(current_bcc);
				current_bcc.clear();
			}
			min_succ_depth[x] = std::min(min_succ_depth[x], min_succ_depth[y]);
			++next_out_arc[x];
		}

		auto end = out_arcs(x).end();
		while (next_out_arc[x] != end)
		{
			int y = get_index(*(next_out_arc[x]));

			if (visited[y])
			{
				if (parent[x] != y)
				{
					min_succ_depth[x] = std::min(min_succ_depth[x], depth[y]);
				}
				++next_out_arc[x];
			}
			else
			{
				Stos.push(x);
				Stos.push(y);
				parent[y] = x;
				depth[y] = depth[x] + 1;
				break;
			}
		}
	}

	int root_child_count = 0;
	for (auto y : out_arcs(root))
	{
		if (parent[y] == root)
			++root_child_count;
	}

	if (root_child_count >= 2)
		is_articulation_point[root] = true;


	return std::move(odpowiedz);
}

template<typename T>
std::vector<T> Graph<T>::cut_vertices()
{
	if (this->node_list.size() == 0)
	{
		return {};
	}

	typedef typename std::vector<T>::iterator Iter;

	std::vector<bool> visited(this->node_list.size(), false);
	std::vector<bool> is_articulation_point(this->node_list.size(), false);
	std::vector<int> parent(this->node_list.size(), -1);
	std::vector<int> depth(this->node_list.size(), std::numeric_limits<int>::max());
	std::vector<int> min_succ_depth(this->node_list.size(), std::numeric_limits<int>::max());
	std::vector<T> articulation_point_list;
	std::vector<Iter> next_out_arc(this->node_list.size());

	const int root = 0;

	std::stack<int> Stos;

	Stos.push(root);
	depth[root] = 0;

	while (!Stos.empty())
	{
		int x = Stos.top();
		Stos.pop();

		if (!visited[x])
		{
			visited[x] = true;
			next_out_arc[x] = out_arcs(x).begin();
			min_succ_depth[x] = depth[x];
		}
		else
		{
			int y = get_index(*(next_out_arc[x]));
			//std::cout << "sprawdzam czy min_succ_depth['" << node_list[y].value << "'] (" << min_succ_depth[y]
			//<< ") >= depth['" << node_list[x].value << "'] (" << depth[x] << ")" << std::endl;
			if (min_succ_depth[y] >= depth[x] && !is_articulation_point[x] && x != root)
			{
				is_articulation_point[x] = true;
				articulation_point_list.push_back(node_list[x].value);
			}
			min_succ_depth[x] = std::min(min_succ_depth[x], min_succ_depth[y]);
			++next_out_arc[x];
		}

		auto end = out_arcs(x).end();
		while (next_out_arc[x] != end)
		{
			int y = get_index(*(next_out_arc[x]));

			if (visited[y])
			{
				if (parent[x] != y)
				{
					min_succ_depth[x] = std::min(min_succ_depth[x], depth[y]);
				}
				++next_out_arc[x];
			}
			else
			{
				Stos.push(x);
				Stos.push(y);
				parent[y] = x;
				depth[y] = depth[x] + 1;
				break;
			}
		}
	}

	int root_child_count = 0;
	for (auto y : out_arcs(root))
	{
		if (parent[y] == root)
			++root_child_count;
	}

	if (root_child_count >= 2)
		articulation_point_list.push_back(node_list[root].value);

	return std::move(articulation_point_list);
}

template<typename T>
typename Graph<T>::Node* Graph<T>::access_vert(T v)     //allows to operate on nodes only with T value
{
	auto end = node_list.end();
	for (auto iter = node_list.begin(); iter != end; ++iter)
	{
		if (iter->value == v) return &(*iter);
	}
	return nullptr;
}

template<typename T>
typename std::vector<T> & Graph<T>::out_arcs(int v)     // returns list of outgoing arcs from v
{
	return node_list[v].neighbours;
}

template<typename T>
int Graph<T>::get_index(T v)
{
	for (int i = 0; i < node_list.size(); i++)        //kurwa, trzeba to przecież znaleźć
	{
		if (node_list[i].value == v)
		{
			return i;
		}
	}
	return -1;
}

#endif
