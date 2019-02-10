/******************************************
 * Jakob Precht
 * jprech18@student.aau.dk
 * A406b
 * Software
 ******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

/******************************************/
/* DEFINES */
/******************************************/

/* File containing all the race data */
#define FILE_PATH                   "cykelloeb.txt"

#define NUMBER_OF_DATA_PER_LINE     7
#define RACE_NAME_SIZE              28
#define RIDER_NAME_SIZE             44
#define TEAM_NAME_SIZE              4
#define COUNTRY_CODE_SIZE           4
#define RANKING_SIZE                4
#define TIME_SIZE                   9

#define DID_NOT_FINISH              "DNF"
#define OVER_TIME_LIMIT             "OTL"
#define RIDER_NAME_SPLITTER         " "

#define ASSIGNMENT_1_NAME           "Results for italian riders over 30 years old"
#define ASSIGNMENT_2_NAME           "All danish riders who has completed one or more of the races"
#define ASSIGNMENT_3_NAME           "The 10 riders who has got the most points in total"
#define ASSIGNMENT_4_NAME           "Rider(s) who has completed Paris Roubaix and Amstel Gold Race with the best time in total"
#define ASSIGNMENT_5_NAME           "Average age among the riders who has placed in the top 10 in one more of the races"

#define ASSIGNMENT_1_COUNTRY_CODE   "ITA"
#define ASSIGNMENT_1_AGE            30
#define ASSIGNMENT_2_COUNTRY_CODE   "DEN"
#define ASSIGNMENT_3_NO_OF_RIDERS   10
#define ASSIGNMENT_4_RACE_1         "ParisRoubaix"
#define ASSIGNMENT_4_RACE_2         "AmstelGoldRace"
#define ASSIGNMENT_5_MAX_RANKING    10

#define DATA_TOTAL_LENGTH           65
#define HEADER_TOTAL_LENGTH         70

/*******************************************/
/* DATA STRUCTURES */
/*******************************************/

typedef struct tm TIME_DATA;

/* RACE DATA struct */
typedef struct {
    char race_name[RACE_NAME_SIZE];
    char rider_name[RIDER_NAME_SIZE];
    char first_name[RIDER_NAME_SIZE];
    char last_name[RIDER_NAME_SIZE];
    int age;
    char team[TEAM_NAME_SIZE];
    char country[COUNTRY_CODE_SIZE];
    char ranking[RANKING_SIZE];
    char time[TIME_SIZE];
    int score;
} RACE_DATA;

/* RIDER DATA struct */
typedef struct {
    char name[RIDER_NAME_SIZE];
    char first_name[RIDER_NAME_SIZE];
    char last_name[RIDER_NAME_SIZE];
    int age;
    char team[TEAM_NAME_SIZE];
    char country[COUNTRY_CODE_SIZE];
    int total_score;
    int number_of_races_attended;
    int number_of_races_completed;
    RACE_DATA *race_data_arr;
} RIDER_DATA;

/* RIDER TIME DATA struct */
typedef struct {
    TIME_DATA time_data;
    RIDER_DATA *rider_data;
} RIDER_TIME_DATA;

/* RACE struct */
typedef struct {
    char name[RACE_NAME_SIZE];
    int number_of_participants;
} RACE;

/******************************************/
/* PROTOTYPES */
/******************************************/

int read_race_data_file(const char *, RACE_DATA **, int *, RIDER_DATA **, int *);

void print_assignment_1(const RIDER_DATA *, int);

void print_assignment_2(const RIDER_DATA *, int);

void print_assignment_3(const RIDER_DATA *, int);

void print_assignment_4(const RIDER_DATA *, int);

void print_assignment_5(const RIDER_DATA *, int);

void print_assignment_header(int, const char *);

void print_a_line_helper(char, int, int);

void print_a_line(char, int);

int is_only_upper_case(const char *);

void make_name_proper(char *, int);

int get_number_of_lines_in_file(FILE *);

FILE *open_file(int *, const char *);

int get_race(const RACE *, int, const char *, RACE *);

void print_race_data(RACE_DATA, int);

void print_rider_data(RIDER_DATA, int);

int compare_riders(const void *, const void *);

int compare_time(const void *, const void *);

