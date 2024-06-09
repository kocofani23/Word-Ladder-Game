#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
@brief: struct data type to represent the nodes of the dictionary linked list
@param1: char word to represent the value of that node
@param2: int to represent if the word is seen before or not
@param3: pointer to next node on the stack
*/
struct dictNode
{
    char *word;
    int seen;
    struct dictNode *nextWord;
};

/**
@brief: struct data type to represent the dictionary linked list we will use
@param1: pointer to the head node of the linked list
*/
struct dictionaryLinkedList
{
    struct dictNode *head;
};


/**
@brief: struct data type to represent the stack we will use
@param1: char word to represent the value of that node
@param2: pointer to next node on the stack
*/
struct stackNode
{
    char *word;
    struct stackNode *next;

};

/**
@brief: struct data type to represent the queue's nodes
@param1: struct Stack to represent the value of the stack in the node
@param2: pointer to next node on the queue
*/
struct queueNode
{
    struct stackNode *stack;
    struct queueNode *next;

};

/**
@brief: struct data type to represent the queue we will use
@param1: pointer to front stack of the queue
@param2: pointer to rear stack of the queue
*/
struct Queue
{
    struct queueNode *front;
    struct queueNode *rear;

};

/**
@brief: function to initialize the dictionary linked list
@returns: pointer to head of dictionary linked list
*/
struct dictionaryLinkedList *initDictionary();

/**
@brief: function to check whether the stack is empty or not
@param1: pointer to stack data type
@returns: 1 if stack is empty else 0
*/
int isStackEmpty(struct stackNode *s);

/**
@brief: function to initialize the stack
@param1: pointer to stack data type
@returns: nothing. void function
*/
void initStack(struct stackNode **s);

/**
@brief: function to push the top element of the stack
@param1: pointer to stack data type
@param2: value of char word to be pushed onto the stack
@returns: 1 if element is successfully pushed else 0
*/
struct stackNode *push(struct stackNode *s, char *word);

/**
@brief: function to pop the top element of the stack
@param1: pointer to stack data type
@param2: value of char word that the popped data will be assigned
@returns: the element on the top of the stack
*/
struct stackNode *pop(struct stackNode **s);

/**
@brief: function to peek the top element of the stack
@param1: pointer to stack data type
@param2: value of char word that the peeked data will be assigned
@returns: the element on the top of the stack
*/
char *peek(struct stackNode *s);

/**
@brief: function to print the stack onto the screen
@param1: pointer to stack data type
@returns: nothing. void function
*/
void displayStack(struct stackNode *s);

/**
@brief: function to initialize the queue data structure
@returns: newly allocated queue data structure
*/
struct Queue* initQueue();

/**
@brief: function to dequeue the first stack of the queue
@param1: pointer to queue data type
@returns: stack data structure
*/
struct stackNode* dequeue(struct Queue* queue);

/**
@brief: function to enqueue a stack at the rear of the queue
@param1: pointer to queue data type
@param2: pointer to stack data structure to be enqueued
@returns: nothing. void function
*/
void enqueue(struct Queue* queue, struct stackNode* stack);

/**
@brief: function to check whether a word is in the dictionary or not before creating the queues and stacks
@param1: pointer to FILE dictionary opened
@param2: char word to be searched for
@returns: 1 if found 0 otherwise
*/
int check(FILE *file, char *word);

/**
@brief: function to check whether a word has the same length as the other word and differ only by one character
@param1: char word to compare to
@param2: char word to be compared with
@returns: 1 if the criteria is met 0 otherwise
*/
int isWordValid(char *word1, char *word2);

/**
@brief: function to implement the main logic behind the word ladder game
@param1: pointer to stack to be firstly enqueued
@param2: pointer to queue that will represent the ladder
@param3: char word end that represents our goal
@param4: pointer to linked list dictionary struct
@returns: the shortest length between the start word and the end word held in a stack data type
*/
struct stackNode *createLadder(struct stackNode *stack, struct Queue *queue, char *end, struct dictionaryLinkedList *dictionary);

/**
@brief: function to create a 2D array to hold the words of the dictionary
@param1: pointer to FILE dictionary
@param2: pointer to linked list dictionary struct
@returns: nothing. void function
*/
void createDictionary(FILE *file, struct dictionaryLinkedList *dictionary);

/**
@brief: function to copy the dequeued stack to a new stack for it to be enqueued again
@param1: pointer to stack data structure
@returns: pointer of new copied stack
*/
struct stackNode *copyStack(struct stackNode *stack);

/**
@brief: function to add the words from the file to the dictionary linked list
@param1: pointer to word char
@param2: pointer to dictionary linked list
@returns: nothing. void function
*/
void addWordtoDictionary(char *word, struct dictionaryLinkedList *dictionary);

/**
@brief: function to set the seen value of a node in the linked list to 1
@param1: pointer to word char
@param2: pointer to dictionary linked list
@returns: nothing. void function
*/
void visitWord(char *word, struct dictionaryLinkedList *dictionary);

