#include "load_balancer.h"
#include "tools.h"
#include <stdlib.h>


load_balancer::load_balancer(string command)
{   
    this->_order = not_sorted;
    this->sort_field = "";
    this->fetch_command_data(command);
    this->set_file_names();
    this->workers_cnt = min(this->workers_cnt, this->files_cnt);
}


void load_balancer::make_worker_pipes()
{
    for (int i = 0; i < workers_cnt; i++)
    {
        int worker_pipe[2];
        if (pipe(worker_pipe) == -1)
        {
            perror("pipe creating failed\n");
            return;
        }
        else
        {
            vector <int> tmp;
            tmp.push_back(worker_pipe[0]);
            tmp.push_back(worker_pipe[1]);
            worker_pipes.push_back(tmp);
        }
    }
}


void load_balancer::set_file_names()
{
    this->files_cnt = 0;
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(dir_path.c_str())) != NULL) {
        while ((entry = readdir (dir)) != NULL) 
        {
            if ((entry->d_name)[0] != '.')
            {
                this->file_names.push_back(entry->d_name);
                this->files_cnt++;
            }            
        }
        closedir (dir);
    } 
    else 
    {
        perror("opening directory failed"); 
        return;
    }
}


void load_balancer::fetch_command_data(string command)
{

    string key = "";
    string value = "";
    bool is_key = true;

    for (int i = 0 ; ; i++)
    {
        if (i == command.size() - 1)
        {
            value.push_back(command[i]);
            this->dir_path = value;
            break;
        }
        
        if (command[i] == '=')
        {
            is_key = false;
            continue;
        }
        else if (command[i] == ' ')
            continue;
        else if (command[i] == '-')
        {
            if (value == "ascending" || value == "descending")
            {
                this->sort_field = key;
                this->_order = value == "ascending" ? ascending : descending;
                is_key = true;
            }
            else if (key == "processes")
            {
                this->workers_cnt = stoi(value);
                is_key = true;
            }
            else
            {
                (this->search_fields).insert({key, value});
                is_key = true;
            }
            key = "";
            value = "";
            is_key = true;
            continue;
        }
    
        (is_key ? key : value).push_back(command[i]);
    }
}


void load_balancer::fork_workers()
{
    for (int i = 0; i < this->workers_cnt; i++)
    {   
        pid_t pid;
        pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            return;
        }
        else if( pid == 0 ) 
        {
            char * argv[3];
            argv[0] = (char*) WORKER_MAIN_EXEC;
            argv[1] = (char*) to_string(this->worker_pipes[i][0]).c_str();
            argv[2] = NULL;
            close(worker_pipes[i][1]);
            if(execv(argv[0], argv) == -1)
                perror("exec failed");
        }
        else
        {
            close(worker_pipes[i][0]);
            continue;
        }        
    }
}


void load_balancer::fork_presenter()
{
    pid_t pid;
    pid = fork();

    string order;
    if (this->_order == ascending)
        order  = ASCENDING;
    else if(this->_order == descending)
        order = DESCENDING;
    else
        order = NOT_SORTED;
    
    if (pid < 0)
    {
        perror("presenter forking failed");
        return;
    }
    else if( pid == 0 ) 
    {
        char * argv[5];
        argv[0] = (char*) PRESENTER_MAIN_EXEC;
        argv[1] = (char*) to_string(this->workers_cnt).c_str();
        argv[2] = (char*) order.c_str();
        argv[3] = (char*) this->sort_field.c_str();
        argv[4] = NULL;
        if(execv(argv[0], argv) == -1)
            perror("presenter execing failed");
    }
}


void load_balancer::make_named_pipe()
{
    remove(FIFO_PATHNAME);
    if(mkfifo(FIFO_PATHNAME, 0666) < 0)
    {
        perror("couldn't create named pipe");
        return;
    }
}


void load_balancer::send_workers_data()
{
    string *workers_data = new string[this->workers_cnt];

    for (int i = 0; i < this->workers_cnt; i++)
        workers_data[i] = "";
        
    for (int i = 0; i < this->workers_cnt; i++)
        workers_data[i] += (dir_path + "-");

    for (int i = 0; i < this->files_cnt; i++)
        workers_data[i % workers_cnt] += (this->file_names[i] + "-");

    for (int i = 0; i < this->workers_cnt; i++)
        workers_data[i] += ("searchfields:-");
    
    for (int i = 0; i < this->workers_cnt; i++)
    {
        map<string, string>::iterator it;
        for (it = search_fields.begin(); it != search_fields.end(); it++)
            workers_data[i] += (it->first + "=" + it->second + "-");
    }
    // sending stuff 
    for (int i = 0; i < workers_cnt; i++)
    {
        write(worker_pipes[i][1], (char *)workers_data[i].c_str(), strlen(workers_data[i].c_str()) );
        close(worker_pipes[i][1]);
    }
}


load_balancer::~load_balancer()
{
    for (int i = 0; i < this->workers_cnt; i++)
        wait(NULL); // for each worker
    wait(NULL);     // for presenter
}


void load_balancer::print_attributes()
{
    cout << "number of files: "   << this->files_cnt   << endl;
    cout << "number of workers: " << this->workers_cnt << endl;
    cout << "dir path: "          << this->dir_path    << endl;

    map<string, string>::iterator it;
    for (it = search_fields.begin(); it != search_fields.end(); it++)
        cout << it->first << " : " << it->second << endl;
    cout << "number of worker pipes created: " << worker_pipes.size() << endl;
}