/*
    No files need to be included.
*/

/* Problem 1
 * Legal ops: ! ~ & ^ | + << >>
 * You can use IF statement and any LOOPs but you can only use Legal ops for their conditions. Otherwise, you will get 0 point for Prob 1.(Even though you get the score on the elice platform)
 * NOTICE: Only use legal ops. If not, you will get 0 point.
 * 10pts
 */
int countZero(unsigned x){
    // ------------------
    // Write your implementation here.
    int cnt = 0; //count bit 1 in x
    unsigned y = 0x80000000;
    unsigned z;
    int test1=0;
    
    while(x){
        
        z = x & y; // if x's MSB is 0, then z is 0
        x = (x<<1);
        
        if(z){ 
            cnt = cnt+1; // count 1 bit 
        }
        
    }
    // ------------------
    return 32-cnt;
}

/* Problem 2
 * Note that no negative number is a power of 4.
 * Legal ops: ! ~ & ^ | + << >>
 * You can use IF statement and any LOOPs but you can only use Legal ops for their conditions. Otherwise, you will ge 0 point  for Prob 2.(Even though you get the score on the elice platform)
 * NOTICE: Only use legal ops. If not, you will get 0 point.
 * 15pts
 */
int isPower4(int x){
    // ------------------  
    int cnt = 0;
    if(!x){ // when x is 0
        return 0;
    }
    if(!(x&0xfffffffe)){ // when x is 1 
        return 1;
    }
    int p2 = x & 0xaaaaaaab ; 
    if(p2){
        return 0;
    }
    while(x){
        if(x&1){ // when LSB is 1
        cnt = cnt + 1;
        }
        
        x = (x>>2);
    }
    if(!(cnt & 0xfffffffe)){
    return 1;
    }
    // ------------------
    return 0;
}



/* Problem 3
 * NOTICE: Only use unsigned type. Otherwise, you will get 0 point  for Prob 3.
 * 25pts
 */
void mult_int_32_to_64(unsigned x, unsigned y, unsigned *xyh, unsigned *xyl)
{
    unsigned sumh=0;
    unsigned suml=0;
    unsigned dup_suml =0;
    unsigned temp=0;
    unsigned dup_x =0;
    unsigned ylsb = 0;
    unsigned cnt = 1;
    unsigned overdup=0;
    unsigned overtemp=0;
    unsigned intemp =0;
    unsigned x_msb = x>>31;
    unsigned y_msb = y>>31;
    
    
    
    if(!x){ //when x=0
        *xyh = 0;
        *xyl = 0;
        return ;
    }
    
    if(!y){ //when y=0
        *xyh = 0;
        *xyl = 0;
        return ;
    }
    if(x_msb==1 && y_msb == 0 ){
        x = ~x +1;
    }
    if(x_msb==0 && y_msb==1){
        //sumh = 0x8000000;
        y = ~y+1;
    }
    if(x_msb==1 && y_msb==1){
        y = ~y+1;
        x = ~x+1;
    }
    
    
    while(y){
        ylsb = y & 1; // lsb
        if(ylsb){
            if(cnt==1){
                suml = suml + x;
                cnt = cnt+1;
                y = y>>1;
            }
            else{
            dup_suml = suml>>(cnt-1);
            overdup = x>>(33-cnt);
            sumh = sumh + overdup;
            dup_x = x<<(cnt-1);
            dup_x = dup_x>>(cnt-1); // remove first (cnt-1) bits to 0
            temp = dup_suml + dup_x;
            overtemp = temp >>(33-cnt);
            intemp = temp<<(cnt-1);
            intemp = intemp>>(cnt-1);
            sumh = sumh + overtemp;
            suml = suml << (33-cnt);
            suml = suml >> (33-cnt);
            suml = suml + (intemp<<(cnt-1));
            cnt = cnt + 1;
            y = y>>1;
            }
        }
        else{
            cnt = cnt + 1;
            y = y>>1;
        }
    }
    
    
    if(x_msb==1 && y_msb==0 ){
        if(suml==0){
        sumh= ~sumh +1;
        *xyh = sumh;
        *xyl = suml;
        return;
        }
        else{
        suml = ~suml+1;
        sumh = ~sumh;
        *xyh = sumh;
        *xyl = suml;
        return;
        }
    }
    if(x_msb==0 && y_msb==1 ){
        if(suml==0){
        sumh= ~sumh +1;
        *xyh = sumh;
        *xyl = suml;
        return;
        }
        else{
        suml = ~suml+1;
        sumh = ~sumh;
        *xyh = sumh;
        *xyl = suml;
        return;
        }
    }
    *xyh = sumh;
    *xyl = suml;
    
    return ;
    
}


