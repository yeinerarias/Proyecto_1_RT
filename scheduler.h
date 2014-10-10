int Lottery_Scheduler(int tickets[5])
{
	int winner;
	int all_tickets = 0;
	int i;
	for(i=0;i<5;i++)
	{
		all_tickets += tickets[i];
	}
	winner = rand() % all_tickets;
	all_tickets = 0;
	for(i=0;i<5;i++)
	{
		all_tickets += tickets[i];
		if(all_tickets > winner)
		{
			return i;
		}
	}
	return 0;
}
