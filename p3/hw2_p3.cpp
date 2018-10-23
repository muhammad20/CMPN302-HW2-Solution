#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sys/time.h>
#include <math.h>

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
        filename = "/home/engineer/Desktop/CCE-C/Senior-1/Fall18/CMPN302/Assignments/hw2/p3/" + filename;
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

void merge(vector<int>&, int, int, int);
void iterative_merge_sort(vector<int> &data);

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
    string sorted_data_file = argv[2];
    string running_time_file = argv[3];

    FileHandler *file_handler = new FileHandler();
    vector<int> data = file_handler->init_vector_from_file(unsorted_data_file);

    StopWatch *stopWatch = new StopWatch();

    stopWatch->start();
    iterative_merge_sort(data);
    int running_time = stopWatch->get_elapsed_ms();

    file_handler->write_vector_to_file(sorted_data_file, data);
    file_handler->write_running_time_to_file(running_time_file, running_time);
    delete file_handler;
    delete stopWatch;
    return 0;
}

void iterative_merge_sort(vector<int> &data)
{
    //get the maximum number that will divide the array.
    int max_division = log2(data.size());
    int size = data.size();

    // current size of subarrays from 1 to vector_size / 2.
    int current_size;

    // start, mid, end of subarray.
    int start;
    int mid;
    int end;

    for(current_size = 1; current_size < size; current_size = current_size * 2)
    {
        for(start = 0; start < size - current_size; start += 2 * current_size)
        {
            mid = start + current_size - 1;
            end = start + 2 * current_size - 1;
            if(end > size - 1)
            {
                end = size - 1;
            }
            merge(data, start, mid, end);
        }
    }
}

void merge(vector<int> &data, int left, int mid, int end)
{
    //left and right vectors lengths
    int n_left_vector = mid - left + 1;
    int n_right_vector = end - mid;

    int* left_vector = new int[n_left_vector];
    int* right_vector = new int[n_right_vector];

    for (int i = 0; i < n_left_vector; i++)
    {
        left_vector[i] = data[i + left];
    }
    for (int i = 0; i < n_right_vector; i++)
    {
        right_vector[i] = data[mid + 1 + i];
    }

    int l = 0;
    int r = 0;
    int k = left;
    while (l < n_left_vector && r < n_right_vector)
    {
        if (left_vector[l] <= right_vector[r])
        {
            data[k] = left_vector[l];
            l++;
        }
        else
        {
            data[k] = right_vector[r];
            r++;
        }
        k++;
    }

    while (l < n_left_vector)
    {
        data[k] = left_vector[l];
        l++;
        k++;
    }

    while (r < n_right_vector)
    {
        data[k] = right_vector[r];
        r++;
        k++;
    }
    delete[] left_vector;
    delete[] right_vector;
}