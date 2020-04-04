#include <stdlib.h>
#include "string.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>
#include <fcntl.h>
#include <vector>

using namespace std;


class presenter
{
private:
    int workers_cnt;
    string sort_field;
    string order;
    vector <vector <string>> received_data;
public:
    presenter(int workers_cnt, string order, string sort_field);
    ~presenter();
    void read_data();
    vector <vector <string>> sort_data(vector <vector <string>> data);
    vector<vector<string>> merge_data(const vector<vector <string>> &data1, const vector<vector <string>> &data2);
    void print_data();
};

