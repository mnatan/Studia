#ifndef POINT
#define POINT

#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>

struct Point
{
	float x;                                //koordynat x
	float y;                                //koordynat y
	char label;

	Point() : x(0), y(0), label('Z') {}                 //konstruktor domyslny tworzy 0,0,Z
	Point(float x_, float y_, char l_) : x(x_), y(y_), label(l_) {}

	friend std::ostream & operator << (std::ostream& out, const Point p);
};

std::ostream & operator << (std::ostream& out, const Point p)
{
	printf(" %c: (%6.2f , %6.2f)",p.label,p.x,p.y);
	return out;
}

float dist(const Point lhs, const Point rhs)
{
	return sqrt(
	           (rhs.x - lhs.x) * (rhs.x - lhs.x) +
	           (rhs.y - lhs.y) * (rhs.y - lhs.y)
	       );
}

struct droga
{
	float dlugosc;
	std::vector<Point> kolejnosc;

	droga(float dl, std::vector<Point> list) : dlugosc(dl), kolejnosc(list) {};

	droga prep(Point x)
	{
		droga tmp = *this;
		tmp.dlugosc += dist(x, tmp.kolejnosc[0]);
		tmp.kolejnosc.insert(tmp.kolejnosc.begin(), x);
		return tmp;
	}
	droga app(Point x)
	{
		droga tmp = *this;
		tmp.dlugosc += dist(x, tmp.kolejnosc.back());
		tmp.kolejnosc.push_back(x);
		return tmp;
	}

	friend std::ostream & operator << (std::ostream& out, const droga p);
};

std::ostream & operator << (std::ostream& out, const droga d)
{
	out << "droga: ";
	for (auto x : d.kolejnosc)
	{
		out << x.label;
	}
	out << ", dlugosc = " << d.dlugosc;
	return out;
}

void wywal_powtorzenia(std::vector<droga> & lista)
{
	std::vector<droga> tmp;
	for ( auto x : lista )
	{
		bool good = true;
		for ( auto y : lista )
		{
			if (
			    (x.kolejnosc[0].label == y.kolejnosc[0].label &&
			     x.kolejnosc.back().label == y.kolejnosc.back().label)
			    ||
			    (x.kolejnosc[0].label == y.kolejnosc.back().label &&
			     x.kolejnosc.back().label == y.kolejnosc[0].label)
			)
			{
				if (y.dlugosc < x.dlugosc)
				{
					good = false;
					break;
				}
			}
		}
		if (good)
		{
			tmp.push_back(x);
		}
	}
	lista = tmp;
}

droga najkrotszy_bitonic(std::vector<Point> lista)
{
	// zmienne pomocnicze
	std::vector<droga> R; //wektor wynikowy
	std::vector<droga> Z; //tmp

	// init AB
	R.push_back(
	    droga(
	        dist(lista[0], lista[1]), { lista[0], lista[1] }
	    )
	);

	// algorytm
	auto i_punkt = lista.begin();
	++i_punkt;			// skip A i B
	++i_punkt;
	for (auto end = lista.end(); i_punkt != end; ++i_punkt)
	{
		Z.clear();
		for (auto i_droga : R)
		{
			Z.push_back(i_droga.prep(*i_punkt));
			Z.push_back(i_droga.app(*i_punkt));
		}
		wywal_powtorzenia(Z);
		R = Z;
	}

	std::cout << "Mamy wyniki:" << std::endl;
	for(auto x: R){
		for(auto y: x.kolejnosc){
			std::cout << y.label;
		}
		std::cout << " = " << x.dlugosc + dist(x.kolejnosc[0], x.kolejnosc.back()) << std::endl;
	}

	// zwracamy minimalną drogę
	return *min_element(R.begin(), R.end(), [](droga x, droga y)
	{
		return x.dlugosc < y.dlugosc;
	}); // TODO
}

#endif
