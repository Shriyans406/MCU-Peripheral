/*
 * 017rtc_lcd.c
 *
 *  Created on: Jul 19, 2026
 *      Author: HP
 */


#include<stdio.h>
#include "ds1307.h"

#include "lcd.h"

int main(void)
{
    // Your code here

	RTC_time_t current_time;
	RTC_date_t current_date;

	printf("RTC test\n");

	if(ds1307_init()){
			printf("RTC init has failed\n");
			while(1);
	}

	current_date.day = FRIDAY;
	current_date.date = 15;
	current_date.month = 1;
	current_date.year = 21;

	current_time.hours = 11;
	current_time.minutes = 59;
	current_time.seconds = 30;
	current_time.time_format = TIME_FORMAT_12HRS_PM;

	ds1307_set_current_date(&current_date);
	ds1307_set_current_time(&current_time);

	ds1307_get_current_time(&current_time);
	ds1307_get_current_date(&current_date);

	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24HRS){
		am_pm = (current_time.time_format) ? "PM" : "AM";
		printf("Current time = %s %s\n",time_to_string(&current_time),am_pm);
	}

    return 0;
}
