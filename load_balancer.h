#include <string.h>
#include <vector>
#include <map>
#include "iostream"
#include "dirent.h"
#include <unistd.h>
#include "iterator"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wait.h>
#include <cstdlib>
#include <fcntl.h>

using namespace std;

#define WORKER_MAIN_EXEC    "worker_main"
#define PRESENTER_MAIN_EXEC "presenter_main"

typedef enum
{
  ascending,
  descending,
  not_sorted
} order;


class load_balancer
{

private:
  map <string, string> search_fields;
  order _order;
  string sort_field;
  int workers_cnt;
  string dir_path;
  int files_cnt;
  vector <string> file_names;
  vector <vector <int>> worker_pipes;

public:
  load_balancer(string command);
  ~load_balancer();
  void fork_workers();
  void fetch_command_data(string command);
  void set_file_names();
  void make_worker_pipes();
  void make_named_pipe();
  void send_workers_data();
  void fork_presenter();

  // for debugging
  void print_attributes();
};



