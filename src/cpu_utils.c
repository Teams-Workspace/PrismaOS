#include "../include/tty.h"
#include "../include/string.h"
#include "cpu_utils.h"
#include "../include/utils.h" // for printk()

// ================= Float printing =================
void print_float(float val) {
    int int_part = (int)val;
    float frac = val - (float)int_part;
    if (frac < 0) frac = -frac;
    int decimal = (int)(frac * 100);

    char buf[32], tmp[16];
    strcpy(buf, "");
    itoas(int_part, tmp);
    strcat(buf, tmp);
    strcat(buf, ".");
    if (decimal < 10) strcat(buf, "0");
    itoas(decimal, tmp);
    strcat(buf, tmp);
    printk("%s", buf);
}

// ================= Integer to string =================
void itoas(int num, char *str) {
    int i = 0, is_neg = 0;
    if (num == 0) { str[i++] = '0'; str[i] = '\0'; return; }
    if (num < 0) { is_neg = 1; num = -num; }
    char temp[10]; int j = 0;
    while (num != 0) { temp[j++] = (num % 10) + '0'; num /= 10; }
    if (is_neg) str[i++] = '-';
    while (j > 0) str[i++] = temp[--j];
    str[i] = '\0';
}

// ================= Center text =================
void print_center(const char *text, int width) {
    int len = strlen(text);
    int pad = (width - len) / 2;
    for (int i = 0; i < pad; i++) printk(" ");
    printk("%s", text);
    for (int i = 0; i < width - len - pad; i++) printk(" ");
}

// ================= Gantt chart =================
// void print_gantt_chart(Process proc[], int n) {
//     char buf[16];
//     int time = 0;

//     // Top border
//     printk("  ");
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < proc[i].burst_time; j++) printk("--");
//         printk(" ");
//     }

//     // Bars with process labels
//     printk("\n|");
//     for (int i = 0; i < n; i++) {
//         buf[0] = 'P';
//         buf[1] = '\0';
//         char temp[16];
//         itoas(proc[i].pid, temp);
//         strcat(buf, temp);

//         int pid_len = strlen(buf);
//         int padding = proc[i].burst_time * 2 - pid_len;
//         int left = padding / 2;
//         int right = padding - left;

//         for (int j = 0; j < left; j++) printk(" "); 
//         printk("%s", buf);
//         for (int j = 0; j < right; j++) printk(" ");
//         printk("|");
//     }

//     // Bottom border
//     printk("\n  ");
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < proc[i].burst_time; j++) printk("--");
//         printk(" ");
//     }

//     // Time row
//     printk("\n0");
//     time = 0;
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < proc[i].burst_time; j++) printk("  ");
//         time += proc[i].burst_time;
//         printk("%d", time);
//     }
//     printk("\n\n");
// }


// ================= Gantt Chart with VGA Colors =================
void print_gantt_chart_vga(Process proc[], int n) {
    int time = 0;
    int unit_width = 2; // Width per time unit
    int colors[] = {
        COLOR_RED, COLOR_GREEN, COLOR_LIGHT_BLUE, 
        COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN
    };

    // ---------- Top border ----------
    terminal_set_colors(COLOR_LIGHT_GREY, COLOR_BLACK);
    printk("  ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < proc[i].burst_time * unit_width; j++)
            printk("-");
        printk(" ");
    }

    // ---------- Process row ----------
    printk("\n|");
    for (int i = 0; i < n; i++) {
        int pid_len = 2; // P1, P2 etc.
        int total_width = proc[i].burst_time * unit_width;
        int left_pad = (total_width - pid_len) / 2;
        int right_pad = total_width - pid_len - left_pad;

        // Print left padding
        terminal_set_colors(COLOR_LIGHT_GREY, COLOR_BLACK);
        for (int j = 0; j < left_pad; j++) printk(" ");

        // Set process color and print process label
        terminal_set_colors(COLOR_BLACK, colors[i % 6]); // font black, background colored
        char buf[4];
        itoas(proc[i].pid, buf);
        printk("P%s", buf);

        // Print right padding
        for (int j = 0; j < right_pad; j++) printk(" ");
        terminal_set_colors(COLOR_LIGHT_GREY, COLOR_BLACK);
        printk("|");
    }

    // ---------- Bottom border ----------
    printk("\n  ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < proc[i].burst_time * unit_width; j++)
            printk("-");
        printk(" ");
    }

    // ---------- Time row ----------
    printk("\n0");
    time = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < proc[i].burst_time * unit_width; j++) printk(" ");
        time += proc[i].burst_time;
        printk("%d", time);
    }
    printk("\n\n");

    // Reset colors
    terminal_set_colors(COLOR_LIGHT_GREY, COLOR_BLACK);
}
