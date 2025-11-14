#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <math.h>
#include <arpa/inet.h>
#include "struct.h"
#include "enqueue_dequeue_buy_sell.h"

#define IP "127.0.0.1"
#define PORT 1234

int create_socket();
void connection(int sockfd);
void *handle_connection(void *);
void *sendMessage (void * sockfd,int user_id);
int receiveMessage (void *sockfd);
queue * createQueue();
void initStats();
void deQueueSell (queue* Q1, queue* Q2,int i);
int enQueue(receiveCommand entoli);
void deQueueCommand(queue *Q1,queue *Q2,commandForDelete entoli);
void checkMatch(int checkMark,queue* Q1,queue* Q2);
void deQueueQ1Sell(queue* Q1);
void deQueueQ2Sell(queue* Q2,int i);
void writeToFile(queue* Q1, queue* Q2,int i,int checkMark);
const char* timestamp();
void deQueueBuy(queue* Q1, queue* Q2,int i);
void deQueueQ1Buy(queue* Q1, int i);
void deQueueQ2Buy(queue* Q2);
void changePriceOfStock(int company_id,float price);
void changeStocks (int user_sold,int user_bought,int stocks_bought, int stocks_sold,int company_id);
int checkStock(receiveCommand entoli);
void addCompletedCommand(int Q1_mark,int Q2_mark);
void *receiveUserId(void *sockfd);

pthread_mutex_t lock;
queue *Q1;
queue *Q2;
basicStats* array;
commandsCompleted* commandCompleted;
int completed_mark;
stocks* user_stock;
double tag1,tag2;

int main (int argc, char* argv[])
{
initStats();
// init command id's for each queue
tag1 = 1.0;
tag2 = 2.0;
Q1=createQueue(); //create queue for selling
Q2=createQueue(); //create queue for buying
while(1)
{
    int sockfd = create_socket();
    connection(sockfd);
}
}

int create_socket()
{
    int sockfd, Dlc;
    struct sockaddr_in serverAddr;
    

    
    sockfd = socket(AF_INET,SOCK_STREAM, 0); //create socket
    if (sockfd < 0)
    {
        printf("error in connection. \n");
        exit(1);
    }

    memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(IP);    
	Dlc = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); 	//assigning a name to a socket(server)
	if(Dlc < 0){
		printf("Error in binding.\n"); 
		exit(1);
	}
	
	//Mark a socket as accepting connections. 
	if(listen(sockfd, 10) == 0)
	{
		printf("Server on\n");
        return sockfd;
	}else
	{
		printf("Error in binding.\n");
        return -1;
	}
    
	

}

void connection(int sockfd)
{   pthread_t thread_id;
    int newSocket;
    socklen_t addr_size;
    struct sockaddr_in newAddr;
	int *newsock;
	
while(1)
{

     newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size); //Accept a connection on a socket.
	  
		if (newSocket < 0)
		{
			exit(1);
		}
		printf("A client has been connected \n");


      newsock=malloc(1);
     *newsock=newSocket;
	  int ret=pthread_create(&thread_id,NULL,handle_connection,(void*)newsock); //create thread for each client connected
	
		if(ret<0)
		{
			printf("no threads created");
			exit(1);
		}
 
}
 
}
 


void *handle_connection(void  *sockfd)
{
	receiveUserId(sockfd); //receive client's id
	while(1)
	{
	
	if((receiveMessage(sockfd)) == 7)
	{	
		free(sockfd); //free client's socket
		pthread_detach(pthread_self()); //detach thread
		return 0;
	}
	
	}
	
return 0;
}


void *receiveUserId(void *sockfd)
{
	
	int user_id=0;
	if(recv(*((int*)sockfd),&user_id,sizeof(int)*2,0)<0) //receive client's id
	{
		printf("error \n");
	}
	else
	{
		sendMessage(sockfd,user_id); //send finished and unfinished commands
	}

	return (void*)0;
}

