// File stdlib.h for CC1B
// with rwsenser changes for adding const where needed to match newer function prototypes...
#ifndef _STDLIB_H_
#define _STDLIB_H_
//include other libraries here
#include <ctype.h>
#include <string.h>
#pragma library 1

int abs(int nbr);
int atoi(const char *s);
int atoib(const char *s, int b);
int dtoi(char *decstr, int *nbr);
char *itoa(int n, char *s);
char *itoab(int n, char *s, int b);
char *itod(int nbr, char *str, int sz);
char *itoo(int nbr, char *str, int sz);
char *itou(int nbr, char *str, int sz);
char *itox(int nbr, char *str, int sz);
char *left(char *str);
int otoi(char *octstr, int *nbr);
char *pad(char *dest, char ch, int n);
char *reverse(char *s);
int sign(int nbr);
int utoi(char *decstr, int *nbr);
int xtoi(char *hexstr, int *nbr);

/**
 * abs -- returns absolute value of nbr
 */
int abs(int nbr) {
  if (nbr < 0) return (-nbr);
  return (nbr);
}

/**
 * atoi(s) - convert s to integer.
 */
int atoi(const char *s) {
  int sgn, n;
  while (isspace(*s)) ++s;
  sgn = 1;
  switch (*s) {
    case '-': sgn = -1;
    case '+': ++s;
  }
  n = 0;
  while (isdigit(*s)) {
    n = 10 * n;
    n += (*s);
    n -= '0';
	s++;
  }
  n *= sgn;
  return (n);
}

/**
 * atoib(s,b) - Convert s to "unsigned" integer in base b.
 *              NOTE: This is a non-standard function.
 */
int atoib(const char *s, int b) {
  int n; char digit;
  n = 0;
  while (isspace(*s)) ++s;
  digit = (127 & *s);
  while (digit >= '0') {
    if (digit >= 'a')      digit -= 87;
    else if (digit >= 'A') digit -= 55;
    else                   digit -= '0';
    if (digit >= b) break;
    n *= b;
	n += digit;
    s++;
    digit = (127 & *s);
  }
  return (n);
}

/**
 * dtoi -- convert signed decimal string to integer nbr
 *         returns field length, else -1 on error
 */
int dtoi(char *decstr, int *nbr) {
  int len, s;
  if ((*decstr)=='-') {
    s=1;
	++decstr;
  }
  else s=0;
  len = utoi(decstr, nbr);
  if (len<0) return -1;
  if ((*nbr)<0) return -1;
  if (s) {
    *nbr = -*nbr;
	return ++len;
  }
  else return len;
}

/**
 * itoa(n,s) - Convert n to characters in s 
 */
char *itoa(int n, char *s) {
  int sgn;
  shrBank char temp;
  char *ptr;
  ptr = s;
  sgn = n;
  if (sgn < 0) n = -n;
  do {
    temp = (char)n % 10;
	*ptr = temp;
    *ptr += '0';
	ptr++;
	n = (char)n/10;
  } while (n > 0);
  if (sgn < 0) *ptr++ = '-';
  *ptr = '\0';
  return reverse(s);
}

/**
 * itoab(n,s,b) - Convert "unsigned" n to characters in s using base b.
 *                NOTE: This is a non-standard function.
 */
char *itoab(int n, char *s, int b) {
  char *ptr; char temp;
  int lowbit;
  ptr = s;
  b >>= 1;
  do {
    lowbit = n & 1;
    n = (n >> 1) & 32767;
    temp = (char)n % (char)b;
	*ptr = temp;
    *ptr <<= 1;
    *ptr += lowbit;
    if(*ptr < 10) *ptr += '0'; else *ptr += 55;
    ++ptr;
	n = (char)n / (char)b;
  } while(n != 0);
  *ptr = 0;
  return reverse(s);
}

/**
 * itod -- convert nbr to signed decimal string of width sz
 *         right adjusted, blank filled; returns str
 *
 *        if sz > 0 terminate with null byte
 *        if sz = 0 find end of string
 *        if sz < 0 use last byte for data
 */
char *itod(int nbr, char *str, int sz) {
  char sgn; char temp;
  if (nbr<0) {nbr = -nbr; sgn='-';}
  else sgn=' ';
  if (sz>0) str[--sz]=0;
  else if (sz<0) sz = -sz;
       else while (str[sz]!=0) ++sz;
  while(sz) {
    temp = (char)nbr % 10;
	str[--sz] = temp;
	str[sz] += '0';
	nbr = (char)nbr / 10;
    if (nbr==0) break;
  }
  if (sz) str[--sz]=sgn;
  while (sz>0) str[--sz]=' ';
  return str;
}

/**
 * itoo -- converts nbr to octal string of length sz
 *         right adjusted and blank filled, returns str
 *
 *        if sz > 0 terminate with null byte
 *        if sz = 0 find end of string
 *        if sz < 0 use last byte for data
 */
