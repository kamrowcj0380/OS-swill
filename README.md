# OS-swill - Windows Scheduler in C++


Important notes:
I didn't know how to use NULL in C++, so if any process has a pid of -1, it is a 'null' process.

Current Issues:
Nothing, just needs to be finished. Current program compiles with g++ -o program swill.exe

Tasks:
Finish the clock cycle loop
  Idea for this:
    use a class for 'current process', which includes the time to burst. Otherwise, add time to age and time to burst to 'Process' class

  In each loop:
    run a task (decrease ticks left for process)
      remove from queue if done
      demote if burst is up
    promote tasks (aging, if interval is up - check every element)
      this includes changing practically every queue, and we might want to change from queues to lists to be able to do this better
    get new process if the old one is done/demoted
 
 
 
