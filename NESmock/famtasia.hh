namespace Famtasia
{

/*
Button        Famtasia   Every Other Emulator
------------  --------  ------------------------
A             20         01
B             10         02
Select        40         04
Start         80         08
Up            04         10
Down          08         20
Left          02         40
Right         01         80
*/

    /* Famtasia buttons. */
    static const unsigned R = 0x01;
    static const unsigned L = 0x02;
    static const unsigned U = 0x04;
    static const unsigned D = 0x08;
    static const unsigned B = 0x10;
    static const unsigned A = 0x20;
    static const unsigned SE = 0x40;
    static const unsigned ST = 0x80;

    static unsigned char FromNormal(unsigned char Data)
    {
        unsigned char Ctrl=0;
        if(Data&0x01) Ctrl|=A;
        if(Data&0x02) Ctrl|=B;
        if(Data&0x04) Ctrl|=SE;
        if(Data&0x08) Ctrl|=ST;
        if(Data&0x10) Ctrl|=U;
        if(Data&0x20) Ctrl|=D;
        if(Data&0x40) Ctrl|=L;
        if(Data&0x80) Ctrl|=R;
        return Ctrl;
    }
    static unsigned char ToNormal(unsigned char Data)
    {
        unsigned char Ctrl=0;
        if(Data& A) Ctrl|=0x01;
        if(Data& B) Ctrl|=0x02;
        if(Data&SE) Ctrl|=0x04;
        if(Data&ST) Ctrl|=0x08;
        if(Data& U) Ctrl|=0x10;
        if(Data& D) Ctrl|=0x20;
        if(Data& L) Ctrl|=0x40;
        if(Data& R) Ctrl|=0x80;
        return Ctrl;
    }
}

class FamtasiaMovie: public Movie
{
    // don't add member vars here.

    class Statetype: public Movie::SaveState
    {
        // don't add member vars here either.
    public:
        void Load(const std::vector<unsigned char>& data)
        {
            // all data is ignored for now.
        }
        void Write(std::vector<unsigned char>& data)
        {
        }
    };

public:
    bool Load(const std::vector<unsigned char>& data)
    {
        if(R32(data[0]) != FOURCC("FMV\032")) return false;
        
        PAL   = false;
        Save  = data[4] & 0x80;
        Ctrl1 = data[5] & 0x80;
        Ctrl2 = data[5] & 0x40;
        FDS   = data[5] & 0x20;
        Ctrl3 = data[5] & 0x08;
        Ctrl4 = data[5] & 0x04;
        
        unsigned bytes_per_frame = 0;
        if(Ctrl1) ++bytes_per_frame;
        if(Ctrl2) ++bytes_per_frame;
        if(Ctrl3) ++bytes_per_frame;
        if(Ctrl4) ++bytes_per_frame;
        if(FDS)   ++bytes_per_frame;
        
        if(!bytes_per_frame)
        {
            fprintf(stderr, "This FMV contains no controllers.\n");
            return false;
        }
        
        FrameCount = (data.size() - 0x90) / bytes_per_frame;
        RecordCount = R32(data[0x0A]) + 1;
        
        EmuName   = ReadAscii(&data[0x10], std::strlen((const char*)&data[0x10]));
        MovieName = ReadAscii(&data[0x50], std::strlen((const char*)&data[0x50]));

        /* TODO: Read save if exists */
        
        Cdata.SetSize(FrameCount);
        
        unsigned pos = 0x90;
        for(unsigned frame=0; frame<FrameCount; ++frame)
        {
            if(Ctrl1) Cdata[frame].Ctrl[0] = Famtasia::ToNormal(data[pos++]);
            if(Ctrl2) Cdata[frame].Ctrl[1] = Famtasia::ToNormal(data[pos++]);
            if(Ctrl3) Cdata[frame].Ctrl[2] = Famtasia::ToNormal(data[pos++]);
            if(Ctrl4) Cdata[frame].Ctrl[3] = Famtasia::ToNormal(data[pos++]);
            if(FDS)   Cdata[frame].FDS     = data[pos++];
        }
        return true;
    }
    
    void Write(std::vector<unsigned char>& data)
    {
        Write32(data, FOURCC("FMV\032"));
        
        Write8(data, 0);
        
        unsigned char tmp = 0x00;
        if(Ctrl1) tmp|= 0x80;
        if(Ctrl2) tmp|= 0x40;
        if(FDS)   tmp|= 0x20;
        if(Ctrl3) tmp|= 0x08;
        if(Ctrl4) tmp|= 0x04;
        Write8(data, tmp);
        
        Write32(data, 0);
        Write32(data, RecordCount-1);
        Write16(data, 0);
        WriteAsciiFZ(data, EmuName, 64);
        WriteAsciiFZ(data, MovieName, 64);
        
        for(unsigned a=0; a<FrameCount; ++a)
        {
            if(Ctrl1) Write8(data, Famtasia::FromNormal(Cdata[a].Ctrl[0]));
            if(Ctrl2) Write8(data, Famtasia::FromNormal(Cdata[a].Ctrl[1]));
            if(Ctrl3) Write8(data, Famtasia::FromNormal(Cdata[a].Ctrl[2]));
            if(Ctrl4) Write8(data, Famtasia::FromNormal(Cdata[a].Ctrl[3]));
            if(FDS)   Write8(data, Cdata[a].FDS);
        }
    }
};
