#include <string>

static const std::wstring ReadAscii(const unsigned char* s, unsigned len)
{
    std::wstring result;
    while(len--) result += *s++;
    return result;
}
static const std::wstring ReadU16str(const unsigned char* s, unsigned len)
{
    std::wstring result;
    while(len > 0) { result += (*s) | (s[1] << 8); s += 2; }
    return result;
}
static const std::wstring ReadUTF8(const unsigned char* s, unsigned len)
{
    std::wstring result;
    const unsigned char*const end = s+len;
    /* Compared to the beast found in iconvircproxy,
     * this utf8 decoder is quite nice. But it isn't
     * as robust.
     */
    while(s<end)
    {
        unsigned val = *s++;
        if(!val) break;
        switch(val >> 4)
        {
            case 0x0F: val &= 0x07; goto b3;
            case 0x0E: val &= 0x0F; goto b2;
            case 0x0D:
            case 0x0C: val &= 0x1F; goto b1;
            default:   val &= 0x7F; break;
            b3: val <<= 6; val |= (*s++) & 63;
            b2: val <<= 6; val |= (*s++) & 63;
            b1: val <<= 6; val |= (*s++) & 63;
        }
        result += (wchar_t)val;
    }
    return result;
}
static void PutUTF8(std::vector<unsigned char>& buf, const std::wstring& s)
{
    for(unsigned a=0; a<s.size(); ++a)
    {
        unsigned n = s[a];
        if(n < 0x80)                 // <=7 bits
            buf.push_back(n);
        else
        {
            if(n < 0x800)            // <=11 bits
                buf.push_back((0xC0 + (n>>6)));
            else
            {
                if(n < 0x10000)      // <=16 bits
                    buf.push_back((0xE0 + (n>>12)));
                else                 // <=21 bits
                {
                    buf.push_back((0xF0 + (n>>18)));
                    buf.push_back((0x80 + ((n>>12)&63)));
                }
                buf.push_back((0x80 + ((n>>6)&63)));
            }
            buf.push_back((0x80 + (n&63)));
        }
    }
}

#ifndef WIN32
static const std::string WsToMb(const std::wstring& s)
{
    char Buf[4096];
    size_t n = std::wcstombs(Buf, s.c_str(), sizeof(Buf));
    if(n == (size_t)-1) return std::string("locale error");
    return std::string(Buf, Buf+n);
}
static const std::wstring MbToWs(const std::string& s)
{
    wchar_t Buf[4096];
    size_t n = std::mbstowcs(Buf, s.c_str(), sizeof(Buf));
    return std::wstring(Buf, Buf+n);
}
#endif
static void WriteAsciiFZ(std::vector<unsigned char>& target, const std::wstring& s, unsigned length)
{
    for(unsigned a=0; a<length; ++a)
        target.push_back(a < s.length() ? s[a] : 0);
}
