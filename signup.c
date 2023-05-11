#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <signal.h>
#include "signup.h"

Capacity week[7] =
    {{"Hetfo", 11},
     {"Kedd", 11},
     {"Szerda", 3},
     {"Csutortok", 3},
     {"Pentek", 11},
     {"Szombat", 3},
     {"Vasarnap", 3}};

struct uzenet
{
  // kezdés longgal!
  long mtype; // ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
  int worker;
};

int main(int argc, char *argv[])
{
  printf("------------------------\n");
  printf("Hi! What would you like to do?\n");
  printf("1. Add a new worker\n");
  printf("2. Modify worker\n");
  printf("3. Delete worker\n");
  printf("4. List all workers\n");
  printf("5. List workers by given day\n");
  printf("6. Transport worker on given day\n");
  printf("Choose a number from 1-6: ");
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
    goodDays = tokenize(days, -1);
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
    printf("---\n");
    printline("list.txt");
    printf("---\n");
    printf("Enter the id of the worker to be modified: ");
    char *in2;
    in2 = readline();
    if (in2 == NULL)
    {
      exit(1);
    }
    char in_b2[sizeof(in2)];
    sscanf(in2, "%s", in_b2);

    int id2;
    id2 = atoi(in_b2);
    if (id2 > 0 && id2 <= getId("list.txt"))
    {
      printf("Enter modified name: ");
      char *name2;
      name2 = readline();
      if (name2 == NULL)
        exit(1);
      if (strcmp(name2, "\n") == 0)
      {
        printf("No name given!\n");
        free(name2);
        return -1;
      }

      for (int i = 0; i < 7; ++i)
      {
        if (strstr(name2, week[i].day) != NULL)
        {
          printf("Invalid name! If your full name contains a day type it in lowercase\n");
          free(name2);
          return -1;
        }
      }

      printf("Enter modified days: ");
      char *days2;
      days2 = readline();
      if (days2 == NULL)
      {
        free(name2);
        return -1;
      }
      if (strcmp(days2, "\n") == 0)
      {
        printf("No days given!\n");
        free(name2);
        free(days2);
        return -1;
      }

      char *goodDays2;
      goodDays2 = tokenize(days2, id2);
      if (goodDays2 == NULL)
      {
        free(name2);
        free(days2);
        return -1;
      }
      if (strcmp(goodDays2, "\n") == 0)
      {
        printf("No good days given!\n");
        free(name2);
        free(days2);
        free(goodDays2);
        return -1;
      }
      modifyById("list.txt", id2, name2, goodDays2);
      free(name2);
      free(days2);
      free(goodDays2);
      printline("list.txt");
    }
    else
    {
      printf("Invalid id\n");
    }
    free(in2);
    break;

  case '3':
    printf("---\n");
    printline("list.txt");
    printf("---\n");
    printf("Enter the id of the worker to be removed: ");
    char *in;
    in = readline();
    if (in == NULL)
    {
      exit(1);
    }
    char in_b[sizeof(in)];
    sscanf(in, "%s", in_b);

    int id;
    id = atoi(in_b);
    if (id > 0 && id <= getId("list.txt"))
    {
      removeById("list.txt", id);
      printline("list.txt");
    }
    else
    {
      printf("Invalid id\n");
    }
    free(in);
    break;

  case '4':
    printf("---\n");
    printf("Listing all workers:\n");
    printline("list.txt");
    break;

  case '5':
    printf("Enter the day: ");
    char *day5;
    day5 = readline();
    if (day5 == NULL)
    {
      return -1;
    }
    char listday5[sizeof(day5)];
    sscanf(day5, "%s", listday5);
    if (findInStruct(week, listday5))
    {
      printf("---\n");
      printf("Listing workers who work on %s\n", listday5);
      printByDay("list.txt", listday5);
      free(day5);
    }
    else
    {
      printf("Invalid day!\n");
      printf("------------------------\n");
      free(day5);
      return -1;
    }
    break;

  case '6':
    printf("Enter the day: ");
    char *day;
    day = readline();
    if (day == NULL)
    {
      return -1;
    }
    char listday[sizeof(day)];
    sscanf(day, "%s", listday);
    if (findInStruct(week, listday))
    {
      printf("---\n");
      printf("Listing workers who work on %s\n", listday);
      printByDay("list.txt", listday);

      int workers;
      workers = countInFile("list.txt", listday, 0);
      printf("Number of workers: %d\n", workers);
      printf("-------\n");

      if (workers > 0)
      {
        int status;

        int pipefd[2];
        char lines1[1024];

        if (pipe(pipefd) == -1)
        {
          perror("Hiba a pipe nyitaskor!");
          exit(EXIT_FAILURE);
        }

        signal(SIGUSR1, handler);

        pid_t bus1 = fork();

        int uzenetsor;
        key_t kulcs;

        kulcs = ftok(argv[0], 1);

        uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
        if (uzenetsor < 0)
        {
          perror("msgget");
          return 1;
        }

        if (bus1 < 0)
        {
          perror("The 1st bus calling was not succesful\n");
          exit(1);
        }

        if (bus1 > 0)
        {
          if (workers > 5)
          {
            signal(SIGUSR2, handler);

            pid_t bus2 = fork();

            char lines2[1024];

            if (bus2 < 0)
            {
              perror("The 2nd bus calling was not succesful\n");
              exit(1);
            }

            int uzenetsor2;
            key_t kulcs2;

            kulcs2 = ftok(argv[0], 2);

            uzenetsor2 = msgget(kulcs2, 0600 | IPC_CREAT);
            if (uzenetsor2 < 0)
            {
              perror("msgget");
              return 1;
            }

            if (bus2 > 0) // parent with more workers than 5 (bus1,bus2)
            {
              close(pipefd[0]); // closing read

              pause();
              pause();

              transport("list.txt", listday, pipefd, 1);

              transport("list.txt", listday, pipefd, 2);
              close(pipefd[1]);

              waitpid(bus1, &status, 0);

              waitpid(bus2, &status, 0);

              fogad(uzenetsor, 1);
              fogad(uzenetsor2, 2);

              status = msgctl(uzenetsor, IPC_RMID, NULL);
              if (status < 0)
                perror("msgctl");

              status = msgctl(uzenetsor2, IPC_RMID, NULL);
              if (status < 0)
                perror("msgctl");
            }
            else // bus2
            {
              kill(getppid(), SIGUSR2);

              close(pipefd[1]);

              int l;

              if (workers > 10)
              {
                for (int i = 5; i < 10; i++)
                {
                  read(pipefd[0], &l, sizeof(l));
                  read(pipefd[0], lines2, l);
                  printf("bus2 transporting: %s\n", lines2);
                }

                kuld(uzenetsor2, 2, 5);
              }
              else
              {
                for (int i = 5; i < workers; i++)
                {
                  read(pipefd[0], &l, sizeof(l));
                  read(pipefd[0], lines2, l);
                  printf("bus2 transporting: %s\n", lines2);
                }

                kuld(uzenetsor2, 2, workers - 5);
              }

              close(pipefd[0]);
            }
          }
          else // parent with less workers than 5 (bus1)
          {
            pause();

            close(pipefd[0]); // closing read

            transport("list.txt", listday, pipefd, 1);
            close(pipefd[1]);

            waitpid(bus1, &status, 0);

            fogad(uzenetsor, 1);

            status = msgctl(uzenetsor, IPC_RMID, NULL);
            if (status < 0)
              perror("msgctl");
          }
        }
        else // bus1
        {
          kill(getppid(), SIGUSR1);

          close(pipefd[1]); // closing write

          int l;
          if (workers > 5)
          {
            for (int i = 0; i < 5; i++)
            {
              read(pipefd[0], &l, sizeof(l));
              read(pipefd[0], lines1, l);
              printf("bus1 transporting: %s\n", lines1);
            }

            kuld(uzenetsor, 1, 5);
          }
          else
          {
            for (int i = 0; i < workers; i++)
            {
              read(pipefd[0], &l, sizeof(l));
              read(pipefd[0], lines1, l);
              printf("bus1 transporting: %s\n", lines1);
            }

            kuld(uzenetsor, 1, workers);
          }
          close(pipefd[0]);
        }
      }
      else
      {
        printf("No workers on the given day!\n");
      }

      free(day);
    }
    else
    {
      printf("Invalid day!\n");
      printf("------------------------\n");
      free(day);
      return -1;
    }
    break;

  default:
    printf("Wrong input!\n");
    printf("------------------------\n");
    return -1;
  }

  // printf("------------------------\n");
  return 0;
}

