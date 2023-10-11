
/*
 * main.c
 *
 *  Created on: Oct 10, 2023
 *      Author: Ebrahim Mostafa
 */


#include "lSTD_types.h"
#include "lBIT_math.h"

#define F_CPU 8000000UL
#include "util/delay.h"

#include "MDIO_interface.h"
#include "HLCD_interface.h"
#include "HLCD_config.h"
#include "HKPD_interface.h"

f32 function(f32 number1,f32 number2 ,u8 operation);
f32 power(u16 base,u16 powerer);

int main(void)
{
	HLCD_voidInit();
	HLCD_voidWriteString("Welcome",0,5);
	_delay_ms(3000);
	HLCD_voidClearScreen();
	f32 ans =0;
	u8 charcter,operation,flag_num2=0;
	f32 number1=0,number2=0;
	u8 flag_dot=0;
	u8 flag_operation_done=0;
	u8 i=1;
	while(1)
	{
		charcter=HKPD_charGetKey();
		_delay_ms(200);
		switch(charcter)
		{
		case '+':
		case '-':
		case '/':
		case '*':
			operation=charcter;
			flag_num2=1;
			flag_dot=0;
			if(flag_operation_done)
			{
				HLCD_voidClearScreen();
				HLCD_voidWriteString("ans",0,0);
				flag_operation_done=0;
				number2=0;
			}
			HLCD_voidWriteChar(charcter);
			i=1;
			break;
		case'=':
			HLCD_voidWriteChar(charcter);
			if(operation=='/'&&number2==0)
			{
				HLCD_voidWriteString("Math ERROR",1,0);
			}
			else
			{
				ans=function(number1,number2,operation);
				HLCD_voidWriteString("ans=",1,0);
				HLCD_voidWriteInteger(ans);
				number1=ans;
			}
			flag_dot=0;
			flag_operation_done=1;
			break;
		case '.':
			HLCD_voidWriteChar(charcter);
			flag_dot=1;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if(flag_operation_done)
			{
				HLCD_voidClearScreen();
				number1=0;number2=0;
				flag_operation_done=0;
				flag_num2=0;
			}
			HLCD_voidWriteChar(charcter);
			if(flag_num2)
			{
				if(flag_dot)
				{
					number2=number2 + ((charcter-'0')/power(10,i));
					i++;
				}
				else
				{
					number2=number2 * 10.0 + (charcter-'0');
				}
			}
			else
			{
				if(flag_dot)
				{
					number1=number1 + ((charcter-'0')/power(10,i));
					i++;
				}
				else
				{
					number1=number1 * 10.0 + (charcter-'0');
				}
			}
			flag_operation_done=0;
						break;

		};
	}
	return 0;
}
f32 function(f32 number1,f32 number2 ,u8 operation)
{
	switch (operation)
	{
	case '+' :
		return number1+number2;
	case '-' :
		return number1-number2;
	case '*' :
		return number1*number2;
	case '/' :
		return number1/number2;
	}
}
f32 power(u16 base,u16 powerer){
	f32 x =1;
	for(int i=0 ;i<powerer;i++){
		x *= base;
	}
	return x;
}

