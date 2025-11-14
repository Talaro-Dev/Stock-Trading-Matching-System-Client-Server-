// struct for reading file
typedef struct file
{
    int user_id;
    int company_id;
    int command_type;
    int number_of_stocks;
    float price;
}file;

// struct to send from client
typedef struct commandFromClient
{
    int user_id;
    int company_id;
    int command_type;
    int number_of_stocks;
    float price;

} commandFromClient;

// struct to send the command who is going to be deleted
typedef struct commandForDelete
{
    int user_id;
    int company_id;
    int command_type_for_delete;
    int number_of_stocks;
    float price;
}commandForDelete;

// struct for the received command
typedef struct receiveCommand
{
    int user_id;
    int company_id;
    int command_type;
    int number_of_stocks;
    float price;    
} receiveCommand;

// struct for completed commands
typedef struct commands
{
    int user_id;
    int company_id;
    int command_type;
    double command_id;
    char timestamp[30];
    int number_of_stocks;
    float price;
} commands;

// queue struct
typedef struct queue
{
    commands commandClient[100];
    int size;
    int front;
    int rear;
    int max;
}queue;

// stats for prices
typedef struct basicStats
{
    int company_id;
    float price;    
}basicStats;

// stocks handler
typedef struct stocks
{
    int user_id;
    int company_id;
    int number_of_stocks;
}stocks;

// command for completed commands
typedef struct commandsCompleted
{
    int user_id;
    int company_id;
    int number_of_stocks;
    float price;
    int command_type;
}commandsCompleted;

// command to send in client for finished unfinished commands
typedef struct commandToClient
{
    int company_id;
    int number_of_stocks;
    float price;
    int command_type;
    int completed;
    int empty;
    int size; 
}commandToClient;

// hello message and mark in order client to know if he will print commands or not
typedef struct sendToClient
{
    int mark;
    char message[30];
}sendToClient;