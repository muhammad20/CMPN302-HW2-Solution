#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sys/time.h>
#include <math.h>
#include <map>

using namespace std;

class StopWatch
{
  private:
    int millis;

  public:
    void start()
    {
        timeval tp;
        gettimeofday(&tp, nullptr);
        millis = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    }

    int get_elapsed_ms()
    {
        timeval tp;
        gettimeofday(&tp, nullptr);
        return tp.tv_sec * 1000 + tp.tv_usec / 1000 - millis;
    }
};

class FileHandler
{
  public:
    vector<int> init_vector_from_file(string filename)
    {
        filename = "/home/engineer/Desktop/CCE-C/Senior-1/Fall18/CMPN302/Assignments/hw2/p4/" + filename;
        ifstream infile(filename, ios::in);

        vector<int> data;

        string line;
        while (getline(infile, line))
        {
            data.push_back(stoi(line));
        }
        return data;
    }

    void write_vector_to_file(string filename, vector<int> data)
    {
        ofstream outfile(filename, ios::out);
        for (int i = 0; i < data.size(); i++)
        {
            outfile << data[i] << endl;
        }
    }

    void write_running_time_to_file(string filename, int running_time)
    {
        ofstream outfile(filename, ios::out);
        outfile << "running time in ms: " << running_time;
        outfile.close();
    }
};

multimap<int, int> create_map_from_vector(vector<int> data, int k);
vector<int> find_range_min(multimap<int, int> data_map, vector<int> data, int k);

int main(int argc, char **argv)
{
    //check if CL args are less than 3 (we check for 4 as argv[0] is the executable path).
    if (argc < 5)
    {
        cout << "Error! arguments missing\n";
        exit(0);
    }

    //take the first 4 args (argv[1], argv[2], argv[3], argv[4]) and ignore the rest if there is any.
    string k_str = argv[1];
    int k = stoi(k_str);
    string unsorted_data_file = argv[2];
    string sorted_data_file = argv[3];
    string running_time_file = argv[4];

    FileHandler *file_handler = new FileHandler();
    vector<int> data = file_handler->init_vector_from_file(unsorted_data_file);

    StopWatch *stopWatch = new StopWatch();
    stopWatch->start();
    multimap<int, int> data_map = create_map_from_vector(data, k);
    vector<int> sorted_data = find_range_min(data_map, data, k);
    int running_time = stopWatch->get_elapsed_ms();

    file_handler->write_vector_to_file(sorted_data_file, sorted_data);
    file_handler->write_running_time_to_file(running_time_file, running_time);

    delete file_handler;
    delete stopWatch;

    return 0;
}

//create a multimap to store the data with its index
//a multimap keeps the order of keys sorted so the data be already sorted
multimap<int, int> create_map_from_vector(vector<int> data, int k)
{
    multimap<int, int> data_map;
    //get first K elements in data
    for (int i = 0; i < k; i++)
    {
        data_map.insert(make_pair(data[i], i));
    }
    return data_map;
}

vector<int> find_range_min(multimap<int, int> data_map, vector<int> data, int k)
{
    vector<int> sorted_data;
    int i = 0;
    while (i < data.size() - k + 1)
    {
        //already first element in multipmap is the minimum
        sorted_data.push_back(data_map.begin()->first);
        data_map.erase(data_map.find(data[i]));
        //insert next element in map
        data_map.insert(make_pair(data[i + k], i + k));
        i++;
    }
    return sorted_data;
}