void *sendMessage (void * sockfd,int user_id)
{
	sendToClient client; //mark and message for client
	commandToClient *commandsForUser; //completed commands
	commandsForUser =(commandToClient*)malloc(sizeof(commandToClient)*100); //allocate memory
	int j=0;
	int mark =0;


	for(int i=0; i<=completed_mark;i++) //check for any completed commands and put them in coomandsForUser
	{
		if(user_id == commandCompleted[i].user_id)
		{
			commandsForUser[j].company_id = commandCompleted[i].company_id;
			commandsForUser[j].number_of_stocks = commandCompleted[i].number_of_stocks;
			commandsForUser[j].price = commandCompleted[i].price;
			commandsForUser[j].command_type = commandCompleted[i].command_type;
			commandsForUser[j].completed = 1;

			commandCompleted[i].user_id = 0;
			j++;
			mark++;
		}
	}
	if(Q1->front >= 0) //check Queue for selling if it is not empty (when empty front = -1)
	{
	  for(int i=0; i<=Q1->rear;i++)
	  {
		  if(user_id == Q1[i].commandClient->user_id)
		  {
		  	commandsForUser[j].company_id = Q1[i].commandClient->company_id;
			commandsForUser[j].number_of_stocks = Q1[i].commandClient->number_of_stocks;
			commandsForUser[j].price = Q1[i].commandClient->price;
			commandsForUser[j].command_type = Q1[i].commandClient->command_type;
			commandsForUser[j].completed = 0;
		
			j++;
			mark++;
	  	}
	  }
	}
	if(Q2->front >=0) //check Queue for buying if it is not empty (when empty front = -1)
	{
		 for(int i=0; i<=Q2->rear;i++)
	 	{
			if(user_id == Q2[i].commandClient->user_id)
		  	{
			commandsForUser[j].company_id = Q2[i].commandClient->company_id;
			commandsForUser[j].number_of_stocks = Q2[i].commandClient->number_of_stocks;
			commandsForUser[j].price = Q2[i].commandClient->price;
			commandsForUser[j].command_type = Q2[i].commandClient->command_type;
			commandsForUser[j].completed = 0;
			j++;
			mark++;
			}
		}
	}
	if (mark == 0) //if client does not have any command in this id send only message
	{	
		strcpy(client.message,"hello from server!!!");
		client.mark = mark;
		if (send(*((int*)sockfd),&client,sizeof(sendToClient),0)<0)
		{
			printf("error");
		
		}
		return (void*)0;
	}
	else if(mark >= 1) //if client has commands send  a message and the commands
	{
		strcpy(client.message,"hello from server!!!");
		client.mark = mark;
		if (send(*((int*)sockfd),&client,sizeof(sendToClient),0)<0)
		{
			printf("error");
		
		}
		if (send(*((int*)sockfd),commandsForUser,sizeof(commandToClient)*100,0)<0)
			{
				printf("error");
			}

	}
	return (void*)0;
}

int receiveMessage (void *sockfd)
{
	receiveCommand entoli; //receive command from client
	
	if(recv(*((int*)sockfd),(void*)&entoli,sizeof(receiveCommand),0)<0)
	{
		printf("error \n");
	}
	
	// do something based on command's type
	else
	{
		if (entoli.command_type == 7)
		{
			printf("client disconnected \n");
			return 7;
			
			
		}
		else if (entoli.command_type == 6)
		{
			commandForDelete ent; //receive command for delete
			if(recv(*((int*)sockfd),(void*)&ent,sizeof(commandForDelete),0)<0)
				{
					printf("error \n");
				}
			//mutex lock and dequeue command	
			pthread_mutex_lock(&lock);
			deQueueCommand(Q1,Q2,ent);
			pthread_mutex_unlock(&lock);
		}
		else if (entoli.command_type == 2 )
		{
			
			if(checkStock(entoli) == 1)
			{
				for (int i =0;i<20;i++)
				{
				if(entoli.company_id == array[i].company_id)
				{
					entoli.price = array[i].price + (array[i].price *0.05); //add the 5% of the price
				}
				}
				// mutex lock to enquue and check command for matching
				pthread_mutex_lock(&lock);
				int checkMark=enQueue(entoli);
				checkMatch(checkMark,Q1,Q2);
				pthread_mutex_unlock(&lock);
				
			}
			else{}	

		}
		// entoles 1 kai 3
		else if (entoli.command_type == 1  || entoli.command_type == 3)
		{
			if(checkStock(entoli) == 1)
			{
				// mutex lock to enqueue and check command for matching
				pthread_mutex_lock(&lock);
				int checkMark=enQueue(entoli);
				checkMatch(checkMark,Q1,Q2);
				pthread_mutex_unlock(&lock);
			}
			else{}

		}

		else if (entoli.command_type == 4)
		{

			if(checkStock(entoli) == 1)
			{
				for (int i =0;i<20;i++)
				{
				if(entoli.company_id == array[i].company_id)
				{
					entoli.price = array[i].price - (array[i].price *0.05); // minus 5% to the price
				}
				}
				//mutex lock to enqueue and check command for matching
				pthread_mutex_lock(&lock);
				int checkMark=enQueue(entoli);
				checkMatch(checkMark,Q1,Q2);
				pthread_mutex_unlock(&lock);
				
			}
			else{}			
		}
	
	
	

	}
	return 0;
}
queue * createQueue ()
{
	queue *Q;
	Q=(queue*)malloc(sizeof(queue)*100);
	
	
	Q->size = 100;
	Q->max = 100;
	Q->front =-1;
	Q->rear = -1;

	return Q;
}

