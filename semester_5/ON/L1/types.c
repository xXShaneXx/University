#include <stdio.h>
#include <float.h>

int main() {
    printf("Float min: %e\n", FLT_MIN);
    printf("Float max: %e\n", FLT_MAX);
    printf("Float epsilon: %e\n", FLT_EPSILON);
    printf("Double min: %e\n", DBL_MIN);
    printf("Double max: %e\n", DBL_MAX);
    printf("Double epsilon: %e\n", DBL_EPSILON);
    printf("Long double min: %Le\n", LDBL_MIN);
    printf("Long double max: %Le\n", LDBL_MAX);
    printf("Long double epsilon: %Le\n", LDBL_EPSILON);
    return 0;
}