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

ListTask::ListTask():size(0),data({}), tmax(1){}

void ListTask::insert(Task& t) {
	size = size + 1;
	data.push_back(t);
}

void ListTask::loadListTask(const string& filename) {
	//time = 0 -> all the duration are the same
	//time = 1 -> different duration
	int time;
	tmax = 1;
	ifstream file(filename);
	if (file) {
		cout << "ouverture du fichier" << endl;
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
				if (duration > tmax) tmax = duration;
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
	// cout << "début find" << endl;
	int i = 0;
	for (auto t : data){
		// cout << t << endl;
		if (t.getId() == id){
			// cout << "trouvééé" << endl;
			return i;
		}
		i ++;
	}
	// cout << "problem find" << endl;
	return -1;
}

	
ostream &operator<<(ostream& o, const ListTask& Lt) {
	o << "tmax = " << Lt.tmax << "\t";
	o << "size = " << Lt.size << endl;
	for (auto t : Lt.data) {
		o << t;
	}
	o << endl;
	return o;
}

void ListTask::visit(Task& t, vector<int>& v){
	t.getVis() = 2;
	// cout << "getVis()" << t.getVis() << endl;
	// cout << *this;
	for (auto& id : (*t.getNext())){
		// cout << id << endl;
		Task& u = data[(*this).find(id)];
		// cout << "après le find" << endl;
		if (u.getVis() == 0){
			visit(u, v);
		}
	}
	// cout << "t : " << t.getId()	<< endl;
	t.getVis() = 1;
	// cout << *this;
	v.insert(v.begin(), t.getId());
	// v.push_back(t.getId());
}


vector<int> ListTask::topoSort(){
	vector<int> v = {};
	cout << "start topo_sort" << endl << endl;
	for (auto& t: data){
		// cout << "v : " << v;
		// cout << *this; 
		if (t.getVis() == 0){
			// cout << "avant visit" << endl;
			(*this).visit(t, v);
			// cout << "après visit" << endl;
		}
	}
	return v;
}
	
ListTask	ListTask::invert(){
	ListTask L;
	L.tmax = tmax;
	for (auto& t : data){
		// cout << L;
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
		// data.erase((*this).find(id).get_allocator());
		// std::erase(data, task);
		(*task.getNext()).push_back(-1);
		for (auto& t : data){
			vector<int>* array = t.getNext();
			for (auto it = array->begin(); it != array->end();){
				if (*it == id){
					// cout << t << endl;
					// cout << id << endl;
					(*t.getNext()).erase(it);
					// cout << t << endl;
				}
				else it++;
			}
		}
}
	
	
ListTask ListTask::multi_process_scheduling (const string& filename){
	ofstream file(filename);
	ListTask L = (*this).invert();
	// cout << "L " << L;
	vector<Task> temp;
	int inc = 0;
	for (int i = 1; i < size; i++){
		file << i << " |";
		temp = {};
		for (auto& t : L.data){
			// cout << t << endl;
			if ((*t.getNext()).empty()){
				t.getVis() = i;
			    file << "\t" << t.getId();
				// L.erase_task(t);
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
	return L;
}
	

	
ListTask ListTask::multi_process_scheduling_duration(const string& filename){
	ofstream file(filename);
	ListTask L = (*this).invert();
	cout << "L " << L;
	vector<Task> temp;
	int inc = 0;
	for (int i = 1; i < size * tmax; i++){
		// temp = {};
		file << i;
		for (auto& t : L.data){
			auto p = std::find(temp.begin(), temp.end(), t);
			// cout << t << endl;
			if ((*t.getNext()).empty() && p == temp.end()){
				t.getVis() = i;
				// L.erase_task(t);
				temp.push_back(t);
				file << "\tdébut de " << t.getId();
				inc++;
			}
		}
		for (auto it =  temp.begin(); it != temp.end(); ) {
			(*it).getDur() -= 1;
			cout << *it << (*it).getDur()  << endl;
			if ((*it).getDur() <= -1){
				L.erase_task(*it);
				file << "\t fin de " << (*it).getId();
				temp.erase(it);
			}else{
				it++;
			}
		}
		if ( inc >= size && temp.empty())  break;
		file << endl;
	}
	// write_solution(filename, L);
	return L;
}



// ListTask ListTask::multi_process_scheduling (){
	// ListTask L = (*this).invert();
	// int inc = 0;
	// vector<int> array;
	// vector<int> temp;
	// for (int i = 0; i < size; i++) {
		// for (auto& t : L.data) {
			// for (auto& it = (*t.getNext()).begin(); it < (*t.getNext()).end(); it++) {
				
			// }
		// }	
	// }
// }

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