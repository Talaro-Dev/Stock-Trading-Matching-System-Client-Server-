void deQueueQ1Sell(queue*Q1);
void deQueueQ2Sell(queue* Q2,int i);
void deQueueBuy(queue* Q1, queue* Q2,int i);
void deQueueQ1Buy(queue* Q1, int i);
void deQueueQ2Buy(queue* Q2);
void deQueueSell (queue* Q1, queue* Q2,int i);
void deQueueCommand(queue *Q1,queue *Q2,commandForDelete entoli);

void deQueueQ1Sell (queue* Q1)
{
	// if front == rear that means that we only have one element in queue
	if (Q1->front == Q1->rear)
	{	
		
		Q1[0].commandClient->number_of_stocks = 0;
		Q1[0].commandClient->company_id =0;
		Q1[0].commandClient->user_id=0 ;
		strcpy(Q1[0].commandClient->timestamp,"0");
		Q1[0].commandClient->command_type=0;
		Q1[0].commandClient->command_id = 0;
		Q1[0].commandClient->price= 0 ;
		Q1->front = Q1->front -1;
		Q1->rear = Q1->rear -1;
	}
	else
	{
		// firtst move the last command to a temp and move every command in queue one position right, then move the temp to the first position in order to be in front and get deleted
		// then move every element one position to the left and make rear smaller by one
		commands temp;
		int y = Q1->rear;
		temp.number_of_stocks = Q1[y].commandClient->number_of_stocks;
		temp.company_id = Q1[y].commandClient->company_id;
		temp.user_id = Q1[y].commandClient->user_id;
		strcpy(temp.timestamp,Q1[y].commandClient->timestamp);
		temp.command_id = Q1[y].commandClient->command_id;
		temp.command_type = Q1[y].commandClient->command_type;
		temp.price = Q1[y].commandClient->price;

		for(int j = (Q1->rear);j>0;j--)
		{
			
			Q1[j] = Q1[j-1];
		}
		
		Q1[0].commandClient->number_of_stocks = temp.number_of_stocks;
		Q1[0].commandClient->company_id = temp.company_id;
		Q1[0].commandClient->user_id= temp.user_id ;
		Q1[0].commandClient->command_id=temp.command_id ;
		strcpy(Q1[0].commandClient->timestamp,temp.timestamp);
		Q1[0].commandClient->command_type= temp.command_type;
		Q1[0].commandClient->price= temp.price ;

		for (int j = 0 ; j<y;j++)
		{
			Q1[j] = Q1[j+1];
		}
		Q1[Q1->rear].commandClient->number_of_stocks = 0;
		Q1[Q1->rear].commandClient->company_id =0;
		Q1[Q1->rear].commandClient->command_id =0;
		Q1[Q1->rear].commandClient->user_id=0 ;
		Q1[Q1->rear].commandClient->command_type=0;
		strcpy(Q1[Q1->rear].commandClient->timestamp,"0");
		Q1[Q1->rear].commandClient->price= 0 ;
		Q1->rear = Q1->rear-1;
	}
	return;
}
void deQueueQ2Sell(queue* Q2,int i)
{

	
	if (Q2->front == Q2->rear)
			{	
				Q2[0].commandClient->number_of_stocks = 0;
				Q2[0].commandClient->company_id =0;
				Q2[0].commandClient->user_id=0 ;
				Q2[0].commandClient->command_id=0;
				strcpy(Q2[0].commandClient->timestamp,"0");
				Q2[0].commandClient->command_type=0;
				Q2[0].commandClient->price= 0 ;
				Q2->front = Q2->front -1;
				Q2->rear = Q2->rear -1;
			}
			else
			// firtst move the last command to a temp and move every command in queue one position right, then move the temp to the first position in order to be in front and get deleted
			// then move every element one position to the left and make rear smaller by one
			{
			commands temp2;
			temp2.number_of_stocks = Q2[i].commandClient->number_of_stocks;
			temp2.company_id = Q2[i].commandClient->company_id;
			temp2.command_id = Q2[i].commandClient->command_id;
			temp2.user_id = Q2[i].commandClient->user_id;
			strcpy(temp2.timestamp,Q2[i].commandClient->timestamp);
			temp2.command_type = Q2[i].commandClient->command_type;
			temp2.price = Q2[i].commandClient->price;
			
			while(i > 0)
			{	
				Q2[i].commandClient->command_type = Q2[i-1].commandClient->command_type;
				Q2[i].commandClient->number_of_stocks = Q2[i-1].commandClient->number_of_stocks;
				Q2[i].commandClient->company_id = Q2[i-1].commandClient->company_id;
				Q2[i].commandClient->command_id = Q2[i-1].commandClient->command_id;
				Q2[i].commandClient->price = Q2[i-1].commandClient->price;
				strcpy(Q2[i].commandClient->timestamp,Q2[i-1].commandClient->timestamp);
				Q2[i].commandClient->user_id = Q2[i-1].commandClient->user_id;
				i--;
			}
			
			Q2[0].commandClient->number_of_stocks = temp2.number_of_stocks;
			Q2[0].commandClient->company_id = temp2.company_id;
			Q2[0].commandClient->user_id= temp2.user_id ;
			Q2[0].commandClient->command_id= temp2.command_id ;
			strcpy(Q2[0].commandClient->timestamp,temp2.timestamp);
			Q2[0].commandClient->command_type= temp2.command_type;
			Q2[0].commandClient->price= temp2.price ;
			
			for(int j =0 ; j<(Q2->rear);j++)
			{
				Q2[j].commandClient->command_type = Q2[j+1].commandClient->command_type;
				Q2[j].commandClient->number_of_stocks = Q2[j+1].commandClient->number_of_stocks;
				Q2[j].commandClient->company_id = Q2[j+1].commandClient->company_id;
				strcpy(Q2[j].commandClient->timestamp,Q2[j+1].commandClient->timestamp);
				Q2[j].commandClient->command_id= Q2[j+1].commandClient->command_id;
				Q2[j].commandClient->price = Q2[j+1].commandClient->price;
				Q2[j].commandClient->user_id = Q2[j+1].commandClient->user_id;	
				
			}
			Q2[Q2->rear].commandClient->number_of_stocks = 0;
			Q2[Q2->rear].commandClient->company_id =0;
			Q2[Q2->rear].commandClient->command_id=0;
			Q2[Q2->rear].commandClient->user_id=0 ;
			strcpy(Q2[Q2->rear].commandClient->timestamp,"0");
			Q2[Q2->rear].commandClient->command_type=0;
			Q2[Q2->rear].commandClient->price= 0 ;
			Q2->rear = Q2->rear -1;
			}
	return;
}

