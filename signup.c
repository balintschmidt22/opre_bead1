#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,create
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct
{
  char *day;
  int maxWorker;
} Capacity;

Capacity week[7] =
    {{"Hetfo", 3},
     {"Kedd", 3},
     {"Szerda", 3},
     {"Csutortok", 3},
     {"Pentek", 3},
     {"Szombat", 3},
     {"Vasarnap", 3}};

char *readline(void);

void printline(const char *fname);

void writeToFile(const char *fname, char *name, char *days);

char *tokenize(char *days);

int countInFile(const char *fname, const char *word);

int searchCapacity(Capacity *week, char *day);

bool findInStruct(Capacity *week, char *token);

int getId(const char *fname, const char *name);

int main(void)
{
  printf("------------------------\n");
  printf("Hi! What would you like to do?\n");
  printf("1. Add a new worker\n");
  printf("2. Modify worker\n");
  printf("3. Delete worker\n");
  printf("4. List all workers\n");

  printf("Choose a number from 1-4: ");
  char mode;
  mode = fgetc(stdin);
  if (mode == EOF || mode == '\n')
  {
    printf("Wrong input!\n");
    printf("------------------------\n");
    return -1;
  }

  int c;
  while ((c = getchar()) != '\n' && c != EOF)
  {
  }
  switch (mode)
  {
  case '1':
    printf("Enter your name: ");
    char *name;
    name = readline();
    if (name == NULL)
      return -1;
    if (strcmp(name, "\n") == 0)
    {
      printf("No name given!\n");
      free(name);
      return -1;
    }

    for (int i = 0; i < 7; ++i)
    {
      if (strstr(name, week[i].day) != NULL)
      {
        printf("Invalid name! If your full name contains a day type it in lowercase\n");
        free(name);
        return -1;
      }
    }
    /*for (unsigned int i = 0; i < strlen(name); i++)
    {
      if (isdigit(name[i]) == 1)
      {
        free(name);
        printf("Name can't contain a number!");
        return -1;
      }
    }*/

    printf("Your name is: %s", name);

    printf("Enter the days: ");
    char *days;
    days = readline();
    if (days == NULL)
    {
      free(name);
      return -1;
    }
    if (strcmp(days, "\n") == 0)
    {
      printf("No days given!\n");
      free(name);
      free(days);
      return -1;
    }

    char *goodDays;
    goodDays = tokenize(days);
    if (goodDays == NULL)
    {
      free(name);
      free(days);
      return -1;
    }
    if (strcmp(goodDays, "\n") == 0)
    {
      printf("No good days given!\n");
      free(name);
      free(days);
      free(goodDays);
      return -1;
    }
    writeToFile("list.txt", name, goodDays);
    free(name);
    free(days);
    free(goodDays);
    break;

  case '2':
    printf("case 2\n");
    break;

  case '3':
    printf("case3\n");
    break;

  case '4':
    printf("---\n");
    printf("Listing all workers:\n");
    printline("list.txt");
    break;

  default:
    printf("Wrong input!\n");
    printf("------------------------\n");
    return -1;
  }

  /*printf("Capacity monday: %s,%i\n", capacity[0].day, capacity[0].maxWorker);*/
  printf("------------------------\n");
  return 0;
}

void writeToFile(const char *fname, char *name, char *goodDays)
{
  FILE *f = fopen(fname, "a");
  if (f == NULL)
  {
    perror("File opening error\n");
    free(name);
    free(goodDays);
    exit(1);
  }

  name[strcspn(name, "\n")] = ' ';

  int id;
  id = getId(fname, name);
  id++;

  fprintf(f, "%d ", id);
  fputs(name, f);
  fputs("-- ", f);
  fputs(goodDays, f);
  fclose(f);
}

void printline(const char *fname)
{
  int c;
  FILE *f = fopen(fname, "r");
  if (f == NULL)
  {
    perror("File opening error\n");
    exit(1);
  }
  while ((c = getc(f)) != EOF)
    putchar(c);
  fclose(f);
}

