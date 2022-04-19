/*
 * Swill is a simulated windows scheduler.
 *
 * IMPORTANT NOTES:
 *		A process with pid == -1 is an empty process, or doesn't exist. If the pid is -1, there isn't any process there.
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
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
		
		int wait_time = 0;
		int tick_in = 0;
		int age = 0;

		//comparison function - this to be used with C++ sorting library found in -> #include <algorithm>
		bool operator < (const Process& p) const {
			return (arrival < p.arrival);
		}

		//constructors
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
	
	//cout << "TQ: " << time_quantum << "\n";
	//cout << "AI: " << aging_interval << "\n";

	//read the input file
    int number_of_lines = -1; //-1, as to not count the header of the file!
    ifstream open_countlines(path);
    string line;

    while (getline(open_countlines, line)) {
		int pid, burst, arrival, priority, ageindex;
		istringstream iss(line);
		
		iss >> pid >> burst >> arrival >> priority >> ageindex;
		if (pid > -1 && burst > -1 && arrival > -1 && priority > -1 && ageindex > -1) {
			++number_of_lines;
		}
	}
    //cout << "Number of processes: " << number_of_lines << "\n";

	//save all of the processes to an array the same length as the number of lines
	//Process *processes;
    //processes = malloc(number_of_lines * sizeof(Process));
	
	vector<Process> processes(number_of_lines);
	
	//cout << "Path: " << path << '\n';
	ifstream infile(path);
	int first = 1;
	int num_processes = 0;
	while (getline(infile, line)) {
		
		if (first == 1) {
			first = 0;
			continue;
		}
		istringstream iss(line);

		int pid;
		int burst;
		int arrival;
		int priority;
		int ageindex;
		
		iss >> pid >> burst >> arrival >> priority >> ageindex;
		
		if (pid > -1 && burst > -1 && arrival > -1 && priority > -1 && ageindex > -1) {
			//This should instead add the processes to the priority queue! This is for early implementation only AAAAA
			Process newProcess(pid, burst, arrival, priority, ageindex);
						
			processes[num_processes] = newProcess;
			num_processes++;

			//cout << ' ' << pid << ' ' << burst << ' ' << arrival << ' ' << priority << ' ' << ageindex << '\n';			
		} else {
			//cout << "Process " << pid << " contained impossible values.\n";
		}
		
	}
	
	//printf("Number of processes to run: %d\n", num_processes);
	
	//utilize a sort on the final array of processes to get a sorted array
	sort(processes.begin(), processes.end());
	
	queue<Process> sorted_process_queue;
	
	for (int i = 0; i < num_processes; i++) {
		sorted_process_queue.push(processes[i]);
	}
	
	//delete &processes;

	
/* testing loop for the processes, to see after they are sorted
	for (int i = 0; i < num_processes; i++) {
		cout << "pid:" << processes[i].pid << "\n";
		cout << "arrival:" << processes[i].arrival << "\n\n";
	}
*/

	//Prepare for clock ticks
	double num_scheduled = 0;
	double total_wait_time = 0;
	double total_turn_time = 0;
	//initialize priority queue
	vector<deque<Process>> priority_queues;
	for (int i = 0; i < 100; i++) {
		priority_queues.push_back(deque<Process>());
	}

	//cout << "Size of priority queue vector:" << priority_queues.size() << "\n";
	
	//Clock tick main loop
	double tick = 0;
	double tick_limit = 10000000000000;
	double current_index = 0;
	double aging = 0;
	double timer = 0; //for time quantum
	
	Process current_process(-1);
	
	for (tick = 0; tick < tick_limit; tick++) {

		bool event = false;
		bool new_process = false;
		//Store new processes for the priority queue
		//check for arrivals
		while (tick == sorted_process_queue.front().arrival) {
			//cout << "pid:" << processes[current_index].pid << "\n";
			//cout << "arrival:" << processes[current_index].arrival << "\n";
			sorted_process_queue.front().tick_in = tick;
			sorted_process_queue.front().wait_time = tick+sorted_process_queue.front().burst;
			priority_queues[sorted_process_queue.front().priority].push_back(sorted_process_queue.front());
			sorted_process_queue.pop();
			current_index++;
			//event = true;
		}

		//handle all of the updates
		//use the queue<Process> updates, make sure that two processes with the same resulting priority are compared not just one after another
		//every element in updates is either promoted, demoted, or otherwise needs to be fixed
		// we might need to use a list instead of a queue, since the aging interval needs to impact every process
		// to push to priority queue:			priority_queues[ PRIORITY_VALUE_TO_PUSH_TO ].push_back( PROCESS_TO_PUSH );
		
		//This is a half solution, more needs to be added. This ignores demotion times, just literally runs top to bottom until done.
		if (current_process.pid > -1) {//pid -1 is known as 'empty', so make sure the current process exists
			current_process.burst -= 1;
			if (current_process.burst == 0) {
				cout << "Clock tick: " << tick << '\n';
				cout << " Pid: " << current_process.pid << " terminates\n"; 
				
				//cout << " EEEEE: " << tick - current_process.wait_time << " wait time! \n"; 
				//cout << " EEEEE: " << tick - current_process.tick_in << " turn time! \n"; 

				total_wait_time += tick - current_process.wait_time;
				total_turn_time += tick - current_process.tick_in;
				//cout << "Tick time:" << tick - current_process.wait_time <<'\n';
				event = true;
				current_process.pid = -1;
			}
		}
		
		//seems redundant, but handles the logic of the previous if the process finished running
		if (current_process.pid == -1) {
			// a new process needs to be called
			for (int i = 99; i > -1; i--) {
				if (priority_queues[i].empty() == false) {
					current_process = priority_queues[i][0];
					priority_queues[i].pop_front();
					
					//printf("The new current process id: %d\n", current_process.pid);
					//cout << ' ' << current_process.pid << ' ' << current_process.burst << ' ' << current_process.arrival << ' ' << current_process.priority << ' ' << current_process.ageindex << '\n';
					if (event == false) {
						cout << "Clock tick: " << tick << '\n';
					}
					cout << " Pid: " << current_process.pid << " runs\n";
					new_process = true;

					//reset timer
					timer = -1; //this will become zero soon enough...
					event = true;
					break;
				}
			}
			if (current_process.pid == -1 && current_index == num_processes) {//done!
				cout << "     :\n     :\n";
				break;
			}
		}
		
		//handle demoted processes
		if (current_process.pid != -1) {
			timer++;
			if (timer >= time_quantum && new_process == false) {
				timer = 0;
				//demote current process
				if (current_process.priority > 0) {
					current_process.priority = current_process.priority - 1;
				}
				priority_queues[current_process.priority].push_back(current_process);

				cout << "Clock tick: " << tick << '\n';
				
				//cout << "Time Quantum expired for process ID " << current_process.pid << '\n';
				cout << " Pid: " << current_process.pid << " TQ expired demoted to queue of priority " << current_process.priority << '\n';
				//get new process
				for (int i = 99; i > -1; i--) {
					if (priority_queues[i].empty() == false) {
						current_process = priority_queues[i].front();
						priority_queues[i].pop_front();
						
						//printf("New current process id: %d\n", current_process.pid);
						//cout << ' ' << current_process.pid << ' ' << current_process.burst << ' ' << current_process.arrival << ' ' << current_process.priority << ' ' << current_process.ageindex << '\n';
						cout << " Pid: " << current_process.pid << " runs\n";
						
						//TIMER IS ALREADY RESET...
						event = true;
						break;
					}
				}
			}
		}

		/*
		 * Tick Math section
		 * anything that changes each tick for a process - time since loaded, age, etc. is updated here
		 */

		//handle aging interval
		//handle promotion if aging interval has passed
		/*
		for (int i = 0; i < 100; i++) {
			if (priority_queues[i].empty() == false) {
				for (int cur_q; cur_q < priority_queues[i].size(); cur_q++) {
					priority_queues[i][cur_q].age += 1;
					if (priority_queues[i][cur_q].age >= aging_interval) {
						priority_queues[i][cur_q].age = 0;
						if (priority_queues[i][cur_q].priority < 99) {
							current_process.priority = current_process.priority + 1;
							//implement aging
							
						} //if false,
						//can't get any higher...
					}

					

					priority_queues[current_process.priority].push_back(current_process);
					current_process = priority_queues[i].front();
					priority_queues[i].pop_front();
					
					printf("New current process id: %d\n", current_process.pid);
					cout << ' ' << current_process.pid << ' ' << current_process.burst << ' ' << current_process.arrival << ' ' << current_process.priority << ' ' << current_process.ageindex << '\n';
					
					current_process.wait_time = tick;

				}
			}
		}*/
		
		//end of Tick Math section

		if (event) {
			cout << "     :\n     :\n";
		}
		
		//cout << tick << "\n";
		
	}
	
	//Calculate statistics
	float avg_wait_time = total_wait_time*1.0 / (current_index);
	float avg_turn_time = total_turn_time*1.0 / (current_index);
	
	cout << '\n';
	cout << "Number of processes scheduled: " << current_index << '\n';
	cout << "Average waiting time: " << avg_wait_time << '\n';
	cout << "Average turnaround time: " << avg_turn_time << '\n';
}

