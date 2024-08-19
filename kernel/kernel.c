#include "../drivers/screen.h"

void main() 
{
    //print project name and our name
    char* osName = "TSOS\nBy - Shon and Tedy\n";

    clear_screen();

    print(osName);
}