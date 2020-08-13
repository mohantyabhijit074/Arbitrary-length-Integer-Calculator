#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intal.h"
#define MAX 1001

//helper functions
static int get_max(int a, int b) {
    return a > b ? a : b;
}
static char* strip_zeros(char *intal, int n) {
    if (n <= 1) {
        intal[0] = '0';
        return intal;
    }
    int p = 0;
    while(intal[p++] == '0');
    p--;
    if (p < n) {
        if (p)
            memmove(intal, intal+p, n - p);
        memset(intal + n - p, '\0', p);
    } else if (p == n) {
        intal[0] = '0';
        intal[1] = '\0';
    }
    return intal;
}

static void create_dividend(char *dividend, char *sub_dividend, int start, int end) {

    int i, index;
    for (i = 0, index = start; index < end; i++, index++)
        sub_dividend[i] = dividend[index];

    sub_dividend[i] = '\0';
}

static char *remove_zero(char *str){
    int i,l=strlen(str);
    for(i=0;i<l && str[i]=='0';i++);
    for(int j=0;j<=l-i;j++)
    {
        str[j]=str[i+j];
    }
    if(i==l)
    {
        str[0]='0';
        str[1]='\0';
    }
    return str;

}
// Returns the comparison value of two intals.
// Returns 0 when both are equal.
// Returns +1 when intal1 is greater, and -1 when intal2 is greater.
int intal_compare(const char* intal1, const char* intal2){
    if(strlen(intal1)<strlen(intal2))
    {
        return -1;
    }
    else if (strlen(intal1)>strlen(intal2))
    {
        return 1;
    }
    else{
        for(int i=0;i<strlen(intal1);i++)
        {
            if(intal1[i]>intal2[i])
                return 1;
            else if (intal1[i]<intal2[i])
                return -1;
        }
    }
    return 0;
}
static char* intal_add_helper(const char* intal1, int n1, const char* intal2, int n2, char *result) {
    int c = 0;
    int ml = get_max(n1, n2);
    int i = n1 - 1;
    int j = n2 - 1;
    int k = ml;
    int temp = 0;
    while(i >= 0 || j >= 0 || k >= 0) {
        temp = 0;
        if (i >= 0)
            temp += (intal1[i]-'0');
        if (j >= 0)
            temp += (intal2[j]-'0');
        temp += c;
        c = temp / 10;
        temp = temp % 10;
        result[k] = (temp+'0');
        --k;
        --j;
        --i;
    }
    strip_zeros(result, ml + 1);
    return result;
}

// Returns the sum of two intals.
char* intal_add(const char* intal1, const char* intal2) {
    int n1 = strlen(intal1);
    int n2 = strlen(intal2);
    int max_len = get_max(n1, n2);

    // Allocate result intal
    char *result = (char*) calloc(sizeof(char), max_len + 2);

    intal_add_helper(intal1, n1, intal2, n2, result);

    return result;
}


// Returns the difference (obviously, nonnegative) of two intals.
char* intal_diff(const char* intal1, const char* intal2){
    const char *i1;
    const char *i2;
     
     if(intal_compare(intal1,intal2)<0)
    {
        i1=intal2;
        i2=intal1;
    }
    else
    {
        i1=intal1;
        i2=intal2;
    }
    int l1=strlen(i1),l2=strlen(i2);
    int i,dif=0,c=0,offset=l1-l2,k=l1-1;
    char *result=(char *)calloc(MAX,sizeof(char));
    result[k]='\0';
    for(i=l2-1;i>=0;i--)
    {
        dif=(i1[i+offset]-'0')-(i2[i]-'0')-c;
        if(dif<0)
        {
            dif+=10;
            c=1;
        }
        else
            c=0;
        
        result[k--]=dif+'0';
    }
    for(i=offset-1;i>=0;i--)
    {
        if(i1[i]=='0' && c==1)
        {
            result[k--]='9';
        }
        else{
            result[k--]=i1[i]-c;
            c=0;
        }
    }
    return remove_zero(result);
}