void deQueueSell (queue* Q1, queue* Q2,int i)
{
		// dequeue or change the stocks based on the stocks that matched (equal,smaller,bigger)
		int value;
			if (Q1[Q1->rear].commandClient->number_of_stocks == Q2[i].commandClient->number_of_stocks)
		{
			deQueueQ1Sell(Q1);
			deQueueQ2Sell(Q2,i);
		}

		else if (Q1[Q1->rear].commandClient->number_of_stocks > Q2[i].commandClient ->number_of_stocks)
		{
			value = (Q1[Q1->rear].commandClient->number_of_stocks) - (Q2[i].commandClient->number_of_stocks);
			Q1[Q1->rear].commandClient->number_of_stocks = value ;
			deQueueQ2Sell(Q2,i);
		
		}
		
		else if (Q1[Q1->rear].commandClient->number_of_stocks < Q2[i].commandClient ->number_of_stocks)
		{
			
			value = (Q2[i].commandClient->number_of_stocks) - (Q1[Q1->rear].commandClient->number_of_stocks);
			Q2[i].commandClient ->number_of_stocks = value;
			
			
			deQueueQ1Sell(Q1);
		}
		
	

		return;
}

void deQueueBuy(queue* Q1, queue* Q2,int i)
{
		// dequeue or change the stocks based on the stocks that matched (equal,smaller,bigger)
		int value;
			if (Q2[Q2->rear].commandClient->number_of_stocks == Q1[i].commandClient->number_of_stocks)
		{
			deQueueQ2Buy(Q2);
			deQueueQ1Buy(Q1,i);
		}

		else if (Q2[Q2->rear].commandClient->number_of_stocks > Q1[i].commandClient ->number_of_stocks)
		{
			value = (Q2[Q2->rear].commandClient->number_of_stocks) - (Q1[i].commandClient->number_of_stocks);
			Q2[Q1->rear].commandClient->number_of_stocks = value ;
			deQueueQ1Buy(Q1,i);
		
		}
		
		else if (Q2[Q2->rear].commandClient->number_of_stocks < Q1[i].commandClient ->number_of_stocks)
		{
			
			value = (Q1[i].commandClient->number_of_stocks) - (Q2[Q2->rear].commandClient->number_of_stocks);
			Q1[i].commandClient ->number_of_stocks = value;
			
			
			deQueueQ2Buy(Q2);
		}
			return;

}

