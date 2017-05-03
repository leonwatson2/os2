/*
*	simulation.h
*	Author: Leon Watson
*	Defines all structures and functions needed for 
*	the simulation of SJF and SRT scheduling algorithms.
*   Includes main function to run SJF and SRT simulation.
*	 
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "simulation.h"

#define MAX_STR 20
#define START_SIZE 52
#define VOID -1
#define CURRENTJOB(s) s->allProcesses[s->currentRunningProcess]

int main(){
	Process **p = calloc(START_SIZE, sizeof(Process *));
	int numberOfProcesses = getProcessInformation(p);
	Schedular *sjf = malloc(sizeof(Schedular));
	Schedular *srt = malloc(sizeof(Schedular));
	initSchedular(sjf, p, numberOfProcesses);
	
	printTableHeader("SJF (Shortest Job First)");
	simulateCyclesSJF(sjf);
	calculateAvgTimes(sjf);
	printAvgTimes(sjf);

	reinitProcesses(p, numberOfProcesses);
	initSchedular(srt, p, numberOfProcesses);
	printTableHeader("SRT (Shortest Remaining Time)");
	simulateCyclesSRT(srt);
	calculateAvgTimes(srt);
	printAvgTimes(srt);
	
		
	free(p);

}//main

void initSchedular(Schedular *s, Process **p, int num){
	s->allProcesses = p;
	s->currentCycle = 0;
	s->currentRunningProcess = VOID;
	s->numberOfProcesses = num;
	s->numberOfContextSwitches = 0;
	s->numberOfFinishedProcesses = 0;
	s->numberOfUnfinisedProcesseses = 0;
}

/*
*	Simulates SJF(Shortest Job First)
*	for Schedular passed in. 
*/
void simulateCyclesSJF(Schedular *s){
	while(s->numberOfFinishedProcesses != s->numberOfProcesses){
		findArrivingProcesses(s);
		findAndRunShortestJob(s);
		incrementCycle(s);
		if(s->currentRunningProcess >= 0)
			runCurrentProcess(s);

	}
}
/*
*	Simulates SRT(Shortest Remaining Time)
*	for Schedular passed in. 
*	
*/
void simulateCyclesSRT(Schedular *s){
	while(s->numberOfFinishedProcesses != s->numberOfProcesses){
		findArrivingProcesses(s);
		findAndRunShortestTimeJob(s);
		incrementCycle(s);
		if(s->currentRunningProcess >= 0)
			runCurrentProcess(s);

	}
}

/*
*	Finds all arriving processes at the current cycle and 
*	sets their arrived flag.
*/
void findArrivingProcesses(Schedular *s){
	for(int i = 0; i < s->numberOfProcesses; i++){
		if(s->allProcesses[i]->arrivalTime == s->currentCycle){
			setArrived(s->allProcesses[i]);
		}
	}
}

/*
*	Finds the shortest job of the arrived processes 
*	that are not finished. Sets the shortest job ad
*	the current running process.
*/
int findAndRunShortestJob(Schedular *s){
	int shortestJob = VOID;
	if(s->currentRunningProcess == VOID || s->allProcesses[s->currentRunningProcess]->finished){

		for(int i = 0; i < s->numberOfProcesses; i++){
			if(s->allProcesses[i]->arrived && !s->allProcesses[i]->finished)
				
				if(shortestJob == VOID 
					|| 
					s->allProcesses[i]->runTime < s->allProcesses[shortestJob]->runTime){
					shortestJob = i;
				}
		}
		if(shortestJob != VOID){
			

			s->currentRunningProcess = shortestJob;
			s->allProcesses[shortestJob]->startTime = s->currentCycle;
			Process *shortestProc = s->allProcesses[shortestJob];
			setResponseTime(shortestProc);
			setStartTime(shortestProc, s->currentCycle);
		}

	} else {
		shortestJob = s->currentRunningProcess;
	}
	return shortestJob;
}

/*
*	Finds the shortest job of the arrived processes 
*	that are not finished. Sets the shortest job ad
*	the current running process.
*/
int findAndRunShortestTimeJob(Schedular *s){
	int shortestTimeJob = VOID;
	

		for(int i = 0; i < s->numberOfProcesses; i++){
			if(s->allProcesses[i]->arrived && !s->allProcesses[i]->finished)
				
				if(shortestTimeJob == VOID 
					|| 
					s->allProcesses[i]->runTime < s->allProcesses[shortestTimeJob]->runTime){
					shortestTimeJob = i;
				}
		}
		if(shortestTimeJob != s->currentRunningProcess){
			if(s->currentRunningProcess != VOID && !s->allProcesses[s->currentRunningProcess]->finished){
				setEndTime(s->allProcesses[s->currentRunningProcess], s->currentCycle);
				printProcess(s->allProcesses[s->currentRunningProcess], true);
				incrementContextSwitch(s);
			}
			s->currentRunningProcess = shortestTimeJob;
			s->allProcesses[shortestTimeJob]->startTime = s->currentCycle;
			Process *shortestProc = s->allProcesses[shortestTimeJob];
			setResponseTime(shortestProc);
			setStartTime(shortestProc, s->currentCycle);
			//increment context switch, if statement if not finished
		}
		
	
	return shortestTimeJob;
}