char *readline(void)
{
  char *line = malloc(100), *linep = line;
  size_t lenmax = 100, len = lenmax;
  int c;

  if (line == NULL)
    return NULL;

  for (;;)
  {
    c = fgetc(stdin);
    if (c == EOF)
      break;

    if (--len == 0)
    {
      len = lenmax;
      char *linen = realloc(linep, lenmax *= 2);

      if (linen == NULL)
      {
        free(linep);
        return NULL;
      }
      line = linen + (line - linep);
      linep = linen;
    }

    if ((*line++ = c) == '\n')
      break;
  }
  *line = '\0';
  return linep;
}

char *tokenize(char *days)
{
  days[strcspn(days, "\n")] = ' ';
  char *foo = NULL;
  char *token;
  char *goodDays = malloc(100 * sizeof(char));

  for (token = strtok_r(days, " ", &foo);
       token != NULL;
       token = strtok_r(NULL, " ", &foo))
  {
    if (findInStruct(week, token))
    {
      if (strstr(goodDays, token) == NULL)
      {
        int cap = searchCapacity(week, token);
        if (countInFile("list.txt", token) < cap)
        {
          printf("Okay, %s is accepted\n", token);
          strcat(goodDays, token);
          strcat(goodDays, " ");
        }
        else
        {
          printf("Sorry, %s is full\n", token);
        }
      }
      else
      {
        printf("%s already given!\n", token);
      }
    }
    else
    {
      printf("%s is not a valid day!\n", token);
    }
  }

  strcat(goodDays, "\n");
  return goodDays;
}

bool findInStruct(Capacity *week, char *token)
{
  for (int i = 0; i < 7; ++i)
  {
    if (strcmp(week[i].day, token) == 0)
    {
      return true;
    }
  }
  return false;
}

int searchCapacity(Capacity *week, char *day)
{
  for (int i = 0; i < 7; ++i)
  {
    if (strcmp(week[i].day, day) == 0)
    {
      return week[i].maxWorker;
    }
  }
  exit(EXIT_FAILURE);
}

int countInFile(const char *fname, const char *word)
{
  FILE *f;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int count = 0;

  f = fopen(fname, "r");
  if (f == NULL)
  {
    printf("File open error!\n");
    exit(EXIT_FAILURE);
  }

  while ((read = getline(&line, &len, f)) != -1)
  {
    if (strstr(line, word) != NULL)
    {
      count++;
    }
  }

  fclose(f);
  if (line)
    free(line);
  return count;
}

int getId(const char *fname, const char *name)
{
  FILE *f = fopen(fname, "r");
  if (f == NULL)
  {
    printf("File open error!\n");
    exit(EXIT_FAILURE);
  }

  fseek(f, 0, SEEK_END);
  long size = ftell(f);

  if (0 == size)
  {
    return 0;
  }
  char buff[sizeof(name) + 120];
  fseek(f, 0, SEEK_SET); // make sure start from 0

  while (!feof(f))
  {
    memset(buff, 0x00, sizeof(name) + 120); // clean buffer
    fscanf(f, "%[^\n]\n", buff);            // read file *prefer using fscanf
  }
  int id;
  sscanf(buff, "%d", &id);

  return id;
}

/*char **tokenize(char *days)
{
  char *foo = NULL;
  char *token;
  char **array;
  array = malloc(7 * sizeof(char *));
  for (int i = 0; i < 7; i++)
    array[i] = malloc(10 * sizeof(char));

  for (token = strtok_r(days, " ", &foo);
       token != NULL;
       token = strtok_r(NULL, " ", &foo))
  {
    if (strcmp(token, "Hetfo") == 0 ||
        strcmp(token, "Kedd") == 0 ||
        strcmp(token, "Szerda") == 0 ||
        strcmp(token, "Csutortok") == 0 ||
        strcmp(token, "Pentek") == 0 ||
        strcmp(token, "Szombat") == 0 ||
        strcmp(token, "Vasarnap") == 0)
    {
      printf("token:%s\n", token);
      strcpy(array[dayCount], token);
      dayCount++;
    }
  }

  return array;
}*/