void deQueueQ2Buy(queue* Q2)
{
	if (Q2->front == Q2->rear)
	{	
		
		Q2[0].commandClient->number_of_stocks = 0;
		Q2[0].commandClient->company_id =0;
		Q2[0].commandClient->user_id=0 ;
		strcpy(Q2[0].commandClient->timestamp,"0");
		Q2[0].commandClient->command_type=0;
		Q2[0].commandClient->command_id = 0;
		Q2[0].commandClient->price= 0 ;
		Q2->front = Q2->front -1;
		Q2->rear = Q2->rear -1;
	}
	else
	{
		commands temp;
		int y = Q2->rear;
		temp.number_of_stocks = Q2[y].commandClient->number_of_stocks;
		temp.company_id = Q2[y].commandClient->company_id;
		temp.user_id = Q2[y].commandClient->user_id;
		strcpy(temp.timestamp,Q2[y].commandClient->timestamp);
		temp.command_id = Q2[y].commandClient->command_id;
		temp.command_type = Q2[y].commandClient->command_type;
		temp.price = Q2[y].commandClient->price;

		for(int j = (Q2->rear);j>0;j--)
		{
			
			Q2[j] = Q2[j-1];
		}
		
		Q2[0].commandClient->number_of_stocks = temp.number_of_stocks;
		Q2[0].commandClient->company_id = temp.company_id;
		Q2[0].commandClient->user_id= temp.user_id ;
		Q2[0].commandClient->command_id=temp.command_id ;
		strcpy(Q2[0].commandClient->timestamp,temp.timestamp);
		Q2[0].commandClient->command_type= temp.command_type;
		Q2[0].commandClient->price= temp.price ;

		for (int j = 0 ; j<y;j++)
		{
			Q2[j] = Q2[j+1];
		}
		Q2[Q2->rear].commandClient->number_of_stocks = 0;
		Q2[Q2->rear].commandClient->company_id =0;
		Q2[Q2->rear].commandClient->command_id =0;
		Q2[Q2->rear].commandClient->user_id=0 ;
		Q2[Q2->rear].commandClient->command_type=0;
		strcpy(Q2[Q2->rear].commandClient->timestamp,"0");
		Q2[Q2->rear].commandClient->price= 0 ;
		Q2->rear = Q2->rear-1;
	}
	return;
}

void deQueueQ1Buy(queue* Q1,int i)
{
	if (Q1->front == Q1->rear)
			{	
				Q1[0].commandClient->number_of_stocks = 0;
				Q1[0].commandClient->company_id =0;
				Q1[0].commandClient->user_id=0 ;
				Q1[0].commandClient->command_id=0;
				strcpy(Q1[0].commandClient->timestamp,"0");
				Q1[0].commandClient->command_type=0;
				Q1[0].commandClient->price= 0 ;
				Q1->front = Q1->front -1;
				Q1->rear = Q1->rear -1;
			}
			else
			{
			commands temp2;
			temp2.number_of_stocks = Q1[i].commandClient->number_of_stocks;
			temp2.company_id = Q1[i].commandClient->company_id;
			temp2.command_id = Q1[i].commandClient->command_id;
			temp2.user_id = Q1[i].commandClient->user_id;
			strcpy(temp2.timestamp,Q1[i].commandClient->timestamp);
			temp2.command_type = Q1[i].commandClient->command_type;
			temp2.price = Q1[i].commandClient->price;
			
			while(i > 0)
			{	
				Q1[i].commandClient->command_type = Q1[i-1].commandClient->command_type;
				Q1[i].commandClient->number_of_stocks = Q1[i-1].commandClient->number_of_stocks;
				Q1[i].commandClient->company_id = Q1[i-1].commandClient->company_id;
				Q1[i].commandClient->command_id = Q1[i-1].commandClient->command_id;
				Q1[i].commandClient->price = Q1[i-1].commandClient->price;
				strcpy(Q1[i].commandClient->timestamp,Q1[i-1].commandClient->timestamp);
				Q1[i].commandClient->user_id = Q1[i-1].commandClient->user_id;
				i--;
			}
			
			Q1[0].commandClient->number_of_stocks = temp2.number_of_stocks;
			Q1[0].commandClient->company_id = temp2.company_id;
			Q1[0].commandClient->user_id= temp2.user_id ;
			Q1[0].commandClient->command_id= temp2.command_id ;
			strcpy(Q1[0].commandClient->timestamp,temp2.timestamp);
			Q1[0].commandClient->command_type= temp2.command_type;
			Q1[0].commandClient->price= temp2.price ;
			
			for(int j =0 ; j<(Q1->rear);j++)
			{
				Q1[j].commandClient->command_type = Q1[j+1].commandClient->command_type;
				Q1[j].commandClient->number_of_stocks = Q1[j+1].commandClient->number_of_stocks;
				Q1[j].commandClient->company_id = Q1[j+1].commandClient->company_id;
				strcpy(Q1[j].commandClient->timestamp,Q1[j+1].commandClient->timestamp);
				Q1[j].commandClient->command_id= Q1[j+1].commandClient->command_id;
				Q1[j].commandClient->price = Q1[j+1].commandClient->price;
				Q1[j].commandClient->user_id = Q1[j+1].commandClient->user_id;	
				
			}
			Q1[Q1->rear].commandClient->number_of_stocks = 0;
			Q1[Q1->rear].commandClient->company_id =0;
			Q1[Q1->rear].commandClient->command_id=0;
			Q1[Q1->rear].commandClient->user_id=0 ;
			strcpy(Q1[Q1->rear].commandClient->timestamp,"0");
			Q1[Q1->rear].commandClient->command_type=0;
			Q1[Q1->rear].commandClient->price= 0 ;
			Q1->rear = Q1->rear -1;
			}
	return;
}

