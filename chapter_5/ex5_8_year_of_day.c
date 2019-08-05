#include <stdio.h>

int day_of_year(int year, int month, int day);
int month_day(int year, int yearday, int *month, int *day);

int main()
{
  int leap_year = 500*4;
  int feb29 = 29;
  int dec = 12;
  int day31 = 31;
  int dec31_yearday_leap = 366;
  int dec31_yearday = 365;
  printf("year %d, month %d, day %d = %d\n",
  	 leap_year-1, dec, day31,day_of_year(leap_year-1, dec, day31));

  int res_month, res_day;
  month_day(leap_year, dec31_yearday_leap,  &res_month, &res_day);
  printf("year %d, yearday %d = month %d, day %d\n",
	 leap_year, dec31_yearday_leap, res_month, res_day);

  // check error cases
  printf("day %d = month_day %d\n",
	 dec31_yearday_leap, month_day(leap_year-1, dec31_yearday_leap,  &res_month,  &res_day));

  printf("year %d, yearday %d month_day = %d\n",
  	 leap_year-1, 1000, month_day(leap_year-1, 1000, &res_month, &res_day));
    
}

static int day_in_month[][13] = {
				 {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, 
				 {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}}; // leap year

int day_of_year(int year, int month, int day) {
  if (year < 1752)
    return -1;

  int leap;
  leap = (year%4 == 0 && year%100 != 100) || year%400 == 0;
  if (month < 1 || month > 12 || year < 1 || day < 1 || day > day_in_month[leap][month])
    return -1;
  
  for (int i = 1; i < month; i++ )
    day += day_in_month[leap][i];

  return day;
}

int month_day(int year, int yearday, int *month, int *day) {
  if (year < 1752)
    return -1;
  
  int leap = (year%4 == 0 && year%100 != 100) || year%400 == 0;
  if (yearday < 1 || (yearday > 365 && !leap) || (yearday > 366 && leap) || year < 1)
    return -1;
    
  int i;
  for (i = 1; yearday > day_in_month[leap][i]; i++)
    yearday -= day_in_month[leap][i];

  *month = i;
  *day = yearday;
  return 1;
}
