/*
*	Team Id		:	eYRC-PD#1260
*	Author List	:	keyur Rakholiya , Earnest Vekariya , Jay parekh , Sanket Bhimani
*	Filename	:	main.c
*	Theme		:	Pizza delivery theme (eYRC)
*	Functions	:	drop(void), flag_reset(void), H1_path(void), H2_path(void), H3_path(void), H4_path(void), H5_path(void),
*					H6_path(void), H7_path(void), H8_path(void), H9_path(void), H10_path(void), H11_path(void), H12_path(void),
*					line_follower(int home, int pizza), node_detector(int home, int pizza), pick_up(void), pizza1(void), pizza2(void), pizza3(void), pizza4(void),
*					pizza5(void), pizza6(void), pizza7(void), pizza8(void), pizza9(void), pizza10(void)
*/
#define regular_order 1
#define pre_order 0
#define R 1
#define G 2
#define B 3

#define S 1
#define M 2
#define L 3

#define cut_off 18


int row=0,y;																//in order timeline after every delivery we will incrase the value of row
int skip_order = 0, next_req_time = 0, prev_req_time = 0,delay_count = 0;	//this variables are related to sorting
unsigned int value;

//this all the flag1 to flag8 are for finding out node and tracing the desire home path on arena
int flag1=0,flag1_check=0, flag2=0,flag2_check=0, flag3=0,flag3_check=0,   flag4=0,flag4_check=0, 
flag5=0, flag5_check=0,   flag6=0,flag6_check=0,   flag7=0,flag7_check=0,flag8=0,flag8_check=0,k,reached =0;

int picked_pizza=0,start=0,i=1,j=1,k=1;
int left_turn=0;									//if we have same pizza on same node then it will decide where to drop by changing this flag value
int right_turn=0;
int count_time=0, count_display=0,a,b,c;			 // this variables are related to seven segment display

int node_count=0;									//this will count the no. of node detected
int color=0,size=0;									//to store value of pizza's flavour and size
int pizza_no;										//it is the no. of pick up point
int home1=0, home2=0,home3=0,home4=0,home5=0,home6=0,home7=0,home8=0,home9=0, home10=0, home11 =0, home12 = 0; // how many times we have delivered pizza to same house that will store 

int  pizza=0, home=0;									//address of perticular pizza and home will be stored 

//order time line, which we are given directly inserted in this 2D array

//       order_time     type    flavour size home home_dist
int timeline[8][6]={	{65   ,    regular_order     ,	B,   S ,  1 ,  10},
						{105   ,    regular_order , B ,  S ,  8 ,  10},
						{170  ,     pre_order     , G ,  S ,  1 ,  20},
						{220  ,     regular_order , R ,  L ,  8 ,  15},
						{230  ,     regular_order , R ,  L ,  1 ,  10},
						{310  ,     pre_order     , G ,  S ,  8,  15},
						{310  ,     pre_order     , G ,  M ,  1 ,  15}
};
int pizza_dist[11]={0,25,22,19,16,13,13,16,19,22,25}; // approx distance to reach at perticular pizza

int pizza_counter[10][2]={0,0}; //all the scanned value of pizza will be stored in this 2D array
int sorting_array = 0;


#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>				// included to support power function
#include "main.h" 				// all init function are written in this file
#include "color+sharp.h"		// color & sharp sensor are included in this file
#include "servo.h"				// function related to servo motor are there in this file
#include "sorting.h"			//shorting function which will decide that which order it should be taken next is written here
#include "scanning.h"			// scanning of all pizza, this type of function are there in this file

int a,direction=0;
int pizza_flag=0;
int readdy=0;
int next_row=0;
int prev_row=0;
int first_time_reached_pizza[11]={0,0,0};
int p=0;

/*
* Function Name: H1_path()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter to H1 home, this will use node to take decision about next turn
* Example Call : H1_path();
*/
void H1_path(void)
{
	forward();
	
	if(reached== 0)
	{
		if(flag1==1 && flag1_check==0)
		{
			if(left_turn==1)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
			else
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
			}
		}
		
		
		if(flag2 == 1 && flag2_check == 0)
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag2_check = 1;
		}
		else if(flag3==1 && flag3_check == 0)
		{
			
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag3_check=1;
		}
		else if(flag4 ==1  || (ShaftCountLeft >25)) //reached at home
		{
			if(home1 == 0)
			{
				forward_mm(60);
				stop();
				left_degrees(110);
				home1=1;
				drop();
				left_degrees(85);
				adjust_left();
				flag_reset();
				reached=1;
			}
			else
			{
				
				stop();
				forward_mm(60);
				right_degrees(90);
				home1=1;
				drop();
				right_degrees(85);
				adjust_right();
				flag_reset();
				reached=1;
			}
		}
	}
	else
	{
		if (flag1 == 1 && flag1_check == 0)
		{
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			flag1_check=1;
		}
		else if(flag2 == 1 && flag2_check==0)
		{
			forward_mm(60);
			right_degrees(90);
			//adjust_right();
			flag_reset();
			j++;
			reached = 0;
			readdy=0;
			picked_pizza=0;
			left_turn=0;
			row = row + 1;
			y =count_time;
			sorting(timeline , row ,y );
			green_led_on();
			
			
			
		}
		
	}	
	
}

