#ifndef  __HEXDUMP_H
#define  __HEXDUMP_H

#define HEXDUMP(ARG) HexDump::dump(ARG, __func__, __LINE__)

class HexDump 
{
    private: 
    public: 
        static void dump(QByteArray* raw, const char* func, int line);

};

#endif  // __HEXDUMP_H 