/* Problem 4
 * NOTICE: Only use unsigned type. Otherwise, you will get 0 point  for Prob 4.
 * 25pts
 */
void float_add(unsigned x, unsigned y, unsigned *result)
{
    // ------------------
    unsigned x_sign = x>>31;
    unsigned y_sign = y>>31;
    unsigned x_exp ,y_exp, x_m, y_m, exp_diff, dup_m;
    unsigned ans_sign, ans_exp, ans_m, answer;
    unsigned cnt = 0;
    
    x_exp = x<<1;
    x_exp = x_exp>>24;
    y_exp = y<<1;
    y_exp = y_exp>>24;
    x_m = x<<9;
    x_m = x_m>>9;
    x_m = x_m + 0x00800000;
    y_m = y<<9;
    y_m = y_m>>9;
    y_m = y_m +0x00800000;
    
    
    if( x_sign==0 && y_sign==0){// x ,y>0
        if(x_exp>=y_exp){
            exp_diff = x_exp - y_exp;
            dup_m = y_m>>(exp_diff);
            ans_m = x_m + dup_m;
            ans_sign = 0;
            ans_exp = x_exp;
            if((ans_m>>24) != 0){
                
                while((ans_m>>24) != 0){
                    cnt = cnt + 1;
                    ans_m = ans_m>>1;
                }
                
                ans_exp = ans_exp + cnt;
            } 
            answer = (ans_sign<<31) + (ans_exp<<23) + (ans_m-0x00800000);
            *result = answer;
            return;
        }
        
        if(x_exp<y_exp){
            exp_diff = y_exp - x_exp;
            dup_m = x_m>>(exp_diff);
            ans_m = y_m + dup_m;
            ans_sign = 0;
            ans_exp = y_exp;
            if((ans_m>>24) != 0){
                
                while((ans_m>>24) != 0){
                    cnt = cnt + 1;
                    ans_m = ans_m>>1;
                }
                
                ans_exp = ans_exp + cnt;
            } 
            answer = (ans_sign<<31) + (ans_exp<<23) + (ans_m-0x00800000);
            *result = answer;
            return;
        }
    
    }
    
    if(x_sign==1 && y_sign==0 ){//x<0 , y>0
        if(x-0x80000000== y){
            answer = 0;
            *result = answer;
            return;
        }
        if(x-0x80000000 >y){
            exp_diff = x_exp - y_exp;
            dup_m = y_m>>(exp_diff);
            ans_m = x_m - dup_m;
            ans_sign = 1;
            ans_exp = x_exp;
            if((ans_m>>23)!=1){
               while((ans_m>>23)!=1){
                    cnt = cnt +1;
                    ans_m = ans_m<<1;
               }
                
                ans_exp = ans_exp - cnt;
            }
            answer = (ans_sign<<31) + (ans_exp<<23) + (ans_m-0x00800000);
        
            *result = answer;
            return;
        }
        
        if(x-0x80000000<y){
            exp_diff = y_exp - x_exp;
            dup_m = x_m>>(exp_diff);
            ans_m = y_m - dup_m;
            ans_sign = 0;
            ans_exp = y_exp;
            if((ans_m>>23)!=1){
               while((ans_m>>23)!=1){
                    cnt = cnt +1;
                    ans_m = ans_m<<1;
               }
                
                ans_exp = ans_exp - cnt;
            }
            answer = (ans_sign<<31) + (ans_exp<<23) + (ans_m-0x00800000);
            *result = answer;
            return;
        }
        
    
    }
    
    if(x_sign==0 && y_sign==1 ){//x>0 , y<0
        if(y-0x80000000== x){
            answer = 0;
            *result = answer;
            return;
        }
        if(y-0x80000000 >x){
            exp_diff = y_exp - x_exp;
            dup_m = x_m>>(exp_diff);
            ans_m = y_m - dup_m;
            ans_sign = 1;
            ans_exp = y_exp;
            if((ans_m>>23)!=1){
               while((ans_m>>23)!=1){
                    cnt = cnt +1;
                    ans_m = ans_m<<1;
               }
                
                ans_exp = ans_exp - cnt;
            }
            answer = (ans_sign<<31) + (ans_exp<<23) + (ans_m-0x00800000);
        
            *result = answer;
            return;
        }
        
        if(y-0x80000000<x){
            exp_diff = x_exp - y_exp;
            dup_m = y_m>>(exp_diff);
            ans_m = x_m - dup_m;
            ans_sign = 0;
            ans_exp = x_exp;
            if((ans_m>>23)!=1){
               while((ans_m>>23)!=1){
                    cnt = cnt +1;
                    ans_m = ans_m<<1;
               }
                
                ans_exp = ans_exp - cnt;
            }
            answer = (ans_sign<<31) + (ans_exp<<23) + (ans_m-0x00800000);
            *result = answer;
            return;
        }
        
    
    }
    
    if( x_sign==1 && y_sign==1){// x ,y<0
        x = x - 0x00800000;
        y = y - 0x00800000;
        if(x_exp>=y_exp){
            exp_diff = x_exp - y_exp;
            dup_m = y_m>>(exp_diff);
            ans_m = x_m + dup_m;
            ans_sign = 1;
            ans_exp = x_exp;
            if((ans_m>>24) != 0){
                
                while((ans_m>>24) != 0){
                    cnt = cnt + 1;
                    ans_m = ans_m>>1;
                }
                
                ans_exp = ans_exp + cnt;
            } 
            answer = (ans_sign<<31) + (ans_exp<<23) + (ans_m-0x00800000);
            *result = answer;
            return;
        }
        
        if(x_exp<y_exp){
            exp_diff = y_exp - x_exp;
            dup_m = x_m>>(exp_diff);
            ans_m = y_m + dup_m;
            ans_sign = 1;
            ans_exp = y_exp;
            if((ans_m>>24) != 0){
                
                while((ans_m>>24) != 0){
                    cnt = cnt + 1;
                    ans_m = ans_m>>1;
                }
                
                ans_exp = ans_exp + cnt;
            } 
            answer = (ans_sign<<31) + (ans_exp<<23) + (ans_m-0x00800000);
            *result = answer;
            return;
        }
    
    }
    // ------------------
}