/*
* Function Name: H8_path()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter to H8 home, this will use node to take decision about next turn
* Example Call : H8_path();
*/
void H8_path(void)
{
	forward();
	if(reached== 0)
	{
		if(flag1==1 && flag1_check==0)
		{
			if(left_turn==1)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
			else
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
			}
		}
		
		if(flag2 == 1 && flag2_check == 0)
		{
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			flag2_check = 1;
		}
		else if(flag3==1 && flag3_check == 0)
		{
			
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			flag3_check=1;
		}
		else if(flag4 ==1) //reached at home
		{
			if(home8 == 0){
				forward_mm(60);
				stop();
				right_degrees(90);
				drop();
				right_degrees(80);
				adjust_right();
				flag_reset();
				reached=1;
			} 
			else
			{
				forward_mm(60);
				stop();
				left_degrees(90);
				drop();
				left_degrees(80);
				adjust_left();
				flag_reset();
				reached=1;
			}
		}
	}
	else
	{
		if (flag1 == 1 && flag1_check == 0)
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag1_check=1;
		}
		else if(flag2 == 1 && flag2_check==0)
		{
			forward_mm(60);
			left_degrees(90);
			//adjust_left();
			flag_reset();
			j++;
			reached = 0;
			readdy=0;
			picked_pizza=0;
			left_turn=0;
			row = row+1;
			y = count_time;
			sorting(timeline ,row , y);
			linefollower(home,pizza);
			
		}
	}
}
/*
* Function Name: H2_path()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter to H2 home, this will use node to take decision about next turn
* Example Call : H2_path();
*/		
void H2_path(void)
{	
	forward();
	if(reached == 0)
	{
		if(flag1==1 && flag1_check==0)
		{
			if(left_turn==1)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
			else
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
			}
		}
		if(flag2 == 1 && flag2_check == 0)
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			
			flag2_check = flag2;
		}
		else if(flag3== 1 && flag3_check== 0)
		{
			forward_mm(30);
			flag3_check=1;
		}
		else if(flag4 == 1 && flag4_check==0)
		{
			forward_mm(60);
			left_degrees(65);
			adjust_left();
			ShaftCountLeft = 0;
			flag4_check= flag4;
			
		}
		
		else if(flag5 == 1)
		{
			if(home5== 0)
			{
				forward_mm(60);
				stop();
				left_degrees(95);  // reached at home
				drop();
				left_degrees(85);
				adjust_left();
				flag_reset();
				reached=1;
				home5=1;
			}
			else
			{
				forward_mm(60);
				stop();
				right_degrees(95);  // reached at home
				drop();
				right_degrees(85);
				adjust_right();
				flag_reset();
				reached=1;
				home5=1;
			}
		}

	}
	else 
	{
		if(flag1 == 1 && flag1_check==0)
		{
			forward_mm(60);
			right_degrees(70);
			adjust_right();
			flag1_check=1;
		}
		else if(flag2 == 1 && flag2_check==0)
		{
			
			forward_mm(30);
			flag2_check=1;
		}
		else if(flag3==1 && flag3_check == 0)
		{
			forward_mm(60);
			right_degrees(90);
			//adjust_right();
			flag_reset();
			j++;
			reached = 0;
			readdy=0;
			picked_pizza=0;
			left_turn=0;
			row = row +1;
			y = count_time;
			sorting(timeline , row ,y);
			linefollower(home,pizza);
		}
		
	}
	
	
}
/*
* Function Name: H7_path()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter to H7 home, this will use node to take decision about next turn
* Example Call : H7_path();
*/
void H7_path(void)
{
	forward();
	if(reached == 0)
	{
		if(flag1==1 && flag1_check==0)
		{
			if(left_turn==1)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
			else
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
			}
		}
		if(flag2 == 1 && flag2_check == 0)
		{
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			
			flag2_check = flag2;
		}
		else if(flag3 == 1 && flag3_check == 0)
		{
			forward_mm(30);
			flag3_check=1;
		}
		else if(flag4 == 1 && flag4_check==0)
		{
			
			forward_mm(60);
			right_degrees(60);
			adjust_right();
			flag4_check=1;
		}
		else if(flag5 == 1)
		{
			if(home7 == 0){
				stop();
				forward_mm(60);
				left_degrees(90);  // reached at home
				drop();
				left_degrees(80);
				adjust_left();
				flag_reset();
				reached=1;
				home7=1;
			}
			else 
			{
				stop();
				forward_mm(60);
				right_degrees(90);  // reached at home
				drop();
				right_degrees(80);
				adjust_right();
				flag_reset();
				reached=1;
			}
		}
	}
	else
	{
		if(flag1 == 1 && flag1_check==0)
		{
			forward_mm(60);
			left_degrees(70);
			adjust_left();
			flag1_check=1;
		}
		else if(flag2==1 && flag2_check == 0)
		{
			forward_mm(30);
			flag2_check=1;
		}
		else if(flag3==1 && flag3_check == 0)
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag3_check=1;
			
			flag_reset();
			j++;
			reached = 0;
			readdy=0;
			picked_pizza=0;
			left_turn=0;
			row=row+1;
			y =count_time;
			sorting(timeline , row ,y );
			linefollower(home,pizza);
		}
		/*	else if(flag4 == 1)
		{
			stop();
			_delay_ms(5000);
			
		}*/
	}
}

