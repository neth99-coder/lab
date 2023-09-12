#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Node {
    int data;
    struct Node* next;
};

int Member(struct Node* head, int value) {
    //printf("value: %d -> ", value);
    struct Node* current = head;
    while (current != NULL && current->data < value) {
        current = current->next;      
    }
    if (current == NULL || current->data > value) {
        return 0; // Value not found
    }else{
        return 1;
    }
}

int Insert(struct Node** head, int value) {
    struct Node* curr = *head;
    struct Node* pred = NULL;
    struct Node* temp ;
    
    while (curr != NULL && curr->data < value){
        pred = curr ;
        curr = curr->next;
    }
    
    if(curr == NULL || curr->data > value){
        temp = (struct Node*)malloc(sizeof(struct Node));
        temp->data = value;
        temp->next = curr;
        if (pred == NULL) {
            *head = temp;
        }else{
            pred->next = temp ;
        }
        return 1;
    }else{
        return 0;
    }


}

int Delete(struct Node** head, int value) {
    struct Node* curr = *head;
    struct Node* pred = NULL;

    while (curr != NULL && curr->data < value) {
        pred = curr;
        curr = curr->next;
    }
    if(curr != NULL && curr->data == value){
        if (pred == NULL) {
            *head = curr->next;
            free(curr);
        } else {
            pred->next = curr->next;
            free(curr);
        }
        return 1;
    }else{
        return 0;
    }
   
}

void PrintList(struct Node* head) {
    struct Node* curr = head;
    while (curr != NULL) {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}

int performMember(struct Node* head, int mMember, int cMember){
    if(cMember < mMember){
        // Perform Member operation
        int value = rand() % ((1 << 16) - 1);
        Member(head, value);
        return ++cMember;
    }else{
        return cMember;
    }
}

int performInsert(struct Node* head, int mInsert, int cInsert){
    if(cInsert < mInsert){
            // Perform Insert operation
            int value;
            do {
                value = rand() % ((1 << 16) - 1);
            } while (Member(head, value));
            Insert(&head, value);
        return ++cInsert;
    }else{
        return cInsert;
    }
}

int performDelete(struct Node* head, int mDelete, int cDelete){
    if(cDelete < mDelete){
            // Perform Delete operation
            int value = rand() % ((1 << 16) - 1);
            Delete(&head, value);
        return ++cDelete;
    }else{
        return cDelete;
    }
}

int main() {
    srand(time(NULL)); // Seed the random number generator with current time

    struct Node* head = NULL; // Initialize an empty linked list
    int n = 100; // Number of initial unique values
    int m = 1000; // Total number of operations
    int mMember = 0.4 * m; // Fraction of Member operations
    int mInsert = 0.3 * m; // Fraction of Insert operations
    int mDelete = 0.3 * m; // Fraction of Delete operations
    int cMember = 0;
    int cInsert = 0;
    int cDelete = 0;
    // Populate the linked list with n random, unique values
    for (int i = 0; i < n; i++) {
        int value;
        do {
            value = rand() % ((1 << 16) - 1); // Generate a random value between 0 and 2^16 - 1
        } while (Member(head, value)); // Ensure it's unique
        Insert(&head, value);
    }

    // Perform m random Member, Insert, and Delete operations
    while (cMember + cInsert + cDelete < m) {
        int operation = rand() % 1000; // Generate a random number between 0 and 999
        //printf("operation %d -> ", operation);
        if (operation < mMember) {
            int count = performMember(head, mMember, cMember);
            if( count != cMember){
                cMember = count;
            }else{
                //perform insert
                cInsert = performInsert(head, mInsert, cInsert);
            }

        } else if (operation < mMember + mInsert) {
            int count = performInsert(head, mInsert, cInsert);
            if(count != cInsert){
                cInsert = count;
            }else{
                //perform delete
                cDelete = performDelete(head, mDelete, cDelete);
            }
        } else {
            int count = performDelete(head, mDelete, cDelete);
            if( count != cDelete){
                cDelete = count;
            }else{
                //perform Member operation
                cMember = performMember(head, mMember, cMember);
            }
        }

    }

    printf("counts: %d %d %d", mMember, mInsert, mDelete);

    // Print the final state of the linked list
    PrintList(head);

    // Clean up: Free memory allocated for the linked list
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}