void get_riders_who_has_completed_a_race(const RIDER_DATA *, int, const char *, RIDER_DATA **, int *);

int calculate_seconds(TIME_DATA);

void
get_rider_who_has_completed_both_races(const RIDER_DATA *, int, const char *, const char *, RIDER_DATA **, int *,
                                       TIME_DATA *);

/******************************************/
/* MAIN */
/******************************************/
int main(int argc, char *argv[]) {
    int i, race_data_arr_size, rider_data_arr_size;
    RACE_DATA *race_data_arr;
    RIDER_DATA *rider_data_arr;
    char input[5];

    /* Read file */
    if (!read_race_data_file(FILE_PATH, &race_data_arr, &race_data_arr_size, &rider_data_arr,
                             &rider_data_arr_size)) {
        /* Exit if there are errors */
        exit(0);
    }

    print_a_line('#', DATA_TOTAL_LENGTH);
    printf("#                                                               #\n");
    printf("#                                                               #\n");
    printf("#                                   #  # #                      #\n");
    printf("#                               #####  # #                      #\n");
    printf("#                       ##########   ##  #                      #\n");
    printf("#                     ########    ###  ##                       #\n");
    printf("#                   #######    #####  ###                       #\n");
    printf("#                   #####    #####   ###                        #\n");
    printf("#                   ####    #####   ####                        #\n");
    printf("#                   ####   ######   #####                       #\n");
    printf("#                    ##    ######   ######                      #\n");
    printf("#                     #    ######    #######                    #\n");
    printf("#                          ######     #######                   #\n");
    printf("#                          #######     ####                     #\n");
    printf("#                            ######                             #\n");
    printf("#                                                               #\n");
    printf("#                                                               #\n");
    printf("#                       ###    ###   #   #                      #\n");
    printf("#                      #   #  #   #  #   #                      #\n");
    printf("#                      #####  #####  #   #                      #\n");
    printf("#                      #   #  #   #   ###                       #\n");
    printf("#                                                               #\n");
    printf("#                                                               #\n");
    printf("#                WELCOME TO THE CYCLING MANAGER                 #\n");
    printf("#                                                               #\n");
    printf("#                    Copyright© Jakob Precht                    #\n");
    printf("#                                                               #\n");
    printf("#                                                               #\n");
    print_a_line('#', DATA_TOTAL_LENGTH);
    printf("\n");

    /* Print all statistics if given the command "--print" */
    if (argc > 1 && strcmp(argv[1], "--print") == 0) {
        print_assignment_1(rider_data_arr, rider_data_arr_size);
        printf("\n");
        print_assignment_2(rider_data_arr, rider_data_arr_size);
        printf("\n");
        print_assignment_3(rider_data_arr, rider_data_arr_size);
        printf("\n");
        print_assignment_4(rider_data_arr, rider_data_arr_size);
        printf("\n");
        print_assignment_5(rider_data_arr, rider_data_arr_size);
        printf("\nPress [ENTER] to Continue...");
        getchar();
    } else {
        printf("Type a number or a command\n\n");
        print_a_line('=', DATA_TOTAL_LENGTH);
        printf("1     | %s\n", ASSIGNMENT_1_NAME);
        print_a_line('-', DATA_TOTAL_LENGTH);
        printf("2     | %s\n", ASSIGNMENT_2_NAME);
        print_a_line('-', DATA_TOTAL_LENGTH);
        printf("3     | %s\n", ASSIGNMENT_3_NAME);
        print_a_line('-', DATA_TOTAL_LENGTH);
        printf("4     | %s\n", ASSIGNMENT_4_NAME);
        print_a_line('-', DATA_TOTAL_LENGTH);
        printf("5     | %s\n", ASSIGNMENT_5_NAME);
        print_a_line('-', DATA_TOTAL_LENGTH);
        printf("exit  | Exit the program\n");
        print_a_line('=', DATA_TOTAL_LENGTH);

        while (1) {
            printf("\nCommand: ");
            scanf("%5s", input);

            if (strcmp(input, "exit") == 0) {
                break;
            } else {
                int number = 0;
                sscanf(input, "%d", &number);
                switch (number) {
                    case 1:
                        printf("\n");
                        print_assignment_1(rider_data_arr, rider_data_arr_size);
                        break;
                    case 2:
                        printf("\n");
                        print_assignment_2(rider_data_arr, rider_data_arr_size);
                        break;
                    case 3:
                        printf("\n");
                        print_assignment_3(rider_data_arr, rider_data_arr_size);
                        break;
                    case 4:
                        printf("\n");
                        print_assignment_4(rider_data_arr, rider_data_arr_size);
                        break;
                    case 5:
                        printf("\n");
                        print_assignment_5(rider_data_arr, rider_data_arr_size);
                        break;
                    default:
                        printf("\nCommand not found\n");
                        break;
                }
            }
        }
    }

    /* Free race data pointers in each rider data */
    for (i = 0; i < rider_data_arr_size; i++) {
        free(rider_data_arr[i].race_data_arr);
    }

    /* Free the arrays */
    free(race_data_arr);
    free(rider_data_arr);

    return 1;
}