/*
* Function Name: H3_path()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter to H3 home, this will use node to take decision about next turn
* Example Call : H3_path();
*/
void H3_path(void)
{
	forward();
	if(reached == 0)
	{
		if(flag1==1 && flag1_check==0)
		{
			if(left_turn==1)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
			else
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
			}
		}
		
		if(flag2==1 && flag2_check==0)
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag2_check = 1;
		}
		else if(flag3 == 1 && flag3_check==0)
		{
			forward_mm(30);
			flag3_check=1;
		}
		else if(flag4==1 && flag4_check==0)
		{
			forward_mm(30);
			flag4_check=1;
		}
		else if(flag6==1 && flag6_check==0)
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag6_check= 1;
		}
		else if(flag7 == 1) // reached at home
		{
			if(home3 == 0)
			{
				forward_mm(60);
				stop();
				left_degrees(100);
				drop();
				left_degrees(80);
				adjust_left();
				flag_reset();
				reached=1;
				home3=1;
			}
			else 
			{
				forward_mm(60);
				stop();
				right_degrees(100);
				drop();
				right_degrees(80);
				adjust_right();
				flag_reset();
				reached=1;
				home3=1;
			}
			
		}
	}
	else
	{
		if(flag1 == 1 && flag1_check == 0)
		{
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			flag1_check = 1;
		}
		else if(flag3 == 1 && flag3_check==0)
		{
			forward_mm(30);
			adjust_left();
			flag3_check=1;
		}
		else if(flag4==1 && flag4_check==0)
		{
			forward_mm(30);
			adjust_left();
			flag4_check=1;
		}
		else if(flag5 == 1 && flag5_check == 0)
		{
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			flag_reset();
			j++;
			reached = 0;
			readdy=0;
			picked_pizza=0;
			left_turn=0;
			row=row+1;
			y =count_time;
			sorting(timeline , row ,y );
			linefollower(home,pizza);
		}
		/*else if(flag6 == 1)
	{
			stop();
			_delay_ms(5000);
			reached = 0;
	}*/
	}

}
/*
* Function Name: H6_path()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter to H6 home, this will use node to take decision about next turn
* Example Call : H6_path();
*/
void H6_path(void)
{
	forward();
	if(reached == 0)
	{
		if(flag1==1 && flag1_check==0)
		{
			if(left_turn==1)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
			else
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
			}
		}
		if(flag2==1 && flag2_check==0)
		{
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			flag2_check = 1;
		}
		else if(flag3 == 1 && flag3_check == 0)
		{
			forward_mm(20);
			adjust_left();
			flag3_check=1;
		}
		else if(flag4 == 1 && flag4_check == 0)
		{
			forward_mm(20);
			adjust_left();
			flag4_check=1;
		}
		else if(flag6==1 && flag6_check==0)
		{
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			flag6_check= flag6;
		}
		else if(flag7 == 1) // reached at home
		{
			if(home6 == 0){
				stop();
				forward_mm(60);
				right_degrees(90);
				drop();
				right_degrees(80);
				adjust_right();
				flag_reset();
				reached=1;
				home6=1;
			}
			else
			{
				stop();
				forward_mm(60);
				right_degrees(90);
				drop();
				stop();
				buzzer_on();
				_delay_ms(2500);
				buzzer_off();
				stop();
				right_degrees(80);
				adjust_right();
				flag_reset();
				reached=1;
			}
		}
	}
	else
	{
		if(flag1 == 1 && flag1_check == 0)
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag1_check = 1;
		}
		else if(flag3 ==1 && flag3_check == 0)
		{
			//forward_mm(30);
			flag3_check=1;
		}
		else if(flag4 == 1 && flag4_check == 0)
		{
			//forward_mm(30);
			flag4_check=1;
		}
		else if(flag5 == 1 && flag5_check == 0)
		{
			forward_mm(60);
			left_degrees(90);
			//adjust_left();
			flag5_check=1;
			
			flag_reset();
			j++;
			reached = 0;
			readdy=0;
			picked_pizza=0;
			left_turn=0;
			row=row+1;
			y =count_time;
			sorting(timeline , row ,y );
			linefollower(home,pizza);
		}
		/*	else if(flag6 == 1)
		{
			stop();
			_delay_ms(5000);
			reached = 0;
		}*/
	}
	
}
/*
* Function Name: H4_path()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter to H4 home, this will use node to take decision about next turn
* Example Call : H4_path();
*/
void H4_path(void)
{
	forward();
	if(reached == 0)
	{
		if(flag1==1 && flag1_check==0)
		{
			if(left_turn==1)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
			else
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
			}
		}	 
		
		if(flag2 == 1 && flag2_check==0)
		{
			forward_mm(60);
			
			left_degrees(80);
			
			adjust_left();
			
			flag2_check=1;
		}
		else if(flag7 == 1 && flag7_check == 0)
		{
			forward_mm(60);
			left_degrees(80);
			adjust_left();
			flag7_check=flag7;
		}
		else if(flag8 == 1) // reached at home
		{
			if(home4 == 0)
			{
				forward_mm(60);
				stop();
				left_degrees(100);
				drop();
				left_degrees(80);
				adjust_left();
				flag_reset();
				reached =1;
				home4=1;
			}
			else
			{
				forward_mm(60);
				stop();
				right_degrees(100);
				drop();
				right_degrees(80);
				adjust_right();
				flag_reset();
				reached =1;
				home4=1;
			}
			
		}
	}
	else
	{
		if(flag1 == 1 && flag1_check == 0)
		{
			forward_mm(60);
			right_degrees(70);
			adjust_right();
			
			flag1_check = 1;
		}
		if(flag2==1 && flag2_check==0)
		{
			forward_mm(30);
			flag2_check=1;
		}
		else if(flag6 == 1 && flag6_check == 0)
		{
			forward_mm(60);
			right_degrees(80);
			adjust_right();
			flag_reset();
			j++;
			reached = 0;
			readdy=0;
			picked_pizza=0;
			left_turn=0;
			row=row+1;
			y =count_time;
			sorting(timeline , row ,y );
			linefollower(home,pizza);
		}
		/*else if(flag7 == 1)
	{
		stop();
		_delay_ms(5000);
		reached=0;
	}	*/
	}
}
/*
* Function Name: H5_path()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter to H5 home, this will use node to take decision about next turn
* Example Call : H5_path();
*/
void H5_path(void)
{
	forward();
	if(reached == 0)
	{
		if(flag1==1 && flag1_check==0)
		{
			if(left_turn==1)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
			else
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
			}
		}
		if(flag2 == 1 && flag2_check==0)
		{
			forward_mm(60);
			right_degrees(90);
			adjust_right();
			flag2_check=1;
		}
		else if(flag3 == 1 && flag3_check == 0)
		{
			velocity(200,200);
			forward_mm(20);
			adjust_left();
			//forward_mm(20);
			flag3_check=1;
		}
		else if(flag4 == 1 && flag4_check == 0)
		{
			velocity(200,200);
			forward_mm(20);
			adjust_left();
			
			flag4_check=1;
		}
		else if(flag6 == 1 && flag6_check==0)
		{
			velocity(200,200);
			forward_mm(20);
			adjust_left();
			//forward_mm(20);
			flag6_check=1;
		}
		
		else if(flag7 == 1 && flag7_check == 0)
		{
			forward_mm(60);
			right_degrees(50);
			adjust_right();
			flag7_check=flag7;
		}
		else if(flag8 == 1) // reached at home
		{
			if(home5 == 0)
			{
				forward_mm(60);
				stop();
				left_degrees(100);
				drop();
				left_degrees(80);
				adjust_left();
				flag_reset();
				reached = 1;
				home5 = 1;
			}
			else
			{
				forward_mm(60);
				stop();
				right_degrees(100);
				drop();
				right_degrees(80);
				adjust_right();
				flag_reset();
				reached = 1;
				
			}
			
		}
	}
	else
	{
		if(flag1 == 1 && flag1_check == 0)
		{
			forward_mm(60);
			left_degrees(70);
			flag1_check = 1;
		}
		else if(flag2 == 1 && flag2_check== 0)
		{
			forward_mm(30);
			flag2_check=1;
		}
		else if(flag4 == 1 && flag4_check == 0)
		{
			forward_mm(30);
			flag4_check=1;
		}
		else if(flag5 == 1 && flag5_check == 0)
		{
			forward_mm(30);
			flag5_check=1;
		}
		else if(flag6 == 1 && flag6_check == 0)
		{
			forward_mm(60);
			left_degrees(90);
			//adjust_left();
			flag6_check=1;
			flag_reset();
			j++;
			reached = 0;
			readdy=0;
			picked_pizza=0;
			left_turn=0;
			row = row +1;
			y = count_time;
			sorting(timeline , row ,y);
			linefollower(home,pizza);
		}
		/*else if(flag7 == 1)
		{
			stop();
			_delay_ms(5000);
			reached=0;
		}*/
	}
}
/*
* Function Name: H9_path()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter to H9 home, this will use node to take decision about next turn
* Example Call : H9_path();
*/
void H9_path(void)
{
	if(reached == 0)
	{
		if(flag1==1 && flag1_check==0)
		{
			if(left_turn==1)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
			else
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
			}
		}
		if(flag3 == 1 && flag3_check == 0) 
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag3_check =1;
		}
		else if(flag4 == 1 )// reached at home
		{
			if(home9 == 0)
			{
				forward_mm(60);
				stop();
				drop();
				left_degrees(180);
				adjust_left();
				flag_reset();
				reached=1;
				home9=1;
			}
			else
			{
				stop();
				forward_mm(100);
				left_degrees(90);
				drop();
				left_degrees(85);
				adjust_left();
				flag_reset();
				reached=1;
				home9=1;
			}
		}
	}
	else
	{
		if(flag1==1 && flag1_check==0)
		{
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			flag1_check=1;
		}
		else if(flag2 == 1 && flag2_check==0)
		{
			flag_reset();
			j++;
			reached = 0;
			readdy=0;
			picked_pizza=0;
			left_turn=0;
			row=row+1;
			y =count_time;
			sorting(timeline , row ,y );
			linefollower(home,pizza);
		}
	}

	
}
/*
* Function Name: H12_path()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter to H12 home, this will use node to take decision about next turn
* Example Call : H12_path();
*/
void H12_path(void)
{
	if(reached == 0)
	{
		if(flag1==1 && flag1_check==0)
		{
			if(left_turn==1)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
			else
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
			}
		}
		if(flag3 == 1 && flag3_check == 0)
		{
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			flag3_check =1;
		}
		else if(flag4 == 1 )// reached at home
		{
			if(home12 == 0)
			{
				stop();
				forward_mm(100);
				drop();
				right_degrees(180);
				adjust_right();
				flag_reset();
				reached=1;
				home12=1;
			}
			else{
				stop();
				right_degrees(90);
				drop();
				right_degrees(85);
				adjust_right();
				flag_reset();
				reached=1;
			}
		}
	}
	else
	{
		if(flag1==1 && flag1_check==0)
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag1_check=1;
			
			
		}
		else if(flag2 == 1 && flag2_check==0)
		{
			flag_reset();
			j++;
			reached = 0;
			readdy=0;
			picked_pizza=0;
			left_turn=0;
			row=row+1;
			y =count_time;
			sorting(timeline , row ,y );
			linefollower(home,pizza);
		}
	}

	
}
/*
* Function Name: H10_path()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter to H10 home, this will use node to take decision about next turn
* Example Call : H10_path();
*/
void H10_path(void)
{
	if(reached == 0)
	{
		if(flag1==1 && flag1_check==0)
		{
			if(left_turn==1)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
			else
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
			}
		}
		if(flag3 == 1 && flag3_check == 0)
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag3_check =1;
		}
		else if(flag4 == 1 && flag4_check==0 )
		{
			forward_mm(60);
			right_degrees(80);
			adjust_right();
			flag4_check=1;
		}
		else if(flag6 == 1 && flag6_check==0)	//reached at home
		{
			if(home10 == 0)
			{
				forward_mm(100);
				drop();
				left_degrees(180);
				adjust_left();
				flag_reset();
				reached=1;
				home10=1;
			}
			else
			{
				forward_mm(60);
				left_degrees(90);
				drop();
				left_degrees(85);
				adjust_left();
				flag_reset();
				reached=1;
			}
		}
	}
	else
	{
		if(flag2==1 && flag2_check==0)
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag2_check=1;
		}
		else if(flag3 == 1 && flag3_check==0)
		{
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			flag3_check=1;
			
		}
		else if(flag4==1 && flag4_check==0)
		{
			flag_reset();
			j++;
			reached = 0;
			readdy=0;
			picked_pizza=0;
			left_turn=0;
			row=row+1;
			y =count_time;
			sorting(timeline , row ,y );
			linefollower(home,pizza);
			
		}
	}
	
}
/*
* Function Name: H11_path()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter to H11 home, this will use node to take decision about next turn
* Example Call : H11_path();
*/
void H11_path(void)
{
	forward();
	if(reached == 0)
	{
		if(flag1==1 && flag1_check==0)
		{
			if(left_turn==1)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
			else
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
			}
		}
		if(flag3 == 1 && flag3_check == 0)
		{
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			flag3_check =1;
		}
		else if(flag4 == 1 && flag4_check==0 )
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag4_check=1;
		}
		else if(flag6 == 1 && flag6_check==0)	//reached at home
		{
			if(home11 == 0)
			{
				forward_mm(100);
				stop();
				drop();
				right_degrees(180);
				adjust_right();
				flag_reset();
				reached=1;
				home11=1;
				
			}
			else
			{
				right_degrees(90);
				drop();
				right_degrees(85);
				adjust_right();
				flag_reset();
				reached=1;
			}
		}
	}
	else
	{
		if(flag2==1 && flag2_check==0)
		{
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			flag2_check=1;
		}
		else if(flag3 == 1 && flag3_check==0)
		{
			forward_mm(60);
			left_degrees(85);
			adjust_left();
			flag3_check=1;
			
			
		}
		else if(flag4==1 && flag4_check==0)
		{
			flag_reset();
			j++;
			reached = 0;
			readdy=0;
			picked_pizza=0;
			left_turn=0;
			row=row+1;
			y =count_time;
			sorting(timeline , row ,y );
			linefollower(home,pizza);
		}
	}
	
}
/*
* Function Name: node_detector(int home, int pizza)
* Input		 : 
* Output		 : 
* Logic		 : this function will help to detect node.
* Example Call : node_detector(1,2);  //pizza will be pick from pick up poit 2 ,and will be delivered tohome 1 
*/

