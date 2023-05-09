void itoa(int num, char* str) {
    int i = 0;
    if(num == 0) {
        str[i++] = '0';
    }
    while(num != 0) {
        int rem = num % 10;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/10;
    }
    str[i] = '\0'; 

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}
