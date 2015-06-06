#ifndef    _COMMON_H_
#define    _COMMON_H_

/***** internal functions (sub routines) *****/

// 1 digit decimal charactor -> number
static unsigned char DecToDigit(char c)
{
    if( c>='0' && c<= '9'){
        return (unsigned char)(c - '0');

    }else{
        return 255;
    }
}

// number -> 1 digit decimal charactor
static char DigitToDec(unsigned char val)
{
    if( val <= 9 ){
        return (char)('0' + val);

    }else{
        return 'Z';
    }
}

// 1 digit hex charactor -> number
static unsigned char HexToDigit(char c)
{
    if( c>='0' && c<= '9'){
        return (unsigned char)(c - '0');

    }else if( (c>='A') && (c<= 'F') ){
        return (unsigned char)(10 + c - 'A');

    }else if( (c>='a') && (c<= 'f') ){
        return (unsigned char)(10 + c - 'a');

    }else{
        return 255;
    }
}

// number -> 1 digit hex charactor
static char DigitToHex(unsigned char val)
{
    if( val <= 9 ){
        return (char)('0' + val);

    }else if( val <= 15){
        return (char)('A' + (val-10));

    }else{
        return 'Z';
    }
}

/***** API functions *****/

// decimal string -> integer
template<class T> int DecToInt(char *str, T *val, int digit)
{
    T acc = 0;
    int result = 0;
    unsigned char  d;
    int i;
    
    for(i=0;i<digit;i++){
        d = DecToDigit(str[i]);
        if(d > 9){
            result = -1;    // error
        }
        acc *= 10;
        acc += (T)d;
    }
    
    if(result == 0){
        *val = acc;
    }
    return result;
}

// integer -> decimal string
template<class T> void IntToDec(char *str, T val, int digit)
{
    int i;
    
    for(i=digit;i>0;i--){
        str[i-1] = DigitToDec((unsigned char)(val % 10));
        val /= 10;
    }
}

// hex string -> integer
template<class T> int HexToInt(char *str, T *val, int digit)
{
    T acc = 0;
    int result = 0;
    unsigned char  d;
    int i;
    
    for(i=0;i<digit;i++){
        d = HexToDigit(str[i]);
        if(d > 15){
            result = -1;    // error
        }
        acc <<= 4;
        acc += (T)d;
    }
    
    if(result == 0){
        *val = acc;
    }
    return result;
}

// integer -> hex string
template<class T> void IntToHex(char *str, T val, int digit)
{
    int i;
    
    for(i=digit;i>0;i--){
        str[i-1] = DigitToHex((unsigned char)(val & 0x000F));
        val >>= 4;
    }
}

#endif