int enQueue(receiveCommand entoli)
{
	// enqueue in sellling queue
	if (entoli.command_type == 1 || entoli.command_type == 2)
	{
		
		//check if queue is full
		if (Q1->rear == 100-1)
		{
			printf("Queue is full.");
		}
		else
		{
			// front == -1 means that the queue is empty different stepss
			if (Q1->front == -1)
				{	
					Q1->rear= Q1->rear + 1;
					Q1->front =0;
					
					Q1[Q1->rear].commandClient->user_id = entoli.user_id;
					Q1[Q1->rear].commandClient->company_id = entoli.company_id;
					Q1[Q1->rear].commandClient->command_type = entoli.command_type;
					Q1[Q1->rear].commandClient->price = entoli.price;
					Q1[Q1->rear].commandClient->number_of_stocks = entoli.number_of_stocks;
					Q1[Q1->rear].commandClient->command_id = tag1;
					strcpy(Q1[Q1->rear].commandClient->timestamp,timestamp());
					Q1->size = Q1->size +1;
					tag1 = tag1 + 0.01;
					printf("Insertion success from client %d \n",Q1[Q1->rear].commandClient->user_id);
				}
			// enqueue to a non empty queue
			else
			{
				Q1->rear= Q1->rear + 1;

				Q1[Q1->rear].commandClient->user_id = entoli.user_id;
				Q1[Q1->rear].commandClient->company_id = entoli.company_id;
				Q1[Q1->rear].commandClient->command_type = entoli.command_type;
				Q1[Q1->rear].commandClient->command_id = tag1;
				Q1[Q1->rear].commandClient->number_of_stocks = entoli.number_of_stocks;
				Q1[Q1->rear].commandClient->price = entoli.price;
				strcpy(Q1[Q1->rear].commandClient->timestamp,timestamp());

				Q1->size = Q1->size -1;
				tag1 = tag1 + 0.01;
				
				printf("Insertion success from client %d \n",Q1[Q1->rear].commandClient->user_id);
				
				
			}
			
			
		}
	}
	// enqueue to buying queue
	else if (entoli.command_type == 3 || entoli.command_type == 4)
	{
		if (Q2->rear == 100-1)
		{
			printf("Queue is full.");
		}

		else
		{   // front == -1 means that the queue is empty different stepss
			if (Q2->front == -1)
				{
					Q2->front =0;
					Q2->rear= Q2->rear + 1;

					Q2[Q2->rear].commandClient->user_id = entoli.user_id;
					Q2[Q2->rear].commandClient->company_id = entoli.company_id;
					Q2[Q2->rear].commandClient->command_id = tag2;
					Q2[Q2->rear].commandClient->command_type = entoli.command_type;
					Q2[Q2->rear].commandClient->number_of_stocks = entoli.number_of_stocks;
					Q2[Q2->rear].commandClient->price = entoli.price;
					strcpy(Q2[Q2->rear].commandClient->timestamp,timestamp());
					
					Q2->size = Q2->size -1;
					tag2 = tag2 + 0.01;
					printf("Insertion success from client %d \n",Q2[Q2->rear].commandClient->user_id);
				}
		
			else
			{   //enqueue a non empty queue
				Q2->rear= Q2->rear + 1;

				Q2[Q2->rear].commandClient->user_id = entoli.user_id;
				Q2[Q2->rear].commandClient->company_id = entoli.company_id;
				Q2[Q2->rear].commandClient->command_id =tag2;
				Q2[Q2->rear].commandClient->command_type = entoli.command_type;
				Q2[Q2->rear].commandClient->number_of_stocks = entoli.number_of_stocks;
				Q2[Q2->rear].commandClient->price = entoli.price;
				strcpy(Q2[Q2->rear].commandClient->timestamp,timestamp());
			
				Q2->size = Q2->size -1;
				tag2 = tag2 + 0.01;
				printf("Insertion success from client %d \n",Q2[Q2->rear].commandClient->user_id);
			}
	
			return 2;
		}
	}
	return 0;
	
}