int kuld(int uzenetsor, long mtype, int worker)
{
  const struct uzenet uz = {mtype, worker};
  int status;

  status = msgsnd(uzenetsor, &uz, sizeof(uz.worker), 0);

  if (status < 0)
    perror("msgsnd");
  return 0;
}

int fogad(int uzenetsor, long mtype)
{
  struct uzenet uz;
  int status;
  // az utolso parameter(0) az uzenet azonositoszama
  // ha az 0, akkor a sor elso uzenetet vesszuk ki
  // ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
  // vesszuk ki a sorbol
  status = msgrcv(uzenetsor, &uz, 1024, mtype, 0);

  if (status < 0)
    perror("msgsnd");
  else
  {
    printf("-------\n");
    if (uz.worker == 1)
    {
      printf("bus%ld transported %d worker!\n", uz.mtype, uz.worker);
    }
    else
    {
      printf("bus%ld transported %d workers!\n", uz.mtype, uz.worker);
    }
  }
  return 0;
}

void transport(const char *fname, const char *listday, int pipefd[2], const int unit)
{
  FILE *f;
  f = fopen(fname, "r");
  if (f == NULL)
  {
    perror("File open error!\n");
    exit(EXIT_FAILURE);
  }
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int count = 0;

  while ((read = getline(&line, &len, f)) != -1)
  {
    if (strstr(line, listday) != NULL)
    {
      if (unit == 1)
      {
        if (count < 5)
        {
          int l = strlen(line) + 1;
          write(pipefd[1], &l, sizeof(l));
          write(pipefd[1], line, l);
        }
      }
      else if (unit == 2)
      {
        if (count >= 5 && count < 10)
        {
          int l = strlen(line) + 1;
          write(pipefd[1], &l, sizeof(l));
          write(pipefd[1], line, l);
        }
      }
      count++;
    }
  }
  fflush(NULL);

  fclose(f);
  if (line)
    free(line);
}

