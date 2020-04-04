#include "worker.h"


int main(int argc, char const *argv[]) 
{
    worker w(atoi(argv[1]));

    w.fetch_read_data(w.receive_data());
    w.extract_files_contents();
    w.send_result_to_presenter(w.do_the_search());

    return 0;
}