int main()
{
    struct stackNode *stack = NULL;
    stack = (struct stackNode*)malloc(sizeof(struct stackNode));
    struct Queue *queue;

    struct dictionaryLinkedList *dictionary;
    dictionary = initDictionary();


    char start[100], end[100];

    FILE *file; // opening the dictionary.txt file from file folder
    file = fopen("dictionary.txt", "r");
    if(file == NULL){
        printf("\nFile opening ERROR: main function file!\n");
        exit(-1);
    }

    printf("Enter word to start the search: ");
    scanf("%s", start); //word to start the ladder
    getchar();
    printf("Enter word to be searched for: ");
    scanf("%s", end); //word to end the ladder

    int len1, len2;
    len1 = strlen(start);
    len2 = strlen(end);

    if(len1 != len2 || !check(file, end)){ //first control of lengths of words
        printf("Word to be searched for does not exist or lengths do not match!\n");
        exit(-1);
    }
    else if(strcmp(start, end) == 0){
        printf("Start and end word are the same. No need to search any further.\n");
        printf("Stack:\n%s\n", start);
    }
    else{

        createDictionary(file, dictionary);
        queue = initQueue();//initialize the queue
        initStack(&stack);
        stack = push(stack, start);

        struct stackNode *finalStack = createLadder(stack, queue, end, dictionary);
        if(finalStack != NULL){
            printf("The resultant stack with the shortest path from %s to %s is: \n", start, end);
            displayStack(finalStack);
        }
        else{
            printf("NO ladder found between %s and %s", start, end);
        }

    }

    fclose(file);
    free(stack);

    return 0;
}


struct dictionaryLinkedList *initDictionary()
{
    struct dictionaryLinkedList *dictionary = (struct dictionaryLinkedList*)malloc(sizeof(struct dictionaryLinkedList));
    if(dictionary == NULL){
        printf("Memory allocation ERROR: initDictionary\n");
        exit(-1);
    }
    dictionary->head = NULL;
    return dictionary;
}


int isStackEmpty(struct stackNode *s)
{
    return (s == NULL); // check whether the stack is still empty or not
}

void initStack(struct stackNode **s)
{
    *s = NULL;// initialize the top of the stack to null
}

struct stackNode *push(struct stackNode *s, char *word)
{
    struct stackNode *newNode = (struct stackNode*)malloc(sizeof(struct stackNode));// initialize a new node to be pushed
    if(newNode == NULL){
        printf("Memory allocation error: push function\n");
        exit(-1);
    }
    newNode->word = strdup(word); // allocate memory and copy word to the newNode
    newNode->next = s; //points to the top of the stack
    return newNode;
}

struct stackNode *pop(struct stackNode **s)
{
    if(isStackEmpty(*s)){
        printf("Stack is empty. Cannot pop!\n"); //if stack is empty return with an error
        exit(-1);
    }
    else{
        struct stackNode *temp = *s; //assign a temporary variable
        *s = (*s)->next; //move one step below the top of the stack
        free(temp);
        return *s; //return the popped element
    }
}

char *peek(struct stackNode *s)
{
    if(isStackEmpty(s)){
        printf("Stack is empty. Cannot peek!\n");
        exit(-1);
    }
    return s->word; //return without popping
}


void displayStack(struct stackNode *s)
{
    if(isStackEmpty(s)){
        printf("Stack is empty. Cannot display!\n");
        return;
    }
    printf("Stack:\n");
    while(s != NULL){
        printf("%s\n", s->word); // print the words of the stack nodes to the screen
        s = s->next;
    }
    printf("\n");
}

struct Queue* initQueue(){

    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    if(queue == NULL){
        printf("Memory allocation ERROR: createQueue\n");
        exit(-1);
    }
    queue->front = queue->rear = NULL; //initialize both pointers to null
    return queue;
}


void enqueue(struct Queue* queue, struct stackNode* stack)
{

    struct queueNode* newNode = (struct queueNode*)malloc(sizeof(struct queueNode));
    if(newNode == NULL){
        printf("Memory allocation ERROR: enqueue function newNode creation!\n");
        exit(-1);
    }
    newNode->stack = stack; //allocate memory for queue's new node
    newNode->next = NULL;

    if(queue->rear == NULL){ // if queue is empty
        queue->front = newNode;
        queue->rear = newNode;
    }
    else{
        queue->rear->next = newNode; // if queue is not empty update accordingly
        queue->rear = newNode;
    }

}

struct stackNode* dequeue(struct Queue* queue)
{
    struct queueNode* temp;
    struct stackNode* stack;

    if (queue->front == NULL) {
        printf("Queue is empty. Cannot dequeue!\n");
        exit(-1);
    }

    temp = queue->front; //allocate temporary variable
    stack = temp->stack;
    queue->front = queue->front->next; // update the front of the queue

    if(queue->front == NULL){
        queue->rear = NULL; // if queue is empty update accordingly
    }

