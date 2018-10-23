#include <iostream>
#include <string>
#include <fstream>
#include <sys/time.h>
#include <unordered_map>
#include <vector>
#include <algorithm>

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
    unordered_map<int, vector<int>> init_map_from_file(string filename)
    {
        unordered_map<int, int> data;
        ifstream infile(filename, ios::in);
        string line;
        int number;
        unordered_map<int, int>::iterator data_iterator;
        while (getline(infile, line))
        {
            number = stoi(line);
            data_iterator = data.find(number);
            if (data_iterator == data.end())
            {
                data[number] = 1;
            }
            else
            {
                data[number]++;
            }
        }

        //reverse keys and values to be able to sort data ascedingly having same frequency
        unordered_map<int, vector<int>> organized_data;
        for (data_iterator = data.begin(); data_iterator != data.end(); data_iterator++)
        {
            vector<int> freq_data = organized_data[data_iterator->second];
            freq_data.push_back(data_iterator->first);
            organized_data[data_iterator->second] = freq_data;
        }
        //an unordered map where the key is the frequency of occurrence of the number in the file
        //and the value is a vector containing the values the occurred with the same frequency
        return organized_data;
    }

    //write data to file from vector (data)
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

vector<int> sort_duplicates_desc(unordered_map<int, vector<int>>);

int main(int argc, char **argv)
{
    //check if CL args are less than 3 (we check for 4 as argv[0] is the executable path).
    if (argc < 4)
    {
        cout << "Error! arguments missing\n";
        exit(0);
    }

    //take the first 3 args (argv[1], argv[2], argv[3]) and ignore the rest if there is any.
    string unsorted_data_file = argv[1];
    string frequent_data_file = argv[2];
    string running_time_file = argv[3];

    //initialize vector from (unsorted_data_file)
    FileHandler *file_handler = new FileHandler();
    unordered_map<int, vector<int>> unsorted_data;
    unsorted_data = file_handler->init_map_from_file(unsorted_data_file);

    StopWatch *stopWatch = new StopWatch();
    stopWatch->start();

    //sort the data according to frequency.
    vector<int> sorted_data = sort_duplicates_desc(unsorted_data);

    //get elapsed time for the running of the algorithm
    int running_time = stopWatch->get_elapsed_ms();

    file_handler->write_vector_to_file(frequent_data_file, sorted_data);
    file_handler->write_running_time_to_file(running_time_file, running_time);

    delete file_handler;
    delete stopWatch;
}

//using counting sort to sort data descendingly
vector<int> sort_duplicates_desc(unordered_map<int, vector<int>> data)
{
    //get the keys of the organized_map and sort them descendingly.
    vector<int> count_arr;

    unordered_map<int, vector<int>>::iterator data_iterator;
    for (data_iterator = data.begin(); data_iterator != data.end(); data_iterator++)
    {
        //put the keys of data map in a vector to sort it descendingly.
        count_arr.push_back(data_iterator->first);

        //sort every vector in data map ascendingly.
        sort(data[data_iterator->first].begin(), data[data_iterator->first].end());
    }

    //sort descendingly using reverse iterators.
    sort(count_arr.rbegin(), count_arr.rend());

    vector<int> sorted_data;
    for (int i = 0; i < count_arr.size(); i++)
    {
        int vector_size = data[count_arr[i]].size();
        for (int j = 0; j < vector_size; j++)
        {
            for (int k = 0; k < count_arr[i]; k++)
            {
                sorted_data.push_back(data[count_arr[i]][j]);
            }
        }
    }
    return sorted_data;
}