void initStats()
{
	// init an array for prices, for stocks , and for completed commands
	array=(basicStats*)malloc(sizeof(basicStats)*20);
	user_stock=(stocks*)malloc(sizeof(stocks)*400);
	commandCompleted=(commandsCompleted*)malloc(sizeof(commandsCompleted)*300);
	int company_id = 100;
	float price = 10;

		// fill started prices
		for (int k=0; k<20;k++)
		{
			array[k].company_id = company_id;
			array[k].price = price;
			company_id++;
			price = price + 10;
			k++;
		}
		int user_id=1;
		company_id=100;
		int number_of_stocks=1000;
		int i =0;
		int k =20;

		// fill firstly stocks to each client
		for(int j=0; j<10;j++)
		{
			while(i<k)
			{
				user_stock[i].user_id = user_id;
				user_stock[i].company_id = company_id;
				user_stock[i].number_of_stocks= number_of_stocks;
				company_id++;
				i++;
			}
			i= i+1;
			k = k+20;
			user_id= user_id + 1;
			company_id=100;
		}
	

	return;
}

void checkMatch(int checkMark,queue* Q1, queue* Q2)
{
	// if checkMark == 1 means the command that came is for selling
	if (checkMark == 1)
	{
		int y = Q1->rear;
		// if it is for selling in specified price
		if (Q1[y].commandClient->command_type == 1)
		{	
		 	
			for (int i=0;i <=(Q2->rear);i++)
			{
				if (Q1[y].commandClient->price == Q2[i].commandClient->price && Q1[y].commandClient->company_id == Q2[i].commandClient->company_id && Q1[y].commandClient->user_id != Q2[i].commandClient->user_id)
					{
						printf("match buy/sell between %d %d for stock %d in price %f \n",Q1[y].commandClient->user_id,Q2[i].commandClient->user_id
						,Q1[y].commandClient->company_id,Q1[y].commandClient->price);
						writeToFile(Q1,Q2,i,checkMark);
						changePriceOfStock(Q1[y].commandClient->company_id,Q2[i].commandClient->price);
						
						if(Q1[y].commandClient->number_of_stocks == Q2[i].commandClient->number_of_stocks)
						{
							changeStocks (Q1[y].commandClient->user_id,Q2[i].commandClient->user_id,Q2[i].commandClient->number_of_stocks,Q1[y].commandClient->number_of_stocks,Q1[y].commandClient->company_id);
						}
						else if(Q1[y].commandClient->number_of_stocks < Q2[i].commandClient->number_of_stocks)
						{
							changeStocks (Q1[y].commandClient->user_id,Q2[i].commandClient->user_id,Q1[y].commandClient->number_of_stocks,Q1[y].commandClient->number_of_stocks,Q1[y].commandClient->company_id);
						}
						else if(Q1[y].commandClient->number_of_stocks > Q2[i].commandClient->number_of_stocks)
						{
							changeStocks (Q1[y].commandClient->user_id,Q2[i].commandClient->user_id,Q2[i].commandClient->number_of_stocks,Q2[i].commandClient->number_of_stocks,Q1[y].commandClient->company_id);
						}
						addCompletedCommand(y,i);						
						deQueueSell(Q1,Q2,i);
						return;
					}
			}
		}
		// if it selling if the price goes 5% and more
		else if(Q1[y].commandClient->command_type == 2)
		{
			for (int i=0;i <=(Q2->rear);i++)
			{
				if(Q1[y].commandClient->price <= Q2[i].commandClient->price && Q1[y].commandClient->company_id == Q2[i].commandClient->company_id && Q1[y].commandClient->user_id != Q2[i].commandClient->user_id)
				{
					printf("match buy/sell between %d %d for stock %d in price %f \n",Q1[y].commandClient->user_id,Q2[i].commandClient->user_id
					,Q1[y].commandClient->company_id,Q1[y].commandClient->price);
					writeToFile(Q1,Q2,i,checkMark);
					changePriceOfStock(Q1[y].commandClient->company_id,Q2[i].commandClient->price);

					if(Q1[y].commandClient->number_of_stocks == Q2[i].commandClient->number_of_stocks)
					{
						changeStocks (Q1[y].commandClient->user_id,Q2[i].commandClient->user_id,Q2[i].commandClient->number_of_stocks,Q1[y].commandClient->number_of_stocks,Q1[y].commandClient->company_id);
					}
					else if(Q1[y].commandClient->number_of_stocks < Q2[i].commandClient->number_of_stocks)
					{
						changeStocks (Q1[y].commandClient->user_id,Q2[i].commandClient->user_id,Q1[y].commandClient->number_of_stocks,Q1[y].commandClient->number_of_stocks,Q1[y].commandClient->company_id);
					}
					else if(Q1[y].commandClient->number_of_stocks > Q2[i].commandClient->number_of_stocks)
					{
						changeStocks (Q1[y].commandClient->user_id,Q2[i].commandClient->user_id,Q2[i].commandClient->number_of_stocks,Q2[i].commandClient->number_of_stocks,Q1[y].commandClient->company_id);
					}	
					addCompletedCommand(y,i);
					deQueueSell(Q1,Q2,i);
				}
			}
		}
	}
	
	// if checkmark is 2 then the command is for buying
	else if (checkMark ==2)
	{
		int y= Q2->rear;
		// command for buying in a specific price
		if(Q2[y].commandClient->command_type == 3)
		{
			for(int i=0;i<=(Q1->rear);i++)
			{
				if (Q2[y].commandClient->price == Q1[i].commandClient->price && Q2[y].commandClient->company_id == Q1[i].commandClient->company_id && Q1[y].commandClient->user_id != Q2[i].commandClient->user_id)
				{
					printf("match buy/sell between %d %d for stock %d in price %f \n",Q2[y].commandClient->user_id,Q1[i].commandClient->user_id
					,Q2[y].commandClient->company_id,Q2[y].commandClient->price);
					writeToFile(Q2,Q1,i,checkMark);
					changePriceOfStock(Q2[y].commandClient->company_id,Q2[y].commandClient->price);

					if(Q2[y].commandClient->number_of_stocks == Q1[i].commandClient->number_of_stocks)
					{
					 changeStocks(Q1[i].commandClient->user_id,Q2[y].commandClient->user_id,Q1[i].commandClient->number_of_stocks,Q2[y].commandClient->number_of_stocks,Q2[y].commandClient->company_id);
					}
					else if(Q2[y].commandClient->number_of_stocks > Q1[i].commandClient->number_of_stocks)
					{
					 changeStocks(Q1[i].commandClient->user_id,Q2[y].commandClient->user_id,Q1[i].commandClient->number_of_stocks,Q1[i].commandClient->number_of_stocks,Q2[y].commandClient->company_id);
					}
					else if(Q2[y].commandClient->number_of_stocks < Q1[i].commandClient->number_of_stocks)
					{
					 changeStocks(Q1[i].commandClient->user_id,Q2[y].commandClient->user_id,Q2[y].commandClient->number_of_stocks,Q2[y].commandClient->number_of_stocks,Q2[y].commandClient->company_id);
					}

					addCompletedCommand(i,y);
					deQueueBuy(Q1,Q2,i);
					return;
				}
			}
		}
		// command for buying if price goes bellow 5%
		else if(Q2[y].commandClient->command_type == 4)
		{
			for(int i=0;i<=(Q1->rear);i++)
			{
				if (Q2[y].commandClient->price >= Q1[i].commandClient->price && Q2[y].commandClient->company_id == Q1[i].commandClient->company_id && Q1[y].commandClient->user_id != Q2[i].commandClient->user_id)
				{
					printf("match buy/sell between %d %d for stock %d in price %f \n",Q2[y].commandClient->user_id,Q1[i].commandClient->user_id
					,Q2[y].commandClient->company_id,Q2[y].commandClient->price);
					writeToFile(Q2,Q1,i,checkMark);
					changePriceOfStock(Q2[y].commandClient->company_id,Q1[y].commandClient->price);
				
					if(Q2[y].commandClient->number_of_stocks == Q1[i].commandClient->number_of_stocks)
					{
					 changeStocks(Q1[i].commandClient->user_id,Q2[y].commandClient->user_id,Q1[i].commandClient->number_of_stocks,Q2[y].commandClient->number_of_stocks,Q2[y].commandClient->company_id);
					}
					else if(Q2[y].commandClient->number_of_stocks > Q1[i].commandClient->number_of_stocks)
					{
					 changeStocks(Q1[i].commandClient->user_id,Q2[y].commandClient->user_id,Q1[i].commandClient->number_of_stocks,Q1[i].commandClient->number_of_stocks,Q2[y].commandClient->company_id);
					}
					else if(Q2[y].commandClient->number_of_stocks < Q1[i].commandClient->number_of_stocks)
					{
					 changeStocks(Q1[i].commandClient->user_id,Q2[y].commandClient->user_id,Q2[y].commandClient->number_of_stocks,Q2[y].commandClient->number_of_stocks,Q2[y].commandClient->company_id);
					}
					addCompletedCommand(i,y);
					deQueueBuy(Q1,Q2,i);
					return;
				}
			}			
		}
	}
}