void node_detector(int home , int pizza)
{
	
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
	
	if(((Left_white_line > cut_off &&  Center_white_line >cut_off ) || (Right_white_line> cut_off && Center_white_line> cut_off )) && (ShaftCountLeft>8 || ShaftCountRight>8))
	{
		
		ShaftCountRight=0; ShaftCountLeft=0;
		
		
		if(flag1 == 1)
		{
			if(flag2 == 1)
			{
				if(flag3 == 1)
				{
					if(flag4 == 1)
					{
						if(flag5 == 1)
						{
							if(flag6 == 1)
							{
								if(flag7 ==1)
								{
									flag8 = 1;
								}
								flag7 = 1;
							}
							flag6=1;
						}
						flag5 =1;
					}
					flag4 =1;
				}
				flag3 =1;
			}
			flag2 = 1;
			
		}
		flag1=1;
		
		// readdy=1 means robot have picked the pizza
		if(readdy==1)
		{
			switch(home) {				//this switch case call proper function of home path
				
			case 1: H1_path();
				break;
			case 2: H2_path();
				break;
			case 3: H3_path();
				break;
			case 4: H4_path();
				break;
			case 5: H5_path();
				break;
			case 6: H6_path();
				break;
			case 7: H7_path();
				break;
			case 8: H8_path();
				break;
			case 9: H9_path();
				break;
			case 10: H10_path();
				break;
			case 11: H11_path();
				break;
			case 12: H12_path();
				break;
			}
		}
		else
		{							
			switch(pizza)			// here ready=0 so robot will goto pick pizza  so this switch will call particular function which will lead robot to pizza counter to pizza place 
			{
			case 1: pizza1(); break;
			case 2: pizza2(); break;
			case 3: pizza3(); break;
			case 4: pizza4(); break;
			case 5: pizza5(); break;
			case 6: pizza6(); break;
			case 7: pizza7(); break;
			case 8: pizza8(); break;
			case 9: pizza9(); break;
			case 10: pizza10(); break;
				
			}
			
		}
	}
}

