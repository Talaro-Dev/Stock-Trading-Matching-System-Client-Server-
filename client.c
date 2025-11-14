#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "struct.h"

#define IP "127.0.0.1"
#define PORT 1234
#include <stdio.h>

void user_interface(int clientSocket,int user_id);
void menu();
void send_information(int clientSocket,int choice,int user_id);
int connect_to_server();
void insert_from_file(int clientSocket);
void receive_from_server(int clientSocket,int user_id);

int main (int argc, char* argv[])
{
	int user_id;
	printf("give your id"); //give client's id as login
	scanf("%d",&user_id);
    int clientSocket = connect_to_server();
	user_interface(clientSocket,user_id);
    return 0;
}

int connect_to_server()
{
    int clientSocket, ret;
    struct sockaddr_in serverAddr;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0); //Create an endpoint for communication.
	if(clientSocket < 0){
		printf("Error in connection.\n");
		exit(1);
	}
    memset(&serverAddr, '\0', sizeof(serverAddr)); //Fill a block of memory with value "\0"
	serverAddr.sin_family = AF_INET;      
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(IP);

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); //connect on a socket
	if(ret < 0){
		printf("Error in connection.\n");
		exit(1);
	}
	printf("Connected to Server.\n");
    return clientSocket;
}


void user_interface(int clientSocket,int user_id)
{	


	receive_from_server(clientSocket,user_id);

	 int choice;
	menu();
	scanf("%d",&choice);
	
	while(choice!=7)
	{
		switch(choice)
		{
			case 1:
			send_information(clientSocket,choice,user_id);
			break;
			case 2:
			send_information(clientSocket,choice,user_id);
			break;
			case 3:
			send_information(clientSocket,choice,user_id);
			break;
			case 4:
			send_information(clientSocket,choice,user_id);
			break;
			case 5:
			send_information(clientSocket,choice,user_id);
			// na stelnei mazika tis entoles 
			break;
			case 6:
			send_information(clientSocket,choice,user_id);
			break;
			default:
			printf("wrong number \n\n");
			break;		
		}
		// receive panta message me vasi deikti an o deiktis einai 1 pes tou poia poulithikan 
		// kai poia tou exoun meinei
		menu();
		scanf("%d",&choice);
	
	}send_information(clientSocket,choice,user_id);
	return;
}

void menu()
{
	printf("Select one of the following commands.\n");
	printf("1) Sell a number of stocks for a specific price. \n");
	printf("2) Sell if the price of the stock goes above 5 %%. \n");
	printf("3) Buy stock in a specific price. \n");
	printf("4) Buy if the price of the stock goes below 5 %%. \n");
	printf("5) Insert from file (testing). \n");
	printf("6) Cancel any of your commands. \n");
	printf("7) Exit \n");

	return;

}

void send_information(int clientSocket,int choice,int user_id)
{
	commandFromClient entoli;

	// commands menu based on his choice
	if(choice == 1)
	{
		entoli.user_id = user_id;
		entoli.command_type = 1;
		printf("Give the id of the stock you want to sell:");
		scanf("%d",&entoli.company_id);
		printf("Give the number of stock you want to sell:");
		scanf("%d",&entoli.number_of_stocks);
		printf("Give the price you want to sell:");
		scanf("%f",&entoli.price);
		if (send(clientSocket,(void*)&entoli,sizeof(commandFromClient),0)<0)
		{
			printf("error");
		}		
	}
	else if (choice == 2)
	{
		entoli.user_id = user_id;
		entoli.command_type = 2;
		printf("Give the number of stock you want to sell:");
		scanf("%d",&entoli.number_of_stocks);
		printf("Give the id of the stock you want to sell:");
		scanf("%d",&entoli.company_id);
		if (send(clientSocket,(void*)&entoli,sizeof(commandFromClient),0)<0)
		{
			printf("error");
		}		
		
		return;		
	}
	else if (choice == 3)
	{
		entoli.user_id = user_id;
		entoli.command_type = 3;
		printf("Give the number of stocks you want to buy:");
		scanf("%d",&entoli.number_of_stocks);
		printf("Give the id of the stock you want to buy:");
		scanf("%d",&entoli.company_id);
		printf("Give the price you want to buy:");
		scanf("%f",&entoli.price);

		if (send(clientSocket,(void*)&entoli,sizeof(commandFromClient),0)<0)
		{
			printf("error");
		}		
		
		return;
	}
	else if (choice == 4)
	{
		entoli.user_id = user_id;
		entoli.command_type = 4;
		printf("Give the number of stocks you want to buy:");
		scanf("%d",&entoli.number_of_stocks);
		printf("Give the id of the stock you want to buy:");
		scanf("%d",&entoli.company_id);

		if (send(clientSocket,(void*)&entoli,sizeof(commandFromClient),0)<0)
		{
			printf("error");
		}		
		
		return;
	}
	else if (choice == 5)
	{

		insert_from_file(clientSocket);	
		
		return;
	}
	else if (choice == 6)
	{
		//init command for delete
		commandForDelete ent; 
		entoli.user_id = user_id;
		entoli.command_type = 6;
		entoli.price=0.0;
		entoli.number_of_stocks=0;
		entoli.company_id=0;
		ent.user_id=user_id;
		printf("Give the company's id:");
		scanf("%d",&ent.company_id);
		printf("sell or buy command (1,2)");
		scanf("%d",&ent.command_type_for_delete);
		printf("Give the stock you want to delete (exact number you have previously entered):");
		scanf("%d",&ent.number_of_stocks);
		printf("Give the price of the stocks you have gave:");
		scanf("%f",&ent.price);
		// send entoli in order server to recognise command type 6
		if (send(clientSocket,(void*)&entoli,sizeof(commandFromClient),0)<0)
		{
			printf("error");
		}
		//send the command that is going to be deleted 
		if (send(clientSocket,(void*)&ent,sizeof(commandForDelete),0)<0)
		{
			printf("error");
		}	
		
	}
	else if (choice == 7)
	{
		entoli.user_id = user_id;
		entoli.command_type = 7;
		
		if (send(clientSocket,(void*)&entoli,sizeof(commandFromClient),0)<0)
		{
			printf("error");
		}		
		
		return;
	

	}
	return;
}

