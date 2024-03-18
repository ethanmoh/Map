#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class connections 
{
private:
        string road, code;
        int start, end;
        double length;
        int pos;

public:
        connections() {};

        connections(string r, string c, int begin, int finish, double len)
        {
                road = r;
                code = c;
                start = begin;
                end = finish;
                length = len;
        }

        void print()
        {
                cout << road << ", " << length << " miles to location " << end <<  endl;
        }

        int get_end()
        {
                return(end);
        }

};

class intersections 
{
private:
        double latitude, longitude, distance;
        string state, place;

        vector <connections> Connections;
public:

        intersections(double lat, double lon, double d, string s, string p)
        {
                latitude = lat;
                longitude = lon;
                distance = d;
                state = s;
                place = p;
        }

        void add_road(connections c)
        {
                Connections.push_back(c);
        }

        vector <connections> find_roads()
        {
                return(Connections);
        }

        void print()
        {
                cout << distance << " miles from " << place << " ," << state;
                cout << endl;
                cout << "roads leading away: " << endl;;
        }

};

        string removespace(string s)
        {
                for(int i = 0; i < s.length(); i++)
                {
                        if(s[i] == ' ')
                        {
                                s.erase(i,1);
                                i--;
                        }
                }
                return s;
        }

        vector <string> fixword(string s)
        {
                vector <string> data;
                string item;
                for (int i = 0; i<s.length(); i++)
                {
                        if(s[i] != ' ')
                                item += s[i];

                else if(!item.empty())
                        {
                                data.push_back(item);
                                item = "";
                        }
                }
                if(!item.empty())
                data.push_back(item);
                return data;
        }

void format_read()
{
        vector <intersections> Intersections;

        ifstream fin("/home/www/class/een318/intersections.txt");
        if (fin.fail())
                cout << "Could not open document";

        while (true)
                {

                        string lat, lon, dist, stat, nam;
                        string line;
                        getline(fin,line);
                        if(line.empty())
                                break;
                        lon.append(line,0,9);
                        lon = removespace(lon);
                        lat.append(line,9,10);
                        lat = removespace(lat);
                        dist.append(line,19,8);
                        dist = removespace(dist);
                        stat.append(line,28,2);
                        nam = line.erase(0,31);

                        double lat_ = stod(lat);
                        double lon_ = stod(lon);
                        double dist_ = stod(dist);

                        intersections inter (lat_,lon_,dist_,stat,nam);
                        Intersections.push_back(inter);

             }
             fin.close();
             ifstream finn("/home/www/class/een318/connections.txt");
                if(finn.fail())
                        cout << "Could not open document";
                else
                {

                        while (true)
                        {
                                string line;
                                getline(finn,line);
                                if(line.empty())
                                        break;

                                vector <string> fixw = fixword(line);

                                string road_ = fixw[0];
                                string code_ = fixw[1];
                                int start_ = stoi(fixw[2]);
                                int end_ = stoi(fixw[3]);
                                double length_ = stod(fixw[4]);

                                connections conn1 (road_,code_,start_,end_,length_);
                                Intersections[start_-1].add_road(conn1);
                                connections conn2 (road_,code_,end_,start_,length_);
                                Intersections[end_-1].add_road(conn2);

                        }
                }
                finn.close();
                cout << endl;
                cout << "Ready to work! " << endl;
                cout << endl;

               while (true)
                {
                        int choice;
                        cout << "Enter a starting position: ";
                        cin >> choice;
                        cout << endl;
                while (true)
                {
                        if(choice > 0 && choice<=Intersections.size())
                        {
                                        vector <connections> current = Intersections[choice-1].find_roads();
                                        cout << "Location " << choice << ", ";
                                        Intersections[choice].print();

                                        for(int i =0; i<current.size(); i++)
                                        {
                                                cout << i+1 << ": ";
                                                current[i].print();
                                        }
                                cout << endl;
                                int choice_ = 0;

                                while(choice_<1)
                                {
                                        cout << "Which road would you like to continue on? ";
                                        cin >> choice_;
                                        if(choice_>current.size())
                                        {
                                                cout << "Not available choice! " << endl;
                                                cout << "Enter a choice from the list above [i] " << endl;
                                                cin >> choice_;
                                        }
                                        int entry = choice_ -1;
                                        choice = current[entry].get_end();
                                }


                        }
                }
                }


}

int main()
{
        format_read();
        return 0;
}
