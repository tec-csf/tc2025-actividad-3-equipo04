/*
Luis Revilla
Diego Moreno
CALENDARIZADOR
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

typedef struct process{
	int id;
	int runTime;
} process_t;

typedef struct node{
	process_t proc;
	struct node *next;
}node_t;

typedef struct cpu{
    int libre;
}cpu_t;

node_t * head=NULL;

int processCount;


//Function to add a new process to the queue
void enqueue(int id, int val){
	node_t *current=head;

	if(processCount==0){//check if we have 0 process waiting
		head->proc.id=id;//just give the value to the first element in the queue
		head->proc.runTime=val;
	}else{//We have to add a new node
		//iterate until we find the end of the queue
		while(current->next!=NULL){
			current=current->next;
		}
		//Add to the end of the queue a new process
		current->next=malloc(sizeof(node_t));
		current->next->proc.runTime=val;
		current->next->proc.id=id;
		current->next->next=NULL;
	}
	processCount++;
}


//function that removes the first element of the queue
process_t dequeue(){
	process_t returningValue;
	if(processCount==1){
		returningValue=head->proc;
	}else{

		node_t *nextProc=head->next;
		returningValue=head->proc;
		free(head);
		head=nextProc;
	}
	processCount--;
	return returningValue;
}
void createProcess(int runtime, int numproc, int id){
    for(int i = 0; i<numproc; i++){
        enqueue(id,runtime);
    }
}
void procesar(node_t *procesadores[], int cpus){

}

int main(int argc, char * argv[]) {
    head = malloc(sizeof(node_t));
	head->next = NULL;
	processCount = 0;
    int cpus, nump, time, tipo;
    node_t *current = head;

    printf("\nIngrese el número de procesadores: \n");
    scanf("%d",&cpus);
    cpu_t * cpu;

    if(cpus>4){
        cpus=4;
    }
    else{
        if(cpus<1){
            cpus=1;
        }
    }
    cpu = malloc(sizeof(cpu)*cpus);
    cpu_t * auxcpu = cpu;
    for(int i = 0; i<cpus; i++){
        (auxcpu+i)->libre=1;
        printf("Cpu esta %d\n",auxcpu->libre);
    }
    printf("¿Cuantos grupos de procesos necesitas?\n");
    scanf("%d",&tipo);
    int numproc [tipo];
    for(int i = 0; i < tipo; i++){
        printf("¿Cuantos procesos del tipo %d necesitas?\n",i);
        scanf("%d",&numproc[i]);
    }
    for(int i = 0; i<tipo; i++){
        printf("¿Que tiempo de ejecución tendran los procesos tipo? %d \n",i);
        scanf("%d",&time);
        createProcess(time, numproc[i], i); //max 4 cpus  
    }
    current = head;
	while (current->next != NULL)
	{
        if((cpu+0)->libre){
            (cpu+0)->libre=0;
            printf("Running process type: %d in CPU: %d\n",current->proc.id,1);
            sleep(current->proc.runTime);
            (cpu+0)->libre=1;
            current = current->next;
            dequeue();
        } 
        if((cpu+1)->libre && cpus>1){
            (cpu+1)->libre=0;
            printf("Running process type: %d in CPU: %d\n",current->proc.id,2);
            sleep(current->proc.runTime);
            (cpu+1)->libre=1;
            current = current->next;
            dequeue();
        }
        if((cpu+2)->libre && cpus>2){
            (cpu+2)->libre=0;
            printf("Running process type: %d in CPU: %d\n",current->proc.id,3);
            sleep(current->proc.runTime);
            (cpu+2)->libre=1;
            current = current->next;
            dequeue();
        }
        if((cpu+3)->libre && cpus>3){
            (cpu+3)->libre=0;
            printf("Running process type: %d in CPU: %d\n",current->proc.id,4);
            sleep(current->proc.runTime);
            (cpu+3)->libre=1;
            current = current->next;
            dequeue();
        }
    }
    //Liberar memoria
    free(cpu);

    return 0;
    /*
    tenemos 2 cpus
    2 tipos de proceso
    tipo 1 tiene 10 procesos de 2s
    tipo 2 tiene 5 procesos de 4s
    

    */
}