// Returns the product of two intals.
char* intal_multiply(const char* intal1, const char* intal2){
    int n1 = strlen(intal1);
    int n2 = strlen(intal2);
    int n3 = n1 + n2;
    char *res = (char*) calloc(sizeof(char), (n3 + 2));
    memset(res, '0', n3+1);
    int c = 0;
    int min_k = n3+1;
    int i = n2 - 1;
    int left = 0;
    int j, k;
    while(i >= 0) {
        c = 0;
        j = n1 - 1;
        k = n3 - left;
        left = left + 1;
        while(j >= 0) {
            int temp = 0;
            if (j >= 0)
                temp += (intal1[j]-'0') * (intal2[i]-'0');
            temp += c;
            c = temp / 10;
            int temp2 = temp % 10;
            if (k >= min_k)
                temp2 += (res[k]-'0');
            c += temp2 / 10;
            res[k] = (temp2 % 10)+'0';
            if (k <= min_k) min_k = k;
            k--;
            j--;
        }
        --i;
        res[k] = (c)+'0';
        if (k <= min_k) min_k = k;
    }
    strip_zeros(res, n3+1);
    res[n3+1] = 0;
    return res;
}
// Returns intal1 mod intal2
// The mod value should be in the range [0, intal2 - 1].
// intal2 > 1
// Implement a O(log intal1) time taking algorithm.
// O(intal1 / intal2) time taking algorithm may exceed time limit.
// O(intal1 / intal2) algorithm may repeatedly subtract intal2 from intal1.
// That will take intal1/intal2 iterations.
// You need to design a O(log intal1) time taking algorithm.
// Generate your own testcases at https://www.omnicalculator.com/math/modulo
char* intal_mod(const char* intal1, const char* intal2){
    if(intal_compare(intal1,intal2)<0){
		char *temp = malloc((strlen(intal1) + 1) * sizeof(char));
  		strcpy(temp, intal1);
  		return remove_zero(temp);
	}
	int n1=strlen(intal1);
	int n2=strlen(intal2);
	char *temp = malloc((strlen(intal1) + 1) * sizeof(char));
  	strcpy(temp, intal1);	
        while (intal_compare(temp, intal2) >= 0){ 
		char *new_div = (char *)calloc(MAX, sizeof(char));
        	create_dividend(temp,new_div, 0,n2);
        	if (intal_compare(new_div, intal2) < 0){
			create_dividend(temp,new_div, 0, n2+ 1);
		}
        	int n_div=strlen(new_div);
		while (intal_compare(new_div, intal2) >= 0) {
			char *temp2=new_div;
            		new_div= intal_diff(new_div, intal2);
			free(temp2);
	        }
		if (intal_compare(new_div, "0") == 0){
			create_dividend(temp,new_div,n_div,n1);
			strcpy(temp,new_div);
		}
        	else{
			char *div_rem = (char *)calloc(MAX, sizeof(char));
            		create_dividend(temp, div_rem,n_div,n1);
            		strcat(new_div,div_rem);
            		strcpy(temp,new_div);
            		free(div_rem);
       		}
        	free(new_div);    
        	temp= remove_zero(temp);
    	}
    	return temp;	

}

// Returns intal1 ^ intal2.
// Let 0 ^ n = 0, where n is an intal.
// Implement a O(log n) intal multiplications algorithm.
// 2^3000 has less than 1000 decimal digits. 3000 intal multiplications may exceed time limit.
char* intal_pow(const char* intal1, unsigned int n){
    char *result=(char *)calloc(MAX,sizeof(char));
	if(intal1[0]=='0' && intal1[1]=='\0'){
		result[0]='0';
		result[1]='\0';
		return result;
	}
	if(n==0){
		result[0]='1';
		result[1]='\0';
		return result;
	}
	result[0]='1';
	result[1]='\0';
	char *temp1=(char *)calloc(MAX,sizeof(char));
	strcpy(temp1,intal1);
	while(n!=0){
		if(n&1){
			char *temp2=result;
			result=intal_multiply(result,temp1);
			free(temp2);
		}
		if(n>1){
	       		char *temp3=temp1;
			temp1=intal_multiply(temp1,temp1);
			free(temp3);
		}
		n=n/2;
	}
	free(temp1);
	return result;
}


