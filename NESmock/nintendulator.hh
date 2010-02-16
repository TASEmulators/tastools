#include <algorithm>

class NintendulatorMovie: public Movie
{
    // don't add member vars here.

    class Statetype: public Movie::SaveState
    {
        // don't add member vars here either.
    public:
        void Load(const std::vector<unsigned char>& data)
        {
            unsigned cboffset = 0x10;
            bool hasmovie = false;
            while (cboffset < data.size())
            {
                uint32_t cbname = R32(data[cboffset]);
                uint32_t cblen = R32(data[cboffset+4]);
                
                unsigned blockbegin = cboffset+8;
                
                if(cbname == FOURCC("CPUS"))
                {
                    regPC = R16(data[blockbegin+0]);
                    regA  = data[blockbegin+2];
                    regX  = data[blockbegin+3];
                    regY  = data[blockbegin+4];
                    regS  = data[blockbegin+5];
                    regP  = data[blockbegin+6];
                    
                    std::copy(&data[blockbegin+10],
                              &data[blockbegin+10+0x800],
                              RAM);
                }
                else if(cbname == FOURCC("PPUS"))
                {
                    std::copy(&data[blockbegin+0],
                              &data[blockbegin+0x1000],
                              ntaRAM);
                    std::copy(&data[blockbegin+0x1000],
                              &data[blockbegin+0x1100],
                              spRAM);
                    std::copy(&data[blockbegin+0x1100],
                              &data[blockbegin+0x1120],
                              palRAM);
                }
                cboffset += cblen + 8;
            }
        }
        void Write(std::vector<unsigned char>& data)
        {
        }
    };
    
public:
    bool Load(const std::vector<unsigned char>& data)
    {
        if(R32(data[0x0]) != FOURCC("NSS\032")) return false;    // NSS+1A
        if(R32(data[0x4]) != FOURCC("0950")) return false;    // 0950 []
                                // [file length]
        if(R32(data[0xC]) != FOURCC("NMOV")) return false;
        
        Save = false;
        
        unsigned cboffset = 0x10;
        bool hasmovie = false;
        while (cboffset < data.size())
        {
            uint32_t cbname = R32(data[cboffset]);
            uint32_t cblen = R32(data[cboffset+4]);
            
            if(cbname == FOURCC("NMOV"))
            {
                if (data[cboffset+8] == 5)
                {
                    Ctrl1 = data[cboffset+9] & 0x01;
                    Ctrl2 = data[cboffset+9] & 0x02;
                    Ctrl3 = data[cboffset+9] & 0x04;
                    Ctrl4 = data[cboffset+9] & 0x08;
                }
                else
                {
                    if (data[cboffset+8] > 1) return false; // not a normal controller
                    if (data[cboffset+9] > 1) return false; // not a normal controller
                    Ctrl1 = data[cboffset+8];
                    Ctrl2 = data[cboffset+9];
                    Ctrl3 = Ctrl4 = false;
                }
                if (data[cboffset+10]) return false; // expansion port controller

                unsigned bytes_per_frame = 0;
                if(Ctrl1) ++bytes_per_frame;
                if(Ctrl2) ++bytes_per_frame;
                if(Ctrl3) ++bytes_per_frame;
                if(Ctrl4) ++bytes_per_frame;

                // dangerous assumption, this could be for another mapper
                FDS = (data[cboffset+11] & 0x7F) == (bytes_per_frame+1);
                
                if(FDS)   ++bytes_per_frame;

                PAL = data[cboffset+11] & 0x80;
                
                unsigned info_length = R32(data[cboffset+16]);

                FrameCount = R32(data[cboffset+20+info_length]) / bytes_per_frame;
                RecordCount = R32(data[cboffset+12]);

                EmuName   = L"Nintendulator 0.950";
                MovieName = ReadAscii(&data[cboffset+20], info_length);

                Cdata.SetSize(FrameCount);

                unsigned pos = cboffset+24+info_length;
                unsigned char lastdisk = 0;
                for(unsigned frame=0; frame<FrameCount; ++frame)
                {
                    if(Ctrl1) Cdata[frame].Ctrl[0] = data[pos++];
                    if(Ctrl2) Cdata[frame].Ctrl[1] = data[pos++];
                    if(Ctrl3) Cdata[frame].Ctrl[2] = data[pos++];
                    if(Ctrl4) Cdata[frame].Ctrl[3] = data[pos++];
                    if(FDS)
                    {
                        if (data[pos])
                            lastdisk = (data[pos] == 0xFF) ? 0 : data[pos];
                        Cdata[frame].FDS = lastdisk;
                        pos++;
                    }
                }
                hasmovie = true;
            }
            else if(!Save)
            {
                Save = true;
                (reinterpret_cast<Statetype&>(State)).Load(data);
            }
            cboffset += cblen + 8;
        }
        return hasmovie;
    }

    void Write(std::vector<unsigned char>& data)
    {
        unsigned char framelen = 0;
        if (Ctrl1) framelen++;
        if (Ctrl2) framelen++;
        if (Ctrl3) framelen++;
        if (Ctrl4) framelen++;
        if (FDS) framelen++;
        if (PAL) framelen |= 0x80;

        unsigned long blocklen = (framelen & 0x7F) * FrameCount + MovieName.size() + 17;

        Write32(data, FOURCC("NSS\032"));
        Write32(data, FOURCC("0950"));
        Write32(data, blocklen+8);
        Write32(data, FOURCC("NMOV"));

        Write32(data, FOURCC("NMOV"));
        Write32(data, blocklen);
        if (Ctrl3 || Ctrl4)
        {
            unsigned char mask = 0;
            if (Ctrl1) mask |= 0x01;
            if (Ctrl2) mask |= 0x02;
            if (Ctrl3) mask |= 0x04;
            if (Ctrl4) mask |= 0x08;
            Write8(data, 5);        // fourscore
            Write8(data, mask);    // controller mask
        }
        else
        {
            Write8(data, Ctrl1 ? 1 : 0);    // port 1
            Write8(data, Ctrl2 ? 1 : 0);    // port 2
        }
        Write8(data, 0);        // exp port

        Write8(data, framelen);    // length + NTSC/PAL

        Write32(data, RecordCount);    // re-records
        Write32(data, MovieName.size()+1);
        WriteAsciiFZ(data, MovieName, MovieName.size());
        Write8(data, 0);        // null terminator

        Write32(data, FrameCount * (framelen & 0x7F));    // movie len

        unsigned char lastdisk = 0;
        for(unsigned a=0; a<FrameCount; ++a)
        {
            if(Ctrl1) Write8(data, Cdata[a].Ctrl[0]);
            if(Ctrl2) Write8(data, Cdata[a].Ctrl[1]);
            if(FDS)
            {
                if (lastdisk != Cdata[a].FDS)
                    Write8(data, Cdata[a].FDS == 0 ? 0xFF : Cdata[a].FDS);
                else
                    Write8(data, 0);
                lastdisk = Cdata[a].FDS;
            }
        }
    }
};
