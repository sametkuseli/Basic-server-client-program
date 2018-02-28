/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: samet
 *
 * Created on 11 Kasım 2017 Cumartesi, 21:16
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */

struct stack_ch
{
    int top;
    int capacity ;
    char *stack;
    
    
};

struct queue_ch
{
    int front, rear, size;
    int capacity;
    char* array;
};

struct machine
{
    int machine_no;
    int log_size;
    struct stack_ch* stack;
    struct queue_ch* queue;
    char* log;
    
};

struct system
{
    
    int nof_client;
    struct machine* server;
    struct machine** clients;

};



struct stack_ch* stack_ch_init(int max_size){

    struct stack_ch* stackptr = (struct stack_ch*)malloc(sizeof(struct stack_ch));
    stackptr->capacity=max_size;
    stackptr->top=-1;
    stackptr->stack=(char*)malloc(stackptr->capacity * sizeof(char));
    return stackptr;
}

struct queue_ch* queue_ch_init(int capacity)
{
    struct queue_ch* queue = (struct queue_ch*) malloc(sizeof(struct queue_ch));
    queue->capacity = capacity;
    queue->front = queue->size = 0; 
    queue->rear = capacity - 1;  
    queue->array = (char*) malloc(queue->capacity * sizeof(char));

    return queue;
}

struct system* system_init(int client_size){

    struct system* system_ptr = (struct system*)malloc(sizeof(struct system));
    system_ptr->nof_client = client_size;
    system_ptr->clients = (struct machine**)malloc(system_ptr->nof_client * sizeof(struct machine*));
    return system_ptr;

}

struct machine* machine_init(int queue_size,int stack_size,int machine_nbr){

    struct machine* machinep = (struct machine*)malloc(sizeof(struct machine)) ;
    
    machinep->machine_no=machine_nbr+1;
    machinep->log_size=1;
    machinep->log=(char*)malloc(sizeof(char)*machinep->log_size);
    machinep->log[machinep->log_size-1]='\0';
    machinep->queue = queue_ch_init(queue_size);
    machinep->stack = stack_ch_init(stack_size);
    return machinep;
}

/*Char stack func*/

int stack_ch_is_full(struct stack_ch *stack);
int stack_ch_is_empty(struct stack_ch *stack);
void push_ch(struct stack_ch *stack, char data);
char pop_ch(struct stack_ch *stack);

/*Char queue*/

int queue_ch_is_full(struct queue_ch* queue);
int queue_ch_is_empty(struct queue_ch* queue);
void insert_queue_ch(struct queue_ch* queue, char item);
int remove_queue_ch(struct queue_ch* queue);
char* print_queue_ch(struct queue_ch* queue);

/*read file func*/

struct system* read_init_input(char* file_name);
struct system* read_command_input(char* file_name,struct system* system);
struct system* run_command_input(char* line,struct system* system);

/*Command func*/

void process_request(struct system* system,int client_number,char process_char_name);
void interrupt_request(struct system* system,int machine_number,char interrupt_name);
void send_server(struct system* system, int client_number);
void operate_command(struct system* system);

/*History log func*/
struct machine* add_log(struct machine* machine,char data);
void print_log_history(char* file_name,struct system* system);

int main(int argc, char** argv) {

    
    
   struct system* system= read_init_input(argv[1]);
    
  
   system=read_command_input(argv[2],system);
    
   print_log_history(argv[3],system);
   
   
    
    return (EXIT_SUCCESS);
}

/*Char stack func*/
int stack_ch_is_full(struct stack_ch* stack){

    if(stack->top==stack->capacity-1){
        return 1;
    }else return 0;
    

}

int stack_ch_is_empty(struct stack_ch* stack){

    if(stack->top == -1)return 1;
    else return 0;
}

void push_ch(struct stack_ch *stack, char data){

    if(stack_ch_is_full(stack)==0){
        
        
        stack->top++;
        stack->stack[stack->top]=data;
        
    }else printf("push operation failled! Stack is full\n");


}

char pop_ch(struct stack_ch *stack){
    
    char data;
    if(stack_ch_is_empty(stack)==0){
        data= stack->stack[stack->top];
        stack->top--;
        
        return data;
    
    }else printf("pop operation failled! Stack is empty\n");

}

/*Char queue*/
int queue_ch_is_full(struct queue_ch* queue){

    if(queue->size==queue->capacity){
       
        return 1;
    }else return 0;

}

int queue_ch_is_empty(struct queue_ch* queue){

       if(queue->size==0){
        return 1;
    }else return 0;


}

void insert_queue_ch(struct queue_ch* queue, char item){
    

     if (queue_ch_is_full(queue))
          
        return;
    
     
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
   
    
}

int remove_queue_ch(struct queue_ch* queue){

if (queue_ch_is_empty(queue))
        return -1;
    char item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;

}

char* print_queue_ch(struct queue_ch* queue){

    int i=0;
    
    char* prtqueue = malloc((queue->size)*sizeof(char));
    
    for(i=0; i<queue->size; i++){
    
        prtqueue[i] = queue->array[(queue->front)+i];
    
    }

    prtqueue[i]='\0';
    return prtqueue;
}

/*read file func*/

struct system* read_init_input(char* file_name){

    FILE *fptr;
    char buff[10];
    int client_size=0,i=0;
    struct system* system;
    
    fptr = fopen(file_name,"r");
    
    
    if(fptr == NULL) {
      perror("Error in opening file");
      return system;
   }
    
    
    fgets(buff, 10, fptr);
    
   
    
