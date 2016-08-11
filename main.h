/*
*Team Id	:	eYRC-PD#1260
*Author List:	keyur Rakholiya , Earnest Vekariya , Jay parekh , Sanket Bhimani
*Filename	:	main.h
*Theme		:	Pizza delivery theme (eYRC)
*Functions  :	adjust_left() , adjust_right() , angle_rotate(unsigned int) , back() , back_mm() , blue_led_on() , buzzer_on() , buzzer_off() , buzzer_pin_config() ,
				displaytime() , forward_mm() , forward() , green_led_on() , init_devices() , ISR(INT4_vect) , ISR(INT5_vect) , ISR(TIMER4_OVF_vect) , led() ,
				led_off(),left_degrees() , left_position_encoder_interrupt_int() , linear_distance_mm() , motion_pin_config() , motion_set() , motors_delay(),
				on_ssd2() , on_ssd3() , on_ssd4() ,port_init() , port_init() , red_led_on() , right() , right_degrees() , right_encoder_pin_config() , right_position_encoder ,
				set_eight() , set_five() , set_four() , set_three() ,set_two() ,set_one() ,set_nine() , soft_left() ,soft_left_2() ,soft_left_2_degrees() , 
				soft_right() , soft_right_2() , timer4_init() , timer5_init() , velocity(unsigned char,unsigned char) 

*/


void port_init();
void timer5_init();
void velocity(unsigned char, unsigned char);
void motors_delay();

unsigned char ADC_Conversion(unsigned char);   
unsigned char ADC_Value;
unsigned char flag = 0,count=0;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;


volatile unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder
volatile unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
volatile unsigned int Degrees; //to accept angle in degrees for turning

int freeze=0;


//This ISR will be used to schedule events like refreshing ADC data
/*
* Function Name: TIMER4_OVF_vect (timer interrupt)
* Input		 : -
* Output		 : -
* Logic		 : it will create the interrupt after 7 ms
* Example Call : this function will be called when timer overflow flag bit overflow
*/

ISR(TIMER4_OVF_vect)
{
	//TIMER4 has overflowed
	TCNT4H = 0xF9; //reload counter high value
	TCNT4L = 0xB3; //reload counter low value
	displaytime(); // this function will print the value on seven segment display var to be printed is int a, int b, int c
	count_display++;// this will incremented in every 7 millisecond
	if(count_display > 715) // this will used for freezing display for 5 second when pizza is dropped 
	{
		freeze = 0; //this will reset freeze flag, and freeze flag will set from drop function
		count_time = count_time+5;
	}
	if(count_display>143 && freeze == 0) //when count_display becomes 143 then 1 second is counted because 143*7 = 1000 (in milli seconds)

	{
		count_time++;					//count of every second
		a = count_time/10;
		if(a>=10)
		{
			a = a%10;						//setting ten digit
		}
		b = count_time%10;				// setting unit digit
		c = count_time/100;				// setting hundred digit
		count_display=0;				// reset count_display
	}
}
/*
* Function Name: adjust_left()
* Input		 : 
* Output		 : 
* Logic		 : it will rotate left until robot comes on the black line
* Example Call : adjust_left();
*/

void adjust_left()
{
	while(1)
	{
		
		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor

		if(Left_white_line<25 && Center_white_line>25 && Right_white_line<25) //this condition will check weather robot is on black line or not and if it is on black line than it will break the loop
		{
			break;
		}
		velocity(200,200);
		left();//this will take left turn until it find black line


	}
}
/*
* Function Name: adjust_right()
* Input		 : 
* Output		 : 
* Logic		 : it will rotate right until robot comes on the black line
* Example Call : adjust_right();
*/



void adjust_right()
{
	while(1)
	{
		

		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor

		if(Left_white_line<25 && Center_white_line>25 && Right_white_line<25)//this condition will check weather robot is on black line or not and if it is on black line than it will break the loop
		{
			break;
		}
		velocity(220,220);
		right(); //it will take right turn until it find black line

	}
}

void left_encoder_pin_config (void)
{
	DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}

//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
void right_encoder_pin_config (void)
{
	DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 4 pin
}

//Function to initialize ports
void port_init()
{
	motion_pin_config(); //robot motion pins config
	left_encoder_pin_config(); //left encoder pin config
	right_encoder_pin_config(); //right encoder pin config
	buzzer_pin_config();


	servo1_pin_config(); //Configure PORTB 5 pin for servo motor 1 operation
	servo2_pin_config(); //Configure PORTB 6 pin for servo motor 2 operation

	adc_pin_config();
	seven_seg_pin();
	
	led();
	
	
	color_sensor_pin_config();//color sensor pin configuration
	

}

void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
	EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
	sei();   // Enables the global interrupt
}

void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
	EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
	sei();   // Enables the global interrupt
}

//ISR for right position encoder
ISR(INT5_vect)
{
	ShaftCountRight++;  //increment right shaft position count
}


//ISR for left position encoder
ISR(INT4_vect)
{
	ShaftCountLeft++;  //increment left shaft position count
}
void motion_pin_config (void)
{
	DDRA = DDRA | 0x0F;
	PORTA = PORTA & 0xF0;
	DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}