/*
* Function Name: pizza10()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter top pizza 10, this will use node to take decision about next turn
* Example Call : pizza10();
*/
void pizza10(void)
{
	
	
	forward();
	if(picked_pizza == 0)
	{
		
		forward();
		if(start == 0)
		{
			if(flag1==1 && flag1_check==0)
			{
				
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
				start=1;
			}
		}
		else
		{
			if(flag1 == 1 && flag1_check == 0)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
		}
		if(flag6 == 1 && flag6_check == 0)
		{
			forward_mm(60);
			right_degrees(90);
			pick_up();			//reached at pizza
			right_degrees(85);
			adjust_right();
			flag6_check=1;
			picked_pizza=1;
			flag_reset();
		}
	}
	else
	{
		forward_mm(20);
		if(flag4 == 1 && flag4_check==0)
		{
			i++;
			readdy=1;
			flag_reset();
			
		}
	}
	
	
}
/*
* Function Name: pizza9()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter top pizza 9, this will use node to take decision about next turn
* Example Call : pizza9();
*/
void pizza9(void)
{

	forward();
	if(picked_pizza == 0)
	{
		
		forward();
		if(start == 0)
		{
			if(flag1==1 && flag1_check==0)
			{
				
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
				start=1;
			}
		}
		else
		{
			if(flag1 == 1 && flag1_check == 0)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
		}
		if(flag5 == 1 && flag5_check == 0)
		{
			forward_mm(60);
			right_degrees(90);
			pick_up();	
			green_led_on();	//reached at pizza
			right_degrees(80);
			adjust_right();
			flag5_check=1;
			picked_pizza=1;
			flag_reset();
		}
	}
	else
	{
		forward_mm(20);
		if(flag3 == 1 && flag3_check==0)
		{
			i++;
			readdy=1;
			flag_reset();
			
		}
	}
	
}
/*
* Function Name: pizza8()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter top pizza 8, this will use node to take decision about next turn
* Example Call : pizza8();
*/
void pizza8(void)
{
	

	forward();
	if(picked_pizza == 0)
	{
		
		forward();
		if(start == 0)
		{
			if(flag1==1 && flag1_check==0)
			{
				
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
				start=1;
			}
		}
		else
		{
			if(flag1 == 1 && flag1_check == 0)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
		}
		if(flag4 == 1 && flag4_check == 0)
		{
			forward_mm(70);
			right_degrees(90);
			blue_led_on(); //reached at pizza
			pick_up();
			right_degrees(80);
			adjust_right();
			flag6_check=1;
			picked_pizza=1;
			flag_reset();
		}
	}
	else
	{
		forward_mm(20);
		if(flag2 == 1 && flag2_check==0)
		{
			i++;
			readdy=1;
			flag_reset();
			
		}
	}
	
}
/*
* Function Name: pizza7()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter top pizza 7, this will use node to take decision about next turn
* Example Call : pizza7();
*/
void pizza7(void)
{
	
	

	forward();
	if(picked_pizza == 0)
	{
		
		forward();
		if(start == 0)
		{
			if(flag1==1 && flag1_check==0)
			{
				
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
				start=1;
			}
		}
		else
		{
			if(flag1 == 1 && flag1_check == 0)
			{
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
		}
		if(flag3 == 1 && flag3_check == 0)
		{
			forward_mm(70);
			right_degrees(90);
			stop();
			red_led_on();
			pick_up();		//reached at pizza
			right_degrees(85);
			adjust_right();
			picked_pizza=1;
			flag_reset();
		}
	}
	else
	{
		forward_mm(20);
		if(flag1 == 1 && flag1_check==0)
		{
			i++;
			readdy=1;
			flag_reset();
			
		}
	}
	
}
/*
* Function Name: pizza6()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter top pizza 6, this will use node to take decision about next turn
* Example Call : pizza6();
*/
void pizza6(void)
{
	

	forward();
	if(picked_pizza == 0)
	{
		
		forward();
		if(start == 0)
		{
			if(flag1==1 && flag1_check==0)
			{
				
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
				start=1;
			}
		}
		else
		{
			if(flag1 == 1 && flag1_check == 0)
			{
				forward_mm(90);
				left_degrees(85);
				adjust_left();
				flag1_check=1;
			}
		}
		if(flag2 == 1 && flag2_check == 0)
		{
			forward_mm(70);
			right_degrees(90);			//reached at pizza
			stop();
			red_led_on();
			pick_up(pizza);
			right_degrees(80);
			adjust_right();
			picked_pizza=1;
			flag_reset();
			i++;
			readdy=1;
		}
	}
	else
	{
		forward_mm(20);
	}
	
}
/*
* Function Name: pizza5()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter top pizza 5, this will use node to take decision about next turn
* Example Call : pizza5();
*/
void pizza5(void)
{
	forward();
	if(picked_pizza == 0)
	{
		
		forward();
		if(start == 0)
		{
			if(flag1==1 && flag1_check==0)
			{
				
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				left_turn=1;
				flag1_check=1;
				start=1;
			}
		}
		else
		{
			if(flag1 == 1 && flag1_check == 0)
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
				left_turn=1;
			}
		}
		if(flag2 == 1 && flag2_check == 0)
		{
			forward_mm(60);
			left_degrees(80);
			adjust_left();
			_delay_ms(20);		
			green_led_on();			//reachded at pizza
			pick_up();
			left_degrees(85);
			adjust_left();
			flag_reset();
			i++;
			picked_pizza=1;
			readdy=1;
			
		}
	}
	else
	{
		forward_mm(20);
	}
}
/*
* Function Name: pizza4()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter top pizza 4, this will use node to take decision about next turn
* Example Call : pizza4();
*/
void pizza4(void)
{
	forward();
	if(picked_pizza == 0)
	{
		
		forward();
		if(start == 0)
		{
			if(flag1==1 && flag1_check==0)
			{
				
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				left_turn=1;
				flag1_check=1;
				start=1;
			}
		}
		else
		{
			if(flag1 == 1 && flag1_check == 0)
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
				left_turn=1;
			}
		}
		if(flag3 == 1 && flag3_check == 0)
		{
			forward_mm(60);
			left_degrees(80);
			adjust_left();
			_delay_ms(15);			//reached at pizza
			pick_up();
			left_degrees(85);
			adjust_left();
			flag_reset();
			picked_pizza=1;
		}
	}
	else
	{
		forward_mm(20);
		if(flag1 == 1 && flag1_check==0)
		{
			i++;
			readdy=1;
			flag_reset();
			
		}
	}
	
}
/*
* Function Name: pizza3()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter top pizza 3, this will use node to take decision about next turn
* Example Call : pizza3();
*/
void pizza3(void)
{
	
	forward();
	if(picked_pizza == 0)
	{
		
		forward();
		if(start == 0)
		{
			if(flag1==1 && flag1_check==0)
			{
				
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				left_turn=1;
				flag1_check=1;
				start=1;
			}
		}
		else
		{
			if(flag1 == 1 && flag1_check == 0)
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
				left_turn=1;
			}
		}
		if(flag4 == 1 && flag4_check == 0)
		{
			forward_mm(60);
			left_degrees(80);
			adjust_left();	
			blue_led_on();		//reached at pizza
			pick_up();
			left_degrees(85);
			adjust_left();
			picked_pizza=1;
			flag_reset();
		}
	}
	else
	{
		forward_mm(20);
		if(flag2 == 1 && flag2_check==0)
		{
			i++;
			readdy=1;
			flag_reset();
			
		}
	}
	
}
/*
* Function Name: pizza2()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter top pizza 2, this will use node to take decision about next turn
* Example Call : pizza2();
*/		
void pizza2(void)
{
	forward();
	if(picked_pizza == 0)
	{
		
		forward();
		if(start == 0)
		{
			if(flag1==1 && flag1_check==0)
			{
				
				forward_mm(60);
				left_degrees(80);
				adjust_left();
				left_turn=1;
				flag1_check=1;
				start=1;
			}
		}
		else
		{
			if(flag1 == 1 && flag1_check == 0)
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
				left_turn=1;
			}
		}
		if(flag5 == 1 && flag5_check == 0)
		{
			forward_mm(60);
			left_degrees(80);
			adjust_left();
			green_led_on();
			pick_up(pizza);			//reached at pizza
			left_degrees(85);
			adjust_left();
			flag5_check=1;
			picked_pizza=1;
			flag_reset();
		}
	}
	else
	{
		forward_mm(20);
		if(flag3 == 1 && flag3_check==0)
		{
			i++;
			readdy=1;
			flag_reset();
			
		}
	}
	
}
/*
* Function Name: pizza1()
* Input		 : 
* Output		 : 
* Logic		 : this function draw robot from pizza counter top pizza 1, this will use node to take decision about next turn
* Example Call : pizza1();
*/
void pizza1(void)
{
	first_time_reached_pizza[1]= 1;

	forward();
	if(picked_pizza == 0)
	{
		
		forward();
		if(start == 0)
		{
			if(flag1==1 && flag1_check==0)
			{
				
				forward_mm(60);
				left_degrees(85);
				adjust_left();
				left_turn=1;
				flag1_check=1;
				start=1;
			}
		}
		else
		{
			if(flag1 == 1 && flag1_check == 0)
			{
				forward_mm(60);
				right_degrees(85);
				adjust_right();
				flag1_check=1;
				left_turn=1;
			}
		}
		if(flag6 == 1 && flag6_check == 0)
		{
			forward_mm(60);
			left_degrees(80);
			adjust_left();
			blue_led_on();
			pick_up(pizza);					//reached at pizza
			left_degrees(85);
			adjust_left();
			flag6_check=1;
			picked_pizza=1;
			flag_reset();
		}
	}
	else
	{
		forward_mm(20);
		if(flag4 == 1 && flag4_check==0)
		{
			i++;
			picked_pizza=1;
			readdy=1;
			flag_reset();
			
		}
	}
}
/*
* Function Name: flag_reset()
* Input		 : 
* Output		 : 
* Logic		 : this function will reset all flag
* Example Call : pizza1();
*/	
void flag_reset(void)
{
	flag1=0; flag1_check=0;
	flag2=0; flag2_check=0;
	flag3=0; flag3_check=0;
	flag4=0; flag4_check=0;
	flag5=0; flag5_check=0;
	flag6=0; flag6_check=0;
	flag7=0; flag7_check=0;
	flag8=0; flag8_check=0;
}
/*
* Function Name: linefollower(int home, int pizza)
* Input		 : 
* Output		 : 
* Logic		 : this function mainly use to follow the line,here two argument are there 1) int home    2) int pizza     this says robot to drop this pizza to that home, 
				here node_detector() function will call and from which apropriat pizza or home function will call
* Example Call : linefollower(1,);  //pizza will be pick from pick up poit 2 ,and will be delivered tohome 1 
*/	
void linefollower(int home , int pizza)
{
	
	
	
	
	while(1)
	{
		
		
		
		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
		
		
		node_detector(home,pizza);
		
		forward();
		
		if(Left_white_line > cut_off &&  Center_white_line <cut_off && Right_white_line <cut_off)
		{
			stop();
			velocity(220,250);
			soft_left();	//soft left
		}
		
		
		if(Left_white_line<cut_off && Center_white_line > cut_off && Right_white_line <cut_off)
		{
			
			forward();
		}
		
		if(Left_white_line< cut_off && Center_white_line <cut_off && Right_white_line >cut_off)
		{
			stop();
			velocity(250,220);
			soft_right();	//soft right
		}
		
	}
}
/*
* Function Name: pick_up()
* Input		 : 
* Output		 : 
* Logic		 : this function will will pick the pizza and glow led according to pizza color
* Example Call : pick_up();
*/

