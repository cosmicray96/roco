#include <project01/project01.h>
#include <project03/project03.h>

#include <stdio.h>

int main() {
	int result = project01_add(1, 2);
	printf("result: %d\n", result);
	
	int result2 = project03_add(1, 2);
	printf("result2: %d\n", result2);
	

	return 0;
}


