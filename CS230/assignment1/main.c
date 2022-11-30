/*
    DO NOT MODIFY THIS CODE 
*/
#include <stdio.h>
#include "skeleton.h"

int main()
{
    /* You can test your implementations through followings*/
	int n, x, y, xyh=0, xyl=0;
	float f1, f2, f3, result_add, result_mult;
    printf("Problem number?: ");
    fflush(stdout);
    scanf("%d", &n);
    if( n ==1 ){
        printf("Problem#1 ");
        printf("x ? ");
        fflush(stdout);
        scanf("%u", &x);
        int res = countZero(x);
        printf("result#1 number of zero in %d : %d\n", x, res);
    }
    else if(n == 2){
        printf("Problem#2 ");
        printf("x ? ");
        fflush(stdout);
        scanf("%d", &x);
        int res = isPower4(x);
        printf("result#2 is %d Power of 4? : %d\n", x, res);
    }
    else if(n ==3){
        printf("Problem#3 ");
        printf("x y ? ");
        fflush(stdout);
        scanf("%d %d", &x, &y);
        mult_int_32_to_64((unsigned)x, (unsigned)y,
                  (unsigned*)&xyh, (unsigned*)&xyl);

        /* the follwing outputs should be same */
        printf("result#3: 0x%08x%08x\n", xyh, xyl);
        printf("Answer#3: 0x%016llx\n",(long long int)x*y); 
        }
    else if(n == 4){
        printf("Problem#4 ");
        printf("f1 f2 ? ");
        fflush(stdout);
        scanf("%f %f", &f1, &f2);
        float_add(*(unsigned*)&f1, *(unsigned*)&f2, (unsigned*)&result_add);

        /* the follwing outputs should be same */
        printf("result#3: %8.5f\n", result_add);
        printf("answer#3: %8.5f\n", f3=f1+f2);

    }
    else if(n ==5){
        printf("Problem#5 ");
        printf("f1 f2 ? ");
        fflush(stdout);
        scanf("%f %f", &f1, &f2);
        float_mult(*(unsigned*)&f1, *(unsigned*)&f2, (unsigned*)&result_mult);

        /* the follwing outputs should be same */
        printf("result#5: %8.5f\n", result_mult);
        printf("answer#5: %8.5f\n", f3=f1*f2);
    
    }
    else{
        printf("Wrong number..");
    }

	return 0;
}