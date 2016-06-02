#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <cstdlib>
#include <ctime>
#include <sys/timeb.h>
#include <map>
#include <vector>
#include <limits>
#include <set>
#include <stack>

#define MAXLONG 999999999

using namespace std;

typedef map<const int*, int>* edges;
typedef map<const int*, int>::iterator edges_it;
typedef map <int, edges> myMap;
typedef pair<const int*, int> city;
typedef vector <city> myVector;
typedef stack <city> tour_pila;

class Tour;
typedef stack <Tour> tour_pila_t;



class Tiempo {
	public:
	clock_t tstart;
	clock_t tend;
	double secs;
	void start(){
		tstart=clock(); 
	}
	void end(){
		tend=clock();
	}
	void print(){
		secs = (double)(tend - tstart) / CLOCKS_PER_SEC;
		printf("Tiempo de ejecucion: %.16g milisegundos\n", secs * 1000.0);
	}
};

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
			temp.push_back(city (it->first,it->second));
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
		for (int i = 0; i < t.size(); i++){
			cout << *(t[i].first);
			if (i!=t.size()-1) cout<< " => ";
			
		}
		cout << ", " << rank << endl;
	}
	city firt_city() {
		return t[0];
	}
};

void Depth_first_search(dgrafo & g, Tour & t, Tour & best_tour, bool pt = 0) {

	int c = *(t.last_city().first);
	if (g.size() == t.size()) {
		city fisrt_city = t.firt_city();
		myVector vecinos = g.get_edges(*(t.last_city().first));
		int p = g.peso(*(t.last_city().first), *(fisrt_city.first));
		city temp(fisrt_city.first, p);
		t.insert(temp);
		if (pt){ t.print();}
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
				Depth_first_search(g, t, best_tour, pt);
				t.remove_last_city();
			}
		}
	}
}




void no_recursive_search(dgrafo & g, Tour & t, Tour & best_tour, bool pt = 0) {

	tour_pila pila;
	city first_c = t.last_city();
	myVector vecinos = g.get_edges(*(first_c.first));
	for (int i=vecinos.size()-1;i>=1;i--){
		pila.push(vecinos[i]);
	}
	while (!pila.empty()){
		city actual = pila.top();
		pila.pop();
		if (actual.first == NULL){// End of child list, back up
			t.remove_last_city();
		}
		else {
			t.insert(actual);
			if (g.size() == t.size()) {
				myVector vecinos = g.get_edges(*(first_c.first));
				int p = g.peso(*(t.last_city().first), *(first_c.first));
				city temp(first_c.first, p);
				t.insert(temp);
				
				if (pt){ t.print();}
				if (best_tour.ranking() > t.ranking()){
					best_tour = t;}
				t.remove_last_city();
				t.remove_last_city();
			} else {
				pila.push(city(NULL,0));
				vecinos = g.get_edges(*(actual.first));
				for (int i=vecinos.size()-1;i>=1;i--){
					if (t.feasible(*(vecinos[i].first))) {
						pila.push(vecinos[i]);
					}
				}
			}
		}
	}
}


void no_recursive_search_reduce(dgrafo & g, Tour & t, Tour & best_tour, bool pt = 0) {

	tour_pila_t pila;
	pila.push(t);
	while (!pila.empty()){
		Tour actual = pila.top();
		pila.pop();
		if (g.size() == actual.size()) {
			myVector vecinos = g.get_edges(*(actual.firt_city().first));
			int p = g.peso(*(actual.last_city().first), *(actual.firt_city().first));
			city temp(actual.firt_city().first, p);
			actual.insert(temp);
			if (pt){ actual.print();}
			if (best_tour.ranking() > actual.ranking()){
				best_tour = actual;}
			//actual.remove_last_city();
		}else{
			myVector vecinos = g.get_edges(*(actual.last_city().first));
			for (int i=vecinos.size()-1;i>=1;i--){
				if (actual.feasible(*(vecinos[i].first))) {
					actual.insert(vecinos[i]);
					pila.push(actual);
					actual.remove_last_city();
				}
			}
		}
	}
}


int main()
{
	dgrafo graf;
	srand (time(NULL));
	int TAM = 5;
	for (int i = 0; i < TAM; i++) {
		graf.insert(i);
	}
	for (int i = 0; i < TAM; i++) {
		for (int j = 0; j < TAM; j++) {
			graf.insert_e(i, j, rand()%10);
		}
	}

	//graf.print();
	
	Tiempo t;

	cout<<endl<<"No Recursive"<<endl;
	t.start();
	Tour best_tour (MAXLONG) ;
	Tour inicio(city(graf.get_p(0), 0));
	no_recursive_search(graf, inicio, best_tour,0);
	best_tour.print();
	t.end();
	t.print();
	
	cout<<endl<<"No Recursive Reduce"<<endl;
	t.start();
	Tour best_tour1 (MAXLONG) ;
	Tour inicio1(city(graf.get_p(0), 0));
	no_recursive_search_reduce(graf, inicio1, best_tour1,0);
	best_tour1.print();
	t.end();
	t.print();
	
	cout<<endl<<"Recursive"<<endl;
	t.start();
	Tour best_tour2 (MAXLONG) ;
	Tour inicio2(city(graf.get_p(0), 0));
	Depth_first_search(graf, inicio2, best_tour2,0);
	best_tour2.print();
	t.end();
	t.print();
	
	return 0;
}