void pick_up(int pizza)
{
			
		
			if(pizza_counter[pizza][0] == R)			//check the condition for turning on the led of red color
			{
				red_led_on();						//this function is called to turn on red led
			}
			else if(pizza_counter[pizza][0] == G)     //check the condition for turning on the led of blue color
			{
				green_led_on();						//this function will turn on the green led
			}
			else if(pizza_counter[pizza][0] == B)		//check the condition for turning on the blue led
			{
				blue_led_on();						//this function will turn on the blue led
			}
	back_mm(140);
	/*_delay_ms(15);
	servo_2(0);
	_delay_ms(1000);
	servo_2_free();*/
	
	
	servo_1(160);
	_delay_ms(1000);
	
	
	forward_mm(60);
	_delay_ms(50);
	servo_2(100);
	_delay_ms(1000);
	servo_2_free();
	
	servo_1(80);
	_delay_ms(1000);
	servo_1_free();

	forward_mm(70);
	




}

/*
* Function Name: drop()
* Input		 : 
* Output		 : 
* Logic		 : this function will will pick the pizza and glow led according to pizza color
* Example Call : drop();
*/
void drop(void)
{
	back_mm(110);
	servo_1(155);
	_delay_ms(1000);

	servo_2(0);
	buzzer_on();
	freeze =1;
	led_off();
	_delay_ms(1000);
	buzzer_off();
	servo_2_free();

	servo_1(80);
	_delay_ms(1000);
	servo_1_free();
	forward_mm(100);
	
}




//Main Function
int main()
{
	init_devices(); //init all devices refer all header file 
	
	color_sensor_scaling(); // from color+sharp.h
	servo_1(80); // set servo to proper position
	_delay_ms(1000);
	//servo_1_free();
	servo_2(0);// set servo to proper position
	_delay_ms(1000);
	servo_2_free();// set servo to proper position

	
	velocity(250,250);
	forward();
	detector(); //scan all pizza at very first time to know all pizza color and size
	y= count_time;	//current time

	sorting(timeline, row , y); // decide which pizza it should pick first
	linefollower(home,pizza); // start delivering!!!
}

