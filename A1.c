#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<string.h>

//global variables
typedef struct 
{
       float sum;
       float dif;
       char* file_name;
       pthread_t new_thread;       
}thread_info;

//this function is called by thread
//it reads a file and returns sum = min + max and dif = min - max
void* threadFunc(void* param)
{
      //very important !!!
      //derefrencing param, otherwise you have to cast it everytime
     thread_info *t_info = param; 

     float min_thread, max_thread, temp; 
     max_thread = FLT_MIN;
     min_thread = FLT_MAX; 
     char buf[100];
     FILE* file_ptr = fopen(t_info->file_name, "r");
     if(file_ptr == NULL)
     {
                 printf("no such file");
     }
     while(fscanf(file_ptr, "%s", buf) == 1)
     {
                 temp = atof(buf);
                 if(temp > max_thread)
                         max_thread = temp;
                 if(temp < min_thread)
                         min_thread = temp;
                 //printf("%f ", temp);                                    
     }
     //printf("In thread file_no = %s  MIN=%f MAX=%f \n", t_info->file_name, min_thread, max_thread);
     t_info->sum = min_thread + max_thread;
     t_info->dif = min_thread - max_thread;
     fclose(file_ptr);                  
}
 
//access command line arguments with argc and argv in main()
int main(int argc, char* argv[])
{   printf("argc = %d \n", argc);

    float sum, dif, min, max, min_final, max_final;
    max_final = FLT_MIN;
    min_final = FLT_MAX;
    thread_info* threads = (thread_info*) malloc(argc * sizeof(thread_info));

    // I'm starting with the second item in the argv list, 
    //as the first one is always the name of the executable itself.
    for (int i = 1; i< argc; i++)
    {
        threads[i].sum = 0;
        threads[i].dif = 0;
        threads[i].file_name = argv[i];
        pthread_create(&threads[i].new_thread, NULL, threadFunc, &threads[i]);
        //printf("%s \n", argv[i]);        
    }
    for(int i = 1; i < argc; i++)
    {
            pthread_join(threads[i].new_thread, NULL);
    }
    for(int i = 1; i < argc; i++) 
    {
            //recalculating min and max for each file
            sum = threads[i].sum;
            dif = threads[i].dif;
            min = (sum + dif)/2;   
            max = (sum - dif)/2; 
            printf("%s SUM=%f DIF=%f MIN=%f MAX=%f \n", threads[i].file_name, sum, dif, min, max);            
            //calculate min and max for all files
            if(min < min_final)
                   min_final = min;
            if(max > max_final) 
                   max_final = max;
    }
    printf("MINIMUM=%f MAXIMUM=%f \n", min_final, max_final);
    
    free(threads);
    return 0;
}
