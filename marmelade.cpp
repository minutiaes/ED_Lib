#include "marmelade.h"


template <class T>
void to_bytes (std::vector<uint8_t> &v, T i) 
{
    
    uint8_t* chars = reinterpret_cast<uint8_t*>(&i);
    // v.insert(v.end(), chars, chars+sizeof(i));
    memcpy(&v[0], chars, sizeof(i));
}


Marmelade::Marmelade()
{
    ;
}


size_t Marmelade::get_detector_response(std::vector<uint8_t> &response_b, uint8_t *buffer, uint32_t buffer_size)
{
    this->buffer = buffer;
    this->buffer_size = buffer_size;
    // std::cout << "msg_no in buffer___: " << static_cast<unsigned int>(buffer[1]) << std::endl;

    MsgHeader header_ans;
    std::vector<uint8_t> payload_b;
    std::vector<uint8_t> header_ans_b;

    if (parse_header() == false)
    {
        return 0;
    }

    // std::cout << "msg_type: " << static_cast<unsigned int>(header.msg_type) << std::endl;
    // std::cout << "msg_no: " << static_cast<unsigned int>(header.msg_no) << std::endl;
    // std::cout << "payload_size: " << static_cast<unsigned int>(header.payload_size) << std::endl;

    if (header.msg_type == static_cast<int>(MsgTypes::DETECTION_REQ))
    {
        // std::cout << "detection req!\n";
        detect_shapes(payload_b);

        header_ans.msg_type = static_cast<uint8_t>(MsgTypes::DETECTION_ANS);
        
    }
    else if (header.msg_type == static_cast<int>(MsgTypes::SET_SETTINGS_REQ))
    {
        // std::cout << "change settings request!\n";
        set_settings(payload_b);

        header_ans.msg_type = static_cast<uint8_t>(MsgTypes::SET_SETTINGS_ANS);
    }
    else if (header.msg_type == static_cast<int>(MsgTypes::GET_SETTINGS_REQ))
    {
        // std::cout << "change settings request!\n";
        get_settings(payload_b);

        header_ans.msg_type = static_cast<uint8_t>(MsgTypes::GET_SETTINGS_ANS);
    }


    header_ans.msg_no = header.msg_no;
    header_ans.payload_size = static_cast<uint32_t>(payload_b.size());

    generate_header(header_ans, header_ans_b);
    response_b.insert(response_b.end(), header_ans_b.begin(), header_ans_b.end());
    response_b.insert(response_b.end(), payload_b.begin(), payload_b.end());
    return 1;
}

const MsgHeader& Marmelade::get_header() const
{
    return header;
}

bool Marmelade::parse_header()
{
    if (buffer_size < 7)
    {
        return false;
    }

    memcpy(&header.msg_type, buffer, 1);
    memcpy(&header.msg_no, buffer+1, 2);
    memcpy(&header.payload_size, buffer+3, 4);

    return true;
}


bool Marmelade::detect_shapes(std::vector<uint8_t> &payload_b)
{
    // std::cout << "detect_shapes()\n";
    uint16_t rows = 0;
    uint16_t cols = 0;
    memcpy(&rows, buffer+7, 2);
    memcpy(&cols, buffer+9, 2);

    // std::cout << "rows: " << static_cast<unsigned int>(rows) << std::endl;
    // std::cout << "cols: " << static_cast<unsigned int>(cols) << std::endl;
    // std::cout << "rows in buffer: " << static_cast<unsigned int>(buffer[0]) << std::endl;
    // std::cout << "msg_no in buffer: " << static_cast<unsigned int>(buffer[1]) << std::endl;

    cv::Mat image(rows, cols, CV_8UC1, buffer+11);

    DetectedShapes result;
    result = detector.detect(image);

    serialize_shapes(payload_b, result);
    // std::cout << "cicle no: " << static_cast<unsigned int>(result.circles.size()) << std::endl;
    // std::cout << "ellipse no: " << static_cast<unsigned int>(result.ellipses.size()) << std::endl;
    // std::cout << "cicle-ellipse no byte: " << static_cast<unsigned int>(payload_b[0])  << ":" << static_cast<unsigned int>(payload_b[1]) << " - "
    // << static_cast<unsigned int>(payload_b[2])  << ":" << static_cast<unsigned int>(payload_b[3]) << std::endl;
    // std::cout << "payload_b size: " << static_cast<unsigned int>(payload_b.size()) << std::endl;
    return true;
}


