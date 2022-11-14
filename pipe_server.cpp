#include <windows.h> 
#include <stdio.h>
#include <iostream>
#include <vector>

#include "opencv2/ximgproc.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "marmelade.h" 


void draw(struct MsgHeader &header, unsigned char buffer[])
{
    std::cout << (int)header.msg_type << std::endl;
    std::cout << header.msg_no << std::endl;
    std::cout << header.payload_size << std::endl;

    uint16_t rows = 0;
    uint16_t cols = 0;
    std::memcpy(&rows, buffer+7, 2);
    std::memcpy(&cols, buffer+9, 2);

    std::cout << rows << " x " << cols << std::endl;

    // unsigned char* img_buffer = new unsigned char[rows*cols];
    // std::memcpy

    cv::Mat image(rows, cols, CV_8UC1, buffer+11);
    cv::namedWindow("Image from Char Array", CV_WINDOW_AUTOSIZE);
    if (!(image.empty()))
    {
        cv::imshow("Image from Char Array", image);
    }
    cv::waitKey(0);
}


int main(void)
{
    Marmelade marmelade = Marmelade();

    HANDLE hPipe;
    DWORD buffer_size = 2500000;
    unsigned char* buffer = new unsigned char[buffer_size];

    DWORD dwRead;
    DWORD dwWrite;
    size_t c = 0;
    int daty[] = {1, 2, 3, 4, 5};
 
    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Foo"),
                            PIPE_ACCESS_DUPLEX,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
                            1,
                            1024 * 64,
                            1024 * 64,
                            NMPWAIT_USE_DEFAULT_WAIT,
                            NULL);
    while (hPipe != INVALID_HANDLE_VALUE)
    {
        // std::cout << "Test 0" << std::endl;
        if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
        {
            std::cout << "Test 1" << std::endl;
            if (ReadFile(hPipe, buffer, buffer_size, &dwRead, NULL) != FALSE)
            {
                std::cout << "Test 2" << std::endl;

                // MsgHeader header;
                // std::memcpy(&header.msg_type, buffer, 1);
                // std::memcpy(&header.msg_no, buffer+1, 2);
                // std::memcpy(&header.payload_size, buffer+3, 4);
                // draw(header, buffer);
                std::vector<uint8_t> result;
                marmelade.get_detector_response(result, buffer, static_cast<uint32_t>(dwRead));
                for (int i=0; i<100; i++)
                {
                    std::cout << std::hex << static_cast<int>(result[i]) << ":";
                }
                std::cout << "\n";

                WriteFile(hPipe, result.data(), static_cast<DWORD>(result.size()), &dwWrite, NULL);
 
                // std::cout << image << std::endl;
                // cv::imwrite("Image.png", image);
                // WriteFile(hPipe, &c, sizeof(c), &dwWrite, NULL);
                // std::cout << c << std::endl;
                // WriteFile(hPipe, daty, sizeof(daty), &dwWrite, NULL);
                // std::cout << daty << std::endl;
                c++;
                std::cout << "Test 4" << std::endl;
            }
            std::cout << "Test 5" << std::endl;
        }
        // std::cout << "Test 6" << std::endl;
        DisconnectNamedPipe(hPipe);
    }
    
    return 0;
}
