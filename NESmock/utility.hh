#include <stdint.h>

static const uint32_t FOURCC(const uint8_t x[4])
{
    return x[0] | (x[1] << 8) | (x[2] << 16) | (x[3] << 24);
}
static const uint16_t TWOCC(const uint8_t x[2])
{
    return x[0] | (x[1] << 8);
}

static const uint32_t FOURCC(const char *x) { return FOURCC((const uint8_t*)x); }
static const uint16_t TWOCC(const char *x) { return TWOCC((const uint8_t*)x); }


#define R32(x) FOURCC(reinterpret_cast<const uint8_t*>(&(x)))
#define R16(x) TWOCC(reinterpret_cast<const uint8_t*>(&(x)))

#include "strfun.hh"

static void Write8(std::vector<unsigned char>& target, uint8_t value)
{
    target.push_back(value);
}
static void Write16(std::vector<unsigned char>& target, uint16_t value)
{
    target.push_back(value & 0xFF);
    target.push_back(value >> 8);
}
static void Write32(std::vector<unsigned char>& target, uint32_t value)
{
    Write16(target, value & 0xFFFF);
    Write16(target, value >> 16);
}