char *itoo(int nbr, char *str, int sz) {
  int digit;
  if (sz>0) str[--sz]=0;
  else if(sz<0) sz = -sz;
       else while (str[sz]!=0) ++sz;
  while(sz) {
    digit = nbr &7;
    nbr = ((char)nbr>>3);
	nbr &= 8191;
    str[--sz]=digit+48;
    if (nbr==0) break;
  }
  while (sz) str[--sz]=' ';
  return str;
}

/**
 * itou -- convert nbr to unsigned decimal string of width sz
 *         right adjusted, blank filled; returns str
 *
 *        if sz > 0 terminate with null byte
 *        if sz = 0 find end of string
 *        if sz < 0 use last byte for data
 */
char *itou(int nbr, char *str, int sz) {
  int lowbit; char temp;
  if (sz>0) str[--sz]=0;
  else if (sz<0) sz = -sz;
       else while (str[sz]!=0) ++sz;
  while(sz) {
    lowbit=nbr&1;
    nbr=(nbr>>1)&32767;  /* divide by 2 */
    temp = (char)nbr % 5;
	str[--sz] = temp;
	str[sz] <<= 1;
	str[sz] += lowbit;
	str[sz] += '0';
	nbr = (char)nbr / 5;
    if (nbr==0) break;
  }
  while (sz) str[--sz]=' ';
  return str;
}

/**
 * itox -- converts nbr to hex string of length sz
 *         right adjusted and blank filled, returns str
 *
 *        if sz > 0 terminate with null byte
 *        if sz = 0 find end of string
 *        if sz < 0 use last byte for data
 */
char *itox(int nbr, char *str, int sz) {
  int digit, offset;
  if (sz>0) str[--sz]=0;
  else if(sz<0) sz = -sz;
       else while (str[sz]!=0) ++sz;
  while(sz) {
    digit = nbr & 15;
	nbr = (char)nbr>>4;
	nbr &= 4095;
    if (digit<10) offset=48; else offset=55;
    str[--sz] = digit+offset;
    if (nbr==0) break;
  }
  while (sz) str[--sz]=' ';
  return str;
}

/**
 * left -- left adjust and null terminate a string
 */
char *left(char *str) {
  char *str2; char *d = str; char temp;
  str2=str;
  while (*str2==' ') ++str2;
  while (*str2) {
    temp = *str2;
    *str = temp;
    str++;
    str2++;
  }
  *str = 0;
  return d;
}

/**
 * otoi -- convert unsigned octal string to integer nbr
 *          returns field size, else -1 on error
 */
int otoi(char *octstr, int *nbr) {
  int d,t; d=0;
  *nbr=0;
  while ((*octstr>='0') && (*octstr<='7')) {
    t=*nbr;
    t <<= 3;
	t += *octstr;
	t -= '0';
	octstr++;
    if ((t>=0) && ((*nbr)<0)) return -1;
    d++; *nbr=t;
  }
  return d;
}

/**
 * Place n occurrences of ch at dest.
 */
char *pad(char *dest, char ch, int n) {
  while (n) {
    n--;
	*dest++ = ch;
  }
  return dest;
}

/**
 * reverse string in place 
 */
char *reverse(char *s) {
  char *j;
  shrBank char temp;
  /* shrBank */ int c;
  j = s + strlen(s);
  j--;
  while (s < j) {
    c = *s;
	temp = *j;
    *s++ = temp;
	// trial fix
    // *j-- = c;
	temp = c;
	*j-- = temp;
  }
  return s;
}

/**
 * sign -- return -1, 0, +1 depending on the sign of nbr
 */
int sign(int nbr) {
  if (nbr > 0)  return 1;
  if (nbr == 0) return 0;
  return -1;
}

/**
 * utoi -- convert unsigned decimal string to integer nbr
 *          returns field size, else -1 on error
 */
int utoi(char *decstr, int *nbr) {
  int d,t; d=0;
  *nbr=0;
  while (((*decstr)>='0') && ((*decstr)<='9')) {
    t=*nbr;
	t = 10*t;
	t += *decstr;
	t -= '0';
	decstr++;
    if ((t>=0)&&((*nbr)<0)) return -1;
    d++; *nbr=t;
  }
  return d;
}

/**
 * xtoi -- convert hex string to integer nbr
 *         returns field size, else -1 on error
 */
int xtoi(char *hexstr, int *nbr) {
  int d, b;  char *cp; char temp;
  d = *nbr = 0; cp = hexstr;
  while (*cp == '0') ++cp;
  while (1) {
    switch (*cp) {
      case '0': case '1': case '2':
      case '3': case '4': case '5':
      case '6': case '7': case '8':
      case '9':                     b=48; break;
      case 'A': case 'B': case 'C':
      case 'D': case 'E': case 'F': b=55; break;
      case 'a': case 'b': case 'c':
      case 'd': case 'e': case 'f': b=87; break;
      default: return (cp - hexstr);
    }
    if (d < 4) ++d; else return (-1);
    *nbr <<= 4;
	temp = *cp++;
    *nbr += temp;
	*nbr -= b;
  }
}

#pragma library 0
#endif
// End of file stdlib.h
