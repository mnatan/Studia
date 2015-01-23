#include <iostream>
#include "Graph.hpp"

using namespace std;

int main()
{
	Graph<int> G;

	int liczba_wieszcholkow, liczba_krawedzi, tmp_val;

	std::cin >> liczba_wieszcholkow;
	std::cin.ignore(256, '\n');

	for (int i = 0; i < liczba_wieszcholkow; i++)
		G.add_vert(i + 1);

	for (int i = 0; i < liczba_wieszcholkow; i++)
	{
		std::cin >> liczba_krawedzi;
		for (int j = 0; j < liczba_krawedzi; j++)
		{
			std::cin >> tmp_val;
			G.add_edge(i + 1, tmp_val);
		}
		std::cin.ignore(256, '\n');
	}

	cout << endl << "Podano graf G:" << endl;
	G.print_edges();
	std::cout << std::endl;

	auto spojne = G.connected_comp();
	std::cout << spojne.size() << " - spójna składowa" << std::endl;
	for (auto i : spojne)
	{
		std::cout << "{";
		for (auto j : i)
		{
			std::cout << j << ", ";
		}
		std::cout << "} ";
	}
	std::cout << std::endl;

	auto dwuspojne = G.biconnected_comp();
	std::cout << dwuspojne.size() << " - dwuspójne składowe" << std::endl;
	for (auto i : dwuspojne)
	{
		std::cout << "{";
		for (auto j : i)
		{
			std::cout << j << ", ";
		}
		std::cout << "} ";
	}
	std::cout << std::endl;

	auto wieszcholki_rodz = G.cut_vertices();
	std::cout << wieszcholki_rodz.size() << " - wieszchołki rozdzielające" << std::endl;
	for ( auto x : wieszcholki_rodz)
	{
		std::cout << x << ",";
	}
	std::cout << std::endl;
	//H.biconnected_comp(4);

	return 0;
}
