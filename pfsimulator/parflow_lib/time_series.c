//
// Created by Benjamin West on 8/17/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time_series.h"
#include <unistd.h>



int CountLinesInFile(char* file_name){
  FILE *fp;
  int count = 1;  // Line counter (result)
  char c;
  fp = fopen(file_name, "r");

  // Check if file exists
  if (fp == NULL)
  {
    printf("Could not open file %s", file_name);
    return 0;
  }
  // Extract characters from file and store in character c
  for (c = getc(fp); c != EOF; c = getc(fp))
    if (c == '\n') // Increment count if this character is newline
      count = count + 1;

  // Close the file
  fclose(fp);
  return count;
}


const char* GetField(char* line_, int num)
{
  //Copy the string to avoid modifying it
  char* line[1024];
  strcpy(line, line_);
  const char* tok;
  for (tok = strtok(line, ",");
       tok && *tok;
       tok = strtok(NULL, ",\n"))
  {
    if (!--num)
      return tok;
  }
  return NULL;
}

const int DeterminePositionInHeader(char* header, char* name){
  int position = 1;
  while(strcmp(GetField(header, position), name)!=0){
    position++;
    if (!GetField(header, position)){
      printf("Couldn't find %s in file header\n", name);
      return -1;
    }
  }
  return position;
}

TimeSeries NewTimeSeries(char* csv_file_name, char* times_name, char* value_name){
  TimeSeries time_series;
  int lines_in_file = CountLinesInFile(csv_file_name);
  int line_number = 0;
  char path[200];

  getcwd(path, 200);
  printf("Current working directory: %s\n", path);
  // we need to know the length of the file to know how much memory to allocate for our timeseries
  time_series.length = lines_in_file - 1;
  double times[time_series.length];
  double values[time_series.length];

  FILE* stream = fopen(csv_file_name,"r");
  if(stream == NULL)
  {
    printf("Error! Could not open file %s\n", csv_file_name);
    exit(1);
  }

  char header[1024];
  fgets(header, 1024, stream);
  int times_position = DeterminePositionInHeader(header, times_name);
  int value_position = DeterminePositionInHeader(header, value_name);
  char line[1024];
  int time_series_position = 0;
  char *tmp;
  char* current_times_string;
  char* current_value_string;
  while (fgets(line, 1024, stream))
  {
    tmp = strdup(line);
    current_times_string = GetField(tmp, times_position);
    current_value_string = GetField(tmp, value_position);
    times[time_series_position] = atof(current_times_string);
    values[time_series_position] = atof(current_value_string);
    time_series_position++;
    free(tmp);
  }

  time_series.times = times;
  time_series.values = values;
  return time_series;
}

//Get the value of the first entry that the requested_times comes after. More concretely, if you have timeses 3 and 5 and ask for
//4 then you get the value at 3. We do a binary search to speed this up.
double GetValue(TimeSeries *time_series, double requested_times){
  double return_value;
  int lower_bound = 0;
  int upper_bound = time_series->length-1;
  int current_position;
// TODO Ben decide if 0 is the right return value for these cases
  if(time_series->length==0){
    printf("Time series has no data in it");
    return 0;
  }
  else if(time_series->times[0] > requested_times){
    printf("Time series start time is after requested requested_times time\n");
    return 0;
  }
  else if(time_series->length==1){
    return time_series->values[0];
  }
  else{
    while(upper_bound!=lower_bound){
      current_position = lower_bound + (upper_bound-lower_bound)/2;
      if(time_series->times[current_position] == requested_times){
        return time_series->values[current_position];
      }
      if(lower_bound + 1 == upper_bound){
        if(time_series->times[upper_bound]<=requested_times){
          return (time_series->values[upper_bound]);
        }
        else{
          return (time_series->values[lower_bound]);
        }
      }
      else if(time_series->times[current_position] > requested_times){
        upper_bound = current_position;
      }
      else{
        lower_bound = current_position;
      }
    }
  }
}

//int main() {
//  TimeSeries *time_series = NewTimeSeries("../test.csv", "times", "value");
//  printf("Value is %f\n", *GetValue(time_series, 2.5));
//  return 0;
//}

