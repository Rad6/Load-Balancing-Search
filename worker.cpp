#include "worker.h"
#include "tools.h"


worker::worker(int read_fd)
{
    this->read_pipe_fd = read_fd;
    this->my_files_cnt = 0;
}


void worker::extract_files_contents()
{
    for (int i = 0; i < this->my_files_cnt; i++)
    {
        ifstream file(this->dir_path + "/" + this->my_file_names[i]);
        string line;

        if (getline(file, line) && i == 0)
            this->my_files_contents.push_back(split_line_by_dash(line));
        
        while (getline(file, line))
            this->my_files_contents.push_back(split_line_by_dash(line));        
    }
    this->extract_all_fields();
}


void worker::extract_all_fields()
{
    for (int i = 0; i < this->my_files_contents[0].size(); i++)
        this->all_fields.push_back(this->my_files_contents[0][i]);
}


string worker::do_the_search()
{
    string result = "";
    string fields[10][2];

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 2; j++)
            fields[i][j] = "";

    map<string, string>::iterator it;
    for (it = search_fields.begin(); it != search_fields.end(); it++)
    {
        for (int i = 0; i < all_fields.size(); i++)
            if (it->first == all_fields[i])
            {
                fields[i][0] = it->first;
                fields[i][1] = it->second;
                break;
            }
    }
    for (int i = 1; i < my_files_contents.size(); i++)
    {
        bool is_in_result = true;
        for (int j = 0; j < my_files_contents[i].size(); j++)
        {
            if (fields[j][0] == "")
                continue;
            else if (fields[j][1] != my_files_contents[i][j])
            {
                is_in_result = false;
                break;
            }
        }
        if (is_in_result)
        {
            for (int k = 0; k < my_files_contents[i].size(); k++)
                result += (my_files_contents[i][k] + "-");
            result += "\n";
        }        
    }
    return result;
}


void worker::send_result_to_presenter(string my_result)
{
    my_result += END_OF_THIS_WORKERS_RESULT;
    mkfifo(FIFO_PATHNAME, 0666);
    int fd = open(FIFO_PATHNAME, O_WRONLY);
    if (fd == -1)
    {
        perror("WORKER : couldnt open fifo\n");
        return;
    }
    write(fd, my_result.c_str(), my_result.size());
    close(fd);
}


string worker::receive_data()
{
    char buff[BUFF_LEN];
    read(this->read_pipe_fd, buff, BUFF_LEN);
    close(this->read_pipe_fd);
    return (string)buff;
}


void worker::fetch_read_data(string data)
{
    string key = "";
    string value = "";
    bool is_key = true;

    int i;
    for (i = 0; ; i++)
    {
        if (data[i] == '-')
            break;
        else
            key.push_back(data[i]);
    }
    i++;
    this->dir_path = key;
    key = "";

    for (;; i++)
    {
        if (data[i] == '-')
        {
            if (key == "searchfields:")
                break;
            else
            {
                this->my_file_names.push_back(key);
                this->my_files_cnt++;
                key = "";
            }  
        }
        else
            key.push_back(data[i]);
    }
    i++;
    key = "";

    for (; i < data.size() ; i++)
    {
        if (data[i] == '-')
        {
            search_fields.insert({key, value});
            key = "";
            value = "";
            is_key = true;
            continue;
        }
        if (data[i] == '=')
        {
            is_key = false;
            continue;
        }
        (is_key ? key : value).push_back(data[i]);   
    }
}

void worker::print_attributes()
{
    cout<<"\n ------ worker attributes ------\n";
    cout << "number of files: " << this-> my_files_cnt << endl;
    cout << "dir path: " << this-> dir_path << endl;

    map<string, string>::iterator it;
    for (it = search_fields.begin(); it != search_fields.end(); it++)
        cout << it->first << " : " << it->second << endl;
    
}

worker::~worker()
{
}