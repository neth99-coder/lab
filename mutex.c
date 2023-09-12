#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
struct Node{
    int data;
    struct Node* next;
};


int n;                 //number of values 
int m;                 //number of operations in each execution
float m_member_frac;   //member fraction
float m_insert_frac;   //insert fraction
float m_delete_frac;   //delete fraction
pthread_mutex_t mutex, mutex1;
int count, count_m, count_i, count_d = 0;
int m_member, m_insert, m_delete;
int thread_count;   //number of threads
struct Node* head_p = NULL;
int* opr_values;

void PrintList() {
    struct Node* curr = head_p;
    int i = 0;
    while (curr != NULL) {
        printf("%d -> ", curr->data);
        curr = curr->next;
        i++;
    }
    printf("NULL\n");
    printf("length %d\n", i);
}

//Member function
int Member(int value){
    printf("Calling Member function with value: %d\n", value);
    count_m++;
    struct Node* curr_p = head_p;
    while(curr_p != NULL && curr_p->data < value){
        curr_p = curr_p->next;
    }
    if(curr_p == NULL || curr_p->data > value){
        return 0;
    }
    else{
        return 1;
    }
}


//insert function
int Insert(int value){
    printf("Calling Insert function with value: %d\n", value);
    count_i++;
    struct Node** head_pp = &head_p; 
    struct Node* curr_p = *head_pp;
    struct Node* pred_p = NULL;
    struct Node* temp_p;
    while(curr_p != NULL && curr_p->data < value){
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
    if(curr_p == NULL || curr_p->data > value){
        temp_p = malloc(sizeof(struct Node));
        temp_p->data = value;
        temp_p->next = curr_p;
        if(pred_p == NULL) //new first node
            *head_pp = temp_p;
        else
            pred_p->next = temp_p; 
        PrintList();
        return 1;
    }
    else{
        PrintList();
        return 0; //value already in the list
    }
}


//delete function
int Delete(int value){
    printf("Calling Delete function with value: %d\n", value);
    count_d++;
    struct Node** head_pp = &head_p;
    struct Node* curr_p = *head_pp;
    struct Node* pred_p = NULL;
    while(curr_p != NULL && curr_p->data < value){
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
    if(curr_p != NULL && curr_p->data == value){
        if(pred_p == NULL){
            *head_pp = curr_p->next;  //deleting the first node in the list
            free(curr_p);
        }
        else{
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        PrintList();
        return 1;
    }else{
        PrintList();
        return 0; //value not in the list
    }
}

void *thread_function(void* rank){ 
 
    //int *opr_values = (int *) arg;
    //long my_rank = (long)rank;
    pthread_mutex_lock( &mutex1 ); 
    int i = count++;
    pthread_mutex_unlock( &mutex1 ); 
    while(i < m){
        //only one thread can get access to the linked-list at a time
        if(count_m < m_member){
            pthread_mutex_lock( &mutex );    
            Member(opr_values[i]);
            pthread_mutex_unlock( &mutex ); 
        }
        else if(count_i < m_insert+ m_member){
            pthread_mutex_lock( &mutex );    
            Insert(opr_values[i]);
            pthread_mutex_unlock( &mutex ); 
        }
        else{
            pthread_mutex_lock( &mutex );    
            Delete(opr_values[i]);
            pthread_mutex_unlock( &mutex );
        }
        pthread_mutex_lock( &mutex1 ); 
        i = count++;
        pthread_mutex_unlock( &mutex1 ); 
        PrintList();
    }
    return NULL;
}


int main(int argc, char* argv[]){
    long thread;
    pthread_t* thread_handles;
    clock_t start, end;
    double cpu_time_used;
    srand (time(NULL));
    int i, ins_value;
    thread_count = 10;
    
    n = 10;
    m = 50;
    m_member_frac = 0.4;
    m_insert_frac = 0.3;
    m_delete_frac = 0.3;
    opr_values = (int *)malloc(m*sizeof(int));
    m_member = m * m_member_frac;
    m_insert = m * m_insert_frac;
    m_delete = m - (m_member + m_insert);
    for (i = 0; i < n; i++){
        ins_value = rand() % 65535; //value should be between 2^16 - 1
        Insert(ins_value);
    }
    for (i = 0; i < m; i++){
        opr_values[i] = rand() % 65535; //value should be between 2^16 - 1
    }
    thread_handles = (pthread_t *)malloc(thread_count*sizeof(pthread_t));
    start = clock();
    pthread_mutex_init(&mutex, NULL);
    for (thread = 0; thread < thread_count; thread++){
        pthread_create(&thread_handles[thread], NULL, thread_function, (void*) thread);
    }
    for (thread = 0; thread < thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("execution time is : %f\n", cpu_time_used);
    PrintList();
    free(thread_handles);
    return 0;
}