//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
	unsigned char PortARestore = 0;

	Direction &= 0x0F; 		// removing upper nibbel for the protection
	PortARestore = PORTA; 		// reading the PORTA original status
	PortARestore &= 0xF0; 		// making lower direction nibbel to 0
	PortARestore |= Direction; // adding lower nibbel for forward command and restoring the PORTA status
	PORTA = PortARestore; 		// executing the command
}

void forward (void) //both wheels forward
{
	motion_set(0x06);
}

void back (void) //both wheels backward
{
	motion_set(0x09);
}

void left (void) //Left wheel backward, Right wheel forward
{
	velocity(200,200);
	motion_set(0x05);
}

void right (void) //Left wheel forward, Right wheel backward
{
	velocity(200,200);
	motion_set(0x0A);
}

void soft_left (void) //Left wheel stationary, Right wheel forward
{
	motion_set(0x04);
}

void soft_right (void) //Left wheel forward, Right wheel is stationary
{
	motion_set(0x02);
}

void soft_left_2 (void) //Left wheel backward, right wheel stationary
{
	motion_set(0x01);
}

void soft_right_2 (void) //Left wheel stationary, Right wheel backward
{
	motion_set(0x08);
}

void stop (void)
{
	motion_set(0x00);
}


//Function used for turning robot by specified degrees
void angle_rotate(unsigned int Degrees)
{
	float ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

	ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
	ShaftCountRight = 0;
	ShaftCountLeft = 0;

	while (1)
	{


		if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
		{

			break;
		}
	}
	stop(); //Stop robot
}

//Function used for moving robot forward by specified distance

void linear_distance_mm(unsigned int DistanceInMM)
{
	float ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

	ReqdShaftCount = DistanceInMM / 5.338; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;

	ShaftCountRight = 0;
	while(1)
	{
		if(ShaftCountRight > ReqdShaftCountInt)
		{
			break;
		}
	}
	stop(); //Stop robot
}



void forward_mm(unsigned int DistanceInMM)
{
	forward();
	linear_distance_mm(DistanceInMM);
}

void back_mm(unsigned int DistanceInMM)
{
	back();
	linear_distance_mm(DistanceInMM);
}

void left_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation 4.090 degrees per count
	left(); //Turn left
	angle_rotate(Degrees);
}



void right_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation 4.090 degrees per count
	right(); //Turn right
	angle_rotate(Degrees);
}


void soft_left_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per count
	soft_left(); //Turn soft left
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}

void soft_right_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per count
	soft_right();  //Turn soft right
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}


//ADC pin configuration
void adc_pin_config (void)
{
	DDRF = 0x00;
	PORTF = 0x00;
	DDRK = 0x00;
	PORTK = 0x00;
}

void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
					For Overriding normal port functionality to OCRnA outputs.
						{WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/

	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

//Function For ADC Conversion
unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}



//Function for velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}

void soft_left_2_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per count
	soft_left_2(); //Turn reverse soft left
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}

void soft_right_2_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation 2.045 degrees per count
	soft_right_2();  //Turn reverse soft right
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}

//Function to initialize all the devices
void init_devices()
{
	cli(); //Clears the global interrupt
	port_init();  //Initializes all the ports
	left_position_encoder_interrupt_init();
	right_position_encoder_interrupt_init();
	adc_init();
	timer1_init();
	timer5_init();
	timer4_init();
	color_sensor_pin_interrupt_init();
	TIMSK4 = 0x01; //timer4 overflow interrupt enable
	sei();   // Enables the global interrupt
	
	
}


void buzzer_pin_config (void)
{
	DDRC = DDRC | 0x08;		//Setting PORTC 3 as output
	PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer
}



void buzzer_on (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore | 0x08;
	PORTC = port_restore;
}

void buzzer_off (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore & 0xF7;
	PORTC = port_restore;
}

void seven_seg_pin(void)
{
	DDRJ = DDRJ | 0xFF;		//set all J L port pin as out put.
	DDRL = DDRL | 0xC7;     //set only 4 pin as output. ss0 ss1 ss2 ss3
}


void on_ssd2(void) // on second digit
{
	PORTL=0x04;	 //pin 30
}

void on_ssd3(void) //on third digit
{
	PORTL=0x02; //pin 27
}


void on_ssd4(void) // on fourth digit
{
	PORTL = 0x01; //pin 28 on
}

// this all the functions are for set value 0 to 9 on seven segment display respectively
void set_zero(void)
{
	PORTJ=0x03;
}
void set_one(void)
{
	PORTJ=0x9F;
}
void set_two(void)
{
	PORTJ=0x25;
}
void set_three(void)
{
	PORTJ=0x0D;
}

void set_four(void)
{
	PORTJ=0x99;
}

void set_five(void)
{
	PORTJ=0x49;
}
void set_six(void)
{
	PORTJ=0x41;
}
void set_seven(void)
{
	PORTJ=0x1F;
}
void set_eight(void)
{
	PORTJ=0x01;
}

