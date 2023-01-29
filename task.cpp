#include "task.hpp"
#include <algorithm>

using namespace std;

Task::Task(int& id, int duration):id(id),visited(0), duration(duration){
	next = new vector<int>;
}

vector<int>* Task::getNext(){
	return next;
}

int Task::getId() {
	return id;
}

int& Task::getVis() {
	return visited;
}

int& Task::getDur() {
	return duration;
}

void Task::push(int& id) {
	next->push_back(id);
}

bool Task::operator==(const Task& t){
	return id == t.id;
}
	

ostream& operator<<(ostream& o, const Task& t) {
	o << "Id = " << t.id << "\t" << "duration = " << t.duration;
	o << "\t next : ";
	for (auto num : *t.next) {
		o << num << ", ";
	}
	o << "visit : " << t.visited;
	o << endl;
	return o;
}

ListTask::ListTask():size(0),data({}){}

void ListTask::insert(Task& t) {
	size = size + 1;
	data.push_back(t);
}

void ListTask::loadListTask(const string& filename) {
	/*time = 0 -> all the duration are the same
	time = 1 -> different duration*/
	int time;
	ifstream file(filename);
	if (file) {
		cout << "ouverture du fichier: ok" << endl;
		int size;
		file >> size;
		file >> time;
		string line;
		getline(file, line);
		for (int i = 0; i < size; i++) {
			getline(file, line);
			istringstream iss(line);
			int id;
			int duration;
			iss >> id;
			if (time == 1){
				iss >> duration;
			}else{
				duration = 1;
			}
			Task t(id, duration);
			while(iss >> id) {
				t.push(id);
			}
			(*this).insert(t);
		}
	}else{
		cout << "file error" << endl;
		exit(1);
	}
}

int ListTask::find(int& id){
	int i = 0;
	for (auto t : data){
		if (t.getId() == id){
			return i;
		}
		i ++;
	}
	return -1;
}

	
ostream &operator<<(ostream& o, const ListTask& Lt) {
	o << "size = " << Lt.size << endl;
	for (auto t : Lt.data) {
		o << t;
	}
	o << endl;
	return o;
}

void ListTask::visit(Task& t, vector<int>& v){
	t.getVis() = 2;
	for (auto& id : (*t.getNext())){
		Task& u = data[(*this).find(id)];
		if (u.getVis() == 0){
			visit(u, v);
		}
	}
	t.getVis() = 1;
	v.insert(v.begin(), t.getId());
}


vector<int> ListTask::topoSort(){
	cout << "L: " << endl  << *(this);
	vector<int> v = {};
	cout << "topological_sort" << endl << endl;
	for (auto& t: data){
		if (t.getVis() == 0){
			(*this).visit(t, v);
		}
	}
	return v;
}
	
ListTask	ListTask::invert(){
	ListTask L;
	for (auto& t : data){	
		int id = t.getId();
		int duration = t.getDur();
		int index = L.find(id);
		
		if (index == -1){
			Task tmp(id, duration);
			L.insert(tmp);
		}
		else L.data[index].getDur() = duration;
		
		for (auto& i : *t.getNext()){
			Task _t(i);
			int f = L.find(i);
			if (f == -1){
				L.insert(_t);
			}
			else {
				_t = L.data[f];
			}
			_t.push(id);
		}
	}
	return L;
}
			

void  ListTask::erase_task(Task& task){
	// task should be present inside the ListTask
		int id = task.getId();
		(*task.getNext()).push_back(-1);
		for (auto& t : data){
			vector<int>* array = t.getNext();
			for (auto it = array->begin(); it != array->end();){
				if (*it == id){
					(*t.getNext()).erase(it);
				}
				else it++;
			}
		}
}
	
	
ListTask ListTask::multi_process_scheduling (const string& filename){
	ofstream file(filename);
	cout << "L: " << endl  << *(this);
	cout << "multi_process_scheduling " << endl;
	ListTask L = (*this).invert();
	vector<Task> temp;
	int inc = 0;
	for (int i = 1; i < size; i++){
		file << i << " |";
		temp = {};
		for (auto& t : L.data){
			if ((*t.getNext()).empty()){
				t.getVis() = i;
			    file << "\t" << t.getId();
				temp.push_back(t);
				inc++;
			}
		}
		for (auto& t : temp) {
			L.erase_task(t);
		}
		if ( inc >= size) break;
	file << endl;
	}
	cout << "result registered in " << filename << endl << endl;
	return L;
}
	

	
ListTask ListTask::multi_process_scheduling_duration(const string& filename){
	cout << "L :" << endl  << *(this);
	cout << "multi_process_scheduling_duration " << endl;
	ofstream file(filename);
	ListTask L = (*this).invert();
	vector<Task> temp;
	int inc = 0;
	int i = 1;
	while (!temp.empty() || inc < size ){ 
		file << i;
		for (auto& t : L.data){
			auto p = std::find(temp.begin(), temp.end(), t);
			if ((*t.getNext()).empty() && p == temp.end()){
				t.getVis() = i;
				temp.push_back(t);
				file << "\tdébut de " << t.getId();
				inc++;
			}
		}
		for (auto it =  temp.begin(); it != temp.end(); ) {
			(*it).getDur() -= 1;
			if ((*it).getDur() <= -1){
				L.erase_task(*it);
				file << "\t fin de " << (*it).getId();
				temp.erase(it);
			}else{
				it++;
			}
		}
		i ++;
		file << endl;
	}
	cout << "result registered in " << filename << endl << endl;
	return L;
}



ostream &operator<<(ostream &o, vector<int> &v){
	for (const auto& uj:v){ o << uj << "\t";}
	return o << endl;
}

// int ListTask::check_path(Task& u, Task& v){
	// deque <int> queue(size);
	// queue.push_front(u);
	// while (file){
		// Task cour = queue.back();
		// queue.pop_back();
		// cour.getVis() = 1;
		// for (auto& i : *cour.getNext()){
			// if (i
		
	
// int ListTask::check_acyclic(){
	// vector<int> file(size);
	// file.push_back(