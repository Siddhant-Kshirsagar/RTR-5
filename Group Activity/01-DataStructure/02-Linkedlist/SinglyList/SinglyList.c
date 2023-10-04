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

struct Node *insert_node_AtFirstPosition(struct Node *first, int input);

struct Node *insert_node_AtLastPosition(struct Node *first, int input);

struct Node *destroy_list(struct Node *first);

struct Node *delete_node(struct Node *first, int position);

struct Node *delete_last_node(struct Node *first);

struct Node *delete_first_node(struct Node *first);

int main(void)
{

    int numNodes = 5;


    struct Node *head = create_List(numNodes);
    printf("Create List\n");
    display_list(head);
    display_list(head);

    head = delete_node(head,3);
    printf("delete node at third position\n");
    display_list(head);

    printf(" ******** Before Deleting Last Node ************\n");
    display_list(head);
    head=delete_last_node(head);

    printf(" ******** After Deleting Last Node ************\n");
    display_list(head);

    printf(" ******** Before Deleting Last Node ************\n");
    display_list(head);
    head = delete_last_node(head);

    printf(" ******** After Deleting Last Node ************\n");
    display_list(head);


    printf(" ******** Before Deleting Last Node ************\n");
    display_list(head);
    head = delete_last_node(head);

    printf(" ******** After Deleting Last Node ************\n");
    display_list(head);

    printf(" ******** Before Inserting First Node ************\n");
    display_list(head);
    head = insert_node_AtFirstPosition(head, 101);
    printf(" ******** After Inserting First Node ************\n");
    display_list(head);

    printf(" ******** Before Inserting First Node ************\n");
    display_list(head);
    head = insert_node_AtFirstPosition(head, 1001);
    printf(" ******** After Inserting First Node ************\n");
    display_list(head);

    printf(" ******** Before Deleting First Node ************\n");
    display_list(head);
    head = delete_first_node(head);
    printf(" ******** After Deleting First Node ************\n");
    display_list(head);

    printf(" ******** Before Inserting Node At Last Position ************\n");
    display_list(head);
    head = insert_node_AtLastPosition(head, 1001);

    printf(" ******** After Inserting Node At Last Position ************\n");
    display_list(head);

    printf(" ******** Before Deleting Last Node ************\n");
    display_list(head);
    head = delete_last_node(head);

    printf(" ******** After Deleting Last Node ************\n");
    display_list(head);

    printf(" ******** Before Deleting Last Node ************\n");
    display_list(head);
    head = delete_last_node(head);

    printf(" ******** After Deleting Last Node ************\n");
    display_list(head);

    printf(" ******** Before Deleting Last Node ************\n");
    display_list(head);
    head = delete_last_node(head);

    printf(" ******** After Deleting Last Node ************\n");
    display_list(head);

    printf(" ******** Before Inserting Node At Last Position ************\n");
    display_list(head);
    head = insert_node_AtLastPosition(head, 1001);

    printf(" ******** After Inserting Node At Last Position ************\n");
    display_list(head);

    printf("Before deleting all list\n");
    display_list(head);
    head = destroy_list(head);
    printf("After deleting all list\n");
    display_list(head);

    return(0);
}

struct Node *create_List(int numNodes)
{
    struct Node * head = NULL;

    head = (struct Node*) malloc(sizeof(struct Node));

    head->data = 10;
    head->next = NULL;

    struct Node *temp = head;

    for(int i = 1; i< numNodes; i++)
    {
        temp->next = (struct Node *) malloc (sizeof(struct Node));

        temp = temp->next;

        temp->data = 10;
        temp->next = NULL;
    }

    return(head);
}

void display_list(struct Node *first)
{
    struct Node *temp = first;
    printf("\n-----------------------------------------------------------------------\n");
    if (temp == NULL)
    {
        printf("List is empty \n");
    }
    else
    {
        while (temp != NULL)
        {

            printf("%d\t", temp->data);

            temp = temp->next;
        }
    }
  
    printf("\n-----------------------------------------------------------------------\n");
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

        return(head);
    }

    struct Node *temp = first;
    for(int i = 1; i<(position - 1) && temp != NULL; i++)
    {
        temp = temp->next;
    }

    //linked is empty, means position is invalid 
    if(temp == NULL)
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
    struct Node *temp = first;
    while(temp !=NULL)
    {
        temp = first->next;
        free(first);
        first = temp;
    }
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
        struct Node *temp = first;
        first = first->next;
        free(temp);
        return(first);
    }

    struct Node *temp = first;
    for(int i = 1; i<(position-1)&&temp!=NULL;i++)
    {
        temp = temp->next;
    }

    if(temp==NULL)
    {
        return(first);
    }

    struct Node *delNode = temp->next;
    temp->next = delNode->next;
    free(delNode);
    delNode = NULL;
    return(first);
}

struct Node *delete_last_node(struct Node *first)
{
    struct Node *temp = first;
    struct Node *delNode = NULL;

    if (temp != NULL)
    {
        if (temp->next == NULL)
        {
            free(temp);
            free(first);
            temp = NULL;
            first = NULL;
        }
        else
        {
            while (temp->next->next != NULL)
            {
                temp = temp->next;
            }

            if (temp != NULL)
            {
                delNode = temp->next;
                temp->next = NULL;
                free(delNode);
                delNode = NULL;
            }
        }
    }
   
 

    return(first);
}

struct Node *insert_node_AtFirstPosition(struct Node *first, int input)
{
    struct Node *temp = (struct Node *)malloc(sizeof(struct Node));

    temp->data = input;
    temp->next = first;
    first = temp;

    return(first);
}

struct Node *delete_first_node(struct Node *first)
{
    struct Node *temp = first;
    if (temp != NULL)
    {
        if (temp->next == NULL)
        {
            free(temp);
            free(first);
            temp = NULL;
            first = NULL;
        }

        else if (temp->next != NULL)
        {
            first = temp->next;
            free(temp);
            temp = NULL;
        }
    }
    

    
  
    return(first);
}

struct Node *insert_node_AtLastPosition(struct Node *first, int input)
{   
    struct Node *Last = (struct Node *)malloc(sizeof(struct Node));
    struct Node *temp = first;

    if (first == NULL)
    {
        Last->data = input;
        Last->next = NULL;
        first = Last;
    }
    else
    {
        while(temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = Last;
        Last->data = input;
        Last->next = NULL;
    }
    
    return(first);
}
