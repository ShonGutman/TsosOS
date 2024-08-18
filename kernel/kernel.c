

#include "../drivers/screen.h"

void main() 
{

    char* osName = "TSOS\nBy - Shon and Tedy\nproject";
    char* numbers = "\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n14\n15\n16\n17\n18\n19\n20\n21\n22test me";

    clear_screen();

    print(osName);
    //we will see that TSOS was removed
    print(numbers);
}