void writeToFile(const char *fname, char *name, char *goodDays)
{
  FILE *f = fopen(fname, "a");
  if (f == NULL)
  {
    perror("File open error!\n");
    exit(EXIT_FAILURE);
  }

  name[strcspn(name, "\n")] = ' ';

  int id;
  id = getId(fname);
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
    perror("File open error!\n");
    exit(EXIT_FAILURE);
  }
  while ((c = getc(f)) != EOF)
    putchar(c);
  fclose(f);
}

void printByDay(const char *fname, const char *listday)
{
  FILE *f;
  f = fopen(fname, "r");
  if (f == NULL)
  {
    perror("File open error!\n");
    exit(EXIT_FAILURE);
  }
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, f)) != -1)
  {
    if (strstr(line, listday) != NULL)
    {
      printf("%s", line);
    }
  }

  fclose(f);
  if (line)
    free(line);
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

char *tokenize(char *days, int id)
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
        if (countInFile("list.txt", token, id) < cap)
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

int countInFile(const char *fname, const char *word, int id)
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

  int lineId;
  while ((read = getline(&line, &len, f)) != -1)
  {
    sscanf(line, "%d", &lineId);
    if (id != lineId)
    {
      if (strstr(line, word) != NULL)
      {
        count++;
      }
    }
  }

  fclose(f);
  if (line)
    free(line);
  return count;
}

int getId(const char *fname)
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
  char buff[1024];
  fseek(f, 0, SEEK_SET); // make sure start from 0

  while (!feof(f))
  {
    memset(buff, 0x00, 1024);    // clean buffer
    fscanf(f, "%[^\n]\n", buff); // read file
  }
  int id;
  sscanf(buff, "%d", &id);

  return id;
}

void removeById(const char *fname, int id)
{
  FILE *f;
  f = fopen(fname, "r");
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  FILE *tmp = fopen("tmp.txt", "w");
  if (f == NULL || tmp == NULL)
  {
    perror("File open error!\n");
    exit(EXIT_FAILURE);
  }

  int lineId;
  while ((read = getline(&line, &len, f)) != -1)
  {
    sscanf(line, "%d", &lineId);
    if (lineId == id)
    {
      printf("OK, %d deleted\n", id);
    }
    else
    {
      fputs(line, tmp);
    }
  }

  fclose(f);
  remove("list.txt");

  rename("tmp.txt", "list.txt");
  fclose(tmp);

  if (line)
    free(line);
}

void modifyById(const char *fname, int id, char *name, char *goodDays)
{
  FILE *f;
  f = fopen(fname, "r");
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  FILE *tmp = fopen("tmp.txt", "w");
  if (f == NULL || tmp == NULL)
  {
    perror("File open error!\n");
    exit(EXIT_FAILURE);
  }

  int lineId;
  while ((read = getline(&line, &len, f)) != -1)
  {
    sscanf(line, "%d", &lineId);
    if (lineId == id)
    {
      name[strcspn(name, "\n")] = ' ';

      fprintf(tmp, "%d ", id);
      fputs(name, tmp);
      fputs("-- ", tmp);
      fputs(goodDays, tmp);
      printf("OK, %d modified\n", id);
    }
    else
    {
      fputs(line, tmp);
    }
  }

  fclose(f);
  remove("list.txt");

  rename("tmp.txt", "list.txt");
  fclose(tmp);

  if (line)
    free(line);
}

void handler(int signumber)
{
  if (signumber == 10)
  {
    printf("Signal from bus1 arrived!\n");
  }
  else if (signumber == 12)
  {
    printf("Signal from bus2 arrived!\n");
  }
}