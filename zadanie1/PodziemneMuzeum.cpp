/*
 *Podziemne Muzeum
 *Marcin Natanek
 *
 *Algorytm Dijkstry
 */

#include <stdio.h>
#include <climits>
#include <iostream>
#include <queue>
#include <list>
#include <sstream>
#define INF INT_MAX

class Coords;

unsigned long n;
unsigned long m;
unsigned long xCel;
unsigned long yCel;
std::priority_queue<Coords> kolejka;

class Coords
{
	public:
	unsigned int x;
	unsigned int y;
	Coords(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	Coords() {};
	friend std::ostream& operator<<(std::ostream& os, const Coords& u)
	{
		std::cout << "(" << u.x << "," << u.y << ")";
		return os;
	}
	friend bool operator> (Coords &cC1, Coords &cC2);
	friend bool operator<= (Coords &cC1, Coords &cC2);

	friend bool operator< (Coords &cC1, Coords &cC2);
	friend bool operator>= (Coords &cC1, Coords &cC2);
};

class Vertex
{
	public:
	unsigned int koszt;
	unsigned int odleglosc;
	bool sprawdzony;
	Coords *poprzedni;
};

Vertex **muzeum;
char **sciezka;

bool operator> (Coords cC1, Coords cC2)
{
	return muzeum[cC1.x][cC1.y].odleglosc < muzeum[cC2.x][cC2.y].odleglosc;
}
bool operator< (Coords cC1, Coords cC2)
{
	return muzeum[cC1.x][cC1.y].odleglosc > muzeum[cC2.x][cC2.y].odleglosc;
}

void wypiszTablice()
{
	for (unsigned long i = 0; i < n ; i++)
	{
		printf("\n    ");
		for (unsigned long j = 0; j < m ; j++)
			printf("%i ", muzeum[i][j].koszt);
	}
}
void wypiszTablice(bool debug)
{
	for (unsigned long i = 0; i < n ; i++)
	{
		printf("\n");
		printf("\n");
		for (unsigned long j = 0; j < m ; j++)
			printf("%i\t", muzeum[i][j].odleglosc);
	}
	printf("\n");
}
void wypiszTablice(int debug)
{
	for (unsigned long i = 0; i < n ; i++)
	{
		printf("\n");
		for (unsigned long j = 0; j < m ; j++)
			if (muzeum[i][j].poprzedni != NULL)
				std::cout << *(muzeum[i][j].poprzedni) << " ";
			else
				std::cout << "NULL" << " ";
	}
	printf("\n");
}
void zapiszSciezke(Coords start)
{
	Coords *iterator = &start;
	while (iterator != NULL)
	{
		sciezka[iterator->x][iterator->y] = 1;
		iterator = muzeum[iterator->x][iterator->y].poprzedni;
	}
}
std::string zapiszKierunki(Coords start)
{
	std::string wynik = "";
	Coords *poprzedni = &start;
	Coords *iterator = muzeum[start.x][start.y].poprzedni;
	while (iterator != NULL)
	{
		if (iterator->x > poprzedni->x)
		{
			wynik.append("S");
		}
		else if (iterator->x < poprzedni->x)
		{
			wynik.append("N");
		}
		else if (iterator->y > poprzedni->y)
		{
			wynik.append("E");
		}
		else if (iterator->y < poprzedni->y)
		{
			wynik.append("W");
		}
		poprzedni = iterator;
		iterator = muzeum[iterator->x][iterator->y].poprzedni;
	}
	return wynik;
}
void wypiszSciezke()
{
	printf("\n");
	for (unsigned long i = 0; i < n ; i++)
	{
		printf("\n    ");
		for (unsigned long j = 0; j < m ; j++)
			if (sciezka[i][j] == true)
				if (i == xCel && j == yCel)
					printf("X ");
				else
					//printf("o ");
					printf("o ");
			else
				printf(". ");
	}
}
void wypiszKolejke(std::priority_queue<Coords> kolejka)
{
	std::cout << "\tStan kolejki:" << std::endl;
	while (!kolejka.empty())
	{
		std::cout << "\t" << kolejka.top() << " = "
		<< "k:" << muzeum[kolejka.top().x][kolejka.top().y].odleglosc
		<< " o:" << muzeum[kolejka.top().x][kolejka.top().y].koszt <<
		std::endl;
		kolejka.pop();
	}
}

void init(Coords u)
{
	for (unsigned long i = 0; i < n ; i++)
		for (unsigned long j = 0; j < m ; j++)
		{
			muzeum[i][j].odleglosc = INF;
			muzeum[i][j].poprzedni = NULL;
			muzeum[i][j].sprawdzony = 0;
		}
	muzeum[u.x][u.y].odleglosc = 0;
	kolejka.push(u);
}
void relax(Coords u, Coords v)
{
	//std::cout << "Sprawdzam czy " << muzeum[v.x][v.y].odleglosc << " > " << muzeum[u.x][u.y].odleglosc << " + " << muzeum[v.x][v.y].koszt << std::endl;
	std::cout << muzeum[v.x][v.y].odleglosc << ">" << muzeum[u.x][u.y].odleglosc << "+" << muzeum[v.x][v.y].koszt;
	if (muzeum[v.x][v.y].odleglosc > muzeum[u.x][u.y].odleglosc + muzeum[v.x][v.y].koszt)
	{
		muzeum[v.x][v.y].odleglosc = muzeum[u.x][u.y].odleglosc + muzeum[v.x][v.y].koszt;
		muzeum[v.x][v.y].poprzedni = new Coords(u.x, u.y);
		std::cout << " TAK";
	}
	std::cout << std::endl;
}
std::list<Coords> znajdzSasiadow(Coords u)
{
	//std::cout << __PRETTY_FUNCTION__ << std::endl;]j
	std::list<Coords> sasiedzi;
	if (u.x > 0)
	{
		sasiedzi.push_front(Coords(u.x - 1, u.y));
		//std::cout << "1Dodaje " << Coords(u.x - 1, u.y) << "=" << muzeum[u.x - 1][u.y].koszt << " do listy." << std::endl;
	}
	if (u.x < n - 1)
	{
		sasiedzi.push_front(Coords(u.x + 1, u.y));
		//std::cout << "2Dodaje " << Coords(u.x + 1, u.y) << "=" << muzeum[u.x + 1][u.y].koszt << " do listy." << std::endl;
	}
	if (u.y > 0)
	{
		sasiedzi.push_front(Coords(u.x, u.y - 1));
		//std::cout << "3Dodaje " << Coords(u.x, u.y - 1) << "=" << muzeum[u.x][u.y - 1].koszt << " do listy." << std::endl;
	}
	if (u.y < m - 1)
	{
		sasiedzi.push_front(Coords(u.x, u.y + 1));
		//std::cout << "4Dodaje " << Coords(u.x, u.y + 1) << "=" << muzeum[u.x][u.y + 1].koszt << " do listy." << std::endl;
	}

	return sasiedzi;
}
void Dijkstra(Coords start)
{
	//std::cout << "Dijkstra init" << std::endl;
	init(start);
	while (!kolejka.empty())
	{
		wypiszKolejke(kolejka);
		Coords u = kolejka.top();
		kolejka.pop();
		std::cout << "Teraz u to:" << u << "=" << muzeum[u.x][u.y].koszt << std::endl;
		muzeum[u.x][u.y].sprawdzony = true;
		std::list<Coords> sasiedzi = znajdzSasiadow(u);
		std::list<Coords>::iterator it;
		for (it = sasiedzi.begin(); it != sasiedzi.end(); it++)
		{
			if (muzeum[it->x][it->y].sprawdzony == false)
			{
				kolejka.push(*it);
				std::cout << "Pushuje " << *it << "do kolejki." << std::endl;
			}
			std::cout << "relaxuje " << u << " z " << *it << std::endl;
			relax(u, *it);
		}
	}
	printf("Dijkstra debug\n");
	printf("koszta:");
	wypiszTablice();
	printf("odleglosc:");
	wypiszTablice(true);
	printf("odwiedzone:");
	wypiszTablice(3);
	printf("\n\n\n");
}

void rozwiaz(int xcel, int ycel)
{
	//printf("\n----------------------------- Obliczanie drogi -----------------------------\n");

	int wynik = 0;
	std::string output = "";

	Dijkstra(Coords(xCel, yCel));
	//wypiszTablice(true);
	//std::cout << wynik << "+=" << muzeum[n - 1][0].odleglosc << std::endl;
	wynik += muzeum[n - 1][0].odleglosc;
	//std::cout << wynik << std::endl;
	zapiszSciezke(Coords(n - 1, 0));
	output.append(zapiszKierunki(Coords(n - 1, 0)));

	Dijkstra(Coords(0, m - 1));
	//wypiszTablice(true);
	//std::cout << wynik << "+=" << muzeum[xCel][yCel].odleglosc << std::endl;
	wynik += muzeum[xCel][yCel].odleglosc;
	//std::cout << wynik << std::endl;
	zapiszSciezke(Coords(xCel, yCel));
	output.append(zapiszKierunki(Coords(xCel, yCel)));

	wynik += muzeum[0][m - 1].koszt;

	//printf("Koszt wycieczki: %d", wynik);
	//printf("\nDroga: ");
	std::cout << wynik << "\t" << output.length() + 1 << "\t" << output << std::endl;

	wypiszTablice(true);
	wypiszSciezke();

	std::cout << std::endl;
	std::cout << std::endl;

	//printf("\n\n----------------------------- Koniec Programu -----------------------------\n");
}

int main()
{
	//printf("----------------------------- Wcztywanie danych -----------------------------\n");
	//printf("Podaj ilość kolumn:  ");
	std::cin >> n;
	//printf("Podaj ilość wierszy: ");
	std::cin >> m;
	std::cin.ignore(256, '\n');
	//printf("Rozmiar Muzeum: %ld x %ld", n, m);

	muzeum = new Vertex*[n];
	for (unsigned long i = 0; i < n ; i++)
		muzeum[i] = new Vertex[m];

	sciezka = new char*[n];
	for (unsigned long i = 0; i < n ; i++)
		sciezka[i] = new char[m];
	for (unsigned long i = 0; i < n ; i++)
		for (unsigned long j = 0; j < m ; j++)
			sciezka[i][j] = 0;

	//printf("Podaj koszta: ");
	for (unsigned long i = 0; i < n ; i++)
	{
		for (unsigned long j = 0; j < m ; j++)
			std::cin >> muzeum[i][j].koszt;
		std::cin.ignore(256, '\n');
	}

	while (std::cin >> xCel)
	{
		//printf("Podaj x celu: ");
		if (xCel != 0) xCel--;
		//printf("Podaj y celu: ");
		std::cin >> yCel;
		if (yCel != 0) yCel--;
		std::cin.ignore(256, '\n');

		rozwiaz(xCel, yCel);
	}

}