void writeToFile(queue* Q1, queue* Q2,int i,int checkMark)
{
	
	// open file to write matching 
	FILE *fp;
	fp = fopen("match_commands.txt","a+");

	// get time of the match
	char log_buff[30];
	int time_len= 0;
	struct tm *tm_info;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	tm_info=localtime(&tv.tv_sec);
	time_len+=strftime(log_buff,sizeof log_buff,"%y/%m/%d %H:%M:%S",tm_info);
	int sussec = tv.tv_usec/1000;
    if (checkMark==1)
	{
	// write to file	
	fprintf(fp,"%0.2f %0.2f %d %0.3f %s:%d\n",Q1[Q1->rear].commandClient->command_id,Q2[i].commandClient->command_id,Q2[i].commandClient->number_of_stocks,Q1[Q1->rear].commandClient->price,log_buff,sussec);
	}
	else if(checkMark==2)
	{
	// write to file 
	fprintf(fp,"%0.2f %0.2f %d %0.3f %s:%d\n",Q1[i].commandClient->command_id,Q2[Q2->rear].commandClient->command_id,Q1[i].commandClient->number_of_stocks,Q2[Q2->rear].commandClient->price,log_buff,sussec);	
	}
	fclose(fp);
}

const char* timestamp()
{

	// create a timestamp
	char log_buff[30];
	char temp_buff[10];
	char * buffer;
	buffer=(char*)malloc(sizeof(char)*30);
	int time_len= 0;
	struct tm *tm_info;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	tm_info=localtime(&tv.tv_sec);
	time_len+=strftime(log_buff,sizeof log_buff,"%y/%m/%d %H:%M:%S",tm_info);
	int sussec = tv.tv_usec/1000;
	sprintf(temp_buff,":%d",sussec);
	strcat(log_buff,temp_buff); 
	strcpy(buffer,log_buff);
	
	return buffer;	
}

