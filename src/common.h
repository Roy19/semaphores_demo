#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>
#include <semaphore.h>

const size_t NUMS = 100;
const size_t SIZE = sizeof(int);
const char *SHM_NAME = "/SHM_DEMO";

const char *producer = "/PRODUCER";
const char *consumer = "/CONSUMER";

#endif