void set_nine(void)
{
	PORTJ=0x09;
}
//TIMER4 initialize - prescale:64
// WGM: 0) Normal, TOP=0xFFFF
// desired value: 1Hz
// actual value:  1.000Hz (0.0%)
void timer4_init(void)
{
	TCCR4B = 0x00; //stop
	TCNT4H = 0xF9; //Counter higher 8 bit value
	TCNT4L = 0xB3; //Counter lower 8 bit value
	OCR4AH = 0x00; //Output Compare Register (OCR)- Not used
	OCR4AL = 0x00; //Output Compare Register (OCR)- Not used
	OCR4BH = 0x00; //Output Compare Register (OCR)- Not used
	OCR4BL = 0x00; //Output Compare Register (OCR)- Not used
	OCR4CH = 0x00; //Output Compare Register (OCR)- Not used
	OCR4CL = 0x00; //Output Compare Register (OCR)- Not used
	ICR4H  = 0x00; //Input Capture Register (ICR)- Not used
	ICR4L  = 0x00; //Input Capture Register (ICR)- Not used
	TCCR4A = 0x00;
	TCCR4C = 0x00;
	TCCR4B = 0x03; //start Timer
}


void displaytime(){
	
	//on_ssd2() function power on the third digit of ss display
	//on_ssd3() function power on the second digit of ss display
	//on_ssd4() function power on the first digit of ss display
	//check condition for thousand digit
	if(c==0)
	{
		on_ssd2(); 
		set_zero();
		_delay_ms(1);
	}
	else if(c==1)
	{
		on_ssd2();
		set_one();
		_delay_ms(1);
	}
	else if(c==2)
	{
		on_ssd2();
		set_two();
		_delay_ms(1);
	}
	
	else if(c==3)
	{
		on_ssd2();
		set_three();
		_delay_ms(1);
	}
	else if(c==4)
	{
		on_ssd2();
		set_four();
		_delay_ms(1);
	}
	
	else if(c==5)
	{
		on_ssd2();
		set_five();
		_delay_ms(1);
	}
	
	else if(c==6)
	{
		on_ssd2();
		set_six();
		_delay_ms(1);
	}
	
	else if(c==7)
	{
		on_ssd2();
		set_seven();
		_delay_ms(1);
	}
	
	else if(c==8)
	{
		on_ssd2();
		set_eight();
		_delay_ms(1);
	}
	
	else if(c==9)
	{
		on_ssd2();
		set_nine();
		_delay_ms(1);
	}
	
	//check condition for ten digit(ss3)

	if(a==0)
	{
		on_ssd3();
		set_zero();
		_delay_ms(1);
	}
	else if(a==1)
	{
		on_ssd3();
		set_one();
		_delay_ms(1);
	}
	
	else if(a==2)
	{
		on_ssd3();
		set_two();
		_delay_ms(1);
	}
	
	else if(a==3)
	{
		on_ssd3();
		set_three();
		_delay_ms(1);
	}
	else if(a==4)
	{
		on_ssd3();
		set_four();
		_delay_ms(1);
	}
	
	else if(a==5)
	{
		on_ssd3();
		set_five();
		_delay_ms(1);
	}
	
	else if(a==6)
	{
		on_ssd3();
		set_six();
		_delay_ms(1);
	}
	
	else if(a==7)
	{
		on_ssd3();
		set_seven();
		_delay_ms(1);
	}
	
	else if(a==8)
	{
		on_ssd3();
		set_eight();
		_delay_ms(1);
	}
	
	else if(a==9)
	{
		on_ssd3();
		set_nine();
		_delay_ms(1);
	}
	
	//check condition for unit digit
	if(b==0)
	{
		on_ssd4();
		set_zero();
		_delay_ms(1);
	}
	
	else if(b==1)
	{
		on_ssd4();
		set_one();
		_delay_ms(1);
	}
	else if(b==2)
	{
		on_ssd4();
		set_two();
		_delay_ms(1);
	}
	else if(b==3)
	{
		on_ssd4();
		set_three();
		_delay_ms(1);
	}
	
	else if(b==4)
	{
		on_ssd4();
		set_four();
		_delay_ms(1);
	}
	
	else if(b==5)
	{
		on_ssd4();
		set_five();
		_delay_ms(1);
	}
	
	else if(b==6)
	{
		on_ssd4();
		set_six();
		_delay_ms(1);
	}
	
	else if(b==7)
	{
		on_ssd4();
		set_seven();
		_delay_ms(1);
	}
	
	else if(b==8)
	{
		on_ssd4();
		set_eight();
		_delay_ms(1);
	}
	
	else if(b==9)
	{
		on_ssd4();
		set_nine();
		_delay_ms(1);
	}
}


// anode pin is connected to pin no . 26
void led(void)
{
	DDRH = DDRH | 0xF0; //set only 4 bit as out put of port h
}

void red_led_on(void)
{
	PORTH = 0xE0;	//pin 6 set as out put for red led
}
void green_led_on(void)
{
	PORTH = 0xD0;	//pin 5 set as out put for green led
}
void blue_led_on(void)
{
	PORTH = 0xB0;	//pin 4 set as out put for blue led
}
void led_off(void)
{
	PORTH = 0x00;
}