void deQueueCommand(queue *Q1,queue *Q2,commandForDelete entoli)
{
    // dequeue command that client wants to get deleted with the same algorithm that used before
    int mark = -1;
    if(entoli.command_type_for_delete == 1)
    {
        for (int i =0;i<=(Q1->rear);i++)
        {
            if (entoli.company_id == Q1[i].commandClient->company_id && entoli.user_id == Q1[i].commandClient->user_id && entoli.number_of_stocks == Q1[i].commandClient->number_of_stocks)
            {
               
                mark = i;
                
            }
		}
          if(mark < 0)
            {
                return;
            }
        	else if (mark>=0 && (Q1->front == Q1->rear))
			{	
				Q1[0].commandClient->number_of_stocks = 0;
				Q1[0].commandClient->company_id =0;
				Q1[0].commandClient->user_id=0 ;
				Q1[0].commandClient->command_id=0;
				strcpy(Q1[0].commandClient->timestamp,"0");
				Q1[0].commandClient->command_type=0;
				Q1[0].commandClient->price= 0 ;
				Q1->front = Q1->front -1;
				Q1->rear = Q1->rear -1;
			}
			else if(mark >= 0)
			{
			commands temp2;
			temp2.number_of_stocks = Q1[mark].commandClient->number_of_stocks;
			temp2.company_id = Q1[mark].commandClient->company_id;
			temp2.command_id = Q1[mark].commandClient->command_id;
			temp2.user_id = Q1[mark].commandClient->user_id;
			strcpy(temp2.timestamp,Q1[mark].commandClient->timestamp);
			temp2.command_type = Q1[mark].commandClient->command_type;
			temp2.price = Q1[mark].commandClient->price;
			
			while(mark > 0)
			{	
				Q1[mark].commandClient->command_type = Q1[mark-1].commandClient->command_type;
				Q1[mark].commandClient->number_of_stocks = Q1[mark-1].commandClient->number_of_stocks;
				Q1[mark].commandClient->company_id = Q1[mark-1].commandClient->company_id;
				Q1[mark].commandClient->command_id = Q1[mark-1].commandClient->command_id;
				Q1[mark].commandClient->price = Q1[mark-1].commandClient->price;
				strcpy(Q1[mark].commandClient->timestamp,Q1[mark-1].commandClient->timestamp);
				Q1[mark].commandClient->user_id = Q1[mark-1].commandClient->user_id;
				mark--;
			}
			
			Q1[0].commandClient->number_of_stocks = temp2.number_of_stocks;
			Q1[0].commandClient->company_id = temp2.company_id;
			Q1[0].commandClient->user_id= temp2.user_id ;
			Q1[0].commandClient->command_id= temp2.command_id ;
			strcpy(Q1[0].commandClient->timestamp,temp2.timestamp);
			Q1[0].commandClient->command_type= temp2.command_type;
			Q1[0].commandClient->price= temp2.price ;
			
			for(int j =0 ; j<(Q1->rear);j++)
			{
				Q1[j].commandClient->command_type = Q1[j+1].commandClient->command_type;
				Q1[j].commandClient->number_of_stocks = Q1[j+1].commandClient->number_of_stocks;
				Q1[j].commandClient->company_id = Q1[j+1].commandClient->company_id;
				strcpy(Q1[j].commandClient->timestamp,Q1[j+1].commandClient->timestamp);
				Q1[j].commandClient->command_id= Q1[j+1].commandClient->command_id;
				Q1[j].commandClient->price = Q1[j+1].commandClient->price;
				Q1[j].commandClient->user_id = Q1[j+1].commandClient->user_id;	
				
			}
			Q1[Q1->rear].commandClient->number_of_stocks = 0;
			Q1[Q1->rear].commandClient->company_id =0;
			Q1[Q1->rear].commandClient->command_id=0;
			Q1[Q1->rear].commandClient->user_id=0 ;
			strcpy(Q1[Q1->rear].commandClient->timestamp,"0");
			Q1[Q1->rear].commandClient->command_type=0;
			Q1[Q1->rear].commandClient->price= 0 ;
			Q1->rear = Q1->rear -1;
			}

    }
    else if (entoli.command_type_for_delete == 2)
    {
      for (int i =0; i<=(Q2->rear);i++)
        {
            if (entoli.company_id == Q2[i].commandClient->company_id && entoli.user_id == Q2[i].commandClient->user_id && entoli.number_of_stocks == Q2[i].commandClient->number_of_stocks)
            {
                mark = i;
            }
        }
             if(mark <0)
            {
                return;
            }
        	else if (mark>=0  && (Q2->rear == Q2->front))
			{	
				Q2[0].commandClient->number_of_stocks = 0;
				Q2[0].commandClient->company_id =0;
				Q2[0].commandClient->user_id=0 ;
				Q2[0].commandClient->command_id=0;
				strcpy(Q2[0].commandClient->timestamp,"0");
				Q2[0].commandClient->command_type=0;
				Q2[0].commandClient->price= 0 ;
				Q2->front = Q2->front -1;
				Q2->rear = Q2->rear -1;
			}
			else
			{
			commands temp2;
			temp2.number_of_stocks = Q2[mark].commandClient->number_of_stocks;
			temp2.company_id = Q2[mark].commandClient->company_id;
			temp2.command_id = Q2[mark].commandClient->command_id;
			temp2.user_id = Q2[mark].commandClient->user_id;
			strcpy(temp2.timestamp,Q2[mark].commandClient->timestamp);
			temp2.command_type = Q2[mark].commandClient->command_type;
			temp2.price = Q2[mark].commandClient->price;
			
			while(mark > 0)
			{	
				Q2[mark].commandClient->command_type = Q2[mark-1].commandClient->command_type;
				Q2[mark].commandClient->number_of_stocks = Q2[mark-1].commandClient->number_of_stocks;
				Q2[mark].commandClient->company_id = Q2[mark-1].commandClient->company_id;
				Q2[mark].commandClient->command_id = Q2[mark-1].commandClient->command_id;
				Q2[mark].commandClient->price = Q2[mark-1].commandClient->price;
				strcpy(Q2[mark].commandClient->timestamp,Q2[mark-1].commandClient->timestamp);
				Q2[mark].commandClient->user_id = Q2[mark-1].commandClient->user_id;
				mark--;
			}
			
			Q2[0].commandClient->number_of_stocks = temp2.number_of_stocks;
			Q2[0].commandClient->company_id = temp2.company_id;
			Q2[0].commandClient->user_id= temp2.user_id ;
			Q2[0].commandClient->command_id= temp2.command_id ;
			strcpy(Q2[0].commandClient->timestamp,temp2.timestamp);
			Q2[0].commandClient->command_type= temp2.command_type;
			Q2[0].commandClient->price= temp2.price ;
			
			for(int j =0 ; j<(Q2->rear);j++)
			{
				Q2[j].commandClient->command_type = Q2[j+1].commandClient->command_type;
				Q2[j].commandClient->number_of_stocks = Q2[j+1].commandClient->number_of_stocks;
				Q2[j].commandClient->company_id = Q2[j+1].commandClient->company_id;
				strcpy(Q2[j].commandClient->timestamp,Q2[j+1].commandClient->timestamp);
				Q2[j].commandClient->command_id= Q2[j+1].commandClient->command_id;
				Q2[j].commandClient->price = Q2[j+1].commandClient->price;
				Q2[j].commandClient->user_id = Q2[j+1].commandClient->user_id;	
				
			}
			Q2[Q2->rear].commandClient->number_of_stocks = 0;
			Q2[Q2->rear].commandClient->company_id =0;
			Q2[Q2->rear].commandClient->command_id=0;
			Q2[Q2->rear].commandClient->user_id=0 ;
			strcpy(Q2[Q2->rear].commandClient->timestamp,"0");
			Q2[Q2->rear].commandClient->command_type=0;
			Q2[Q2->rear].commandClient->price= 0 ;
			Q2->rear = Q2->rear -1;
			}

    }

	return;
}