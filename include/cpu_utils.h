#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include "../include/tty.h"

typedef struct {
    int pid;
    int burst_time;
    int waiting_time;
    int turnaround_time;
} Process;

// Utilities
void print_float(float val);               // Print float safely
void itoas(int num, char *str);           // Integer to string
void print_center(const char *text, int width); // Center text in width
void print_gantt_chart_vga(Process proc[], int n); // Print dynamic Gantt chart

#endif
