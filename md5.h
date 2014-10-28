#ifndef MD5_H
#define MD5_H
#include <stdint.h>

class md5
{
public:
    md5();
    virtual ~md5();
    int getHitNum();
protected:
private:
    void to_bytes(uint32_t, uint8_t*);
    uint32_t to_int32(const uint8_t*);
    void md5Algo(const uint8_t*, size_t, uint8_t*);
    void buildDic();
    //void buildList();
    void toCapital(char*, int);
    void addNum(char*);
    void compare(char*, char*);
};

#endif // MD5_H