// Returns Greatest Common Devisor of intal1 and intal2.
// Let GCD be "0" if both intal1 and intal2 are "0" even though it is undefined, mathematically.
// Use Euclid's theorem to not exceed the time limit.
char* intal_gcd(const char* intal1, const char* intal2){
    char *temp1 = (char*) calloc(sizeof(char), MAX);
    temp1[0] = '0'; // set temp1 to "0"
    temp1[1] = '\0';
    if (strcmp(intal1, "0") == 0 && strcmp(intal2, "0") == 0) return temp1; // if both are zero, return zero
    strcpy(temp1, intal1); // set temp1 to intal1

    char *temp2 = (char*) calloc(sizeof(char), MAX);
    strcpy(temp2, intal2); // set temp2 to intal2

    int cmp = 0;
    while ((cmp = intal_compare(temp2, "0")) != 0) {
        char *temp = intal_mod(temp1, temp2); // temp = temp1 % temp2
        free(temp1);
        temp1 = temp2;
        temp2 = temp;
    }
    free(temp2);
    return temp1;

}

// Returns nth fibonacci number.
// intal_fibonacci(0) = intal "0".
// intal_fibonacci(1) = intal "1".
char* intal_fibonacci(unsigned int n){
    char *a=(char *)calloc(MAX,sizeof(char));
	if(n==0){
		a[0]='0';
		a[1]='\0';
		return a;
	}
	if(n==1){
		a[0]='1';
		a[1]='\0';
		return a;
	}
	a[0]='0';
	a[1]='\0';
	char *b=(char *)calloc(MAX,sizeof(char));
	b[0]='1';
	b[1]='\0';
	char *c;
    	for(int i=1;i<n;i++){
		c=intal_add(a,b);
        	strcpy(a,b);
        	strcpy(b,c);
        	free(c);
    	}
    	free(a);
    	return b;

}

// Returns the factorial of n.
char* intal_factorial(unsigned int n)
{
    char *result = (char *)calloc(MAX,sizeof(char));
    strcpy(result,"1");
    char *fact=(char *)calloc(MAX,sizeof(char));
    strcpy(fact,"1");
    for(int i=1;i<=n;i++)
    {
        char *a=result;
        char *b=fact;
        result = intal_multiply(result,fact);
        fact=intal_add(fact,"1");

        free(a);
        free(b);
    }
    free(fact);
    return result;
}

// Returns the Binomial Coefficient C(n,k).
// 0 <= k <= n
// C(n,k) < 10^1000 because the returning value is expected to be less than 10^1000.
// Use the Pascal's identity C(n,k) = C(n-1,k) + C(n-1,k-1)
// Make sure the intermediate intal values do not cross C(n,k).
// Use Dynamic Programming. Use extra space of not more than O(k) number of intals. Do not allocate the whole O(nk) DP table.
// Don't let C(1000,900) take more time than C(1000,500). Time limit may exceed otherwise.
char* intal_bincoeff(unsigned int n, unsigned int k){
    if (k>n)
    {
        char *pascal=(char *)calloc(2,sizeof(char));
        strcpy(pascal,"0");
        return pascal;
    }
    if(k>n/2)
    {
        k=n-k;
    }
    char *pascal[k + 1];
    for (int i = 0; i < k + 1; i++) {
        pascal[i] = (char *) calloc(MAX, sizeof(char));
        pascal[i][0]='0';
	    pascal[i][1]='\0';
    }
    pascal[0][0]='1';
    pascal[0][1]='\0';
    int ind;
    for (int i = 1; i <= n; i++) {
	if(i<k){
		ind=i;
	}
	else{
		ind=k;
	}
        for (int j = ind; j > 0; j--) {
            char *temp = pascal[j];
            pascal[j] = intal_add(pascal[j], pascal[j - 1]);
            free(temp);
        }
    }
    for (int i = 0; i < k; i++){
	free(pascal[i]);
    }
    return pascal[k];
}

// Returns the offset of the largest intal in the array.
// Return the smallest offset if there are multiple occurrences.
// 1 <= n <= 1000
int intal_max(char **arr, int n){
    char *maximum=(char *)calloc(MAX,sizeof(char));
    strcpy(maximum,arr[0]);
    int max_index=0;
    for(int i=1;i<n;i++)
    {
        if(intal_compare(maximum,arr[i])<0)
        {
            strcpy(maximum,arr[i]);
            max_index=i;
        }
    }

    free(maximum);
    return max_index;
}