void insert_from_file(int clientSocket)
{
	// open file 
	 file txt;
	 receiveCommand entoli;
	FILE* fp;
	// give space to token in order to get from files
	char *tok =(char*)malloc(sizeof(char)*256);

	if((fp=fopen("file.txt","r")) == NULL)
		{
			printf("Error opening file \n");
			exit(1);
		}
		
	char line[256];
	while (fgets(line,100,fp))
	{
		
		tok = strtok(line," ");
		txt.user_id = atoi(tok);
		tok = strtok(NULL," ");
		txt.company_id = atoi(tok);
		tok = strtok(NULL," ");
		txt.command_type = atoi(tok);
		tok = strtok(NULL," ");
		txt.number_of_stocks = atoi(tok);

		// if command type is 1 or 3 get also the price from the file
		if (txt.command_type == 1 || txt.command_type == 3)
		{
			tok = strtok(NULL," ");
			txt.price = atof(tok);
			
		}
		else
		{
			txt.price =0;
		
		}
		entoli.user_id = txt.user_id;
		entoli.command_type = txt.command_type;
		entoli.company_id=txt.company_id;
		entoli.number_of_stocks=txt.number_of_stocks;
		entoli.price = txt.price;
		
		// send command to client
		if (send(clientSocket,(void*)&entoli,sizeof(commandFromClient),0)<0)
		{
			printf("error");
		}	

	}
	
	// close file
	fclose(fp);
	return;
}

void receive_from_server(int clientSocket,int user_id)
{
	sendToClient client; //receive commands completed or not when client logins
	commandToClient *commandForUser; 
	commandForUser=(commandToClient*)malloc(sizeof(commandToClient)*100); //allocate space for command

	// send id to server
	if(send(clientSocket,&user_id,sizeof(int),0)<0)
	{
		printf("error \n");
	}

	// receive completed and uncompleted commands
		if(recv(clientSocket,&client,sizeof(sendToClient),0)<0)
	{
		printf("error \n");
	}
	else
	{
	}

	if(client.mark != 0)
	{
		if(recv(clientSocket,commandForUser,sizeof(commandToClient)*100,0)<0)
		{
			printf("error \n");
		}
		else
		{
			printf("commands completed \n");
			for(int i=0;i<client.mark;i++)
				{ 
					
					if(commandForUser[i].completed == 1)
					{
						printf("%d %d %d %0.2f \n",commandForUser[i].company_id,commandForUser[i].command_type,commandForUser[i].number_of_stocks,commandForUser[i].price);
					}
			
					
				}
			printf("uncompleted commands \n");
			for(int i =0;i<client.mark;i++)
			{
				if(commandForUser[i].completed == 0)
				{
					printf("%d %d %d %02.f \n",commandForUser[i].company_id,commandForUser[i].command_type,commandForUser[i].number_of_stocks,commandForUser[i].price);
				}
			}
		}
	}
}