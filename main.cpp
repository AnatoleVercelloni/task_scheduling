#include <iostream>
#include <fstream>
#include <vector>
#include "task.hpp"



int main(){
	
	
	// ListTask Lt0;
	// Lt0.loadListTask("bonhomme_de_neige.txt");
	// vector<int> sort = Lt0.topoSort();
	// cout << "result: " << sort << endl;
	
	// ListTask Lt1;
	// Lt1.loadListTask("bonhomme_de_neige.txt");
	// ListTask L1 =  Lt1.multi_process_scheduling("result_multi.txt");
	
	ListTask Lt2;
	Lt2.loadListTask("bonhomme_de_neige_dur.txt");
	ListTask L2 =  Lt2.multi_process_scheduling_duration("result_multi_duration.txt");
	
	
	return 0;
}
