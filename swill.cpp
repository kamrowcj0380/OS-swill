#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;


/*
 * Create a class to store individual process information
 */
class Process {
	public:
		int pid;
		int burst;
		int arrival;
		int priority;
		int ageindex;
		Process() {
			pid = 0;
			burst = 0;
			arrival = 0;
			priority = 0;
			ageindex = 0;
		}
		Process(int p_id) {
			pid = p_id;
			burst = 0;
			arrival = 0;
			priority = 0;
			ageindex = 0;
		}
		Process(int p_id, int burst_time, int arrival_time, int priority_num, int age_index) {
			pid = p_id;
			burst = burst_time;
			arrival = arrival_time;
			priority = priority_num;
			ageindex = age_index;
		}
	
};


/*
 * Helper QuickSort function
 */
int makeSplit(Process arr[], int start, int end) {

	int pivot = arr[start].arrival;

	int count = 0;
	for (int i = start + 1; i <= end; i++) {
		if (arr[i].arrival <= pivot)
			count++;
	}

	// Giving pivot element its correct position
	int pivotIndex = start + count;
	swap(arr[pivotIndex], arr[start]);

	// Sorting left and right parts of the pivot element
	int i = start, j = end;

	while (i < pivotIndex && j > pivotIndex) {

		while (arr[i].arrival <= pivot) {
			i++;
		}

		while (arr[j].arrival > pivot) {
			j--;
		}

		if (i < pivotIndex && j > pivotIndex) {
			swap(arr[i++], arr[j--]);
		}
	}

	return pivotIndex;
}

/*
 * Main QuickSort function
 */
void quickSort(Process arr[], int start, int end) {

	// base case
	if (start >= end)
		return;

	// split the array to left and right
	int pivot = makeSplit(arr, start, end);

	// quicksort left
	quickSort(arr, start, pivot-1);

	// quicksort right
	quickSort(arr, pivot+1, end);
}

/*
 * Main function
 */
int main (int argc, char *argv[]) {
	//test arguments
	string path;
	if(argc == 1) {
		cout << "Include a path to the input file in the arguments." << '\n';
		return 0;
	} else {
		path = argv[1];
	}

	//Prompt user for time quantum and aging interval
	int time_quantum; 
	int aging_interval;

	cout << "Enter time quantum (integer only): ";
	cin >> time_quantum;
	
	cout << "Enter aging interval (integer only): ";
	cin >> aging_interval;
	
	cout << "TQ: " << time_quantum << "\n";
	cout << "AI: " << aging_interval << "\n";

	//read the input file
    int number_of_lines = -1; //-1, as to not count the header of the file!
    ifstream open_countlines(path);
    string line;

    while (getline(open_countlines, line))
        ++number_of_lines;
    cout << "Number of processes: " << number_of_lines << "\n";

	//save all of the processes to an array the same length as the number of lines
	Process processes[number_of_lines];
	
	cout << "Path: " << path << '\n';
	ifstream infile(path);
	int first = 1;
	int counter = 0;
	while (getline(infile, line)) {
		
		if (first == 1) {
			first = 0;
			continue;
		}
		std::istringstream iss(line);

		int pid;
		int burst;
		int arrival;
		int priority;
		int ageindex;
		
		iss >> pid >> burst >> arrival >> priority >> ageindex;
		
		if (pid > -1 && burst > -1 && arrival > -1 && priority > -1 && ageindex > -1) {
			//This should instead add the processes to the priority queue! This is for early implementation only AAAAA
			Process newProcess(pid, burst, arrival, priority, ageindex);
						
			processes[counter] = newProcess;
			counter++;

			//cout << ' ' << pid << ' ' << burst << ' ' << arrival << ' ' << priority << ' ' << ageindex << '\n';			
		} else {
			//cout << "Process " << pid << " contained impossible values.\n";
		}
		
	}
	
	//utilize quicksort on the final array of processes to get a sorted array
	quickSort(processes, 0, counter - 1);

/* testing loop for the processes, to see after they are sorted
	for (int i = 0; i < counter; i++) {
		cout << "pid:" << processes[i].pid << "\n";
		cout << "arrival:" << processes[i].arrival << "\n\n";
	}
*/

	//Prepare for clock ticks
	int num_scheduled = 0;
	int wait_times[counter];
	int turn_times[counter];
	//initialize priority queue
	vector<queue<Process>> priority_queues;
	for (int i = 0; i < 100; i++) {
		priority_queues.push_back(queue<Process>());
	}

	cout << "Size of priority queue vector:" << priority_queues.size() << "\n";
	
	//Clock tick main loop
	int tick = 0;
	int tick_limit = 100000;
	int current_index = 0;
	for (tick = 0; tick < tick_limit; tick++) {

		//Queue for any process which needs an update in the current cycle (arrival, promotion, demotion)
		queue<Process> updates;

		bool event = false;
		//Store new processes for the priority queue
		//check for arrivals
		while (tick == processes[current_index].arrival) {
			cout << "pid:" << processes[current_index].pid << "\n";
			cout << "arrival:" << processes[current_index].arrival << "\n";
			current_index++;
			event = true;

			//add the item to the correct queue
			updates.push(processes[current_index]);
		}

		//store demoted processes
		//handle aging interval
		//handle promotion if aging interval has passed

		//handle all of the updates
		//use the queue<Process> updates, make sure that two processes with the same resulting priority are compared not just one after another
		//every element in updates is either promoted, demoted, or otherwise needs to be fixed
		// we might need to use a list instead of a queue, since the aging interval needs to impact every process
		// to push to priority queue:			priority_queues[ PRIORITY_VALUE_TO_PUSH_TO ].push( PROCESS_TO_PUSH );
		
		if (event) {
			cout << "     :\n     :\n";
		}
		
		//if (priority queue is empty) {//done!
		//	break;
		//}
	}

	//Calculate statistics
	int avg_wait_time = 0;
	int avg_turn_time = 0;
	for (int i = 0; i < current_index; i++) {
		avg_wait_time += wait_times[i];
		avg_turn_time += turn_times[i];
	}
	avg_wait_time = avg_wait_time / current_index+1;
	avg_turn_time = avg_turn_time / current_index+1;
	
	cout << '\n';
	cout << "Number of processes scheduled: " << current_index+1 << '\n';
	cout << "Average waiting time: " << avg_wait_time << '\n';
	cout << "Average turnaround time: " << avg_turn_time << '\n';
}

