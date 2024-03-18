#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

double inf = -1;

struct road{
        string name, code;
        int a, b;
        double len;

        road(string n, string c, int a_, int b_, double l){
                name = n;
                code = c;
                a = a_;
                b = b_;
                len = l;
        }
        void print(){
                cout<<name<<" "<<code<<" "<<a<<" "<<b<<" "<<len<<endl;
        }
        int from(int p){
                if(a == p) return b;
                else return a;
        }
};

class node{
private:
        int pos;
        double lon, lat, dist;
        string state, place;
        vector<road *> exits;

public:
        double mintime;
        bool visited;
        node(int i, double l, double la, double d, string s, string p){
                pos = i;
                lon = l;
                lat = la;
                dist = d;
                state = s;
                place = p;
                mintime = inf;
                visited = false;
        }
        void add_exit(road * r){
                exits.push_back(r);
        }
        void print(){
                cout<<"location "<<pos<<", "<<dist<<" miles from "<<place<<",  "<<state<<". \n";
                for(int i = 0; i < exits.size(); i++){
                        cout<<i+1<<" - "<<exits[i]->name<<", "<<exits[i]->len<<" miles to location ";
                        if(exits[i]->a == pos) cout<<exits[i]->b<<".\n";
                        else cout<<exits[i]->a<<".\n";
                }
        }
        road * get_exit(int i){
                return exits[i];
        }
        int num_exits(){
                return exits.size();
        }
        int get_pos(){
                return pos;
        }
        string name(){
                return place;
        }
        double getlat(){return lat;}
        double getlon(){return lon;}
};

struct PriorityQueue{
        vector<node *> heap;
        int L;

        PriorityQueue(){
                heap.push_back(NULL);
                L = 0;
        }
        void swap(int i, int j){
                node * temp = heap[i];
                heap[i] = heap[j];
                heap[j] = temp;
        }
        int least(node * a, int apos, node * b, int bpos){
                if(a->mintime < b->mintime) return apos;
                else return bpos;
        }
        void moveup(int poi){
                while(true){
                        if(poi == 1)break;
                        if(heap[poi]->mintime < heap[poi/2]->mintime){
                                swap(poi, poi/2);
                                poi = poi/2;
                                if(poi == 1)break;
                        }
                        else break;
                }
        }
        void movedown(int poi){
                while(true){
                        int left = poi*2, right = left+1;
                        if(left > L)break;
                        int smallest = least(heap[left],left, heap[right], right);

                        if(left == L){
                                if(heap[left]->mintime < heap[poi]->mintime){
                                        swap(left, poi);
                                }
                                break;
                        }
                        if(heap[smallest]->mintime < heap[poi]->mintime){
                                swap(smallest, poi);
                                poi = smallest;
                        }
                        else break;
                }
        }
void add(node * n){
                if(n == NULL)cout<<" Null Added \n";
                heap.push_back(n);
                L++;
                if(L == 1) return;
                int poi = L;
                moveup(poi);
        }
        void update(node * n, double e){
                for(int i = 1; i <= L; i++){
                        if(heap[i] == n){
                                if(heap[i]->mintime > e){
                                        heap[i]->mintime = e;
                                        moveup(i);
                                        return;
                                }
                                else {
                                        heap[i]->mintime = e;
                                        movedown(i);
                                        return;
                                }
                        }
                }
        }
        node * remove (){
                if(heap[L] == NULL)cout<<"pos L = "<< L<< endl;
                node * item = heap[1];
                int poi = 1;
                swap(1, L);
                heap.pop_back();
                L--;
                movedown(poi);
                return item;
        }
        bool isempty(){
                if(L == 0) return true;
                else return false;
        }
        int getL(){return L;}
};

string dir(double lon1, double lat1, double lon2, double lat2){
        string direction;
        if(lat1 > lat2)direction += 'S';
        else if(lat1 < lat2)direction += 'N';
        if(lon1 < lon2)direction += 'E';
        else if(lon1 > lon2)direction += 'W';

        return direction;
}
class map
{
private:
        vector<node *> nodes;
public:
        map(){};
        void add(node * n){
                nodes.push_back(n);
        }
        node * get_node(int i){
                return nodes[i];
        }
        node * operator[](int i){
                return nodes[i];
        }

double mintime(node * start, node * destination)
{
                start->mintime = 0;
                PriorityQueue Q;
                Q.add(start);
                while(true)
                {
                        node * here = Q.remove ();
                        if(here == NULL){cout<<"Path not made \n";break;}
                        here->visited = true;
                        if(here == destination)break;
                        for(int i = 0; i < here->num_exits(); i++)
                        {
                                road * r = here->get_exit(i);
                                node * there = nodes[r->from(here->get_pos())];
                                if(there->visited)
                                	{continue;}
                                double timeahead = here->mintime + r->len;
                                if(there->mintime == inf)
                                {
                                        there->mintime = timeahead;
                                        Q.add(there);
                                }
                                else if(there->mintime > timeahead)
                                {
                                        Q.update(there, timeahead);
                                }
                  		}
                }
                return destination->mintime;
}

