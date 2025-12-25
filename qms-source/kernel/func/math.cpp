/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "math.h"
#include "string.h"
#include "../memory.h"

int compare(const char* a, const char* b) {
    bool neg_a = (a[0] == '-'), neg_b = (b[0] == '-');
    if (neg_a && !neg_b) return -1;
    if (!neg_a && neg_b) return 1;
    
    const char* v1 = neg_a ? a + 1 : a;
    const char* v2 = neg_b ? b + 1 : b;
    int len1 = strlen(v1), len2 = strlen(v2);
    
    if (len1 > len2) return neg_a ? -1 : 1;
    if (len1 < len2) return neg_a ? 1 : -1;
    
    int cmp = strcmp(v1, v2);
    return neg_a ? -cmp : cmp;
}

char* attach_minus(char* res) {
    if (strcmp(res, "0") == 0) return res;
    char* final_res = new char[strlen(res) + 2];
    final_res[0] = '-';
    strcpy(final_res + 1, res);
    delete[] res;
    return final_res;
}

char* big_add_raw(const char* a, const char* b) {
    int i = strlen(a)-1, j = strlen(b)-1, k = 0, carry = 0;
    int max_l = (i > j ? i : j) + 2;
    char* res = new char[max_l + 1];
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry + (i >= 0 ? a[i--]-'0' : 0) + (j >= 0 ? b[j--]-'0' : 0);
        res[k++] = (sum % 10) + '0';
        carry = sum / 10;
    }
    res[k] = '\0';
    for (int s=0, e=k-1; s<e; s++, e--) { char t=res[s]; res[s]=res[e]; res[e]=t; }
    return res;
}

char* big_sub_raw(const char* a, const char* b) {
    int i = strlen(a)-1, j = strlen(b)-1, k = 0, borrow = 0;
    char* res = new char[i + 2];
    while (i >= 0) {
        int sub = (a[i--]-'0') - (j >= 0 ? b[j--]-'0' : 0) - borrow;
        if (sub < 0) { sub += 10; borrow = 1; } else borrow = 0;
        res[k++] = sub + '0';
    }
    while (k > 1 && res[k-1] == '0') k--;
    res[k] = '\0';
    for (int s=0, e=k-1; s<e; s++, e--) { char t=res[s]; res[s]=res[e]; res[e]=t; }
    return res;
}

char* add(const char* a, const char* b) {
    bool neg_a = (a[0] == '-'), neg_b = (b[0] == '-');
    const char* v1 = neg_a ? a + 1 : a;
    const char* v2 = neg_b ? b + 1 : b;

    if (neg_a == neg_b) {
        char* r = big_add_raw(v1, v2);
        return neg_a ? attach_minus(r) : r;
    }
    if (strcmp(v1, v2) == 0) { char* r = new char[2]; r[0]='0'; r[1]='\0'; return r; }
    if (compare(v1, v2) > 0) {
        char* r = big_sub_raw(v1, v2);
        return neg_a ? attach_minus(r) : r;
    } else {
        char* r = big_sub_raw(v2, v1);
        return neg_b ? attach_minus(r) : r;
    }
}

char* sub(const char* a, const char* b) {
    char* b_neg;
    if (b[0] == '-') { b_neg = new char[strlen(b)]; strcpy(b_neg, b + 1); }
    else { b_neg = new char[strlen(b) + 2]; b_neg[0] = '-'; strcpy(b_neg + 1, b); }
    char* res = add(a, b_neg);
    delete[] b_neg;
    return res;
}

char* mul(const char* a, const char* b) {
    bool res_neg = (a[0] == '-') != (b[0] == '-');
    const char* v1 = (a[0] == '-') ? a + 1 : a;
    const char* v2 = (b[0] == '-') ? b + 1 : b;
    
    int l1 = strlen(v1), l2 = strlen(v2);
    int* tmp = new int[l1 + l2];
    for (int x=0; x<l1+l2; x++) tmp[x] = 0;
    for (int i=l1-1; i>=0; i--)
        for (int j=l2-1; j>=0; j--)
            tmp[i+j+1] += (v1[i]-'0')*(v2[j]-'0');
    for (int i=l1+l2-1; i>0; i--) { tmp[i-1] += tmp[i]/10; tmp[i] %= 10; }
    
    char* rs = new char[l1+l2+1];
    int k=0, s=0;
    while (s < l1+l2-1 && tmp[s] == 0) s++;
    while (s < l1+l2) rs[k++] = tmp[s++] + '0';
    rs[k] = '\0';
    delete[] tmp;
    return res_neg && strcmp(rs, "0") != 0 ? attach_minus(rs) : rs;
}

char* div(const char* a, const char* b) {
    const char* v2 = (b[0] == '-') ? b + 1 : b;
    if (strcmp(v2, "0") == 0) { char* e = new char[6]; strcpy(e, "ERROR"); return e; }
    
    bool res_neg = (a[0] == '-') != (b[0] == '-');
    const char* v1 = (a[0] == '-') ? a + 1 : a;
    
    char* rem = new char[2]{'0','\0'};
    char* res = new char[strlen(v1) + 1];
    int rp = 0;

    for (int i = 0; i < (int)strlen(v1); i++) {
        char* t1 = mul(rem, "10");
        char cur[2] = { v1[i], '\0' };
        char* next_rem = add(t1, cur);
        delete[] rem; delete[] t1; rem = next_rem;

        int count = 0;
        while (compare(rem, v2) >= 0) {
            
            char* sub_res = sub(rem, v2); 
            delete[] rem; 
            rem = sub_res;
            count++;
        }
        res[rp++] = count + '0';
    }
    res[rp] = '\0';
    int st = 0; while (res[st] == '0' && res[st+1] != '\0') st++;
    char* f = new char[strlen(res+st)+1]; strcpy(f, res+st);
    delete[] res; delete[] rem;
    return res_neg && strcmp(f, "0") != 0 ? attach_minus(f) : f;
}
