#include <vector>
#include <string.h>


#define FIFO_PATHNAME "/tmp/myfifo"


#define Name         "Name"
#define Platform     "Platform"
#define Year         "Year"
#define Genre        "Genre"
#define Publisher    "Publisher"
#define NA_Sales     "NA_Sales"
#define EU_Sales     "EU_Sales"
#define JP_Sales     "JP_Sales"
#define Other_Sales  "Other_Sales"
#define Global_Sales "Global_Sales"
#define ASCENDING    "ascending"
#define DESCENDING   "descending"
#define NOT_SORTED   "not_sorted"
#define END_OF_THIS_WORKERS_RESULT "$\n"


vector <string> split_line_by_dash(string line)
{
    vector <string> result;
    string word = "";
    for (int i = 0; i < line.size(); i++)
    {        
        if (line[i] == '-')
        {
            if (line[i -1] == ' ' && line[i + 1] == ' ')
            {
                word.pop_back();
                i++;
            }
            result.push_back(word);
            word = "";
            continue;
        }
        else if (i == line.size() - 1)
        {
            word.push_back(line[i]);
            result.push_back(word);
            break;
        }
        word.push_back(line[i]);
    }
    return result;
}

vector<string> split_by_line(string str)
{
    vector<string> result;
    string tmp = "";
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == '\n')
        {
            result.push_back(tmp);
            continue;
        }
        tmp.push_back(str[i]);
    }
    return result;
}

int get_index(string field)
{
    if (field == Name)
        return 0;
    if (field == Platform)
        return 1;
    if (field == Year)
        return 2;
    if (field == Genre)
        return 3;
    if (field == Publisher)
        return 4;
    if (field == NA_Sales)
        return 5;
    if (field == EU_Sales)
        return 6;
    if (field == JP_Sales)
        return 7;
    if (field == Other_Sales)
        return 8;
    if (field == Global_Sales)
        return 9;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

bool isFloat(string str)
{
    try
    {
        stof(str);
    }
    catch(const invalid_argument e)
    {
        return false;
    }
    return true;
}
