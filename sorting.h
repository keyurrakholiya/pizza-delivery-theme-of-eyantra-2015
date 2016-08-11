int next_row , prev_row;
int deliverd_home[13]={0,0,0};
/*
* Function name:    buzzer_blink
* Input        :
* Output       :
* Logic        : it will blink the buzzer
* example call : buzzer_blink();
*/
void buzzer_blink()
{

	stop();
	buzzer_on();
	_delay_ms(20);
	buzzer_off();
	_delay_ms(20);
	buzzer_on();
	_delay_ms(20);
	buzzer_off();
	_delay_ms(20);

}
/*
timeline[8][6] : this 2D array has all information about order time ,flavour , size ,preorder or regular , all home distances from pizza counter.
				after every dilevery row will increment by one. and sorting function will call from there.

				|  ordertime  |  ordertype  |  flavour   |  size   |   home distance  |
				|             |             |            |         |                  |
				|             |             |            |         |                  |
				|             |             |            |         |                  |
				|             |             |            |         |                  |
				|             |             |            |         |                  |
				|             |             |            |         |                  |

*/
/*
* Function name:  sorting
* Input        : timeline , row ,time
* Output       :
* Logic        : we are checking eight different possibilities to sort the time line. after one dilivery sorting function will be called,
				and according to current time it will sort the timeline and give next adress for pizza and home.and after that line follower
				function will called with two input pizza, home.
* example call : buzzer_blink();
*/

