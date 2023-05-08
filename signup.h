#ifndef SIGNUP_H
#define SIGNUP_H

typedef struct
{
  char *day;
  int maxWorker;
} Capacity;

char *readline(void);

void printline(const char *fname);

void writeToFile(const char *fname, char *name, char *days);

char *tokenize(char *days, int id);

int countInFile(const char *fname, const char *word, int id);

int searchCapacity(Capacity *week, char *day);

bool findInStruct(Capacity *week, char *token);

int getId(const char *fname);

void printByDay(const char *fname, const char *listday);

void removeById(const char *fname, int id);

void modifyById(const char *fname, int id, char *name, char *goodDays);

void handler(int signumber);

#endif