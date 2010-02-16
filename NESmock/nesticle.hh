class NesticleMovie: public Movie
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
        return false;
    }

    void Write(std::vector<unsigned char>& data)
    {
    }
};