    client_size = atoi(&buff[0])-1;
    
    printf("client size: %d\n",client_size);
    
    system = system_init(client_size);
    
    
    for(i=0; i<client_size;i++){
        
        
        fgets(buff, 10, fptr);
        
        system->clients[i]=machine_init(atoi(&buff[0]),atoi(&buff[2]),i);
        
        printf("machine %d queue size: %d stack size: %d\n",system->clients[i]->machine_no,system->clients[i]->queue->capacity,system->clients[i]->stack->capacity);
        
    }
    
    
    
    fgets(buff, 10, fptr);
    
    system->server= machine_init(atoi(&buff[0]),atoi(&buff[2]),i);
    
    printf("machine %d queue size: %d stack size: %d\n",system->server->machine_no,system->server->queue->capacity,system->server->stack->capacity);

    return system;
    
}


/*Command func*/

void process_request(struct system* system,int client_number,char process_char_name){

    int client_index=client_number-1;
   
    if(queue_ch_is_full(system->clients[client_index]->queue)){
    
        system->clients[client_index]=add_log(system->clients[client_index],'1');
    }else{
    
    insert_queue_ch(system->clients[client_index]->queue,process_char_name);
    }
    
    
    

}

void interrupt_request(struct system* system,int machine_number,char interrupt_name){

    int client_index=machine_number-1;
    if(machine_number<=system->nof_client){
    
        if(stack_ch_is_full(system->clients[client_index]->stack)){
        
            system->clients[client_index]=add_log(system->clients[client_index],'2');
        }else{
        
            push_ch(system->clients[client_index]->stack,interrupt_name);
        }
    
    
    }else if(machine_number==system->nof_client+1){
    
        if(stack_ch_is_full(system->server->stack)){
        
        system->server=add_log(system->server,'2');
            
        }else{
        
         push_ch(system->server->stack,interrupt_name);
        }
        
       
    }
    
    
}

void send_server(struct system* system, int client_number){

    char tmp;
    int client_index=client_number-1;
    
    if(stack_ch_is_empty(system->clients[client_index]->stack)){
    
        if(queue_ch_is_empty(system->clients[client_index]->queue)){
        
        
        /*record log 3*/
            system->clients[client_index]=add_log(system->clients[client_index],'3');
            return;
        }else{
        
            tmp=remove_queue_ch(system->clients[client_index]->queue);
            system->clients[client_index]=add_log(system->clients[client_index],tmp);
        
        }
    
        
    }else{
    
    
        tmp = pop_ch(system->clients[client_index]->stack);
        system->clients[client_index]=add_log(system->clients[client_index],tmp);
    }
    
    if(queue_ch_is_full(system->server->queue)){
    
    system->server=add_log(system->server,'1');
    
    }else{
    
        insert_queue_ch(system->server->queue,tmp);
    
    }
    
    

}

void operate_command(struct system* system){

    char tmp;
    if(stack_ch_is_empty(system->server->stack)){
    
        
        if(queue_ch_is_empty(system->server->queue)){
        
        
        /*log 3*/
            system->server=add_log(system->server,'3');
        }else{
        
            tmp=remove_queue_ch(system->server->queue);
            system->server=add_log(system->server,tmp);
        }
        
    
    }else{
    
       
        tmp=pop_ch(system->server->stack);
        system->server=add_log(system->server,tmp);
    
    }


}


struct system* read_command_input(char* file_name,struct system* system){

    FILE *fptr;
    char line[20];
    int line_length=0,i=0;
    
   fptr = fopen(file_name,"r");


       if(fptr == NULL) {
      perror("Error in opening file");
      return system;
   }
   
   fgets(line,20,fptr);
   line_length= atoi(line);
    
   for(i=0; i<line_length; i++){
   
       fgets(line,20,fptr);
       system=run_command_input(line, system);
       
       
   
   }
   
   printf("end of program!\n");
   return system;
}

struct system* run_command_input(char* line,struct system* system){

    int machine_number=-1;
    
        machine_number = atoi(&line[2]);
        
        
    if(line[0]=='A'){
    
        
        
        process_request(system,machine_number,line[4]);
    
    
    
    }else if(line[0]=='I'){
    
        interrupt_request(system,machine_number,line[4]);
        
    
    }else if(line[0]=='S'){
    
        send_server(system, machine_number);
    
    }else if(line[0]=='O'){
    
        operate_command(system);
    
    }else{
    
    
    
    }

        return system;


}


struct machine* add_log(struct machine* machine,char data){

    machine->log=(char*)realloc(machine->log,sizeof(char)*(machine->log_size+1));
    machine->log[machine->log_size-1]=data;

    machine->log_size++;
    machine->log[machine->log_size-1]='\0';
    printf("machine no: %d reccorded log: %c\n",machine->machine_no,data);
    return machine;

}

void print_log_history(char* file_name,struct system* system){

    FILE *fptr;
    int i=0,j=0;
    
    fptr = fopen(file_name,"w");
    
    for(i=0; i<system->nof_client; i++){
        
        
        for(j=0; j<system->clients[i]->log_size; j++){
        
           
            fputc(system->clients[i]->log[j],fptr);
            fputc(' ',fptr);
        }
        fputc('\n',fptr);
    
    }

    for(i=0; i<system->server->log_size; i++){
    
        fputc(system->server->log[i],fptr);
        fputc(' ',fptr);
    
    }
    fputc('\n',fptr);
}
