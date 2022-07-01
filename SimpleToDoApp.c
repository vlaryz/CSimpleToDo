#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define H 5861498
#define V 5861512
#define L 5861502
#define N 5861504
#define D 5861494
#define FILE_NAME "todoList.txt"

void info();
void alertInvalidArgs();
void showAvailableCommands();
void processCommand(int argc, char *argv[]);
void createNewTask(char *task);
void markCompleted(int id);
void getAllTasks();
int getLineCount();

const unsigned long hash(const char *str) { // I hate bit shiftings
    unsigned long hash = 5381;  
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

int main(int argc, char *argv[])
{
    if(argc == 1) // Check if arguments exists
        info();

    processCommand(argc, argv);
}

void info()
{
    printf("Unknown command. Type with -h to get list of commands\n");
}

void alertInvalidArgs()
{
    printf("Invalid arguments. Type with -h to get list of commands and arguments\n");
}

void showAvailableCommands()
{
    printf("\t-h\t\t to get help\n");
    printf("\t-v\t\t to get version of an app\n");
    printf("\t-l\t\t to list tasks\n");
    printf("\t-n \"<task>\"\t to add a todo\n");
    printf("\t-d <id>\t\t to mark completed and delete\n");
}

int getLineCount()
{
    FILE *file;
    file = fopen(FILE_NAME, "r");
    if(file == NULL)
    {
        printf("Currently you don't have any todo's\n");
        return 0;
    }
    
    int lines = 0;
    while(!feof(file))
    {
        char ch = fgetc(file);
        if(ch == '\n')
        {
            lines++;
        }
    }
    fclose(file);
    return lines;
}

void createNewTask(char *task)
{
    if(task[0] == '\"')
    {
        printf("zeba");
        return;
    }
    FILE *file;
    char *output = task;
    //    strcpy(output, task);
    file = fopen(FILE_NAME, "a");

    if(file == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    fprintf(file, "%s", strcat(task, ";0\n"));
    fclose(file);
    printf("Task added to list successefully\n");
}

void getAllTasks()
{
    FILE *file;
    file = fopen(FILE_NAME, "r");

    if(file == NULL)
    {
        printf("Error can't get todo's list or list is empty");
        exit(2);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    size_t counter = 0;

    printf("%10s|\t%s\n","id","Task");
    printf("=========================================\n");

    while ((read = getline(&line, &len, file)) != -1)
    {
        char delim[] = ";";
        char *ptr = strtok(line, delim);
        printf("%10d|\t%10s\n", counter, ptr);
        counter++;
    }
    fclose(file);
}

void markCompleted(int id)
{
    FILE *src, *dst;
    src = fopen(FILE_NAME, "r");
    dst = fopen("SimpleToDoTmp.txt", "w");

    if(src == NULL || dst == NULL)
    {
        printf("Error can't get todo's list or list is empty\n");
        exit(3);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int counter = 0;

    while ((read = getline(&line, &len, src)) != -1)
    {
        if(counter == id)
        {
            counter++;
            continue;
        }
            
        fprintf(dst, "%s", line);
        counter++;
    }
    fclose(src);
    fclose(dst);
    remove(FILE_NAME);

    char fname[] = FILE_NAME;
    rename("SimpleToDoTmp.txt", fname);
}

void processCommand(int argc, char *argv[])
{
    char *command = argv[1];
    switch(hash(command))
    {
    case H:
        showAvailableCommands();
        break;
    case V:
        printf("SimpleToDo version 1.00\n");
        printf("Author Vladislav Ryzovas\n");
        break;
    case L:
        getAllTasks();
        break;
    case N:
        if(argc != 3)
            exit(1);
        createNewTask(argv[argc - 1]);
        break;
    case D:
        if(argc != 3)
            exit(1);
        markCompleted(atoi(argv[argc - 1]));
        break;
    default:
        info();
        break;
    }
}
