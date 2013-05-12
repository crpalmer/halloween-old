// File ctype.h for CC1B
#ifndef _CTYPE_H_
#define _CTYPE_H_
//include other libraries here
#pragma library 1

bit isalnum(char ch);
bit isalpha(char ch);
bit isascii(char ch);
bit iscntrl(char ch);
bit isdigit(char ch);
bit isgraph(char ch);
bit islower(char ch);
bit isprint(char ch);
bit ispunct(char ch);
bit isspace(char ch);
bit isupper(char ch);
bit isxdigit(char ch);
char toascii(char ch);
char tolower(char ch);
char toupper(char ch);

/**
 * Test if character is alphanumeric.
 *
 * ch Character to be tested
 * return True if ch is alphanumeric
 */
bit isalnum(char ch) {
  bit result = isalpha(ch);
  result |= isdigit(ch);
  return result;
}

/**
 * Test if character is alphabetic.
 *
 * ch Character to be tested
 * return True if ch is alphabetic
 */
bit isalpha(char ch) {
  return (ch<='z' && ch>='a') || (ch<='Z' && ch>='A');
}

/**
 * Test if character is an ascii character.
 *
 * return 'true' if c is an ASCII character (0-127)
*/
bit isascii(char ch) {
  return (ch < 128);
}

/**
 * Test if character is a control character (0-31 or 127).
 *
 * ch Character to be tested
 * return True if ch is a control character
 */
bit iscntrl(char ch) {
  return (ch <= 31) || (ch == 127);
}

/**
 * Test if character is a digit.
 *
 * ch Character to be tested
 * return True if ch is a digit
 */
bit isdigit(char ch) {
  return ch<='9' && ch>='0';
}

/**
 * Test if character is a printable character other than a space (33-126).
 *
 * ch Character to be tested
 * return True if ch is a printable character other than a space
 */
bit isgraph(char ch) {
  return ch>=33 && ch<=126;
}

/**
 * Test if character is a lowercase alphabetic.
 *
 * ch Character to be tested
 * return True if ch is a lowercase alphabetic
 */
bit islower(char ch) {
  return ch<='z' && ch>='a';
}

/**
 * Test if character is a printable character (32-126).
 *
 * ch Character to be tested
 * return True if ch is a printable character
 */
bit isprint(char ch) {
  return ch>=32 && ch<=126;
}

/**
 * Test if character is a punctuation character.
 * (all but control and alphanumeric)
 *
 * ch Character to be tested
 * return True if ch is a punctuation character
 */
bit ispunct(char ch) {
  return !isalnum(ch) && !iscntrl(ch);
}

/**
 * Test if character is a space, tab or newline
 *
 * ch Character to be tested
 * return True if ch is a space, tab or newline
 */
bit isspace(char ch) {
  return ch==' ' || ch=='\t' || ch=='\n';
}

/**
 * Test if character is an uppercase alphabetic.
 *
 * ch Character to be tested
 * return True if ch is an uppercase alphabetic
 */
bit isupper(char ch) {
  return ch<='Z' && ch>='A';
}

/**
 * Test if character is a hexadecimal digit.
 *
 * ch Character to be tested
 * return True if ch is a hexadecimal digit
 */
bit isxdigit(char ch) {
  ch.5 = 0;
  return (ch<='9' && ch>='0') || (ch<='F' && ch>='A');
}

/**
 * Convert character to equivalent ascii value.
 *
 * ch Character to be converted
 * return ascii equivalent of ch
 */
char toascii(char ch) {
  return (ch);
}

/**
 * Convert character to lowercase.
 *
 * ch Character to be converted
 * return lowercase of ch if uppercase, else c
 */
char tolower(char ch) {
  if (ch<='Z' && ch>='A') return (ch+32);
  return (ch);
}

/**
 * Convert character to uppercase.
 *
 * ch Character to be converted
 * return uppercase of ch if it is lowercase, else c
 */
char toupper(char ch) {
  if (ch<='z' && ch>='a') return (ch-32);
  return (ch);
}

#pragma library 0
#endif
// End of file ctype.h
