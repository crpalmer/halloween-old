// DATA STORED IN PROGRAM MEMORY

#pragma chip SX28 // select device


// THE 'const' TYPE MODIFIER IS RECOMMENDED FOR ALL ROM DATA
const int16 tx[10] = { 1, 2, 1000, -34, -30000, 2, 100, 23, 0, 0};

const char str[] = "String 1";

const struct  {
  const char *str;
} txtp[] = {
  "A table of pointer to strings",
  "Monday", "Tuesday",
  "MyString",
  "MyString",  // equal strings and substrings are merged to save space
  "String",    // merged with substring
};


// NOTE: const data that are not read are NOT stored!
const char notAccessed[] = "This data is removed because it is not read";

void sub(uns8 i)
{
    // reading the 'const' table
    int16 t = tx[i];

    const char *p = str;
    i = *p;
    p = "Hello world";
    i = *p;

    char si;
    p = txtp[si].str;
    i = *p;
}




// **********************************************************************
// **********************************************************************
// **********************************************************************

// DATA CAN BE STORED DIRECTLY IN PROGRAM MEMORY
// NOTE: This is for special needs, when 'const' tables is not sufficient

#define CDATA_START  0x600
#pragma cdata[CDATA_START]  // start of cdata block

// STORE 12 bits data
#pragma cdata[] = 0xFFF, 0x200, 0x100

// CONSTANT EXPRESSIONS ARE EVALUATED BEFORE STORING
#pragma cdata[] = 0x100, (10<<2) + 346,\
                  10, 456, 1000


#define D8(l,h)  l + h*128
#define D16(x)  x%0x100, x/0x100

#pragma cdata[] = D8(10,20), D16(23453)

// AUTOMATIC ADDRESS CALCULATION
#pragma cdata.ID0 = 0x10, 200+300
#pragma cdata.ID1 = "Hello world\0"
#pragma cdata.ID2 = "Another string\r\n" "merged"

// STORE A TABLE OF 12 BIT ADDRESSES TO ROM CODE
#pragma cdata.ID_TABLE = ID0, ID1, ID2  // store addresses

#pragma cdata.CDATA_END    // end of cdata block


#pragma origin CDATA_END  // program code follow right after


void write(uns16 strID)
{
    while (1)  {
        MODE = strID.high8;
        W = strID % 256;
        iread();
        if (W == '\0')
            return;
        // write W to device
        strID ++;
    }
}


// **********************************************************************
// **********************************************************************
// **********************************************************************

/* NOTE: functions using labels in the directly stored data must be
   defined after the data definitions */

void pointers( void)
{
    write(ID1);
    write(ID2);
}


page0 void main(void)
{
    sub(3);
    pointers();
}


