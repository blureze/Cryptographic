#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#include "md5.h"
#include "data.h"

#define max_length 8
#define min_length 3
#define guess_times 4000000000

using namespace std;

vector<data*> dic;
int dic_size;
long long int guess = 0;
int hit_num = 0;

void md5Algo(const uint8_t*, size_t, uint8_t*);

// Constants are the integer part of the sines of integers (in radians) * 2^32.
const uint32_t k[64] =
{
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

// r specifies the per-round shift amounts
const uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                      5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
                     };

// leftrotate function definition

md5::md5()
{
    //ctor
    char word[15];
    int len;

    buildDic();     // build a dictionary that the word_length < 12
    //printf("size = %d\n",dic_size);
    for(int i = 0; i < dic_size; i++)
    {
        if(guess > guess_times) break;
        strcpy(word,(dic[i])->word);
        len = strlen(word);
        printf("word = %s\n",word);
        toCapital(word,len-1);     // change character to Capital
    }
}

md5::~md5()
{
    //dtor
}

bool sortDic(const data *a, const data *b)
{
   /* if(a->length < b->length)
    {
        printf("%s, %s\n",a->word,b->word);
        system("pause");
        return strcmp(a->word,b->word);
    }
    else    return false;*/
    return a->length < b->length;
}

void md5::buildDic()
{
    FILE *fp;
    char word[30];

    if((fp = fopen("dictionary.txt","r")) == NULL)
    {
        printf("Cannot open dictionary.txt\n");
        return;
    }
    while(!feof(fp))
    {
        fgets(word,sizeof(word),fp);

        int len = strlen(word)-1;   // Notice that '\n' is also count in length
        bool store = true;

        if(len > max_length || len < min_length)  continue;    // just keep the words with length between 3 and 8

        for(int j = 0; j < len; j++)
        {
            if(!isalpha(word[j]))
            {
                store = false;
                break;
            }
        }

        if(store)
        {
            word[len] = '\0';

            data *wordToTest = new data(len,word);
            dic.push_back(wordToTest);
        }
    }
    fclose(fp);
    dic_size = dic.size();
    sort(dic.begin(),dic.end(),sortDic);

    /*for(int i = 0; i < 20; i++)
    {
        printf("%s \n",(dic[i])->word);
    }*/

}

void md5::toCapital(char* word, int index)
{
    if(index == -1)
    {
        //printf("word = %s\n",word);
        addNum(word);
        return;
    }

    toCapital(word,index-1);    // remain the same character without change it to capital

    char new_word[15];
    strcpy(new_word,word);
    if(islower(new_word[index]))
        new_word[index] -= 32;
    else
        new_word[index] += 32;

    toCapital(new_word,index-1);
}

void md5::addNum(char* word)
{
    int i,cnt = 1, num = -1;
    int word_len;
    char tmp[15];
    char t[5] = "";
    char new_word[15], ori_word[15];
    char num_string[15];
    uint8_t result[16];

    memset(new_word,' ',sizeof(new_word));    // init

    cnt = 1000;

    for(i = 0; i < cnt; i++)
    {
        if(guess > guess_times) break;

        num++;

        char code[35] = "";
        sprintf(tmp,"%d",num);    // convert int num to string
        strcpy(num_string,tmp);
        /**case1: int + word **/
        strcpy(new_word,strcat(tmp,word));
        word_len = strlen(new_word);
        //printf("1.new_word = %s\n",new_word);
        md5Algo((uint8_t*)new_word, word_len, result);

        for(int j = 0; j < 16; j++)
        {
            sprintf(t,"%2.2x",result[j]);
            strcat(code,t);
        }
        //printf("code = %s\n",code);
        compare(new_word,code);


        /**case2: word + int **/
        strcpy(ori_word,word);
        strcpy(new_word,strcat(ori_word,num_string));
        word_len = strlen(new_word);
        //printf("2.new_word = %s\n",new_word);
        md5Algo((uint8_t*)new_word, word_len, result);

        for(int j = 0; j < 16; j++)
        {
            sprintf(t,"%2.2x",result[j]);
            strcat(code,t);
        }
        //printf("code = %s\n",code);
        compare(new_word,code);
    }
}

