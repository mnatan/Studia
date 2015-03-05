#ifndef POINT
#define POINT

#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>

struct Point {
	float x, y; // Coordinates
	char label;

	Point() : x (0), y (0), label ('0') {}
	Point (float nx, float ny, char nlabel) : x (nx), y (ny), label (nlabel) {}
};

bool operator==(const Point& a, const Point& b)
{
	// We assume the labels are unique
	return a.label == b.label;
}

std::ostream& operator<<(std::ostream& out, const Point& p)
{
	return out << " " << p.label << ": ( " << p.x << " , " << p.y << ")";
}

// Calculate euclidean distance between two points
float dist(const Point lhs, const Point rhs)
{
	float x_diff = rhs.x - lhs.x;
	float y_diff = rhs.y - lhs.y;
	return sqrt(x_diff * x_diff + y_diff * y_diff);
}

struct Path {
	float length;
	std::vector<Point> order;

	Path(float nl, std::vector<Point> list) : length (nl), order (list) {};

	// Return a path with x prepended at the beginning,
	// doesn't modify the original path.
	Path prepend(Point x)
	{
		Path tmp = *this; 
		tmp.length += dist(x, tmp.order[0]);
		tmp.order.insert(tmp.order.begin(), x);
		return tmp;
	}

	// Return a path with x appended at the end,
	// doesn't modify the original path.
	Path append(Point x)
	{
		Path tmp = *this;
		tmp.length += dist(x, tmp.order.back());
		tmp.order.push_back(x);
		return tmp;
	}
};

std::ostream& operator<<(std::ostream& out, const Path p)
{
	out << "Path: ";
	for (auto x : p.order)
		out << x.label;
	out << ", length = " << p.length;
	return out;
}

inline bool same_endpoints(const Path& a, const Path& b)
{
	return (a.order[0] == b.order[0] && a.order.back() == b.order.back())
	       || (a.order[0] == b.order.back() && a.order.back() == b.order[0]);
}

// From paths with the same endpoints leave only the shortest one
std::vector<Path> remove_suboptimal(const std::vector<Path>& list)
{
	std::vector<Path> tmp;
	for (auto x : list) {
		bool x_is_shortest = true;
		for (auto y : list) {
			if (same_endpoints(x, y) and (y.length < x.length)) {
				x_is_shortest = false;
				break;
			}
		}
		// If no shorter path with the same endpoints as x was found,
		// add it to the result
		if (x_is_shortest)
			tmp.push_back(x);
	}
	return std::move(tmp);
}

// Take a list of points sorted by their x coordinate and calculate
// the shortest bitonic tour
Path optimal_bitonic_tour(std::vector<Point> list)
{
	using std::begin;
	using std::end;

	std::vector<Path> result;

	// init AB
	result.push_back(Path (dist (list[0], list[1]), { list[0], list[1] }));

	// skip A and B which are already in the result vector and iterate over
	// all the other points to find the best path
	for (auto i_point = begin(list) + 2; i_point != end(list); ++i_point) {
		std::vector<Path> TMP;
		for (auto i_path : result) {
			TMP.push_back(i_path.prepend (*i_point));
			TMP.push_back(i_path.append (*i_point));
		}
		result = remove_suboptimal(TMP);
	}

	// Print all closed bitonic tours 
	//std::cout << "Mamy wyniki:" << std::endl;
	//for (auto x : result) {
		//for (auto y : x.order)
			//std::cout << y.label;
		//std::cout << " = " << x.length + dist (x.order[0], x.order.back()) << std::endl;
	//}

	// Pick the shortest tour from TMP and return it
	return *min_element (result.begin(), result.end(),
	[] (Path x, Path y) { return x.length < y.length; });
}

#endif
