#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Calendar program

int g_month_start_day;
char g_weekday[3], g_month_start_weekday[3];

int get_no_of_days_in_month(int month, int year) {
    // April, June, September, November have 30 days
    // February has 28 / 29 days depending on leap years
    //      Leap year occurs on every year divisible by 4 but not if divisible by 100
    //      Leap year is also when a year is divisible by 400
    //      Leap year means February has 29 days
   
    int months[12];

    for (int i = 0; i < 12; i++) {
        months[i] = 31;

        if (i == 3 || i == 5 || i == 8 || i == 10) {
            months[i] = 30;
        } else if (i == 1) {
            months[i] = 28;

            if (
                (year % 4 == 0 && year % 100 != 0) ||
                year % 400 == 0
            ) {
                months[i] = 29;
            }
        }
    }

    return months[month - 1];
}

void get_first_day_of_the_month(int day, int month, int year) {
    // 01.01.1900. was a Monday
    // year always has 12 months
    // month has {get_no_of_days_in_month(month, year)} days

    char days[7][10];
    char start;
    int help_year, help_month, help_day;
    int number_of_days_between_dates = 0, number_of_days_between_dates_start;
    int index_of_day, index_of_start_day;

    strcpy(days[0], "MO");
    strcpy(days[1], "TU");
    strcpy(days[2], "WE");
    strcpy(days[3], "TH");
    strcpy(days[4], "FR");
    strcpy(days[5], "SA");
    strcpy(days[6], "SU");

    if (year == 1900 && month == 1 && day == 1) {
        // If date is 01.01.1900. we know it is Monday
        start = 0;
    } else if (
        year > 1900 ||
        (year == 1900 && month > 1) || 
        (year == 1900 && month == 1 && day > 1) ||
        (year == 1900 && month == 1 && day == 1)
    ) {
        // Here we count days that are bigger then 01.01.1900.
        // By simply adding days to it
        for (int i = 1900; i <= year; i++) {
            // i - year

            if (i == year) {
                help_month = month; 
            } else {
                help_month = 12;
            }
            
            for (int j = 1; j <= help_month; j++) {
                // j - month

                if (i == year && j == help_month) {
                    help_day = day - 1;
                } else {
                    help_day = get_no_of_days_in_month(j, i);
                }

                number_of_days_between_dates += help_day;
            }
        }

        // To get the start day of the month
        // We just subtract total number of days with chosen day number
        // And add 1 day because then it will be 01. of the month
        number_of_days_between_dates_start = number_of_days_between_dates - day + 1;
    } else {
        // Do the backwards calculation
    }

    for (int k = 0; k < 7; k++) {
        // Why {number_of_days_between_dates - k} ?
        // Because if e.g. 45290 / 7 is not 0
        // Then we do (45290 - 1) / 7
        // If that is 0, 45290th day is Tuesday
        // Because 45290 is not Monday, so if we subtract 1 from it
        // 45290 must be Tuesday because 45289 is Monday

        if ((number_of_days_between_dates - k) % 7 == 0) {
            index_of_day = k;
        }
       
        if ((number_of_days_between_dates_start - k) % 7 == 0) {
            index_of_start_day = k;
        }
    }

    g_month_start_day = index_of_start_day;
    strcpy(g_month_start_weekday, days[index_of_start_day]);
    strcpy(g_weekday, days[index_of_day]);
}