/*******************************************/
/* READ FILE */
/*******************************************/

/* Read race data file */
int read_race_data_file(const char *file_name,
                        RACE_DATA **out_race_data_arr,
                        int *out_race_data_arr_size,
                        RIDER_DATA **out_rider_data_arr,
                        int *out_rider_data_arr_size) {
    int i, j, res,
            number_of_lines,
            number_of_races = 0,
            score,
            already_in_array;
    char *temp_str;
    RACE *race_arr, temp_race;
    RACE_DATA race_data;
    FILE *file;

    /* Exit program if can't open the file or the file is empty */
    if ((file = open_file(&number_of_lines, file_name)) == NULL) {
        printf("\nCould not read/find file: \"%s\"\n", file_name);
        fclose(file);
        return 0;
    }

    /* Allocate space for the array */
    *out_race_data_arr = malloc(sizeof(RACE_DATA) * number_of_lines);
    race_arr = malloc(sizeof(RACE) * number_of_lines);

    /* Set start size */
    *out_race_data_arr_size = 0;

    /* Get data from file */
    while ((res = fscanf(file,
                         "%27s \" %43[^\"] \" | %d %3s %3s | %3s %8s\r\n",
                         race_data.race_name,
                         race_data.rider_name,
                         &race_data.age,
                         race_data.team,
                         race_data.country,
                         race_data.ranking,
                         race_data.time)) != EOF) {
        /* Check if line was formatted correctly */
        if (res == NUMBER_OF_DATA_PER_LINE) {
            /* Check if the race has already been added to the array */
            already_in_array = 0;
            for (i = 0; i < number_of_races; i++) {
                if (strcmp(race_arr[i].name, race_data.race_name) == 0) {
                    already_in_array = 1;
                    race_arr[i].number_of_participants++;
                    break;
                }
            }

            /* If the race name is not in the array */
            if (!already_in_array) {
                RACE race;
                strcpy(race.name, race_data.race_name);
                race.number_of_participants = 1;
                race_arr[number_of_races++] = race;
            }

            /* Set first and last name to "" */
            race_data.first_name[0] = '\0';
            race_data.last_name[0] = '\0';

            /* Split rider name with " " */
            temp_str = strtok((char *) race_data.rider_name, RIDER_NAME_SPLITTER);
            while (temp_str != NULL) {
                /* The name is part of the last name if it is all upper case */
                if (is_only_upper_case(temp_str)) {
                    /* Make first letter upper case rest lower case */
                    make_name_proper(temp_str, (int) strlen(temp_str));
                    if (race_data.last_name[0] == '\0') {
                        strcpy(race_data.last_name, temp_str);
                    } else {
                        strcat(race_data.last_name, " ");
                        strcat(race_data.last_name, temp_str);
                    }
                } else {
                    if (race_data.first_name[0] == '\0') {
                        strcpy(race_data.first_name, temp_str);
                    } else {
                        strcat(race_data.first_name, " ");
                        strcat(race_data.first_name, temp_str);
                    }
                }
                temp_str = strtok(NULL, RIDER_NAME_SPLITTER);
            }

            /* Formatted rider name */
            strcpy(race_data.rider_name, race_data.first_name);
            strcat(race_data.rider_name, " ");
            strcat(race_data.rider_name, race_data.last_name);

            /* Add race data to array */
            (*out_race_data_arr)[(*out_race_data_arr_size)++] = race_data;
        }
    }

    /* Close file */
    fclose(file);

    /* Set start size */
    *out_rider_data_arr_size = 0;

    /* Allocate space for the array */
    *out_rider_data_arr = malloc(sizeof(RIDER_DATA) * (*out_race_data_arr_size));

    /* Calculate scores */
    for (i = 0; i < *out_race_data_arr_size; i++) {
        score = 0;
        /* Did the rider complete the race */
        if (strcmp((*out_race_data_arr)[i].ranking, OVER_TIME_LIMIT) == 0) {
            /* Get 1 for completing the race */
            score += 1;
        }
            /* Did the rider finish within the time limit */
        else if (strcmp((*out_race_data_arr)[i].ranking, OVER_TIME_LIMIT) != 0 &&
                 strcmp((*out_race_data_arr)[i].ranking, DID_NOT_FINISH) != 0) {
            /* Get 3 points for being inside the time limit */
            score += 3;

            /* If we can get the race from the array */
            if (get_race(race_arr, number_of_races, (*out_race_data_arr)[i].race_name, &temp_race)) {
                int ranking_value;
                /* Successful conversion gives a result of 1 */
                if (sscanf((*out_race_data_arr)[i].ranking, "%d", &ranking_value) == 1) {
                    /* Extra points (M -P)/13 where M is the number of participants in the race and P is the rider's ranking in the race */
                    score += (temp_race.number_of_participants - ranking_value) / 13;

                    switch (ranking_value) {
                        case 1:
                            /* Get 10 points for first place */
                            score += 10;
                            break;
                        case 2:
                            /* Get 5 points for second place */
                            score += 5;
                            break;
                        case 3:
                            /* Get 2 points for third place */
                            score += 2;
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        /* Set score */
        (*out_race_data_arr)[i].score = score;

        /* Is the rider already in the array */
        already_in_array = 0;
        for (j = 0; j < *out_rider_data_arr_size; j++) {
            if (strcmp((*out_rider_data_arr)[j].name, (*out_race_data_arr)[i].rider_name) == 0) {
                already_in_array = 1;
                /* Add to total score */
                (*out_rider_data_arr)[j].total_score += score;
                /* Add one to number of races attended */
                (*out_rider_data_arr)[j].race_data_arr[(*out_rider_data_arr)[j].number_of_races_attended++] = (*out_race_data_arr)[i];
                /* Add one to number of races completed */
                if (strcmp((*out_race_data_arr)[i].ranking, DID_NOT_FINISH) != 0)
                    (*out_rider_data_arr)[j].number_of_races_completed++;
                break;
            }
        }

        /* If the rider is not in the array */
        if (!already_in_array) {
            RIDER_DATA temp_rider_data;
            /* Allocate space for the race data concerning the rider */
            temp_rider_data.race_data_arr = malloc(sizeof(RACE_DATA) * number_of_races);
            /* Set start value for number of races attended */
            temp_rider_data.number_of_races_attended = 0;
            /* Point to the race data in the array */
            temp_rider_data.race_data_arr[temp_rider_data.number_of_races_attended++] = (*out_race_data_arr)[i];
            /* Number of races completed */
            temp_rider_data.number_of_races_completed = 0;
            if (strcmp((*out_race_data_arr)[i].ranking, DID_NOT_FINISH) != 0)
                temp_rider_data.number_of_races_completed = 1;
            /* Copy values */
            strcpy(temp_rider_data.name, (*out_race_data_arr)[i].rider_name);
            strcpy(temp_rider_data.first_name, (*out_race_data_arr)[i].first_name);
            strcpy(temp_rider_data.last_name, (*out_race_data_arr)[i].last_name);
            temp_rider_data.age = (*out_race_data_arr)[i].age;
            strcpy(temp_rider_data.team, (*out_race_data_arr)[i].team);
            strcpy(temp_rider_data.country, (*out_race_data_arr)[i].country);
            temp_rider_data.total_score = score;

            /* Add rider data to the array */
            (*out_rider_data_arr)[(*out_rider_data_arr_size)++] = temp_rider_data;
        }
    }

    free(race_arr);

    return 1;
}

/*******************************************/
/* ASSIGNMENTS */
/*******************************************/

/*
 * Assignment 1
 * Results for italian riders over 30 years old
 */
void print_assignment_1(const RIDER_DATA *rider_data_arr, int rider_data_arr_size) {
    int i;

    /* Print assignment header */
    print_assignment_header(1, ASSIGNMENT_1_NAME);

    for (i = 0; i < rider_data_arr_size; i++) {
        /* The rider is over the given age and from the given country */
        if (strcmp(rider_data_arr[i].country, ASSIGNMENT_1_COUNTRY_CODE) == 0 &&
            rider_data_arr[i].age > ASSIGNMENT_1_AGE) {
            print_rider_data(rider_data_arr[i], 0);
        }
    }
}

/*
 * Assignment 2
 * All danish riders who has completed one or more of the races
 */
void print_assignment_2(const RIDER_DATA *rider_data_arr, int rider_data_arr_size) {
    int i, temp_rider_data_arr_size;
    RIDER_DATA *temp_rider_data_arr;

    /* Print assignment header */
    print_assignment_header(2, ASSIGNMENT_2_NAME);

    /* Get all the danish riders who has completed a race */
    get_riders_who_has_completed_a_race(rider_data_arr,
                                        rider_data_arr_size,
                                        ASSIGNMENT_2_COUNTRY_CODE,
                                        &temp_rider_data_arr,
                                        &temp_rider_data_arr_size);

    /* Print all the riders */
    print_a_line('=', DATA_TOTAL_LENGTH);
    for (i = 0; i < temp_rider_data_arr_size; i++) {
        print_rider_data(temp_rider_data_arr[i], 1);
        if (i != temp_rider_data_arr_size - 1)
            print_a_line('-', DATA_TOTAL_LENGTH);
    }
    print_a_line('=', DATA_TOTAL_LENGTH);

    free(temp_rider_data_arr);
}

/*
 * Assignment 3
 * The 10 riders who has got the most points in total
 */
void print_assignment_3(const RIDER_DATA *rider_data_arr, int rider_data_arr_size) {
    int i;

    /* Print assignment header */
    print_assignment_header(3, ASSIGNMENT_3_NAME);

    /* Sort the riders after their total score and last name */
    qsort((void *) rider_data_arr,
          (size_t) rider_data_arr_size,
          sizeof(rider_data_arr[0]),
          compare_riders);

    /* Print the first 10 riders in the sorted array */
    print_a_line('=', DATA_TOTAL_LENGTH);
    for (i = 0; i < ASSIGNMENT_3_NO_OF_RIDERS; i++) {
        printf("%2d. ", i + 1);
        print_rider_data(rider_data_arr[i], 2);
        if (i != ASSIGNMENT_3_NO_OF_RIDERS - 1)
            print_a_line('-', DATA_TOTAL_LENGTH);
    }
    print_a_line('=', DATA_TOTAL_LENGTH);
}

/*
 * Assignment 4
 * Rider(s) who has completed Paris Roubaix and Amstel Gold Race with the best time in total
 */
void print_assignment_4(const RIDER_DATA *rider_data_arr,
                        int rider_data_arr_size) {
    RIDER_DATA *temp_rider_data_arr;
    TIME_DATA time_data;
    int i, temp_rider_data_arr_size;

    /* Print assignment header */
    print_assignment_header(4, ASSIGNMENT_4_NAME);

    get_rider_who_has_completed_both_races(rider_data_arr,
                                           rider_data_arr_size,
                                           ASSIGNMENT_4_RACE_1,
                                           ASSIGNMENT_4_RACE_2,
                                           &temp_rider_data_arr,
                                           &temp_rider_data_arr_size,
                                           &time_data);

    /* Print the results */
    print_a_line('=', DATA_TOTAL_LENGTH);
    for (i = 0; i < temp_rider_data_arr_size; i++) {
        print_rider_data(temp_rider_data_arr[i], 3);

        if (i != temp_rider_data_arr_size - 1)
            print_a_line('-', DATA_TOTAL_LENGTH);
    }
    print_a_line('=', DATA_TOTAL_LENGTH);
    print_a_line('=', DATA_TOTAL_LENGTH);
    printf("Best time\n");
    print_a_line('-', DATA_TOTAL_LENGTH);
    printf("%d hour%s, %d minute%s and %d second%s\n",
           time_data.tm_hour,
           time_data.tm_hour != 1 ? "s" : "",
           time_data.tm_min,
           time_data.tm_sec != 1 ? "s" : "",
           time_data.tm_sec,
           time_data.tm_sec != 1 ? "s" : "");
    print_a_line('=', DATA_TOTAL_LENGTH);

    free(temp_rider_data_arr);
}

/*
 * Assignment 5
 * Average age among the riders who has placed in the top 10 in one more of the races
 */
void print_assignment_5(const RIDER_DATA *rider_data_arr, int rider_data_arr_size) {
    int i, j, res, ranking, number_of_riders = 0;
    double total_age = 0;

    /* Print assignment header */
    print_assignment_header(5, ASSIGNMENT_5_NAME);

    /* Get the total age of all the riders who has placed in the top */
    for (i = 0; i < rider_data_arr_size; i++) {
        for (j = 0; j < rider_data_arr[i].number_of_races_attended; j++) {
            res = sscanf(rider_data_arr[i].race_data_arr[j].ranking, "%d", &ranking);
            if (res == 1 && ranking <= ASSIGNMENT_5_MAX_RANKING) {
                total_age += (double) rider_data_arr[i].age;
                number_of_riders++;
                break;
            }
        }
    }

    /* Print the age */
    print_a_line('=', DATA_TOTAL_LENGTH);
    printf("Average age\n");
    print_a_line('-', DATA_TOTAL_LENGTH);
    printf("%.2f\n", total_age / (double) number_of_riders);
    print_a_line('=', DATA_TOTAL_LENGTH);
}

/***********************************/
/* FUNCTIONS */
/***********************************/

/* Print assignment header */
void print_assignment_header(int as_number, const char *as_header) {
    print_a_line('#', HEADER_TOTAL_LENGTH);
    printf("ASSIGNMENT %d\n", as_number);
    print_a_line('-', HEADER_TOTAL_LENGTH);
    printf("%s\n", as_header);
    print_a_line('#', HEADER_TOTAL_LENGTH);
    printf("\n");
}

/*
 * Get the rider who has completed the two races with the best time overall
 * Used in assignment 4
 */
void get_rider_who_has_completed_both_races(const RIDER_DATA *rider_data_arr, int rider_data_arr_size,
                                            const char *race_1_name, const char *race_2_name,
                                            RIDER_DATA **out_rider_data_arr, int *out_rider_data_arr_size,
                                            TIME_DATA *out_time_data) {

    int i, j, res, attended_race_1, attended_race_2, rider_time_data_arr_size = 0;
    RIDER_DATA rider_data;
    RACE_DATA race_data;
    TIME_DATA time_data, time_data_total;
    RIDER_TIME_DATA *rider_time_data_arr;

    /* Allocate space for the rider time data array */
    rider_time_data_arr = malloc(sizeof(RIDER_TIME_DATA) * rider_data_arr_size);

    /* Set hour, min and sec to 0 in TIME_DATA */
    memset(out_time_data, 0, sizeof(TIME_DATA));

    for (i = 0; i < rider_data_arr_size; i++) {
        rider_data = rider_data_arr[i];
        if (rider_data.number_of_races_completed > 1) {
            RIDER_TIME_DATA temp_rider_time_data;

            /* Set hour, min and sec to 0 in TIME_DATA */
            memset(&time_data_total, 0, sizeof(TIME_DATA));
            /* Reset flags */
            attended_race_2 = 0;
            attended_race_1 = 0;

            for (j = 0; j < rider_data.number_of_races_attended; j++) {
                race_data = rider_data.race_data_arr[j];
                /* If ranking is not DNF */
                if (strcmp(race_data.ranking, DID_NOT_FINISH) != 0) {
                    /* If race name is equal race name 1 or race name 2 */
                    if (strcmp(race_data.race_name, race_1_name) == 0 ||
                        strcmp(race_data.race_name, race_2_name) == 0) {
                        /* If race name is equal race name 1 */
                        if (strcmp(race_data.race_name, race_1_name) == 0)
                            attended_race_1 = 1;
                            /* If race name is equal race name 2 */
                        else if (strcmp(race_data.race_name, race_2_name) == 0)
                            attended_race_2 = 1;

                        /* Convert race data time string to TIME DATA */
                        res = sscanf(race_data.time, "%d:%d:%d", &time_data.tm_hour, &time_data.tm_min,
                                     &time_data.tm_sec);
                        /* Did we convert successfully and get 3 values */
                        if (res == 3) {
                            time_data_total.tm_hour += time_data.tm_hour;
                            time_data_total.tm_min += time_data.tm_min;
                            time_data_total.tm_sec += time_data.tm_sec;
                        }
                    }
                }
            }

            /* If attended both races and seconds are 0 or less than the one already selected */
            if (attended_race_2 == 1 && attended_race_1 == 1) {
                mktime(&time_data_total);
                temp_rider_time_data.time_data = time_data_total;
                temp_rider_time_data.rider_data = (RIDER_DATA *) &rider_data_arr[i];
                rider_time_data_arr[rider_time_data_arr_size++] = temp_rider_time_data;
            }
        }
    }

    /* Sort the riders after their total time */
    qsort((void *) rider_time_data_arr,
          (size_t) rider_time_data_arr_size,
          sizeof(rider_time_data_arr[0]),
          compare_time);

    /* Find out how many has got the best time */
    for (i = 0; i < rider_time_data_arr_size; i++) {
        /* Calculate seconds for both the time datas */
        if (calculate_seconds(rider_time_data_arr[0].time_data) != calculate_seconds(rider_time_data_arr[i].time_data))
            break;
    }
    /* Set new size of array */
    rider_time_data_arr_size = i;

    /* Start value for size */
    *out_rider_data_arr_size = 0;
    /* Allocate space for the return array */
    *out_rider_data_arr = malloc(sizeof(RIDER_DATA) * rider_time_data_arr_size);
    /* Get the output rider data */
    for (i = 0; i < rider_time_data_arr_size; i++) {
        (*out_rider_data_arr)[(*out_rider_data_arr_size)++] = *(rider_time_data_arr[i].rider_data);
    }
    /* Set the output time data */
    *out_time_data = rider_time_data_arr[0].time_data;
    /* Free the array */
    free(rider_time_data_arr);
}

/*
 * Get danish riders who has completed a race
 * Used in assignment 2
 */
void get_riders_who_has_completed_a_race(const RIDER_DATA *rider_data_arr, int rider_data_arr_size, const char *country_code,
                                         RIDER_DATA **out_rider_data_arr,
                                         int *out_rider_data_arr_size) {    
    int i;

    /* Start value for size */
    *out_rider_data_arr_size = 0;
    /* Allocate space for the return array */
    *out_rider_data_arr = malloc(sizeof(RIDER_DATA) * rider_data_arr_size);

    for (i = 0; i < rider_data_arr_size; i++) {
        /* If the rider is from the given country and has completed a race */
        if (strcmp(rider_data_arr[i].country, country_code) == 0 && rider_data_arr[i].number_of_races_completed > 0) {
            (*out_rider_data_arr)[(*out_rider_data_arr_size)++] = rider_data_arr[i];
        }
    }
}

/* Get RACE struct from race name */
int get_race(const RACE *race_arr, int race_arr_size, const char *race_name, RACE *out_race) {
    int i;
    for (i = 0; i < race_arr_size; i++) {
        if (strcmp(race_arr[i].name, race_name) == 0) {
            *out_race = race_arr[i];
            return 1;
        }
    }
    return 0;
}

/* Print race data */
void print_race_data(RACE_DATA race_data, int format) {
    switch (format) {
        case 0:
            printf("%-45s%7s%4s%9d\n", race_data.race_name, race_data.time, race_data.ranking, race_data.score);
            break;
        default:
            break;
    }
}

/* Print rider data */
void print_rider_data(RIDER_DATA rider_data, int format) {
    int i;
    switch (format) {
        case 0:
            print_a_line('=', DATA_TOTAL_LENGTH);
            printf("%-45s%-4d%-4s%-4s%8d\n", rider_data.name, rider_data.age, rider_data.team, rider_data.country,
                   rider_data.total_score);
            print_a_line('-', DATA_TOTAL_LENGTH);
            for (i = 0; i < rider_data.number_of_races_attended; i++) {
                print_race_data(rider_data.race_data_arr[i], 0);
            }
            print_a_line('=', DATA_TOTAL_LENGTH);
            break;
        case 1:
            printf("%-45s%-4d%-4s%12d\n", rider_data.name, rider_data.age, rider_data.team,
                   rider_data.number_of_races_completed);
            break;
        case 2:
            printf("%-41s%-4d%-4s%-4s%8d\n", rider_data.name, rider_data.age, rider_data.team, rider_data.country,
                   rider_data.total_score);
            break;
        case 3:
            printf("%-45s%-4d%-4s%-4s\n", rider_data.name, rider_data.age, rider_data.team, rider_data.country);
            break;
        default:
            break;
    }
}

/***********************************/
/* COMPARE FUNCTIONS */
/***********************************/

/* Sort rider scores highest total score first if equal sort after lastname */
int compare_riders(const void *elem1, const void *elem2) {
    const RIDER_DATA *first = elem1, *second = elem2;

    /* Highest total score first */
    if (first->total_score < second->total_score)
        return 1;
    else if (first->total_score > second->total_score)
        return -1;
    else {
        /* Sort after last name */
        return strcmp(first->last_name, second->last_name);
    }
}

/* Sort time data with the smallest time first */
int compare_time(const void *elem1, const void *elem2) {
    const RIDER_TIME_DATA *first = elem1, *second = elem2;

    /* Smallest hour first */
    if (first->time_data.tm_hour > second->time_data.tm_hour)
        return 1;
    else if (first->time_data.tm_hour < second->time_data.tm_hour)
        return -1;
    else {
        /* Smallest minute first */
        if (first->time_data.tm_min > second->time_data.tm_min)
            return 1;
        else if (first->time_data.tm_min < second->time_data.tm_min)
            return -1;
        else {
            /* Smallest second first */
            if (first->time_data.tm_sec > second->time_data.tm_sec)
                return 1;
            else if (first->time_data.tm_sec < second->time_data.tm_sec)
                return -1;
            else {
                return 0;
            }
        }
    }
}

/***********************************/
/* HELPER FUNCTIONS */
/***********************************/

/* Calculate total seconds for the time data */
int calculate_seconds(TIME_DATA time_data) {
    return time_data.tm_hour * 60 * 60 +
           time_data.tm_min * 60 +
           time_data.tm_sec;
}

/* Helper for the print a line function */
void print_a_line_helper(char c, int counter, int len) {
    if (counter < len) {
        printf("%c", c);
        print_a_line_helper(c, counter + 1, len);
    } else
        printf("\n");
}

/* Print a line of a given char to the console */
void print_a_line(char c, int len) {
    print_a_line_helper(c, 0, len);
}

/* Check if a string only has upper case letters */
int is_only_upper_case(const char *str) {
    return *str == '\0' ? 1 : (islower(*str) ? 0 : is_only_upper_case(str + 1));
}

/* Make word proper function */
void make_name_proper(char *str, int str_len) {
    if (*str != '\0') {
        *str = (char) (strlen(str) == str_len ? toupper(*str) : tolower(*str));
        make_name_proper(str + 1, str_len);
    }
}

/* Get number of lines in a file */
int get_number_of_lines_in_file(FILE *fp) {
    int ch = 0, lines = 0;

    /* The file is not open */
    if (fp == NULL)
        return 0;

    /* Check how many LF there are in the file */
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n')
            lines++;
    }

    /* Add one more because the last line does not end with a \n */
    lines++;

    /* Reset file pointer to the beginning of the file */
    rewind(fp);

    return lines;
}

/* Open a file and count the lines */
FILE *open_file(int *number_of_lines, const char *file_name) {
    FILE *fp;

    fp = fopen(file_name, "r");
    if (fp == NULL)
        return NULL;

    /* Get number of lines in the file */
    *number_of_lines = get_number_of_lines_in_file(fp);
    return fp;
}
