#include "Point.hpp"
#include <iostream>
#include <vector>

int main()
{
	int number_of_points;
	std::cin >> number_of_points;
	std::cout << "  " << number_of_points << " = liczba puntow\n";
	std::vector<Point> points;
	float x, y;
	char label = 'A';
	for (int i = 0; i < number_of_points; i++) {
		std::cin >> x >> y;
		points.push_back(Point(x, y, label));
		std::cout << Point(x, y, label) << std::endl;
		++label;
	}

	std::cout << "przez wszystkie w jedna strone\n ";
	float straight;
	auto end = points.end();
	--end;
	for (auto it = points.begin(); it != end;) {
		std::cout << it->label;
		straight += dist(*it, *(it+1));
		++it;
	}
	std::cout << "A";
	straight += dist(points[0], points.back());
	printf(" -->   %6.2f\n",straight);

	// Calculate the shortest bitonic tour
	auto result = optimal_bitonic_tour(points);
	std::cout << "===WYNIK===" << std::endl;
	for (auto x : result.order)
		std::cout << x.label;
	std::cout << result.order[0].label;
	std::cout << ", length = " <<
		result.length + dist(result.order[0], result.order.back()) << '\n';

	return 0;
}
