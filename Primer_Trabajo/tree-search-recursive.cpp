#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <vector>
#include <limits>
#include <set>

#define MAXLONG 999999999

using namespace std;

typedef map<const int*, int>* edges;
typedef map<const int*, int>::iterator edges_it;
typedef map <int, edges> myMap;
typedef pair<const int*, int> city;
typedef vector <city> myVector;

class dgrafo {
	myMap g;

public:

	dgrafo() {
	}
	void insert(int _id) {
		g[_id]= new map<const int*, int>;
	}
	void insert_e(int origen, int _destino, int _peso) {
		g[origen]->insert( city(&(g.find(_destino)->first), _peso));
	}
	const int * get_p(int _id) {
		if (g.find(_id) == g.end()) return NULL;
		return (&(g.find(_id)->first));
	}
	void print() {
		for (myMap::iterator it = g.begin(); it != g.end(); ++it) {
			cout << it->first<<endl;
			print_e(it->second);
			cout << endl;
		}
	}

	int peso(int a, int b) {
		if (g.find(a) == g.end()) return -1;
		edges t = g[a];
		if (get_p(b))
			return (*t)[get_p(b)];
		else
			return -1;
	}

	void print_n(int origen) {
		print_e(g[origen]);
	}
	
	myVector get_edges(int origen) {
		edges m = g[origen];
		myVector temp;
		for (edges_it it = m->begin(); it != m->end(); ++it)
			temp.push_back(pair<const int*, int> (it->first,it->second));
		return temp;
	}

	long size() {
		return g.size();
	}

private:

	void print_e(edges m) {
		for (edges_it it = m->begin(); it != m->end(); ++it)
			cout << "  =>" << *(it->first) << " , " << it->second << '\n';
	}

};



class Tour {

public:
	myVector t;
	long rank;
	Tour() {
		rank = 0;
	}
	Tour(long n) {
		rank = n;
	}
	Tour(city id) {
		rank = id.second;
		t.push_back(id);
	}
	void insert(city c) {
		t.push_back(c);
		rank += c.second;
	}
	long ranking() {
		return rank;
	}
	long size() {
		return t.size();
	}
	void operator = (Tour _t) {
		this->t = _t.t;
		this->rank = _t.rank;
	}
	bool feasible(int city) {
		for (int i = 0; i < t.size(); i++) {
			if (*(t[i].first) == city)
				return false;
		}
		return true;
	}
	city last_city() {
		return t.back();
	}
	void remove_last_city() {
		rank -= t.back().second;
		t.pop_back();
	}
	void print() {
		for (int i = 0; i < t.size(); i++) 
			cout << *(t[i].first) << " => ";
		cout << ", " << rank << endl;
	}
	city firt_city() {
		return t[0];
	}
};




void Depth_first_search(dgrafo & g, Tour & t, Tour & best_tour) {
	//t.print();
	int c = *(t.last_city().first);
	if (g.size() == t.size()) {
		city fisrt_city = t.firt_city();
		myVector vecinos = g.get_edges(*(t.last_city().first));
		int p = g.peso(*(t.last_city().first), *(fisrt_city.first));
		city temp(fisrt_city.first, p);
		t.insert(temp);
		//t.print();
		if (best_tour.ranking() > t.ranking()) {
			best_tour = t;
		}
		t.remove_last_city();
	}
	else {
		myVector vecinos = g.get_edges(c);
		for (int i = 0; i < vecinos.size(); i++) {
			if (t.feasible(*(vecinos[i].first))) {
				t.insert(vecinos[i]);
				Depth_first_search(g, t, best_tour);
				t.remove_last_city();
			}
		}
	}
}



int main()
{
	dgrafo graf;
	srand (time(NULL));
	int TAM = 10;
	for (int i = 0; i < TAM; i++) {
		graf.insert(i);
	}
	for (int i = 0; i < TAM; i++) {
		for (int j = 0; j < TAM; j++) {
			graf.insert_e(i, j, rand()%10);
		}
	}

	//graf.print();

	Tour best_tour (MAXLONG) ;
	Tour inicio(city(graf.get_p(0), 0));

	Depth_first_search(graf, inicio, best_tour);

	best_tour.print();
	
	return 0;
}
