#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

typedef uint32_t NewFloat;

NewFloat bits_string_to_nfloat(char* s){
     NewFloat f = 0;
     for(int i = 0; i < 32; i++){
          if(s[i] == '1') {
            f |= 1 << (31-i);
        }
     }
     return f;
}

void nfloat_debug(NewFloat f){
    unsigned int max = 1<<(31);
    for (int i = 0; i < 32; i++){
        if (i == 1 || i == 6) {
            printf(" ");
        }
        if (f&max) printf("1");
        else printf("0");
        
        f = f << 1;
        if (i == 31) {
            printf("\n");
        }
    }
}

int whole_binary(int whole, char* binary) {
    int i = 0;
    for (; whole > 0; i++){
        if (whole % 2 == 1) {
            binary[i] = '1';
        } else {
            binary[i] = '0';
        }
        whole = whole / 2;
    }
    return (i-1);
}

void frac_binary(float frac, char* binary) {
    int i = 0;
    float sub = 0.5;
    for (; frac > 0; i++){
        if (frac >= sub){
            binary[i] = '1';
            frac = frac - sub;
        } else {
            binary[i] = '0';
        }
        sub = sub / 2;     
    }
}

NewFloat float_to_nfloat(float f) {
    char * whole_bin = (char*)malloc(100), *frac_bin = (char*)malloc(100), *exp_bin = (char*)malloc(100);
    char s_float[33];
    if (f >= 0) {
        s_float[0] = '0';
    } else {
        s_float[0] = '1';
        f = f*(-1);
    }
    int whole = (int)f, bias, exp;
    float frac = f-whole;
    bias = whole_binary(whole, whole_bin);
    frac_binary(frac, frac_bin);
    exp = bias + 15;
    whole_binary(exp, exp_bin);
    for (int i = 1; i < 32; i++) {
        if (i < 6) {
            s_float[i] = exp_bin[5-i];
        } else if (i < bias + 6) {
            s_float[i] = whole_bin[bias+5-i];
        } else {
            s_float[i] = frac_bin[i-bias-6];
        }
    }

    NewFloat nf = bits_string_to_nfloat(s_float);
    free(whole_bin);
    free(frac_bin);
    free(exp_bin);
    return nf;
}

float exponify(int base, int exp) {
    float num = 1;
    if (exp > 0) {
        for (int i = 0; i < exp; i++) {
            num = num * base;
        }
    } else {
        exp = exp * (-1);
        for (int i = 0; i < exp; i++) {
            num = num / base;
        }
    }
    return num;
}

void nfloat_print(NewFloat f) {
    unsigned int max = 1<<(31);
    int exp = -15, f_whole = 0, f_frac = 0;
    //print negative sign if necessary
    if (f&max) {
        printf("-");
    }
    f = f << 1;
    //find exponent
    for (int i = 0; i < 5; i++){
        if (f&max) {
            exp += exponify(2,4-i);
        }
        f = f << 1;
    }
    //get whole number
    if (exp >= 0) {
        f_whole += exponify(2,exp);
        exp -= 1;
    } else if (exp >= -14){
        f_frac += exponify(2,exp) * 10000000;
        exp -= 1;
    }
    while (exp >= 0){
        if (f&max) {
            f_whole += exponify(2,exp);
        }
        exp -= 1;
        f = f << 1;
    }
    //get fraction part
    for (int i = 0; i < 26; i++) {
        if (f&max) {
            f_frac += exponify(2,exp-i) * 10000000;
        }
        f = f << 1;
    }
    printf("%d.%.7d\n", f_whole, f_frac);
}

NewFloat nfloat_double(NewFloat f) {
    unsigned int max = 1<<(31);
    int exp = 1;
    char sign, *exp_bin = (char*)malloc(100);
    if (f&max) {
        sign = '1';
    }
    else {
        sign = '0';
    }
    f = f << 1;
    for (int i = 0; i < 5; i++){
        exp_bin[i] = '0';
        if (f&max) {
            exp += exponify(2,4-i);
        }
        f = f << 1;
    }
    whole_binary(exp, exp_bin);
    for(int i = 0; i < 5; i++){
        f = f >> 1;
          if(exp_bin[4-i] == '1') {
            f |= 1 << (31);
        }
     }
    f = f >> 1;
    if (sign == '1') {
        f |= 1 << (31);
    }
    free(exp_bin);
    return f;
}

NewFloat nfloat_add(NewFloat a, NewFloat b);


int main(void) {
    int part_num;
    char s1[40];
    float f1, f2;
    NewFloat n1;

    // This will scan the input for you! Uncomment pieces once you implement
    // the functions for that part
    while (1) {
        int res = scanf(" p%d", &part_num);

        if (res != 1) {
            break;
        } else if (part_num == 1) {
            scanf("%s", s1);
            n1 = bits_string_to_nfloat(s1);
            nfloat_debug(n1);
        } else if (part_num == 2) {
            scanf(" %f", &f1);
            n1 = float_to_nfloat(f1);
            nfloat_debug(n1);
        } else if (part_num == 3) {
            scanf("%s", s1);
            n1 = bits_string_to_nfloat(s1);
            nfloat_print(n1);
        } else {
            scanf("%s ", s1);

            if (s1[0] == 'd') {
                scanf("%f", &f1);
                n1 = float_to_nfloat(f1);
                n1 = nfloat_double(n1);
            } else {
                scanf("%f %f", &f1, &f2);
                //n1 = float_to_nfloat(f1);
                //n2 = float_to_nfloat(f2);
                //n1 = nfloat_add(n1, n2);
            }

            nfloat_print(n1);
        }
    }
}