void md5::compare(char* word, char* result)
{
    bool hit = false;
    char password[35];
    FILE *fp1,*fp2;

    if((fp1 = fopen("HashedPassword.txt","r")) == NULL)
    {
        printf("Cannot open HashedPassword.txt\n");
        return;
    }

    while(!feof(fp1))
    {
        guess++;
        if(guess > guess_times) break;

        fgets(password,sizeof(password),fp1);
        password[32] = '\0';
        //printf("password = %s\n",password);
        //printf("result = %s\n",result);

        if((strcmp(password,result)) == 0)
        {
            hit = true;
            break;
        }
    }

    if(hit)
    {
        fp2 = fopen("Output.txt","a");
        fprintf(fp2,"%s\t%s\n",word,result);
        fclose(fp2);
        hit_num++;
        hit = false;
    }

    fclose(fp1);
}

int md5::getHitNum()
{
    return  hit_num;
}

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

void md5::to_bytes(uint32_t val, uint8_t *bytes)
{
    bytes[0] = (uint8_t) val;
    bytes[1] = (uint8_t) (val >> 8);
    bytes[2] = (uint8_t) (val >> 16);
    bytes[3] = (uint8_t) (val >> 24);
}

uint32_t md5::to_int32(const uint8_t *bytes)
{
    return (uint32_t) bytes[0]
           | ((uint32_t) bytes[1] << 8)
           | ((uint32_t) bytes[2] << 16)
           | ((uint32_t) bytes[3] << 24);
}

void md5::md5Algo(const uint8_t *initial_msg, size_t initial_len, uint8_t *digest)
{

    // These vars will contain the hash
    uint32_t h0, h1, h2, h3;

    // Message (to prepare)
    uint8_t *msg = NULL;

    size_t new_len, offset;
    uint32_t w[16];
    uint32_t a, b, c, d, i, f, g, temp;

    // Initialize variables - simple count in nibbles:
    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;

    //Pre-processing:
    //append "1" bit to message
    //append "0" bits until message length in bits ¡Ý 448 (mod 512)
    //append length mod (2^64) to message

    for (new_len = initial_len + 1; new_len % (512/8) != 448/8; new_len++)
        ;

    msg = (uint8_t*)malloc(new_len + 8);
    memcpy(msg, initial_msg, initial_len);
    msg[initial_len] = 0x80; // append the "1" bit; most significant bit is "first"
    for (offset = initial_len + 1; offset < new_len; offset++)
        msg[offset] = 0; // append "0" bits

    // append the len in bits at the end of the buffer.
    to_bytes(initial_len*8, msg + new_len);
    // initial_len>>29 == initial_len*8>>32, but avoids overflow.
    to_bytes(initial_len>>29, msg + new_len + 4);

    // Process the message in successive 512-bit chunks:
    //for each 512-bit chunk of message:
    for(offset=0; offset<new_len; offset += (512/8))
    {

        // break chunk into sixteen 32-bit words w[j], 0 ? j ? 15
        for (i = 0; i < 16; i++)
            w[i] = to_int32(msg + offset + i*4);

        // Initialize hash value for this chunk:
        a = h0;
        b = h1;
        c = h2;
        d = h3;

        // Main loop:
        for(i = 0; i<64; i++)
        {

            if (i < 16)
            {
                f = (b & c) | ((~b) & d);
                g = i;
            }
            else if (i < 32)
            {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            }
            else if (i < 48)
            {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;
            }
            else
            {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }

            temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;

        }

        // Add this chunk's hash to result so far:
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;

    }

    // cleanup
    free(msg);

    //var char digest[16] := h0 append h1 append h2 append h3 //(Output is in little-endian)
    to_bytes(h0, digest);
    to_bytes(h1, digest + 4);
    to_bytes(h2, digest + 8);
    to_bytes(h3, digest + 12);
}
