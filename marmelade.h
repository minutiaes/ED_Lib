#include <iostream>
#include <vector>
#include "edlib_detector.h"


struct MsgHeader
{
    uint8_t msg_type = 0;
    uint16_t msg_no;
    uint32_t payload_size;
};


enum MsgTypes
{
    ERROR_ANS,
    DETECTION_REQ,
    DETECTION_ANS,
    SET_SETTINGS_REQ,
    SET_SETTINGS_ANS,
    GET_SETTINGS_REQ,
    GET_SETTINGS_ANS
};


struct CircleB
{
    std::vector<uint8_t> center_x = std::vector<uint8_t>(8, 0);
    std::vector<uint8_t> center_y = std::vector<uint8_t>(8, 0);
    std::vector<uint8_t> r = std::vector<uint8_t>(8, 0);
};


struct EllipseB
{
    std::vector<uint8_t> center_x = std::vector<uint8_t>(8, 0);
    std::vector<uint8_t> center_y = std::vector<uint8_t>(8, 0);
    std::vector<uint8_t> width = std::vector<uint8_t>(4, 0);
    std::vector<uint8_t> height = std::vector<uint8_t>(4, 0);
    std::vector<uint8_t> theta = std::vector<uint8_t>(8, 0);
};

struct SettingsB
{
    std::vector<uint8_t> gradient_threshold = std::vector<uint8_t>(4, 0);
    std::vector<uint8_t> anchor_threshold = std::vector<uint8_t>(4, 0);
    std::vector<uint8_t> scan_interval = std::vector<uint8_t>(4, 0);
    std::vector<uint8_t> min_path_length = std::vector<uint8_t>(4, 0);
    std::vector<uint8_t> sigma = std::vector<uint8_t>(8, 0);
    std::vector<uint8_t> sum_flag = std::vector<uint8_t>(1, 0);
};


class Marmelade
{
public:
    Marmelade();
    size_t get_detector_response(std::vector<uint8_t> &response_b, uint8_t *buffer, uint32_t buffer_size);
    const MsgHeader& get_header() const;

private:
    EDLibDetector detector = EDLibDetector();
    uint8_t *buffer;
    uint32_t buffer_size;
    MsgHeader header;
    bool parse_header();
    bool detect_shapes(std::vector<uint8_t> &payload_b);
    bool set_settings(std::vector<uint8_t> &payload_b);
    bool get_settings(std::vector<uint8_t> &payload_b);
    void serialize_shapes(std::vector<uint8_t> &payload_b, DetectedShapes &shapes);
    void generate_header(MsgHeader &header, std::vector<uint8_t> &header_b);
};