        double mintime(int start, int end){
                return mintime(nodes[start], nodes[end]);
        }

        vector<road *> get_path(node * start, node * destination){
                vector<road *> path;
                double c_mintime = destination->mintime;
                node * ptr = destination;
                while(c_mintime > 0){
                        for(int i = 0; i < ptr->num_exits(); i++){
                                road * r  = ptr->get_exit(i);
                                node * there = nodes[r->from(ptr->get_pos())];
                                double dif = abs(c_mintime - r->len - there->mintime);
                                if(dif < .1){   
                                        path.push_back(r);              
                                        ptr = there;                   
                                        c_mintime = there->mintime;
                                }
                        }
                }
                return path;
        }

        void directions(int start, int end){
                mintime(start, end);
                vector<road *> path = get_path(nodes[start], nodes[end]);
                node * curr = nodes[start];
                cout<<"The directions are: \n";
                for(int i = path.size()-1; i >= 0; i--)
                {
                        cout<<"from intersection "<<curr->get_pos()<<" take "<<path[i]->name<<" "<<path[i]->len<<" "<<dir(curr->getlon(),curr->getlat(),nodes[path[i]->from(curr->get_pos())]->getlon(),nodes[path[i]->from(curr->get_pos())]->getlat());
                        cout << " to intersection" << nodes[path[i]->from(curr->get_pos())]->get_pos();
                        cout << endl;
                        curr = nodes[path[i]->from(curr->get_pos())];
                }
        }

          void cardirections(int start, int end){
                mintime(start, end);
                cout<<"getting path...\n";
                vector<road *> path = get_path(nodes[start], nodes[end]);
                node * curr = nodes[start];
                cout<<"The directions are: \n";
                int pathlength;
                for(int i = path.size()-1; i >= 0; i--)
                {
                        cout<<"from intersection "<<curr->get_pos()<<" take "<<path[i]->name<<" "<<path[i]->len<<" "<<dir(curr->getlon(),curr->getlat(),nodes[path[i]->from(curr->get_pos())]->getlon(),nodes[path[i]->from(curr->get_pos())]->getlat());
                        cout << " to intersection" << nodes[path[i]->from(curr->get_pos())]->get_pos();
                        cout << endl;
                        curr = nodes[path[i]->from(curr->get_pos())];
                }
 
        vector <int> equals;
        int count = 0;
        for(int i = path.size()-1; i >= 0; i--)
        {
        
        if(path[i]->name == path[i-1]->name)
                {
                        count +=1;
                }
        if(path[i]->name != path[i-1]->name)
                {
                        equals.push_back(count+1);
                        count = 0;
                }
        }

for(int i = 0; i<equals.size(); i++)
{
        cout << equals[i] << endl;
}
        }

};

string despace(string s){
        for(int i = 0; i < s.length(); i++){
                if(s[i] == ' '){
                        s.erase(i, 1);
                        i--;
                }
        }
        return s;
}

vector<string> breakup(string s){
        vector<string> data;
        string item;
        for(int i = 0; i < s.length(); i++){
                if(s[i] != ' '){
                        item+=s[i];
                }
                else if(!item.empty()){
                        data.push_back(item);
                        item = "";
                }
        }
        if(!item.empty()){data.push_back(item);}
        return data;
}
map map_from_files(string intersections, string roads){
        map m;

        ifstream fin(intersections);
        if(fin.fail()){cout<<"failed.\n";}
        else{
                int pos = 0;
                while(true){
                        string lat, lon, dist, state, name;
                        string line;
                        getline(fin, line);
                        if(line.empty()){break;}

                        lon.append(line, 0, 9);
                        lon = despace(lon);
                        lat.append(line, 9, 10);
                        lat = despace(lat);
                        dist.append(line, 19, 8);
                        dist = despace(dist);
                        state.append(line, 28, 2);
                        name = line.erase(0, 31);

                        m.add(new node(pos, stod(lon), stod(lat), stod(dist), state, name));
                        pos++;
                }
                fin.close();
        }

        
        ifstream fi(roads);
        if(fi.fail()){cout<<"failed.\n";}
        else{
                while(true){
                        string line;
                        getline(fi, line);
                        if(line.empty())break;
                        vector<string> breakupd = breakup(line);
                        road * r = new road(breakupd[0], breakupd[1], stoi(breakupd[2]), stoi(breakupd[3]), stod(breakupd[4]));
                        m[stoi(breakupd[2])]->add_exit(r);
                        m[stoi(breakupd[3])]->add_exit(r);
                }
                fi.close();
        }

        return m;
}

struct Town
{
	int NumCode;
	string state;
	string name;
	int population;
	double area;
	double latitude;
	double longitude;
	int intersection;
	double distance;

	Town()
	{ }