void runCurrentProcess(Schedular *s){
	int proc = s->currentRunningProcess;
	Process *p = s->allProcesses[proc];
	

	p->timeInCPU++;
	if(p->runTime <= p->timeInCPU){
		p->finished = true;
		setEndTime(p, s->currentCycle);
		printProcess(p, true);
		incrementFinishedProcesses(s);
	}

}

void incrementContextSwitch(Schedular *s){
	s->numberOfContextSwitches++;
}
void incrementFinishedProcesses(Schedular *s){
	s->numberOfFinishedProcesses++;
}

void incrementCycle(Schedular *s){
	s->currentCycle++;
	Process *p;
	for(int i = 0; i < s->numberOfProcesses; i++){
		p = s->allProcesses[i];
		if(!p->finished){

			if(p->arrived){
				p->turnAroundTime++;
				if(i != s->currentRunningProcess){
					p->waitTime++;
				}
			}
		}
	}
}

/*
*	Calculates the average wait time, turnaround time.
*/
void calculateAvgTimes(Schedular *s){
	Process **p = s->allProcesses;
	float totalWaitTime = 0;
	float totalTurnAround = 0;
	float totalRespTime = 0;
	for(int i = 0; i < s->numberOfProcesses; i++){
		totalWaitTime += p[i]->waitTime;
		totalTurnAround += p[i]->turnAroundTime;
		totalRespTime += p[i]->responseTime;
	}
	float avgWaitTime = totalWaitTime/s->numberOfProcesses;
	float avgTurnTime = totalTurnAround/s->numberOfProcesses;
	float avgRespTime = totalRespTime/s->numberOfProcesses;
	s->avgWaitTime = avgWaitTime;
	s->avgTurnTime = avgTurnTime;
	s->avgResponseTime = avgRespTime;
}



/*
*	Gets the process information in the format 
*	name arrivalTime runTime Priority
*	P0 	 0 			 350 	 0
*	P1 	 0 			 125 	 0
*	. . .
*	From file redirection.
*	Reallocates Process p pointer as needed.
*	Returns the number of processes.
*/
int getProcessInformation(Process **p){
	int currentSize = START_SIZE;
	int i = -1;
	
	do{
		i++;
		
		p[i] = malloc(sizeof(Process));
		initProcess(p[i]);
		
	}while(scanf("%s %d %d %d", 
				p[i]->name, 
				&p[i]->arrivalTime, 
				&p[i]->runTime, 
				&p[i]->priority) > 0);

	return i;
}
void reinitProcesses(Process **p, int numberOfProcesses){
	for(int i = 0; i < numberOfProcesses; i++){
		initProcess(p[i]);
	}
}

void initProcess(Process *p){
	p->waitTime = 0;
	p->turnAroundTime = 0;
	p->responseTime = VOID;
	p->timeInCPU = 0;
	p->startTime = 0;
	p->endTime = 0;
	p->arrived = p->finished = false;
}

void setArrived(Process *p){
	p->arrived = true;
}

void setResponseTime(Process *p){
	if(p->responseTime < 0){
		p->responseTime = p->waitTime;
	}
}

void setStartTime(Process *p, int sTime){
	p->startTime = sTime;
}

void setEndTime(Process *p, int eTime){
	p->endTime = eTime;
}


//Printing functions
void printTableHeader(char* name){
	printf("\n==>%s\n", name);
	printf("job \tariv \trunt \tprio \tstrt \tendt \tturn \twait \tresp\n");
	printf("--------------------------------------------------------------------\n"); 
}

void printAvgTimes(Schedular *s){
	printf("Average Wait Time: %.3f\n", s->avgWaitTime);	
	printf("Average Turnround Time: %.3f\n", s->avgTurnTime);
	printf("Average Response Time: %.3f\n", s->avgResponseTime);	
	printf("Number of Preemptions: %d\n", s->numberOfContextSwitches);	
}


void printProcesses(Process **p, int numberOfProcesses){
	printf("All Processes\n");
	printf("job \tariv \trunt \tprio \tstrt \tendt \tturn \twait \tresp\n");
	printf("--------------------------------------------------------------------\n"); 
	for (int i = 0; i < numberOfProcesses ; i++)
	{
		printProcess(p[i], true);
	}
}

void printProcess(Process *p, bool all){
		if(!all){
			printf("job \tariv \trunt \tprio \tstrt \tendt \tturn \twait \tresp\n");
		}
		printf("%s \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d\n", 
			p->name, 
			p->arrivalTime, 
			p->runTime, 
			p->priority, 
			p->startTime,
			p->endTime,
			p->turnAroundTime,
			p->waitTime,
			p->responseTime);
}
