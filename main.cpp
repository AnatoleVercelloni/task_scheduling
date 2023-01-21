#include <iostream>
#include <fstream>
#include <vector>
#include "task.hpp"



int main(){
	ListTask Lt;
	Lt.loadListTask("task.txt");
	// Lt = Lt.invert();
	cout << Lt << endl;
	ListTask L =  Lt.multi_process_scheduling_duration("result.txt");
	// ListTask L2 =  Lt.multi_process_scheduling();
	cout << L;
	// cout << L2;
	// vector<int> sort = Lt.topoSort();
	// cout << sort << endl;
	return 0;
}
