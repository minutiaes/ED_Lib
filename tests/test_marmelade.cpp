#include <cassert>
#include <chrono>
#include "test_marmelade.h"

TestMarmelade::TestMarmelade()
{
    ;
}

TestMarmelade::ParseHeader::ParseHeader()
{
    ;
}

void TestMarmelade::run_all()
{
    // parse_header.run_all();
    detect.run_all();
}

/* -------------------------------------------------------------------------- */
/*                                 ParseHeader                                */
/* -------------------------------------------------------------------------- */
void TestMarmelade::ParseHeader::small_buffer()
{
    std::cout << "TestMarmelade::ParseHeader::small_buffer starts ...\n";

    Marmelade marmelade = Marmelade();

    uint8_t buff[] = {
        0x01,                       // msg_type
        0x05, 0x00,                 // msg_no     
        0x64, 0x00, 0x00            //payload_size
    };

    uint32_t buff_size = sizeof(buff)/sizeof(buff[0]);

    std::vector<uint8_t> resp_b;
    size_t ret = marmelade.get_detector_response(resp_b, buff, buff_size);

    assert(ret == 0);

    std::cout << "TestMarmelade::ParseHeader::small_buffer ends ...\n";
}

void TestMarmelade::ParseHeader::nominal()
{
    std::cout << "TestMarmelade::ParseHeader::nominal starts ...\n";
    Marmelade marmelade = Marmelade();

    uint8_t buff[] = {
        0x01,                       // msg_type
        0x05, 0x00,                 // msg_no     
        0x64, 0x00, 0x00, 0x00      //payload_size
    };
    int expected[] = {
        1,
        5,
        100
    };

    uint32_t buff_size = sizeof(buff)/sizeof(buff[0]);

    std::vector<uint8_t> resp_b;
    size_t ret = marmelade.get_detector_response(resp_b, buff, buff_size);

    assert(ret > 0);

    MsgHeader header = marmelade.get_header();
    assert(header.msg_type == expected[0]);
    assert(header.msg_no ==expected[1]);
    assert(header.payload_size == expected[2]);
    std::cout << "TestMarmelade::ParseHeader::nominal ends ...\n";
}


void TestMarmelade::ParseHeader::run_all()
{
    std::cout << "TestMarmelade::ParseHeader starts ...\n";
    small_buffer();
    nominal();
    std::cout << "TestMarmelade::ParseHeader ends ...\n";
}


/* -------------------------------------------------------------------------- */
/*                                   Detect                                   */
/* -------------------------------------------------------------------------- */

void TestMarmelade::Detect::nominal()
{
    std::cout << "TestMarmelade::Detect::nominal starts ...\n";
    Marmelade marmelade = Marmelade();

    char filename[] = "billiard.jpg";
    cv::Mat image = cv::imread(filename, 0);

    std::vector<uint8_t> buff{
        0x01,                       // msg_type
        0x01, 0x00,                 // msg_no
    };

    uint32_t payload_size = image.rows * image.cols + 4;
    uint8_t payload_size_b[4] = {0};
    memcpy(payload_size_b, &payload_size, sizeof(payload_size));

    buff.insert(buff.end(), payload_size_b, payload_size_b + sizeof(payload_size));

    uint16_t rows = static_cast<uint16_t>(image.rows);
    uint16_t cols = static_cast<uint16_t>(image.cols);

    uint8_t rows_b[2] = {0};
    uint8_t cols_b[2] = {0};

    memcpy(rows_b, &rows, sizeof(rows));
    memcpy(cols_b, &cols, sizeof(cols));

    buff.insert(buff.end(), rows_b, rows_b + sizeof(rows));
    buff.insert(buff.end(), cols_b, cols_b + sizeof(cols));

    buff.insert(buff.end(), image.data, image.data + image.rows*image.cols);

    // std::cout << buff.size() << std::endl;
    // for (int i=0; i<50; i++)
    // {
    //     std::cout << std::hex << static_cast<int>(buff[i]) << std::endl;
    // }

    std::vector<uint8_t> resp_b;
    uint32_t buff_size = static_cast<uint32_t>(buff.size()); 
    marmelade.get_detector_response(resp_b, buff.data(), buff_size);
    std::cout << "test_marmelade resp_b size: " << static_cast<int>(resp_b.size()) << std::endl;



    
    // int expected[] = {
    //     1,
    //     5,
    //     100
    // };

    // size_t buff_size = sizeof(buff)/sizeof(buff[0]);

    // std::vector<uint8_t> resp_b;
    // size_t ret = marmelade.get_detector_response(resp_b, buff, buff_size);

    // assert(ret > 0);

    // MsgHeader header = marmelade.get_header();
    // assert(header.msg_type == expected[0]);
    // assert(header.msg_no ==expected[1]);
    // assert(header.payload_size == expected[2]);

    std::cout << "TestMarmelade::Detect::nominal ends ...\n";
}

void TestMarmelade::Detect::run_all()
{
    std::cout << "TestMarmelade::Detect starts ...\n";
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    nominal();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "TestMarmelade::Detect ends ...\n";
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[us]" << std::endl;
}


int main()
{
    TestMarmelade().run_all();

    return 1;
}
//     /* ------------------------- Test change_settings() ------------------------- */



//     /* -------------------------- Test detect_shapes() -------------------------- */

