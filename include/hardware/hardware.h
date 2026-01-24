#ifndef HARDWARE_H
#define HARDWARE_H

typedef struct {
    int id;
    char* name;
    int debug;
} Hardware;

void hardware_init(Hardware* hw, int id, const char* name);
void hardware_log(Hardware* hw, const char* message);

#endif // HARDWARE_H

