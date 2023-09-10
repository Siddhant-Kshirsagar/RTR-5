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
    printf("-------------------------------------\n");
    while(temp!=NULL)
    {
        
        printf("%d\t",temp->data);

        temp = temp->next;
    }
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

    return(first);
}
