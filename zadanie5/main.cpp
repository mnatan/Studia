#include "Point.hpp"
#include <iostream>
#include <vector>

int main()
{
	//wczytaj punkty
	int number_of_points;
	scanf("%d", &number_of_points);
	printf("  %d  = liczba puntow\n", number_of_points);
	std::vector<Point> punkty;
	float x;
	float y;
	char label = 'A';
	for (int i = 0; i < number_of_points; i++)
	{
		scanf("%f%f", &x, &y);
		punkty.push_back(Point(x, y, label));
		std::cout << Point(x, y, label) << std::endl;
		label++;
	}

	printf("przez wszystkie w jedna strone\n ");
	float prosto;
	auto end = punkty.end();
	--end;
	for (auto it = punkty.begin(); it != end;)
	{
		printf("%c", (*it).label);
		prosto += dist(*it, *(it+1));
		++it;
	}
	printf("A");
	prosto += dist(punkty[0], punkty.back());
	printf(" -->   %6.2f\n",prosto);

	//oblicz najkrotsza sciezke
	auto wynik = najkrotszy_bitonic(punkty);
	std::cout << "===WYNIK===" << std::endl;
	for (auto x : wynik.kolejnosc)
	{
		std::cout << x.label;
	}
	std::cout << wynik.kolejnosc[0].label;
	std::cout << ", dlugosc = " << wynik.dlugosc + dist(wynik.kolejnosc[0], wynik.kolejnosc.back()) << std::endl;

	return 0;
}
