#include "../include/utils.h"
#include "../include/tty.h"

void print_logo()
{
    printk("\t                                          \n");
    printk("\t\t\t\t   _____                     ____    _____ \n");
    printk("\t\t\t\t  / ____|                   / __ \\  / ____|\n");
    printk("\t\t\t\t | |      ___   _ __   ___ | |  | || (___  \n");
    printk("\t\t\t\t | |     / _ \\ | '__| / _ \\| |  | | \\___ \\ \n");
    printk("\t\t\t\t | |____| (_) || |   |  __/| |__| | ____) |\n");
    printk("\t\t\t\t  \\_____|\\___/ |_|    \\___| \\____/ |_____/ \n");
    printk("\t                                          \n");
}

void about(char *version)
{
    printk("\n\tCoreOS - v%s - A simple 32-bit Ring 0 operating system", version);
    printk("\n\tProvided under the GNU General Public License v3.0");
    printk("\n\tCreated by: Saad Ali, Ghazanfar Pasha & Roshaan Idrees");
    printk("\n\tGitHub repository: https://github.com/Teams-Workspace/CoreOS");
    printk("\n\tContributors:");
    printk("\n\t-> https://github.com/0X1Saad");
    printk("\n\t-> https://github.com/itsmesaadali");
    printk("\n");
}
