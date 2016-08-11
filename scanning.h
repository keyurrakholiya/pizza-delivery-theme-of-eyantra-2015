/*
*Team Id	:	eYRC-PD#1260
*Author List:	keyur Rakholiya , Earnest Vekariya , Jay parekh , Sanket Bhimani
*Filename	:	Scanning.h
*Theme		:	Pizza delivery theme (eYRC)
*Functions  :	node_detector2(void) , detector , glow_led(int) , pizza_scanner(void) , set_value(void)


#define R 1
#define G 2
#define B 3

#define S 1
#define M 2
#define L 3

we have define this in main function for our simplicity in writing and reading of code.
*/



/*
* Function Name: detector
* Input		 : 
* Output		 : 
* Logic		 : to follow the black line using three white line sensor
* Example Call : detector();
*/

void detector(void)
{
	while(1)		//to repeat the same process for following black line 
	{
		
		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
		
		
		node_detector2();			//to detec node in arena this function is called
		if(node_count==21)			
		{
			y = count_time;
			sorting(timeline , row ,y);
			break;
		}
		forward();
		
		if(Left_white_line > 25 &&  Center_white_line <25 && Right_white_line <25)
		{
			stop();
			velocity(220,250);
			soft_left();	//soft left
		}
		
		
		if(Left_white_line<25 && Center_white_line >25 && Right_white_line <25)
		{
			
			velocity(250,250);
			forward();
		}
		
		if(Left_white_line< 25 && Center_white_line <25 && Right_white_line >25)
		{
			stop();
			velocity(250,220);
			soft_right();	//soft right
		}
	}
}

/*
* Function Name: node_detector2
* Input		 : 
* Output		 : 
* Logic		 : to detect the node in the arena only for the scanning of pizza
* Example Call : node_detector2();
*/
void node_detector2(void)
{
	Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
	
	if(((Left_white_line > cut_off &&  Center_white_line > cut_off ) || (Right_white_line> cut_off && Center_white_line> cut_off )) && (ShaftCountLeft>13 || ShaftCountRight>13))
	{
		ShaftCountRight=0,ShaftCountLeft=0;		//this are the safety variables, which is enable to detect only one NODE at a time.
		node_count++;							// node count will increase after one node is detected properly
		
		switch(node_count)
		{
		case 1:
			forward_mm(60);		//robot reaches at pizza counter and take left for scanning of left side's counter
			left_degrees(85);
			adjust_left();		//this function is written in 'main.h'
			break;
			
		case 2:
			stop();
			forward_mm(20);		
			pizza_scanner();	//robot will reach at pick up point 5 and scan the pizza
			pizza_no=5;
			set_value();		//it will store the hight and color according to scanning
			break;
			
		case 3:
			stop();
			forward_mm(20);
			pizza_scanner();	 //robot will reach at pick up point 4 and scan the pizza
			pizza_no = 4;
			set_value();	
			break;
			
		case 4: 
			stop();
			forward_mm(20);
			pizza_scanner();	//robot will reach at pick up point 3 and scan the pizza
			pizza_no=3;
			set_value();
			break;
			
		case 5:
			stop();
			forward_mm(20);
			pizza_scanner();	//robot will reach at pick up point 2 and scan the pizza
			pizza_no=2;
			set_value();
			break;
			
		case 6:
			stop();
			forward_mm(20);
			pizza_scanner();	//robot will reach at pick up point 1 and scan the pizza
			forward_mm(50);
			right_degrees(175);			//robot will take 180 and go straight ignoring node and will reach on the other and of pick up point
			adjust_right();				//see the position of sensors on bot.
			pizza_no=1;
			set_value();
			break;
			
		case 16:			
			forward_mm(110);
			right_degrees(180);			//robot will reach at pick up point 10 and scan the pizza
			adjust_right();
			stop();
			pizza_scanner();
			pizza_no=10;
			set_value();
			break;
			
		case 17:
			stop();
			forward_mm(20);
			pizza_scanner();			//robot will reach at pick up point 9 and scan the pizza
			pizza_no = 9;
			set_value();
			break;
			
		case 18:
			stop();
			forward_mm(20);
			pizza_scanner();			//robot will reach at pick up point 8 and scan the pizza
			pizza_no=8;
			set_value();
			break;
			
		case 19:
			stop();
			forward_mm(20);
			pizza_scanner();			//robot will reach at pick up point 7 and scan the pizza
			pizza_no = 7;
			set_value();
			break;
			
		case 20:
			stop();
			forward_mm(20);
			pizza_scanner();			//robot will reach at pick up point 6 and scan the pizza
			pizza_no = 6;
			set_value();
			break;
			
		case 21:					//now it will reaches at pick up point and reddy for pick up the pizza
			forward_mm(60);
			right_degrees(85);
			adjust_right();
			forward_mm(130);
			right_degrees(180);
			start = 1;
			break;
			
			
		}
		
	}
}