	Town(int NumCodes, const string& states, const string& names, int populations, double areas, double latitudes, double longitudes, int intersections, double distances)
	{
		NumCode = NumCodes;
		state = states;
		name = names;
		population = populations;
		area = areas;
		latitude = latitudes;
		longitude = longitudes;
		intersection = intersections;
		distance = distances;
	}

	void print()
	{
			cout << endl;
		    cout << "NUMBER: " << NumCode << " " << "STATE: " << state << endl
			<< "NAME: " << name << " " << "POPULATION: " << population << endl
			<< "AREA: " << area << " " << "LATITUDE: " << latitude << endl
			<< "LONGITUDE: " << longitude << " " << "INTERSECTION: " << intersection << endl
			<< "DISTANCE: " << distance << endl;
	}

	int get_intersection()
	{
		return intersection;
	}
	
};

struct HashIn
{
	Town key;
	HashIn* next;

	HashIn(const Town& p)
	{
		key = p;
		next = NULL;
	}
};

int Hash(const string & s)
{
	int h = 1294721;
	for (int i = 0; i < s.length(); i += 1)
		h = h * 691 + s[i];
	if (h < 0)
		h = -h;
	return h;
}

int TownHashToo(const string& name, const string& state) 
{
	return Hash(name + state);
}

int TownHash(const Town& p) 
{
	return Hash(p.name + p.state);
}

Town * splitTown(const string& line) 
{
	int NumCode = stoi(line.substr(0, 8));
	
	string state = line.substr(8, 2);

	string city;
	for (int i = 10; i < 49; i++) {
		string arr;
		string arr2;

		arr = line.substr(i, 1);
		arr2 = line.substr(i + 1, 1);

		if (arr == " " && arr2 == " ")
			break;
		else
			city = city + arr;
	}

	string population_string = line.substr(58, 8);
	int population = stoi(population_string);

	string area_string = line.substr(66, 14);
	double area = stof(area_string);

	string latitude_string = line.substr(81, 12);
	double latitude = stof(latitude_string);

	string longitude_string = line.substr(90, 12);
	double longitude = stof(longitude_string);

	string intersection_string = line.substr(101, 5);
	double intersection = stof(intersection_string);

	string distance_string = line.substr(106, 8);
	double distance = stof(distance_string);

	Town* r = new Town(NumCode, state, city, population, area, latitude, longitude, intersection, distance);
	return r;
}

class Hashtable
{
protected:
	int size;
	HashIn ** table;

public:
	Hashtable(int amt)
	{
		size = amt;
		table = new HashIn * [size];
		for (int i = 0; i < size; i += 1)
			table[i] = NULL;
	}

	void insert(HashIn* h)
	{
		int pos = TownHash(h->key) % size;
		h->next = table[pos];
		table[pos] = h;
	}

	HashIn * Search(const string& name, const string& state)
	{
		int pos = TownHashToo(name, state) % size;
		HashIn * find = table[pos];
		while (find != NULL)
		{
			if (find->key.name == name && find->key.state == state)
				return find;
			find = find->next;
		}
		return NULL;
	}


};

int main()
{
	ifstream fin("/home/www/class/een318/named-places.txt");
	if (!fin.is_open())
	{
		cout << "Error opening the file\n";
		exit(1);
	}

	vector <Town*> OfTowns;
	while (!fin.eof())
	{
		string line;
		getline(fin, line);
		if (line.size() == 0)
			continue;
		Town* p = splitTown(line);
		OfTowns.push_back(p);
	}

	Hashtable* ht = new Hashtable(OfTowns.size() * 2);
	for (Town* p : OfTowns)
	{
		HashIn* k = new HashIn(*p);
		delete p;
		ht->insert(k);
	}
	
		cout << "Enter the starting location (Townname, State) \n";
		bool pass = true;
		string name;
		string state;	
		cin >> name;
		cin >> state;
		cout << "Enter the end location (Townname, State) \n";
		bool go = true;
		string name2;
		string state2;
		cin >> name2;
		cin >> state2;

		HashIn * In = ht->Search(name, state);

			if (In == nullptr)
			{
				cout << endl;
				cout << "Not found \n";
				cout << endl;
				pass = false;
			}

			if (pass == true)
			{
				cout << "Successfully found!" << endl;
			    In->key.print();
			    cout << "\n";
			}

			HashIn * Im = ht->Search(name2, state2);

			if (Im == nullptr)
			{
				cout << endl;
				cout << "Not found \n";
				cout << endl;
				go = false;
			}

			if (go == true)
			{
				cout << "Successfully found!" << endl;
			    Im->key.print();
			    cout << "\n";
			}

		map m = map_from_files("/home/www/class/een318/intersections.txt", "/home/www/class/een318/connections.txt");
        int a = In->key.get_intersection();
        int b = Im->key.get_intersection();
        m.directions(a, b);
        //m.cardirections(a,b);
        return 0;
} 
