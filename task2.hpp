#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>

using namespace std;

class Task{
	int id;
	int duration;
	int visited;
	vector<Task> next;
	
	public:
	Task(int& id, int duration = 1);
	vector<Task> getNext();
	int getId();
	int& getVis();
	int& getDur();
	void push(int& id);
	friend ostream& operator<<(ostream& o, const Task& t);
	// friend void ListTask::visit(Task& t, vector<int>& v);
};

class ListTask {
	int size;
	vector<Task> data;
	int tmax;
	
	public:
	ListTask();
	void loadListTask(const string& filename);
	void insert(Task& t);
	friend ostream& operator<<(ostream& o, const ListTask& Lt);
	void visit(Task& t, vector<int>& v);
	vector<int> topoSort();
	int find(int &id);
	ListTask	invert();
	void  erase_task(Task& task);
	ListTask multi_process_scheduling(const string& filename);
	ListTask multi_process_scheduling_duration(const string& filename);
	// int check(ListeTask& Lt);
};


ostream &operator<<(ostream &o, vector<int> &v);