#undef NDEBUG

#include "../marmelade.h"


class TestMarmelade
{
public:
    TestMarmelade();
    class ParseHeader
    {
    public:
        ParseHeader();
        void small_buffer();
        void nominal();

        void run_all();
    };
    class Detect
    {
    public:
        void nominal();

        void run_all();
    };
    ParseHeader parse_header = ParseHeader();
    Detect detect = Detect();

    void run_all();
};