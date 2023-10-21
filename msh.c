//Mary-Rose Tracy
//ID#:1001852752
//Assignment 1: shell
//for some reason it exited out of the program, when I took a long break. thankfully I had a back up
#define _GNU_SOURCE
//okay I'm gonna start here and work from the ground up because the preexisting code is confusing me exponentially
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
//my includes 
#include <stdbool.h>

// my Defines
#define MAX_HIST_COMMAND 16 //because 15+1
#define MAX_PID_COMMAND 25
//trevor Bakker defines
//Note: I was getting confused what was the professors variables bs my variables.
//thus, I'm going to do alot of my variable in CAPITALS. also I want to hyphonate
#define WHITESPACE " \t\n"      
#define MAX_COMMAND_SIZE 255
#define MAX_NUM_ARGUMENTS 11     
char COMOHIST[MAX_HIST_COMMAND][MAX_COMMAND_SIZE];
int PIDTAK = 0; //The pid track
//user Allowed Input
bool UAI = true; 
//user to aloow the input
int CPID[MAX_PID_COMMAND]; //C = command here
int CTRAC = 0;
//let's put all functions on the top and the main in the bottom. 
//looks like we need a function for printing the pids & also the 15 print history
void PrintfPID(int j)
{
  for(int i=0;i<j;i++)
  {
    printf("%d: %d\n",i,CPID[i]);
  }
}
void PrintfHIST(int j)
{
  for(int i=0;i<j;i++)
  {
    printf("%d: %s",i,COMOHIST[i]);
  }
}
//we need to make the computer aware of these functions before it runs into the main
int main()
{
  char *command_string=(char*)malloc(MAX_COMMAND_SIZE); //TB
  while(1)
  {
    if (UAI)
    {
      printf("msh> "); //from step 1
      while (!fgets(command_string, MAX_COMMAND_SIZE, stdin));
    }
    char *TOK[MAX_NUM_ARGUMENTS]; //TOK = token
    //earased the function cus that was getting confusing
    int TC=0; //token of count
    char *argument_ptr=NULL;//tb
    char *working_string=strdup(command_string); //tb
    char *head_ptr=working_string; //tb
    UAI=true; //reintterate that the pointer is true
    while(((argument_ptr=strsep(&working_string, WHITESPACE))!=NULL)&&(TC<MAX_NUM_ARGUMENTS)) //tb
    {
      TOK[TC]=strndup(argument_ptr, MAX_COMMAND_SIZE);
      if(strlen(TOK[TC])==0) //tb but token with tokencount
      {
        TOK[TC]=NULL; //tb
      }
      TC++; //tb
    }
    // Now print the tokenized input as a debug check
    // \TODO Remove this for loop and replace with your shell functionality
    //now here comes the fun part. NOTE: I earased all of this part and redid it because it was erroring
    if(TOK[0]!=NULL&&command_string[0]!='!')
    {
      if (CTRAC<MAX_HIST_COMMAND)
      {
        strcpy(COMOHIST[CTRAC],command_string);
        CTRAC++;
      }
      else
      {
        //first we do our ints for rows & columns
        int INTOR;
        int INTOC;
        //learned this in I think algorithms class
        for (INTOR=0;INTOR<MAX_HIST_COMMAND;INTOR++)
        {
          for(INTOC=0;INTOC<MAX_COMMAND_SIZE;INTOC++)
          {
            COMOHIST[INTOR][INTOC]=COMOHIST[INTOR+1][INTOC];
          }
        }
        strcpy(COMOHIST[MAX_HIST_COMMAND-1],command_string);
      }
    }
    if (TOK[0]==NULL)
    {
      continue;
    }
    //now we need to use specific strings to command the shell
    else if((strcmp("quit",TOK[0])==0)||(strcmp("exit",TOK[0])==0))
    {
      exit(1);
    }

    else if(strcmp("showpids",TOK[0])==0)
    {
      PrintfPID(PIDTAK);
    }
    else if(strcmp("history",TOK[0])==0)
    {
      PrintfHIST(CTRAC);
    }
    else if(strcmp("cd",TOK[0])==0)
    {
      if (chdir(TOK[1])==-1)
      {
        printf("%s: Directory not found.\n",TOK[1]);
      }
    }
    else if(command_string[0]=='!')
    {
      int INDX=atoi(&command_string[1]);
      if(INDX>CTRAC)
      {
        printf("Command not in history.\n");
      }
      strcpy(command_string, COMOHIST[INDX-1]);
      UAI=false;
      continue;
    }
    //not here for bin
    //hmmm I'm gonna commit this and then work on the usr, bin stuff that one is stumping me
    else
    {
      pid_t pid;
      pid=fork();
      if(pid==-1)
      {
        perror("Fork Failed.");
        exit(EXIT_FAILURE);
      }
      else if(pid==0)
      {
        int ESC=execvp(TOK[0],TOK);
        if (ESC==-1)
        {
          printf("%s: Command not found.\n",TOK[0]);
        }
        exit(1);
      }
      else
      {
        if(PIDTAK<MAX_PID_COMMAND)
        {
          CPID[PIDTAK]=pid;
          PIDTAK++;
        }
        else
        {
          int i=0;
          for(i=0;i<PIDTAK-1;i++)
          {
            CPID[i]=CPID[i+1];
          }
          CPID[MAX_PID_COMMAND-1]=pid;
        }
        int STAT;
        waitpid(pid, &STAT, 0);
      }
    }
    free(head_ptr);
  }//free(command_string); was erroring really bad 
  return 0;
}