void draw_calendar(int day, int month, int year) {
    int weekdays[7];
    char s_weekdays[7][3];
    int last_index, last_day = 0, over_limit = 0;

    printf("\n");
    printf("Showing for date: %d.%d.%d.\n", day, month, year);
    printf("\n");

    get_first_day_of_the_month(day, month, year);

    // printf("Weekday: %s\n", g_weekday);
    // printf("Mo Start Index: %d\n", g_month_start_day);
    // printf("Mo Start Weekday: %s\n", g_month_start_weekday);

    printf("+----+----+----+----+----+----+----+\n");
    printf("| MO | TU | WE | TH | FR | SA | SU |\n");
    printf("+----+----+----+----+----+----+----+\n");

    last_index = g_month_start_day;

    for (int i = 0; i < 6; i++) {
        // i - represents weeks on a calendar (rows)

        for (int j = 0; j < 7; j++) {
            // j - are days in the week

            weekdays[j] = 0;

            if (over_limit == 0) {
                if (last_index > j) {
                    // Setting empty spaces until we find 
                    // a day (j) when the month starts
                    weekdays[j] = 0;
                } else if (last_index == j) {
                    // Setting the start of the month
                    weekdays[j] = 1;
                } else {
                    // j == 0 is Monday and on each Monday we need to
                    // take the last Sundays number and add 1 to it
                    // That's why we have {last_day} - to remember
                    // Sunday's value
                    if (j == 0) {
                        weekdays[j] = last_day + 1;
                    } else {
                        // This is just taking the number from a day
                        // before it, and increments it to be the next day
                        weekdays[j] = weekdays[j - 1] + 1;
                    }
                }
            }

            if (weekdays[j] >= get_no_of_days_in_month(month, year)) {
                over_limit = 1;
            }

            // Creating the string instead of integer
            // Need this to show empty spaces
            if (weekdays[j] == 0) {
                strcpy(s_weekdays[j], "  ");
            } else {
                // Converting int to string
                sprintf(s_weekdays[j], "%02d", weekdays[j]);
            }
        }

        last_day = weekdays[6];
        last_index = -1;

        printf(
            "| %s | %s | %s | %s | %s | %s | %s |\n",
            s_weekdays[0], s_weekdays[1], s_weekdays[2], s_weekdays[3], s_weekdays[4], s_weekdays[5], s_weekdays[6]
        );
    }
    
    printf("+----+----+----+----+----+----+----+\n");
}

int month_name_checker(char month[]) {
    char lowercase[10];
    int month_number = 0;

    for (int i = 0; i < 10; i++) {
        lowercase[i] = tolower(month[i]);
    }

    if (
        strcmp(lowercase, "january") == 0 ||
        strcmp(lowercase, "jan") == 0 ||
        strcmp(lowercase, "1") == 0
    ) {
        month_number = 1;
    }
    
    if (
        strcmp(lowercase, "february") == 0 ||
        strcmp(lowercase, "feb") == 0 ||
        strcmp(lowercase, "2") == 0
    ) {
        month_number = 2;
    }
    
    if (
        strcmp(lowercase, "march") == 0 ||
        strcmp(lowercase, "mar") == 0 ||
        strcmp(lowercase, "3") == 0
    ) {
        month_number = 3;
    }
    
    if (
        strcmp(lowercase, "apirl") == 0 ||
        strcmp(lowercase, "apr") == 0 ||
        strcmp(lowercase, "4") == 0
    ) {
        month_number = 4;
    }
    
    if (
        strcmp(lowercase, "may") == 0 ||
        strcmp(lowercase, "5") == 0
    ) {
        month_number = 5;
    }
    
    if (
        strcmp(lowercase, "jun") == 0 ||
        strcmp(lowercase, "6") == 0
    ) {
        month_number = 6;
    }
    
    if (
        strcmp(lowercase, "july") == 0 ||
        strcmp(lowercase, "jul") == 0 ||
        strcmp(lowercase, "7") == 0
    ) {
        month_number = 7;
    }
    
    if (
        strcmp(lowercase, "august") == 0 ||
        strcmp(lowercase, "aug") == 0 ||
        strcmp(lowercase, "8") == 0
    ) {
        month_number = 8;
    }
    
    if (
        strcmp(lowercase, "september") == 0 ||
        strcmp(lowercase, "sep") == 0 ||
        strcmp(lowercase, "9") == 0
    ) {
        month_number = 9;
    }
    
    if (
        strcmp(lowercase, "october") == 0 ||
        strcmp(lowercase, "oct") == 0 ||
        strcmp(lowercase, "10") == 0
    ) {
        month_number = 10;
    }
    
    if (
        strcmp(lowercase, "november") == 0 ||
        strcmp(lowercase, "nov") == 0 ||
        strcmp(lowercase, "11") == 0
    ) {
        month_number = 11;
    }
    
    if (
        strcmp(lowercase, "december") == 0 ||
        strcmp(lowercase, "dec") == 0 ||
        strcmp(lowercase, "12") == 0
    ) {
        month_number = 12;
    }

    return month_number;
}

int main() {
    char input_month[10] = "apr";
    int year = 2026;
    int day = 1;
    int month;

    printf("Which year do you want the calendar to be shown for?\n");
    scanf("%d", &year);
    
    printf("Which month do you want the calendar to be shown for?\n");
    printf("* You can write month name, month number or short name of the month (three letters).\n");
    scanf("%s", &*input_month);

    printf("Which day do you want the calendar to be shown for?\n");
    scanf("%d", &day);

    month = month_name_checker(input_month);

    draw_calendar(day, month, year);

    return 0;
}