// Returns the offset of the smallest intal in the array.
// Return the smallest offset if there are multiple occurrences.
// 1 <= n <= 1000
int intal_min(char **arr, int n){
    char *min = arr[0];
    int min_index = 0;

    // Iterating over the remaining elements
    for (int i = 1; i < n; i++) {

        // Comparing the elements and setting the max value
        if (intal_compare(min, arr[i]) > 0) {
            min = arr[i];
            min_index = i;
        }
    }
    return min_index;
}

// Returns the offset of the first occurrence of the key intal in the array.
// Returns -1 if the key is not found.
// 1 <= n <= 1000
int intal_search(char **arr, int n, const char* key){
    for (int i = 0; i < n; i++)
    {
        if (intal_compare(arr[i], key) == 0)
            return i;
    }
    return -1;    
}

// Returns the offset of the first occurrence of the key intal in the SORTED array.
// Returns -1 if the key is not found.
// The array is sorted in nondecreasing order.
// 1 <= n <= 1000
// The implementation should be a O(log n) algorithm.
int intal_binsearch(char **arr, int n, const char* key){
    int i=0,j=n-1,m;
    int mid=-1;
    while(i<=j)
    {
        m=i+(j-i)/2;//to handle overflow
        if(intal_compare(arr[m],key)==0)
        {
            mid = m;
            j=m-1;
        }
        else if(intal_compare(arr[m],key)==1)
        {
            j=m-1;
        }
        else
        {
            i=m+1;
        }
        
    }
    if(mid!=-1)
        return mid;
    else
    {
        return -1;
    }
    
}
// Sorts the array of n intals.
// 1 <= n <= 1000
// The implementation should be a O(n log n) algorithm.
static void merge(char **arr,int left,int middle,int right){
    int i,j,k;
    int n1=middle-left+1;
    int n2=right-middle;
    char **arr_left=(char **)calloc(n1,sizeof(char *));
    char **arr_right=(char **)calloc(n2,sizeof(char *));
    for(i=0;i<n1;i++){
        arr_left[i]=arr[left+i];
    }
    for(j=0;j<n2;j++){
        arr_right[j]=arr[middle+1+j];
    }
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = left; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
    { 
        if (intal_compare(arr_left[i],arr_right[j])<0) 
        { 
            arr[k] = arr_left[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = arr_right[j]; 
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) 
    { 
        arr[k] = arr_left[i]; 
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) 
    { 
        arr[k] = arr_right[j]; 
        j++; 
        k++; 
    } 
	free(arr_left);
	free(arr_right);
}
static void mergesort(char **arr,int left,int right)
{   int middle;
    if(left<right)
    {
        middle=left+(right-left)/2;
        mergesort(arr,left,middle);
        mergesort(arr,middle+1,right);
	    merge(arr,left,middle,right);
    }
} 
void intal_sort(char **arr, int n) {
    mergesort(arr,0,n-1);
}
// Coin-Row Problem - Dynamic Programming Solution
// There is a row of n coins whose values are some positive integers C[0..n-1].
// The goal is to pick up the maximum amount of money subject to the constraint that
// no two coins adjacent in the initial row can be picked up.
// 1 <= n <= 1000
// The implementation should be O(n) time and O(1) extra space even though the DP table may be of O(n) size.
// Eg: Coins = [10, 2, 4, 6, 3, 9, 5] returns 25
char* coin_row_problem(char **arr, int n){
    char *t1 = (char*) calloc(MAX, sizeof(char));
    t1[0] = '0';
    char *t2 = (char*) calloc(MAX, sizeof(char));
    strcpy(t2, arr[0]);
    char *t3 = (char*) calloc(MAX, sizeof(char));
    char *p = t1, *c = t2, *nt = t3;
    for(int i = 1; i < n; ++i) {
        intal_add_helper(arr[i], strlen(arr[i]), p, strlen(p), nt);
        strcpy(p, c);
        if(intal_compare(nt, c) == 1) {
            strcpy(c, nt);
        }
    }
    if (t1 != c) free(t1);
    if (t2 != c) free(t2);
    if (t3 != c) free(t3);
    return c;
}
