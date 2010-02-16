class VirtuaNESMovie: public Movie
{
    // don't add member vars here.

    class Statetype: public Movie::SaveState
    {
        // don't add member vars here either.
    private:
        struct VMVstateheader
        {
            char ID[12]; // "VirtuaNES ST"
            uint_least16_t reserved;
            uint_least16_t blockversion;
            uint_least32_t ext0;
            uint_least16_t ext1;
            uint_least16_t ext2;
            int_least32_t moviestep;
            int_least32_t movioffset;
        };
        struct VMVstateblockheader
        {
            char ID[8];
            uint_least16_t reserved;
            uint_least16_t blockversion;
            uint_least32_t blocksize;
        };
    public:
        void Load(const std::vector<unsigned char>& data)
        {
            bool global_header_received = false;
            // all data is ignored for now.
            for(unsigned offset=0;
                offset + sizeof(VMVstateblockheader) <= data.size();
                )
            {
                const VMVstateblockheader& hdr = *(const VMVstateblockheader*)&data[offset];
                if(!global_header_received)
                {
                    offset += sizeof(VMVstateheader);
                    global_header_received = true;
                    continue;
                }
                offset += sizeof(hdr);
                
                if(::memcmp(hdr.ID, "REG DATA", 8) == 0)
                {
                    this->regPC = *(const uint_least16_t*)&data[offset+0];
                    this->regA  = *(const uint_least8_t*)&data[offset+2];
                    this->regX  = *(const uint_least8_t*)&data[offset+3];
                    this->regY  = *(const uint_least8_t*)&data[offset+4];
                    this->regS  = *(const uint_least8_t*)&data[offset+5];
                    this->regP  = *(const uint_least8_t*)&data[offset+6];
                    
                    this->ppu[0] = *(const uint_least8_t*)&data[offset+64+0];
                    this->ppu[1] = *(const uint_least8_t*)&data[offset+64+1];
                    this->ppu[2] = *(const uint_least8_t*)&data[offset+64+2];
                    this->ppu[3] = *(const uint_least8_t*)&data[offset+64+3];
                }
                if(::memcmp(hdr.ID, "RAM DATA", 8) == 0)
                {
                    // ignore
                    ::memcpy(this->RAM,       &data[offset+0],     0x800);
                    ::memcpy(this->palRAM+ 0, &data[offset+0x800], 0x10);
                    ::memcpy(this->palRAM+16, &data[offset+0x810], 0x10);
                    ::memcpy(this->spRAM,     &data[offset+0x820], 0x100);
                }
                if(::memcmp(hdr.ID, "MMU DATA", 8) == 0)
                {
                    const unsigned char* cpu_mem_type/*[8]*/  = &data[offset+0];
                    const unsigned char* cpu_mem_page/*[8]*/  = &data[offset+8];
                    const unsigned char* ppu_mem_type/*[12]*/ = &data[offset+8+8];
                    const unsigned char* ppu_mem_page/*[12]*/ = &data[offset+8+8+12];
                    const unsigned char* cram_used/*[8]*/     = &data[offset+8+8+12+12];

                    unsigned tmp = offset+8+8+12+12+8;

                    for(unsigned a=3; a<8; ++a)
                        if(cpu_mem_type[a] != 0)
                        {
                            ::memcpy(this->wRAM, &data[tmp], 0x2000);
                            tmp += 0x2000;
                        }

                    ::memcpy(this->ntaRAM, &data[tmp], 0x1000);
                    tmp += 0x1000;

                    for(unsigned b=0,a=0; a<8; ++a)
                        if(cram_used[a] != 0)
                        {
                            fprintf(stderr, "Loading chrram %u/8\n", a);
                            
                            if(b < 0x2000)
                                ::memcpy(&this->chrRAM[b], &data[tmp], 0x1000);
                            
                            tmp += 0x1000;
                            b += 0x1000;
                        }
                }
                if(::memcmp(hdr.ID, "MMC DATA", 8) == 0)
                {
                    // TODO (mapper state)
                    unsigned size = sizeof(this->mapperbytes);
                    if(size > hdr.blocksize) size = hdr.blocksize;
                    ::memcpy(this->mapperbytes, &data[offset], size);
                }
                if(::memcmp(hdr.ID, "CTR DATA", 8) == 0)
                {
                    // TODO
                }
                if(::memcmp(hdr.ID, "SND DATA", 8) == 0)
                {
                    // ignore
                }
                if(::memcmp(hdr.ID, "DISKDATA", 8) == 0)
                {
                    // ignore
                }
                if(::memcmp(hdr.ID, "EXCTRDAT", 8) == 0)
                {
                    // ignore
                }
                offset += hdr.blocksize;
            }
        }
        void Write(std::vector<unsigned char>& data)
        {
        }
    };
public:
    bool Load(const std::vector<unsigned char>& data)
    {
        if(ReadAscii(&data[0], 12) != L"VirtuaNES MV") return false;
        
        unsigned version = R16(data[0x0C]);
        
        Save  = true; // VirtuaNES movies are always savestate-basde
        Ctrl1 = data[0x10] & 0x01;
        Ctrl2 = data[0x10] & 0x02;
        Ctrl3 = data[0x10] & 0x04;
        Ctrl4 = data[0x10] & 0x08;
        FDS   = false;
        
        RecordCount = R32(data[0x1C]);
        // TODO: ROM CRC
        // TODO: render method
        // TODO: irq type
        // TODO: frame ireq
        
        PAL = data[0x23] & 0x1;
        
        unsigned SaveBegin, SaveEnd, CtrlBegin;
        
        switch(version)
        {
            case 0x0200:
            {
                CtrlBegin = R32(data[0x2C]);
                SaveBegin = 0x34;
                SaveEnd   = CtrlBegin;
                FrameCount = 3600*60*5; // assume something big but not too big
                // TODO: Movie CRC (0x30)
                break;
            }
            case 0x0300:
            {
                SaveBegin  = R32(data[0x2C]);
                SaveEnd    = R32(data[0x30]);
                CtrlBegin  = R32(data[0x34]);
                
                FrameCount = R32(data[0x38]);
                
                // TODO: Movie CRC (0x3C)
                break;
            }
            default:
            {
                fprintf(stderr, "Invalid VirtuaNES movie version: %04X\n", version);
                return false;
            }
        }
        
        if(SaveBegin > SaveEnd || SaveEnd > data.size())
        {
            fprintf(stderr, "Invalid VirtuaNES movie file: SaveBegin=%u, SaveEnd=%u, Filesize=%u\n",
                SaveBegin, SaveEnd, (unsigned)data.size());
            return false;
        }
        
        std::vector<unsigned char> statedata(data.begin()+SaveBegin, data.begin()+SaveEnd);

        (reinterpret_cast<Statetype&>(State)).Load(statedata);
        
        Cdata.SetSize(FrameCount);
        
        unsigned pos = CtrlBegin;
        for(unsigned frame=0; frame<FrameCount; ++frame)
        {
            const bool masks[4] = {Ctrl1,Ctrl2,Ctrl3,Ctrl4};
            for(unsigned ctrl=0; ctrl<4; ++ctrl)
            {
                if(!masks[ctrl]) continue;
               
                /* Check for commands */
                for(;;)
                {
                    if(pos >= data.size())
                    {
                        Cdata.SetSize(FrameCount = frame);
                        goto End;
                    }
                    unsigned char Data = data[pos];
                    if(Data < 0xF0) break;
                    /* It's a command */
                    ++pos;
                    if(Data == 0xF0)
                    {
                        unsigned char byte1 = data[pos++];
                        unsigned char byte2 = data[pos++];
                        if(byte2 == 0)
                        {
                            /* command (NESCOMMAND) byte1, 0 */
                        }
                        else
                        {
                            /* command NESCMD_EXCONTROLLER, byte1 */
                            /* sets the extra controller to given value */
                        }
                    }
                    else if(Data == 0xF3)
                    {
                        /* 4 bytes as params */
                        unsigned dwdata = R32(data[pos]); pos += 4;
                        /* SetSyncExData(dwdata) */
                    }
                }

                Cdata[frame].Ctrl[ctrl] = data[pos++];
            }
        }
    End:
        Save=true;
        
        return true;
    }

    void Write(std::vector<unsigned char>& data)
    {
    }
};
