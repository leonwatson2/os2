/*
*	simulation.h
*	Author: Leon Watson
*	Declares all structures and functions needed for 
*	the simulation of SJF and SRT scheduling algorithms. 
*/

#define ARRIVED(x,y) x <= y

typedef struct process{
	char name[20];
	int arrivalTime;
	int runTime;
	int priority;
	int timeInCPU; 
	int startTime;
	int endTime;
	int turnAroundTime;
	int responseTime;
	int waitTime;

	bool arrived;
	bool finished;

} Process;

typedef struct Schedular
{
	Process **allProcesses;
	float avgTurnTime;
	float avgWaitTime;
	float avgResponseTime;

	int currentCycle;
	int currentRunningProcess;
	int numberOfContextSwitches;
	int numberOfFinishedProcesses;
	int numberOfProcesses;
	int numberOfUnfinisedProcesseses;
} Schedular;

int getProcessInformation(Process **p);
void printProcesses(Process **s, int numberOfProcesses);
void printTableHeader(char* name);
void reinitProcesses(Process **p, int  numberOfProcesses);

//Scheduler Methods
int findAndRunShortestJob(Schedular *s);
int findAndRunShortestTimeJob(Schedular *s);

void calculateAvgTimes(Schedular *s);
void findArrivingProcesses(Schedular *s);

void incrementCycle(Schedular *s);
void incrementFinishedProcesses(Schedular *s);
void incrementContextSwitch(Schedular *s);

void initSchedular(Schedular *s, Process **p, int num);
void runCurrentProcess(Schedular *s);
void simulateCyclesSJF(Schedular *s);
void simulateCyclesSRT(Schedular *s);
void printAvgTimes(Schedular *s);

//Process Methods
void initProcess(Process *p);
void printProcess(Process *p, bool all);
void setArrived(Process *p);
void setStartTime(Process *p, int sTime);
void setEndTime(Process *p, int eTime);
void setResponseTime(Process *p);