bool Marmelade::set_settings(std::vector<uint8_t> &payload_b)
{
    // std::cout << "change_settings()\n";

    DetectorSettings new_settings;

    memcpy(&new_settings.gradient_threshold, buffer+7, 4);
    memcpy(&new_settings.anchor_threshold, buffer+11, 4);
    memcpy(&new_settings.scan_interval, buffer+15, 4);
    memcpy(&new_settings.min_path_length, buffer+19, 4);
    memcpy(&new_settings.sigma, buffer+23, 8);
    memcpy(&new_settings.sum_flag, buffer+31, 1);

    detector.settings = new_settings;

    payload_b.push_back(1);
    return true;
}

bool Marmelade::get_settings(std::vector<uint8_t> &payload_b)
{
    SettingsB _settings;

    to_bytes(_settings.gradient_threshold, detector.settings.gradient_threshold);
    to_bytes(_settings.anchor_threshold, detector.settings.anchor_threshold);
    to_bytes(_settings.scan_interval, detector.settings.scan_interval);
    to_bytes(_settings.min_path_length, detector.settings.min_path_length);
    to_bytes(_settings.sigma, detector.settings.sigma);
    to_bytes(_settings.sum_flag, detector.settings.sum_flag);

    payload_b.insert(payload_b.end(), _settings.gradient_threshold.begin(), _settings.gradient_threshold.end());
    payload_b.insert(payload_b.end(), _settings.anchor_threshold.begin(), _settings.anchor_threshold.end());
    payload_b.insert(payload_b.end(), _settings.scan_interval.begin(), _settings.scan_interval.end());
    payload_b.insert(payload_b.end(), _settings.min_path_length.begin(), _settings.min_path_length.end());
    payload_b.insert(payload_b.end(), _settings.sigma.begin(), _settings.sigma.end());
    payload_b.insert(payload_b.end(), _settings.sum_flag.begin(), _settings.sum_flag.end());

    return true;
}


void Marmelade::serialize_shapes(std::vector<uint8_t> &payload_b, DetectedShapes &shapes)
{
    std::vector<uint8_t> circles_no(2, 0);
    std::vector<uint8_t> ellipses_no(2, 0);
    to_bytes(circles_no, static_cast<uint16_t>(shapes.circles.size()));
    to_bytes(ellipses_no, static_cast<uint16_t>(shapes.ellipses.size()));

    payload_b.insert(payload_b.end(), circles_no.begin(), circles_no.end());
    payload_b.insert(payload_b.end(), ellipses_no.begin(), ellipses_no.end());

    for (int i=0; i<shapes.circles.size(); i++)
    {
        CircleB circle;
        to_bytes(circle.center_x, shapes.circles[i].center.x);
        to_bytes(circle.center_y, shapes.circles[i].center.y);
        to_bytes(circle.r, shapes.circles[i].r);

        payload_b.insert(payload_b.end(), circle.center_x.begin(), circle.center_x.end());
        payload_b.insert(payload_b.end(), circle.center_y.begin(), circle.center_y.end());
        payload_b.insert(payload_b.end(), circle.r.begin(), circle.r.end());
    }

    for (int i=0; i<shapes.ellipses.size(); i++)
    {
        EllipseB ellipse;
        to_bytes(ellipse.center_x, shapes.ellipses[i].center.x);
        to_bytes(ellipse.center_y, shapes.ellipses[i].center.y);
        to_bytes(ellipse.width, shapes.ellipses[i].axes.width);
        to_bytes(ellipse.height, shapes.ellipses[i].axes.height);
        to_bytes(ellipse.theta, shapes.ellipses[i].theta);

        payload_b.insert(payload_b.end(), ellipse.center_x.begin(), ellipse.center_x.end());
        payload_b.insert(payload_b.end(), ellipse.center_y.begin(), ellipse.center_y.end());
        payload_b.insert(payload_b.end(), ellipse.width.begin(), ellipse.width.end());
        payload_b.insert(payload_b.end(), ellipse.height.begin(), ellipse.height.end());
        payload_b.insert(payload_b.end(), ellipse.theta.begin(), ellipse.theta.end());
    }
}

void Marmelade::generate_header(MsgHeader &header_, std::vector<uint8_t> &header_b)
{
    std::vector<uint8_t> msg_type(1);
    std::vector<uint8_t> msg_no(2);
    std::vector<uint8_t> payload_size(4);

    to_bytes(msg_type, header_.msg_type);
    to_bytes(msg_no, header_.msg_no);
    to_bytes(payload_size, header_.payload_size);

    header_b.insert(header_b.end(), msg_type.begin(), msg_type.end());
    header_b.insert(header_b.end(), msg_no.begin(), msg_no.end());
    header_b.insert(header_b.end(), payload_size.begin(), payload_size.end());
}
