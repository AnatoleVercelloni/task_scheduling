#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <algorithm>

using namespace std;

class Task{
	
	const int id;
	int duration;
	int visited;
	vector<int>* next;
	
	public:
	
	Task(const int& id, int duration = 1);
	Task operator= (const Task& t);
	bool operator==(const Task& t);
	vector<int>* getNext();
	int getId();
	int& getVis();
	int& getDur();
	void push(int& id);
	friend ostream& operator<<(ostream& o, const Task& t);
};

class ListTask {
	
	int size;
	vector<Task> data;
	ListTask invert();
	void visit(Task& t, vector<int>& v);

	public:
	
	ListTask();
	void loadListTask(const string& filename);
	void insert(Task& t);
	friend ostream& operator<<(ostream& o, const ListTask& Lt);
	vector<int> topoSort();
	int find(int &id);
	void erase_task(Task& task);
	ListTask multi_process_scheduling(const string& filename);
	ListTask multi_process_scheduling_duration(const string& filename);
};


ostream &operator<<(ostream &o, vector<int> &v);