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
int count, count_m, count_i, count_d = 0;
int m_member, m_insert, m_delete;
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
    struct Node** head_pp = &head_p; //////////// diff
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
        return 1;
    }
    else{
        return 0; //value already in the list
    }
}


//delete function
int Delete(int value){
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
        return 1;
    }else{
        return 0; //value not in the list
    }
}

int main(int argc, char* argv[]){
    clock_t start, end;
    double cpu_time_used;
    srand (time(NULL));
    int i, ins_value;
    
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
    start = clock();
    for (i = 0; i < m; i++){
        opr_values[i] = rand() % 65535; //value should be between 2^16 - 1
    }
    while(count < m){
        //only one thread can get access to the linked-list at a time
        if(count < m_member){
            Member(opr_values[count]);
        }
        else if(count < m_insert + m_member){
            Insert(opr_values[count]);; 
        }
        else{
            Delete(opr_values[count]);
        }
        count++;
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("execution time is : %f\n", cpu_time_used);
    PrintList();
    return 0;
}