/* Problem 5
 * NOTICE: Only use unsigned type. Otherwise, you will get 0 point  for Prob 5.
 * 25pts
 */
void float_mult(unsigned x, unsigned y, unsigned *result)
{
    unsigned x_sign = x>>31;
    unsigned y_sign = y>>31;
    unsigned x_exp ,y_exp, x_m, y_m, exp_diff, dup_m;
    unsigned ans_sign, ans_exp, ans_m, answer;
    unsigned cnt = 0;
    unsigned mul_cnt =0;
    unsigned xyh, xyl, vxyh, vxyl,xyh2,xyl2;
    unsigned dup_1, lsb, chk_round;
    
    //unsigned mul_m;
    
    x_exp = x<<1;
    x_exp = x_exp>>24;
    y_exp = y<<1;
    y_exp = y_exp>>24;
    x_m = x<<9;
    x_m = x_m>>9;
    x_m = x_m + 0x00800000;
    y_m = y<<9;
    y_m = y_m>>9;
    y_m = y_m +0x00800000;
    
        if(x==0 || y==0){
        answer = 0;
        *result = answer;
        return;
        }
        
        mult_int_32_to_64(x_m,y_m,&xyh,&xyl);
        dup_1 = *(&xyh);
        vxyh = *(&xyh);
        vxyl = *(&xyl);
        while(dup_1!=1){
            mul_cnt = mul_cnt + 1;
            dup_1 = dup_1>>1;
        }
      
        ans_m = (vxyh<<(23-mul_cnt)) + (vxyl>>(9+mul_cnt));
        lsb = ans_m & 1;
        chk_round = (vxyl>>(8+mul_cnt)) & 1;
        
        if(chk_round==1){// rounding
        ans_m = ans_m +1;
        }
       
        ans_sign = x_sign ^ y_sign;
        ans_exp = x_exp + y_exp -127 +mul_cnt -14; // +mul_cnt -14 : because of nomalization

        answer = (ans_sign<<31) + (ans_exp<<23) + (ans_m-0x00800000);
       
        *result = answer;
        return;
        
    
}