void changePriceOfStock(int company_id,float price)
{
	// change prices in prices array
	
	for (int k=0; k<20;k++)
	{
		if(company_id == array[k].company_id)
		{
			printf("old price %0.3f \n",array[k].price);
			array[k].price = price;
			printf("new price %0.3f \n",array[k].price);
		}
	}
	return;
}

void changeStocks (int user_sold,int user_bought,int stocks_bought, int stocks_sold,int company_id)
{
	// change stocks for each client	
	// go to a specified area of the stocks array based on user's id
	switch (user_bought)
	{
		case 1:
			for (int i=0;i<20;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks + stocks_sold;
			 	 printf("user bought new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		case 2:
			for (int i=20;i<40;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks + stocks_sold;
			 	 printf("user bought new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}		
		break;
		case 3:
			for (int i=40;i<60;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks + stocks_sold;
			 	 printf("user bought new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}		
		break;
		case 4:
			for (int i=60;i<80;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks + stocks_sold;
			 	 printf("user bought new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		case 5:
			for (int i=80;i<100;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks + stocks_sold;
			 	 printf("user bought new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		case 6:
			for (int i=100;i<120;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks + stocks_sold;
			 	 printf("user bought new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		case 7:
			for (int i=120;i<140;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks + stocks_sold;
			 	 printf("user bought new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		case 8:
			for (int i=140;i<160;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks + stocks_sold;
			 	 printf("user bought new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		case 9:
			for (int i=160;i<180;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks + stocks_sold;
			 	 printf("user bought new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		case 10:
			for (int i=180;i<200;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks + stocks_sold;
			 	 printf("user bought new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		default:
			printf("error \n");
		break;
	}
	switch (user_sold)
	{
		case 1:
			for (int i=0;i<20;i++)
		{
			if(user_stock[i].company_id == company_id)
			{
			  user_stock[i].number_of_stocks = user_stock[i].number_of_stocks - stocks_sold;
			  printf("user sold new stocks %d\n",user_stock[i].number_of_stocks);
			}
		}
		break;
		case 2:
			for (int i=20;i<40;i++)
		{
			if(user_stock[i].company_id == company_id)
			{
			  user_stock[i].number_of_stocks = user_stock[i].number_of_stocks - stocks_sold;
			  printf("user sold new stocks %d\n",user_stock[i].number_of_stocks);
			}
		}
		break;
		case 3:
			for (int i=40;i<60;i++)
		{
			if(user_stock[i].company_id == company_id)
			{
			  user_stock[i].number_of_stocks = user_stock[i].number_of_stocks - stocks_sold;
			  printf("user sold new stocks %d\n",user_stock[i].number_of_stocks);
			}
		}
		break;
		case 4:
			for (int i=60;i<80;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks - stocks_sold;
			 	 printf("user sold new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		case 5:
			for (int i=80;i<100;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks - stocks_sold;
			  	printf("user sold new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		case 6:
			for (int i=100;i<120;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks - stocks_sold;
			  	printf("user sold new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		case 7:
			for (int i=120;i<140;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks - stocks_sold;
			  	printf("user sold new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}		
		break;
		case 8:
			for (int i=140;i<160;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks - stocks_sold;
			  	printf("user sold new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		case 9:
			for (int i=160;i<180;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks - stocks_sold;
			  	printf("user sold new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		case 10:
			for (int i=180;i<200;i++)
			{
				if(user_stock[i].company_id == company_id)
				{
			 	 user_stock[i].number_of_stocks = user_stock[i].number_of_stocks - stocks_sold;
			  	printf("user sold new stocks %d\n",user_stock[i].number_of_stocks);
				}
			}
		break;
		default:
		break;
		
	}	
		
}
int checkStock(receiveCommand entoli)
{
	// check if client has the stocks that he is going to sell
	switch (entoli.user_id)
	{
		case 1:
			for(int i=0; i<20;i++)
			{
				if(entoli.company_id == user_stock[i].company_id)
				{
					if(entoli.number_of_stocks <= user_stock[i].number_of_stocks)
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}
		break;
		case 2:
			for(int i=20; i<40;i++)
			{
				if(entoli.company_id == user_stock[i].company_id)
				{
					if(entoli.number_of_stocks <= user_stock[i].number_of_stocks)
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}
		break;
		case 3:
			for(int i=40; i<60;i++)
			{
				if(entoli.company_id == user_stock[i].company_id)
				{
					if(entoli.number_of_stocks <= user_stock[i].number_of_stocks)
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}		

		break;
		case 4:
			for(int i=60; i<80;i++)
			{
				if(entoli.company_id == user_stock[i].company_id)
				{
					if(entoli.number_of_stocks <= user_stock[i].number_of_stocks)
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}
		break;
		case 5:
			for(int i=80; i<100;i++)
			{
				if(entoli.company_id == user_stock[i].company_id)
				{
					if(entoli.number_of_stocks <= user_stock[i].number_of_stocks)
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}
		break;
		case 6:
			for(int i=100; i<120;i++)
			{
				if(entoli.company_id == user_stock[i].company_id)
				{
					if(entoli.number_of_stocks <= user_stock[i].number_of_stocks)
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}
		break;
		case 7:
			for(int i=120; i<140;i++)
			{
				if(entoli.company_id == user_stock[i].company_id)
				{
					if(entoli.number_of_stocks <= user_stock[i].number_of_stocks)
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}
		break;
		case 8:
			for(int i=140; i<160;i++)
			{
				if(entoli.company_id == user_stock[i].company_id)
				{
					if(entoli.number_of_stocks <= user_stock[i].number_of_stocks)
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}
		break;
		case 9:
			for(int i=160; i<180;i++)
			{
				if(entoli.company_id == user_stock[i].company_id)
				{
					if(entoli.number_of_stocks <= user_stock[i].number_of_stocks)
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}
		break;
		case 10:
			for(int i=180; i<200;i++)
			{
				if(entoli.company_id == user_stock[i].company_id)
				{
					if(entoli.number_of_stocks <= user_stock[i].number_of_stocks)
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}
		break;
		default:
		break;

	}
	return 0;
}



void addCompletedCommand(int Q1_mark,int Q2_mark)
{
	// add completed commands to the specified array
	int check_mark = -1;

	for(int i =0;i<completed_mark;i++)
	{
		if(commandCompleted[i].user_id == 0)
		{
			check_mark = i;
			break;
		}
		else
		{
			check_mark = -1;
		}
	}
	if(check_mark != -1)
	{
		commandCompleted[check_mark].user_id=Q1[Q1_mark].commandClient->user_id;
		commandCompleted[check_mark].company_id=Q1[Q1_mark].commandClient->company_id;

		if(Q1[Q1_mark].commandClient->number_of_stocks >=Q2[Q2_mark].commandClient->number_of_stocks)
		{
			
			commandCompleted[check_mark].number_of_stocks=Q2[Q2_mark].commandClient->number_of_stocks;
		}
		else if(Q1[Q1_mark].commandClient->number_of_stocks < Q2[Q2_mark].commandClient->number_of_stocks)
		{
			
			commandCompleted[check_mark].number_of_stocks = Q1[Q1_mark].commandClient->number_of_stocks;
		}
		
		commandCompleted[check_mark].price=Q1[Q1_mark].commandClient->price;
		commandCompleted[check_mark].command_type=Q1[Q1_mark].commandClient->command_type;
		
	}
	else
	{
		commandCompleted[completed_mark].user_id=Q1[Q1_mark].commandClient->user_id;
		commandCompleted[completed_mark].company_id=Q1[Q1_mark].commandClient->company_id;
		if(Q1[Q1_mark].commandClient->number_of_stocks >=Q2[Q2_mark].commandClient->number_of_stocks)
		{
			
			commandCompleted[completed_mark].number_of_stocks=Q2[Q2_mark].commandClient->number_of_stocks;
		}
		else if(Q1[Q1_mark].commandClient->number_of_stocks < Q2[Q2_mark].commandClient->number_of_stocks)
		{
			
			commandCompleted[completed_mark].number_of_stocks=Q1[Q1_mark].commandClient->number_of_stocks;
		}
		commandCompleted[completed_mark].price=Q1[Q1_mark].commandClient->price;
		commandCompleted[completed_mark].command_type=Q1[Q1_mark].commandClient->command_type;
		completed_mark++;
	}

	// arxi queue 2
	for(int i =0;i<completed_mark;i++)
	{
		if(commandCompleted[i].user_id == 0)
		{
			check_mark = i;
			break;
		}
		else
		{

			check_mark = -1;
		}
	}
	
	if(check_mark != -1)
	{
		commandCompleted[check_mark].user_id=Q2[Q2_mark].commandClient->user_id;
		commandCompleted[check_mark].company_id=Q2[Q2_mark].commandClient->company_id;
		if(Q1[Q1_mark].commandClient->number_of_stocks >=Q2[Q2_mark].commandClient->number_of_stocks)
		{
			
			commandCompleted[check_mark].number_of_stocks=Q2[Q2_mark].commandClient->number_of_stocks;
		}
		else if(Q1[Q1_mark].commandClient->number_of_stocks < Q2[Q2_mark].commandClient->number_of_stocks)
		{
			
			commandCompleted[check_mark].number_of_stocks=Q1[Q1_mark].commandClient->number_of_stocks;
		}
		commandCompleted[check_mark].price=Q2[Q2_mark].commandClient->price;
		commandCompleted[check_mark].command_type=Q2[Q2_mark].commandClient->command_type;
	}

	else
	{
	commandCompleted[completed_mark].user_id=Q2[Q2_mark].commandClient->user_id;
	commandCompleted[completed_mark].company_id=Q2[Q2_mark].commandClient->company_id;
	if(Q1[Q1_mark].commandClient->number_of_stocks >=Q2[Q2_mark].commandClient->number_of_stocks)
	{
		
		commandCompleted[completed_mark].number_of_stocks=Q2[Q2_mark].commandClient->number_of_stocks;
	}
	else if(Q1[Q1_mark].commandClient->number_of_stocks < Q2[Q2_mark].commandClient->number_of_stocks)
	{

		commandCompleted[completed_mark].number_of_stocks=Q1[Q1_mark].commandClient->number_of_stocks;
	}
	commandCompleted[completed_mark].price=Q2[Q2_mark].commandClient->price;
	commandCompleted[completed_mark].command_type=Q2[Q2_mark].commandClient->command_type;	
	completed_mark++;
	}
	return;	

}