/*
* Function Name: set_value
* Input		 : 
* Output		 : 
* Logic		 : it will store hight and color for particular location(pizza pick up points)
* Example Call : set_value();
*/
void set_value(void)					
{ 
	if(color == R)						
	{
		pizza_counter[pizza_no][0] = R;	//red pizza is detected and its value will store in array of pizza_counter(pick up points)
		
	}
	if(color == G)
	{
		pizza_counter[pizza_no][0] =G;	//green pizza is detected and its value will store in array of pizza_counter(pick up points)
	}
	if(color == B)
	{
		pizza_counter[pizza_no][0] = B;	//blue pizza is detected and its value will store in array of pizza_counter(pick up points)
	}
/*	if(color == 0)
	{
		pizza_counter[pizza_no][0] = 0; //NO pizza is detected and its value will store in array of pizza_counter(pick up points)
	}
	*/
	
	//condition for storing the hight of pizza at same location
	if(size == S)
	{
		pizza_counter[pizza_no][1] =S;			//small size pizza is detected and its value will store in array of pizza_counter(pick up points)
	}
	if(size == M)
	{
		pizza_counter[pizza_no][1] =M;			 //medium size pizza is detected and its value will store in array of pizza_counter(pick up points)
	}
	if(size == L)
	{
		pizza_counter[pizza_no][1]= L;			//large size is detected and its value will store in array of pizza_counter(pick up points)
	}	
	if(size == 0)
	{
		pizza_counter[pizza_no][1]= 0;
	}
}



/*
* Function Name: pizza_scanner
* Input		 : 
* Output		 : 
* Logic		 : it will scan the size and color of pizza by using sharp distance sensor and color sensor(see the bot)
* Example Call : set_value();
*/
void pizza_scanner(void)
{
	red_read();				
	green_read();
	blue_read();
	
	if((red > green) && (red > blue))		//this is condition for detection red pizza 
	{
		color = R;							//red color detected
		red_led_on();
	}
	else  if((green > blue) && (green>red))		//this is condition for detection green pizza 
	{
		color = G;
		green_led_on();
	}
	else if((blue > red)&& (blue > green))		//this is condition for detection blue pizza 
	{
		color = B;
		blue_led_on();
	}
	/*else if((no_ff >red) && (no_ff>blue) && (no_ff>green))		//this is condition for detection NO pizza 
	{
		color=0;	
	}*/
	
	sharp = ADC_Conversion(13);					 //this function will give the distance value from the sensor 
	value = Sharp_GP2D12_estimation(sharp);
	if(value > 80 && value< 112)		//large block
	//lcd_print(1,6,1,1);
	size = L;
	
	if(value >112 && value <150)		//med block
	//lcd_print(1,6,2,1);
	size = M;
	
	if(value >151 && value < 199)
	//lcd_print(1,6,3,1);		
	size =S;					//small
	
	if(value > 200)
	//lcd_print(1,6,0,1);			//Prints Value Of Distanc in MM measured by Sharp Sensor.
	size = 0;
}


/*
* Function Name:glow_led(int pizza)
* Input		 : 
* Output		 : 
* Logic		 : it will scan the size and color of pizza by using sharp distance sensor and color sensor(see the bot)
* Example Call :glow_led(int pizza)--this variable will enable us enable us to glow the led of the particular color
* Notes        : this function is called when the robot is reached to pick up the pizza at the pick up point
*/
void glow_led(int pizza)
{
	pizza_no = pizza;

	
}
