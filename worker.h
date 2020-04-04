#include "string.h"
#include "iostream"
#include "fstream"
#include "vector"
#include "map"
#include "unistd.h"
#include "iterator"
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <fcntl.h>


using namespace std;

#define BUFF_LEN 2048


class worker
{
private:
    int read_pipe_fd;
    vector <string> my_file_names;
    map <string, string> search_fields;
    string dir_path;
    int my_files_cnt;
    vector < vector <string> > my_files_contents;
    vector < vector <string> > results;
    vector <string> all_fields;
    void extract_all_fields();

public:
    worker(int read_fd);
    string receive_data();
    void fetch_read_data(string data);
    void extract_files_contents();
    string do_the_search();
    void send_result_to_presenter(string my_result);
    ~worker();

    // for debugging
    void print_attributes();
};