    free(temp);
    return stack; // return the previous front of the queue
}

int isQueueEmpty(struct Queue* queue)
{
    return (queue->front == NULL); // if queue's front is still pointing to null means it's empty
}

void displayQueue(struct Queue* queue)
{
    struct queueNode* temp = queue->front;
    printf("Queue:\n");
    while(temp != NULL){
        displayStack(temp->stack); //print every element of the queue which is a stack by calling the respective function
        temp = temp->next;
    }
    printf("\n");
}

int check(FILE *file, char *word)
{

    char temp[100];
    rewind(file);

    while(fscanf(file, "%s", temp) != EOF){
        if(strcmp(temp, word) == 0){ //if word is in dictionary assign 1 to flag
            return 1;
        }
    }
    return 0;
}

int isWordValid(char *word1, char *word2)
{
    int count = 0;
    int i;
    int len1 = strlen(word1);
    int len2 = strlen(word2);

    if(len1 != len2){ // if lengths are not the same return 0
        return 0;
    }

    for(i=0; i<len1; i++){
        if(word1[i] != word2[i])
            count++; // check how many characters are different
        if(count > 1){
            return 0;
        }
    }

    return 1; // if there is only one different character return true else false
}

void createDictionary(FILE *file, struct dictionaryLinkedList *dictionary)
{
    char word[100];
    rewind(file);

    while(fscanf(file, "%s", word) != EOF){ //iterate through the dictionary file and copy words to dictionary struct
        addWordtoDictionary(word, dictionary);
    }
}

void addWordtoDictionary(char *word, struct dictionaryLinkedList *dictionary)
{
    struct dictNode *newNode = (struct dictNode*)malloc(sizeof(struct dictNode));
    if(newNode == NULL){
        printf("Memory allocation ERROR: addWordtoDictionary function newNode creation!\n");
        return;
    }

    newNode->word = strdup(word);
    newNode->seen = 0;
    newNode->nextWord = NULL;

    if(dictionary->head == NULL){
        dictionary->head = newNode;
    }
    else{
        struct dictNode *curr = dictionary->head;
        while(curr->nextWord != NULL){
            curr = curr->nextWord;
        }
        curr->nextWord = newNode;
    }

}


struct stackNode *copyStack(struct stackNode *stack)
{
    struct stackNode* newStack = NULL; // initialize new stack to NULL

    if (stack == NULL)
        return NULL; // If stack is null, return null

    struct stackNode* curr = stack; // pointer to traverse the original stack
    struct stackNode* prev = NULL; // pointer to keep track of the previous node in the new stack

    while(curr != NULL){
        struct stackNode* newNode = (struct stackNode*)malloc(sizeof(struct stackNode)); // allocate memory for new node
        if (newNode == NULL) {
            printf("Memory allocation ERROR: copyStack function newNode creation!\n");
            exit(-1);
        }
        newNode->word = strdup(curr->word); // copy the word from the original stack to the new node
        newNode->next = NULL; // initialize the next pointer of the new node to NULL

        if(prev == NULL){
            newStack = newNode; // if this is the first node in the new stack, set it as the new stack
        } else {
            prev->next = newNode; // connect the previous node to the new node
        }

        prev = newNode; // update the previous pointer to the current new node
        curr = curr->next; // move to the next node in the original stack
    }

    return newStack; // return the copied stack
}

void visitWord(char *word, struct dictionaryLinkedList *dictionary)
{
    if(dictionary->head == NULL){
        printf("Dictionary empty!\n");
        return;
    }

    struct dictNode *curr = dictionary->head;
    while(curr != NULL && strcmp(curr->word, word) != 0){
        curr = curr->nextWord;
    }
    curr->seen = 1;
}

struct stackNode *createLadder(struct stackNode *stack, struct Queue *queue, char *end, struct dictionaryLinkedList *dictionary)
{
    enqueue(queue, stack); // enqueue the stack first

    struct stackNode *currStack;
    struct stackNode *newStack;
    char *currWord;

    while(!isQueueEmpty(queue)){

        currStack = dequeue(queue); // dequeue the first stack of the queue
        currWord = peek(currStack); // take the top word of the stack without popping
        visitWord(currWord, dictionary);

        if (strcmp(currWord, end) == 0) {
            return currStack; // if the word peeked from the dequeued stack is the same as our goal, return
        }

        struct dictNode *curr = dictionary->head;
        while(curr != NULL){ // iterate until the end of the dictionary
            if(isWordValid(currWord, curr->word) && (curr->seen == 0)){

                newStack = copyStack(currStack); // copy the dequeued stack to the new stack for enqueueing once again
                newStack = push(newStack, curr->word); // push the new word on top of the newStack
                curr->seen = 1;

                enqueue(queue, newStack); // Enqueue the new stack into the queue
            }
            curr = curr->nextWord;
        }
    }

    return NULL; // return NULL if no ladder is found
}
