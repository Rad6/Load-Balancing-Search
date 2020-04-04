#include "load_balancer.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, const char *argv[]) 
{

    string command;

    while (true)
    {
        getline(cin, command);
        if (command == "quit")
            break;
        load_balancer lb(command);
        lb.make_named_pipe();
        lb.fork_presenter();
        lb.make_worker_pipes();
        lb.fork_workers();
        lb.send_workers_data();
    }
    return 0;
}