void sorting(int timeline[][6], int row ,int time)
{  y = time;
	next_row= row+1;
	prev_row =row-1 ;

	//this condition is checking, CURRENT order is regular and also determined the require pizza location and home which has ordered this.

	if((y >= timeline[row][0] ) && (timeline[row][1]==regular_order) && (skip_order == 0) && ((y + timeline[row][5]  + 20 ) <(timeline[row][0] + 50)) && (deliverd_home[row] == 0))		//count time is grater then order time (regular order)
	{

		k=1;
		while(k<11)
		{
			if((pizza_counter[k][0] == timeline[row][2]) && (pizza_counter[k][1] == timeline[row][3]))
			{
				pizza=k;                //robot will go for pick reuried pizza
				break;
			}
			k++;
		}
		home = timeline[row][4];
		deliverd_home[row] = 1;         //this flag will become one when perticular home  will delivered
	}

	//this condition is checking , current order is preorder or not . and also determined whether the order will deliver in time or not
	else if(((y+ timeline[row][5] + 20)> (timeline[row][0] -50))	&&	(timeline[row][1] == pre_order ) && ((y+ timeline[row][5] + 20) <(timeline[row][0]+50) && (skip_order == 0)) && (deliverd_home[row] == 0)) //reachtime > order time -50
	{
		home = timeline[row][4];
		deliverd_home[row] = 1;

		for(k = 1 ; k<11 ; k++)
		{
			if((pizza_counter[k][0] == timeline[row][2]) && (pizza_counter[k][1] == timeline[row][3]))
			{
				pizza=k;
				break;
			}

		}
	}

	//this condition is checking for next order is pre_order or not?
	else if(((y + timeline[next_row][5]) +20 > (timeline[next_row][0] - 50 ))&& ( timeline[next_row][1]== pre_order)&& (skip_order == 0))
	{
		skip_order = 1;
		for(k = 1 ; k<11 ; k++)
		{
			if((pizza_counter[k][0] == timeline[next_row][2]) && (pizza_counter[k][1] == timeline[next_row][3]) )
			{
				pizza=k;
				break;
			}
		}

		prev_req_time =	pizza_dist[k] + (2*timeline[row][5]);           //calculate previous order requriment time which we will use in next conditions



		//now it will go for this next order
		for(k = 1 ; k<11 ; k++)
		{
			if((pizza_counter[k][0] == timeline[next_row][2] )&& (pizza_counter[k][1] == timeline[next_row][3] ))
			{
				pizza=k;
				break;
			}

		}

		home = timeline[next_row][4];           //go to next home
		deliverd_home[next_row] = 1;
	}


	else if((y >= timeline[next_row][0] ) && (timeline[next_row][1]==regular_order) && (skip_order == 0))
	{
		skip_order = 1;
		for(k = 1 ; k<11 ; k++)
		{
			if((pizza_counter[k][0] == timeline[row][2]) && (pizza_counter[k][1] == timeline[row][3]) )
			{
				pizza=k;
				break;
			}

		}

		prev_req_time =	pizza_dist[k] + (2*timeline[row][5]);       //calculating prev req. time
		home = timeline[next_row][4];
		deliverd_home[next_row] = 1;
	}

	//if any order is left by solving above conditions , now this condition will check previous order will delivered or not in remaining time
	//current order is delivered
	else if(skip_order==1)
	{
		skip_order = 0;
		next_req_time = timeline[next_row][5] + 20;                 //calculating next req. time for next order.

		delay_count = timeline[next_row][0] - y - prev_req_time;    //delay count = robot has to wait for next order

		//this condition is checking that , in time of waiting robot should diliver previous order or not.
		if(delay_count > next_req_time)
		{
			home= timeline[prev_row][4];
			deliverd_home[prev_row] = 1;
			for(k = 1 ; k<11 ; k++)
			{
				if((pizza_counter[k][0] == timeline[prev_row][2] )&& (pizza_counter[k][1] == timeline[prev_row][3] ))
				{
					pizza=k;
					break;
				}
			}
		}

		//if there is no suffiesient time for previous order then , it will choose next order
		else
		{
			if(timeline[next_row][1] == pre_order)       //condition for next order is pre or regular
			{
				home= timeline[next_row][4];
				deliverd_home[next_row] = 1;
				for(k = 1 ; k<11 ; k++)
				{
					if((pizza_counter[k][0] == timeline[next_row][2] )&& (pizza_counter[k][1] == timeline[next_row][3] ))
					{
						pizza=k;
						break;
					}
				}
			}
			else
			{
				int k = timeline[next_row][0];           //next order is regular then robot have to wait for this time
				while(y< k)
				{
					stop();
					buzzer_blink();
					_delay_ms(500);
					y++;
				}

				forward();

				home= timeline[next_row][4];
				deliverd_home[next_row] = 1;
				for(k = 1 ; k<11 ; k++)
				{
					if((pizza_counter[k][0] == timeline[next_row][2] )&& (pizza_counter[k][1] == timeline[next_row][3] ))
					{
						pizza=k;
						break;
					}
				}
			}
		}
	}

	//for regular order , first it wait up to order come
	else if((y < timeline[row][0] ) && (timeline[row][1]==regular_order) && (skip_order == 0))
	{
		int k = timeline[row][0];   // k = current time
		while(y < k)
		{	stop();
			buzzer_blink();
			_delay_ms(480);
			y++;
		}
		forward();

		home = timeline[row][4];
		deliverd_home[next_row] = 1;
		for(k = 1 ; k<11 ; k++)
		{
			if((pizza_counter[k][0] == timeline[row][2] )&& (pizza_counter[k][1] == timeline[row][3] ))
			{
				pizza=k;
				break;
			}
		}

	}

	//it will check for next regular order time
	else if((y < timeline[next_row][0] ) && (timeline[next_row][1]==regular_order) && (skip_order == 0))
	{
		int k = timeline[next_row][0];

		while(y < k)
		{
			stop();
			buzzer_blink();
			_delay_ms(480);
			y++;
		}
		
		home = timeline[next_row][4];
		deliverd_home[next_row] = 1;
		for(k = 1 ; k<11 ; k++)
		{
			if((pizza_counter[k][0] == timeline[next_row][2] )&& (pizza_counter[k][1] == timeline[next_row][3] ))
			{
				pizza=k;
				break;
			}
		}

	}

	//now if current home is delivered robot will go for next order
	else if(deliverd_home[row] == 1)
	{
		if(timeline[next_row][1] == regular_order)
		{
			int k= timeline[next_row][0];
			while(y< k)
			{
				stop();
				buzzer_blink();
				_delay_ms(480);
				y++;
			}
			
			forward();
			home= timeline[next_row][4];
			deliverd_home[next_row] =1 ;
			for(k = 1 ; k<11 ; k++)
			{
				if((pizza_counter[k][0] == timeline[next_row][2] )&& (pizza_counter[k][1] == timeline[next_row][3] ))
				{
					pizza=k;
					break;
				}
			}

		}
		else{
			home= timeline[next_row][4];
			deliverd_home[next_row] =1 ;
			for(k = 1 ; k<11 ; k++)
			{
				if((pizza_counter[k][0] == timeline[next_row][2] )&& (pizza_counter[k][1] == timeline[next_row][3] ))
				{
					pizza=k;
					break;
				}
			}
		}
	}
	linefollower(home,pizza);       // home and pizza is address to perticular home which is passing in linefollower function
}




