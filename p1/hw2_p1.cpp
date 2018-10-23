#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <fstream>

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

//nodes of integer data
class Node
{
    friend class LinkedList;
    friend class FileHandler;

  private:
    Node *next;
    int data;
};

class LinkedList
{
    friend class FileHandler;

  private:
    Node *head;
    int length;

  public:
    LinkedList()
    {
        head = nullptr;
        length = 0;
    }

    void add_back(int data)
    {
        if (length == 0)
        {
            head = new Node();
            head->data = data;
            length++;
            return;
        }
        Node *tail = head;
        while (tail->next != nullptr)
        {
            tail = tail->next;
        }
        Node *new_node = new Node();
        new_node->data = data;
        tail->next = new_node;
        length++;
    }

    void add_front(int data)
    {
        if (length == 0)
        {
            head = new Node();
            head->data = data;
            length++;
            return;
        }
        Node *new_node = new Node();
        new_node->data = data;
        new_node->next = head;
        head = new_node;
        length++;
    }

    //concatenate 2 lists of size 2
    void concatenate(LinkedList *list)
    {
        if (list->head->next != nullptr)
        {
            list->head->next->next = head;
            head = list->head;
        }
        else
        {
            list->head->next = head;
            head = list->head;
        }
        length = length + list->length;
    }

    void delete_list()
    {
        Node *current = head;
        Node *next;
        while (current != nullptr)
        {
            next = current->next;
            free(current);
            current = next;
        }
        length = 0;
    }

    //done
    void print()
    {
        Node *printer;
        printer = this->head;
        int counter = 0;
        while (printer != nullptr)
        {
            cout << "Node " << counter << ": " << printer->data << endl;
            printer = printer->next;
            counter++;
        }
        cout << endl
             << endl;
    }

    //using a divide-and-conquer-like approach
    void reverse_list_v1()
    {
        if (length == 0 || length == 1)
        {
            return;
        }
        if (length == 2)
        {
            Node *next_node = head->next;
            head = next_node;
            next_node->next = nullptr;
            return;
        }
        Node *reverser = head;
        int duos;
        if (length % 2 == 0)
        {
            duos = length / 2;
        }
        else
        {
            duos = (length / 2) + 1;
        }
        //a list of duos
        LinkedList *lists[duos];
        int counter = 0;
        LinkedList *list;
        while (reverser != nullptr)
        {
            list = new LinkedList();
            list->add_front(reverser->data);
            reverser = reverser->next;
            if (reverser != nullptr)
            {
                list->add_front(reverser->data);
                if (reverser->next != nullptr)
                {
                    reverser = reverser->next;
                }
            }
            lists[counter] = list;
            counter++;
        }
        //delete list to save memory.
        delete_list();
        // initialize current list with the end of lists.
        LinkedList *reversed_list = new LinkedList();
        reversed_list->head = lists[0]->head;
        reversed_list->length = 2;
        for (int i = 1; i < duos; i++)
        {
            reversed_list->concatenate(lists[i]);
        }
        this->head = reversed_list->head;
    }

    //the simplest approach, iterate with 3 pointers and change pointing direction
    void reverse_list_v2()
    {
        Node *prev = nullptr;
        Node *curr = head;
        Node *next = nullptr;
        while (curr != nullptr)
        {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;
    }
};

class FileHandler
{
  public:
    //initializes a linked list from a file
    LinkedList *init_list_from_file(string filename)
    {
        LinkedList *list = new LinkedList();
        ifstream infile(filename, ios::in);
        string line;
        Node *tail = list->head;
        while (!infile.eof())
        {
            getline(infile, line);
            Node *adder = new Node();
            adder->data = stoi(line);
            if (tail == nullptr)
            {
                list->head = adder;
                tail = list->head;
            }
            else
            {
                tail->next = adder;
                tail = adder;
            }
            list->length++;
        }
        infile.close();
        return list;
    }

    //write to file reversed linked list
    void write_reversed_list_to_file(string filename, LinkedList *list)
    {
        ofstream outfile(filename, ios::out);
        Node *writer = new Node();
        writer = list->head;
        while (writer != nullptr)
        {
            outfile << writer->data;
            if (writer->next != nullptr)
            {
                outfile << endl;
            }
            writer = writer->next;
        }
        outfile.close();
    }

    void write_running_time_to_file(string filename, int running_time)
    {
        ofstream outfile(filename, ios::out);
        outfile << "running time in ms: " << running_time;
        outfile.close();
    }
};

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

    //initializing the linked list from (unsorted_data_file)
    LinkedList *list = new LinkedList();
    FileHandler *file_handler = new FileHandler();
    list = file_handler->init_list_from_file(unsorted_data_file);

    //starting a stopwatch to calculate runtime
    StopWatch *stopWatch = new StopWatch();
    stopWatch->start();

    //reverse the linked list
    list->reverse_list_v1();

    //stop the stopwatch and get the runtime
    int running_time = stopWatch->get_elapsed_ms();

    //write the reversed data to (sorted_data_file) and the running time to (running_time_file)
    file_handler->write_reversed_list_to_file(sorted_data_file, list);
    file_handler->write_running_time_to_file(running_time_file, running_time);

    delete file_handler;
    delete stopWatch;
}