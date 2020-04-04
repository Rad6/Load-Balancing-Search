#include "presenter.h"
#include "tools.h"


int sort_index;
bool is_ascending;

presenter::presenter(int workers_cnt, string order, string sort_field)
{
    this->workers_cnt = workers_cnt;
    this->order = order;
    this->sort_field = sort_field;
    sort_index = get_index(sort_field);
    is_ascending = this->order == ASCENDING;
}

bool comp(const vector<string> &a,const vector<string> &b)
{
    if (isFloat(a[sort_index]))
        return is_ascending ? stof(a[sort_index]) < stof(b[sort_index]) : stof(a[sort_index]) > stof(b[sort_index]);
    else
        return is_ascending ? a[sort_index] < b[sort_index] : a[sort_index] > b[sort_index];
}

void presenter::read_data()
{
    char buff[4096];
    int workers_done = 0;
    vector <vector <string>> recieved;
    int fd = open(FIFO_PATHNAME, O_RDONLY);
    while(true)
    {
    	memset(buff, '\0', sizeof(buff));
        if (read(fd, buff, 4096) <= 0)
            continue;
        recieved.clear();
        string tmp = "";
        for (int i = 0; i < strlen(buff); i++)
        {
            tmp.push_back(buff[i]);
            if (buff[i] == '\n')
            {
                if (tmp == END_OF_THIS_WORKERS_RESULT)
                {
                    workers_done++;
                    tmp = "";
                    continue;
                }
                recieved.push_back(split_line_by_dash(tmp));
                tmp = "";
                continue;
            }
        }
        if (this->order != NOT_SORTED)
            recieved = this->sort_data(recieved);
        received_data = this->merge_data(recieved, this->received_data);

        if (workers_done >= workers_cnt)
            break;
    }
    close(fd);
    std::cout << "Done!\n";
}


vector <vector <string>> presenter::sort_data(vector <vector <string>> data)
{
    if (data.size() == 0)
        return data;
    sort(data.begin(), data.end(), comp);
    return data;
}


vector<vector<string>> presenter::merge_data(const vector<vector <string>> &data1, const vector <vector <string>> &data2)
{
    vector<vector<string>> result;
    if (data1.size() == 0)
        return this->received_data;
    
    if (this->order == NOT_SORTED)
    {
        for (int i = 0; i < data1.size(); i++)
            result.push_back(data1[i]);
        return result;
    }
    if (data2.size() == 0)
    {
        for (int i = 0; i < data1.size(); i++)
            result.push_back(data1[i]);
        return result;
    }

    result.resize(data1.size() + data2.size());
    std::merge(data1.begin(), data1.end(), data2.begin(), data2.end(), result.begin(), comp);
    
    return result;
}


void presenter::print_data()
{
    std::cout << "-------------------------- result --------------------------\n";
    for (int i = 0; i < this->received_data.size(); i++)
        for (int j = 0; j < this->received_data[i].size(); j++)
        {
            std::cout << this->received_data[i][j];
            if (j != this->received_data[i].size() - 1)
                std::cout << " - ";
        }
    std::cout << endl;
}

presenter::~presenter()
{
}
