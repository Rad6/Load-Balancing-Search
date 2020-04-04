all : worker presenter main


worker : tools.h worker.h worker.cpp worker_main.cpp
	g++ worker.cpp worker_main.cpp -o worker_main
presenter : tools.h presenter.h presenter.cpp presenter_main.cpp
	g++ presenter.cpp presenter_main.cpp -o presenter_main
main : tools.h load_balancer.h load_balancer.cpp main.cpp
	g++ load_balancer.cpp main.cpp -o main


clean:
	rm -f -r *.o worker_main main presenter_main