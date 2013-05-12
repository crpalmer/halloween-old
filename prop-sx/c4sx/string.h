// File string.h for CC1B
// with rwsenser changes for adding const where needed to match newer function prototypes...
#ifndef _STRING_H_
#define _STRING_H_
//include other libraries here
#pragma library 1

char *strcat(char *s, const char *t);
char *strchr(const char *s, char c);
int strcmp(const char *s, const char *t);
char *strcpy(char *s, const char *t);
int strlen(const char *s);
char *strncat(char *s, const char *t, int n);
int strncmp(const char *s, const char *t, int n);
char *strncpy(char *s, const char *t, int n);
char *strrchr(const char *s, char c);

/**
 * concatenate t to end of s 
 * s must be large enough
 */
char *strcat(char *s, const char *t) {
  char *d = s + strlen(s);
  shrBank char temp;
  while (*t) {
    temp = *t;
	*d = temp;
	d++;
	t++;
  }
  *d = 0;
  return (s);
}

/**
 * return pointer to 1st occurrence of c in s, else 0
 */
char *strchr(const char *s, char c) {
  while (*s) {
    if (*s == c) return (s);
    ++s;
  }
  return (0);
}

/**
 * return <0,   0,  >0 according to s<t, s=t, s>t
 */
int strcmp(const char *s, const char *t) {
  char temp = *t;
  while (*s == temp) {
    if (*s == 0) return (0);
    ++s;
	++t;
	temp = *t;
  }
  // rwsenser fix was (*s - *t)
  return (*s - temp);
}

/**
 * copy t to s 
 */
char *strcpy(char *s, const char *t) {
  char *d = s;
  shrBank char temp;
  while (*t) {
    temp = *t;
	*d = temp;
	d++;
	t++;
  }
  *d = 0;
  return (s);
}

/**
 * return length of string s (fast version)
 */
int strlen(const char *s) {
  char n = 0;
  while (*s) {
    s++;
    n++;
  }
  return (n);
}

/**
 * concatenate n bytes max from t to end of s 
 * s must be large enough
 */
char *strncat(char *s, const char *t, int n) {
  char *d = s + strlen(s);
  char temp;
  while (n) {
    n--;
    if (*t) {
	  temp = *t;
	  *d = temp;
	  d++;
	  t++;
    }
	else break;
  }
  *d = 0;
  return (s);
}

/**
 * strncmp(s,t,n) - Compares two strings for at most n
 *                  characters and returns an integer
 *                  >0, =0, or <0 as s is >t, =t, or <t.
 */
int strncmp(const char *s, const char *t, int n) {
  shrBank char temp;
  while (n) {
    n--;
    temp = *t;
    if (*s == temp) {
      if (*s == 0) return (0);
      ++s;
	  ++t;
    }
  }
  if (n) return (*s - *t);
  return (0);
}

/**
 * copy n characters from t to s (null padding)
 */
char *strncpy(char *s, const char *t, int n) {
  char *d = s;
  char temp;
  while (n > 0) {
    n--;
	if (*t) {
      temp = *t;
	  *d = temp;
	  d++;
	  t++;
	  continue;
	}
    while (n > 0) {
	  n--;
	  *d = 0;
	  d++;
    }
	return (s);
  }
  *d = 0;
  return (s);
}

/**
 * strrchr(s,c) - Search s for rightmost occurrance of c.
 * s      = Pointer to string to be searched.
 * c      = Character to search for.
 * Returns pointer to rightmost c or NULL.
 */
char *strrchr(const char *s, char c) {
  const char *ptr = 0;
  while (*s) {
    if (*s == c) ptr = s;
    ++s;
  }
  return (ptr);
}

#pragma library 0
#endif
// End of file string.h
