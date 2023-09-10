#include<stdio.h>
#include<stdlib.h>

struct Node
{
    int data;
    struct Node *next;
};


struct Node * create_List(int numNodes);

void display_list(struct Node *first);

struct Node *insert_node(struct Node * first, int input, int position);

struct Node *destroy_list(struct Node *first);

struct Node *delete_node(struct Node *first, int position);

int main(void)
{

    int numNodes = 10;


    struct Node *head = create_List(numNodes);
    printf("Create List\n");
    display_list(head);

    head = insert_node(head,100,1);
    printf("insert node at first position\n");
    display_list(head);

    
    head = insert_node(head,101,3);
    printf("insert node at third position\n");
    display_list(head);

    
    head = insert_node(head,102,4);
    printf("insert node at forth position\n");
    display_list(head);

    
    head = insert_node(head,104,10);
    printf("insert node at 10th position\n");
    display_list(head);

    head = insert_node(head,110,20); // not changes occured because this position is out of range 
    printf("insert node at 20th position\n");
    display_list(head);

    head = delete_node(head,3);
    printf("delete node at third position\n");
    display_list(head);

    head = delete_node(head,1);
    printf("delete node at 1st position\n");
    display_list(head);

    head = delete_node(head,20); // not changes occured because this position is out of range
     printf("delete node at 20th position\n");
    display_list(head);

    head = destroy_list(head);

    printf("destroy list \n");

    display_list(head);

    return(0);
}

struct Node *create_List(int numNodes)
{
    struct Node * head = NULL;

    head = (struct Node*) malloc(sizeof(struct Node));

    head->data = 10;
    head->next = head;

    struct Node *temp = head;

    for(int i = 1; i< numNodes; i++)
    {
        temp->next = (struct Node *) malloc (sizeof(struct Node));

        temp = temp->next;

        temp->data = 10;
        temp->next = NULL;
    }

    //in circular list last node next points to the first (head) node
    temp->next = head;
    return(head);
}

void display_list(struct Node *first)
{
    struct Node *temp = first;
    printf("-------------------------------------\n");
    if(first == NULL) // if list is empty (null) then return from here no need to execute other code
    {
        return;
    }
    // in circular linked list when we traverse throught the list we need to stop when we get back again to first node 
    // so we are using do while loop 
    do
    {
        printf("%d\t",temp->data);
        temp = temp->next;
    } while (temp!=first);
    
    printf("-------------------------------------\n");
}

struct Node *insert_node(struct Node * first, int input, int position)
{
    if(first == NULL && position !=1)
    {
        return(first);
    }

    if(position == 1)
    {
        struct Node *head = (struct Node*)malloc(sizeof(struct Node));
        head->data = input;
        head->next = first;
        //In circular linked list we need to points last node to the first node 
        struct Node *temp = first;
        do
        {
            temp = temp->next;
        } while (temp->next !=first);
        
        temp->next = head; // here we points last node to first 

        return(head);
    }

    struct Node *temp = first;
    for(int i = 1; i<(position - 1) && temp->next != first; i++)
    {
        temp = temp->next;
    }

    // we are returning from here because we are not adding node at last and after last position
    if(temp->next == first)
    {
        return(first);
    }

    struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));

    newNode->data = input;
    newNode->next = temp->next;

    temp->next = newNode;

    return(first);

}

struct Node *destroy_list(struct Node *first)
{
    struct Node *head = first;
    struct Node *temp = first;
    while(temp->next != head)
    {
        temp = first->next;
        free(first);
        first = temp;
    }

    free(temp);
    temp = NULL;
    return(temp);
}

struct Node *delete_node(struct Node *first, int position)
{
    if(first == NULL)
    {
        return(first);
    }
    if(position == 1)
    {
        //if list has only 1 node 
        if(first->next == first) 
        {
            free(first);
            return(NULL);
        }

        struct Node *temp = first;
        first = first->next;

        struct Node *temp1 = first;
        while(temp1->next !=temp)
        {
            temp1= temp1->next;
        }
        temp1->next = first;

      
        free(temp);
        temp = NULL;
        return(first);
    }

    struct Node *temp = first;
    for(int i = 1; i<(position-1)&&temp->next!=first;i++)
    {
        temp = temp->next;
    }

    if(temp->next==first)
    {
        return(first);
    }

    struct Node *delNode = temp->next;
    temp->next = delNode->next;
    free(delNode);
    delNode = NULL;
    return(first);
}
