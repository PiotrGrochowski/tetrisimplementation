#include <windows.h>
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cstdint> // use the C++ header
#include <chrono>  // std::chrono::time_point
#include <ratio>   // std::ratio
#include <thread>  // std::this_thread::sleep_until

template<std::intmax_t FPS>
class frame_rater {
public:
    frame_rater() :                 // initialize the object keeping the pace
        time_between_frames{ 1 },   // std::ratio<1, FPS> seconds
        tp{ std::chrono::steady_clock::now() }
    {}

    void sleep() {
        // add to time point
        tp += time_between_frames;

        // and sleep until that time point
        std::this_thread::sleep_until(tp);
    }

private:
    // a duration with a length of 1/FPS seconds
    std::chrono::duration<long double, std::ratio<1, FPS>> time_between_frames;

    // the time point we'll add to in every loop
    std::chrono::time_point<std::chrono::steady_clock, decltype(time_between_frames)> tp;
};

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD start = {0, 0};
    DWORD b = 0;
    CHAR_INFO screen[2000];
    for (int i=0; i<2000; i++){
        screen[i].Char.AsciiChar=32;
        screen[i].Attributes = 7;
    }
    for (int i=0; i<19; i++){
        screen[i].Char.AsciiChar="TETRIS COLOR PICKER"[i];
    }
    for (int i=0; i<23; i++){
        screen[i+480].Char.AsciiChar="Select background color"[i];
    }
    for (int i=0; i<64; i++){
        screen[i+560].Char.AsciiChar=219;
        screen[i+640].Char.AsciiChar=219;
        screen[i+960].Char.AsciiChar=219;
        screen[i+1040].Char.AsciiChar=219;
    }
    for (int i=0; i<64; i++){
        screen[i+560].Attributes=i/4;
        screen[i+640].Attributes=i/4;
        screen[i+960].Attributes=i/4;
        screen[i+1040].Attributes=i/4;
    }
    for (int i=0; i<23; i++){
        screen[i+880].Char.AsciiChar="Select foreground color"[i];
    }
    for (int i=0; i<12; i++){
        screen[i+1280].Char.AsciiChar="Select shade"[i];
    }
    for (int i=0; i<4; i++){
        screen[i+1360].Char.AsciiChar=32;
        screen[i+1440].Char.AsciiChar=32;
    }
    for (int i=4; i<8; i++){
        screen[i+1360].Char.AsciiChar=176;
        screen[i+1440].Char.AsciiChar=176;
    }
    for (int i=8; i<12; i++){
        screen[i+1360].Char.AsciiChar=177;
        screen[i+1440].Char.AsciiChar=177;
    }
    for (int i=12; i<16; i++){
        screen[i+1360].Char.AsciiChar=178;
        screen[i+1440].Char.AsciiChar=178;
    }
    for (int i=16; i<20; i++){
        screen[i+1360].Char.AsciiChar=219;
        screen[i+1440].Char.AsciiChar=219;
    }
    for (int i=0; i<80; i++){
        screen[i+320].Attributes=31;
    }
    uint8_t row = 0;
    uint8_t flag = 0;
    uint8_t o = 0;
    uint8_t x1 = 0;
    uint8_t x2[7] = {0, 0, 0, 0, 0, 0, 0};
    uint8_t x3[7] = {7, 7, 7, 7, 7, 7, 7};
    uint8_t x4[7] = {2, 2, 2, 2, 2, 2, 2};
    CHAR_INFO pieces[7];
    for (int i=0; i<7; i++){
        pieces[i].Char.AsciiChar=177;
        pieces[i].Attributes = 7;
    }
    for (int i=0; i<4; i++){
        screen[320+i+x1*4].Char.AsciiChar=94;
    }
    for (int i=0; i<4; i++){
        screen[720+i+x2[x1]*4].Char.AsciiChar=94;
    }
    for (int i=0; i<4; i++){
        screen[1120+i+x3[x1]*4].Char.AsciiChar=94;
    }
    for (int i=0; i<4; i++){
        screen[1520+i+x4[x1]*4].Char.AsciiChar=94;
    }
    char grays[5];
    grays[0]=32; grays[1]=176; grays[2]=177; grays[3]=178; grays[4]=219;
    for (int i=0; i<28; i++){
        screen[i+160].Attributes=x2[i/4]*16+x3[i/4];
        screen[i+240].Attributes=x2[i/4]*16+x3[i/4];
        screen[i+160].Char.AsciiChar=grays[x4[i/4]];
        screen[i+240].Char.AsciiChar=grays[x4[i/4]];
    }
    SMALL_RECT testscreenrect;
    testscreenrect.Left = 0;
    testscreenrect.Top = 0;
    testscreenrect.Right = 79;
    testscreenrect.Bottom = 24;
    COORD testscreensize = {80, 25};
    COORD teststart = {0, 0};
    uint8_t keys[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    WriteConsoleOutput(hConsole, screen, testscreensize, teststart, &testscreenrect);
        frame_rater<60> fr;
    while (-1) {
        testscreenrect.Left = 0;
        testscreenrect.Top = 0;
        testscreenrect.Right = 79;
        testscreenrect.Bottom = 24;
        teststart = {0, 0};
        if ((uint16_t(GetAsyncKeyState(38)) / 32768) != keys[0]){
            keys[0] ^= 1;
            if (keys[0] == 1){
                if (o < 4 && o != 0){
                    for (int i=0; i<80; i++){
                        screen[i+320+(o*400)].Attributes=7;
                    }
                    o--;
                    for (int i=0; i<80; i++){
                        screen[i+320+(o*400)].Attributes=31;
                    }
                }
            }
        }
        if ((uint16_t(GetAsyncKeyState(40)) / 32768) != keys[1]){
            keys[1] ^= 1;
            if (keys[1] == 1){
                if (o < 3){
                    for (int i=0; i<80; i++){
                        screen[i+320+(o*400)].Attributes=7;
                    }
                    o++;
                    for (int i=0; i<80; i++){
                        screen[i+320+(o*400)].Attributes=31;
                    }
                }
            }
        }
        if ((uint16_t(GetAsyncKeyState(37)) / 32768) != keys[2]){
            keys[2] ^= 1;
            if (keys[2] == 1){
                if (o == 0){
                    if (x1 < 7 && x1 != 0){
                        for (int i=0; i<4; i++){
                            screen[320+i+x1*4].Char.AsciiChar=32;
                        }
                        for (int i=0; i<4; i++){
                            screen[720+i+x2[x1]*4].Char.AsciiChar=32;
                        }
                        for (int i=0; i<4; i++){
                            screen[1120+i+x3[x1]*4].Char.AsciiChar=32;
                        }
                        for (int i=0; i<4; i++){
                            screen[1520+i+x4[x1]*4].Char.AsciiChar=32;
                        }
                        x1--;
                        for (int i=0; i<4; i++){
                            screen[320+i+x1*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<4; i++){
                            screen[720+i+x2[x1]*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<4; i++){
                            screen[1120+i+x3[x1]*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<4; i++){
                            screen[1520+i+x4[x1]*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<20; i++){
                            screen[i+1360].Attributes=x2[x1]*16+x3[x1];
                            screen[i+1440].Attributes=x2[x1]*16+x3[x1];
                        }
                    }
                }
                if (o == 1){
                    if (x2[x1] < 16 && x2[x1] != 0){
                        for (int i=0; i<4; i++){
                            screen[720+i+x2[x1]*4].Char.AsciiChar=32;
                        }
                        x2[x1]--;
                        for (int i=0; i<4; i++){
                            screen[720+i+x2[x1]*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<4; i++){
                            screen[i+160+x1*4].Attributes=x2[x1]*16+x3[x1];
                            screen[i+240+x1*4].Attributes=x2[x1]*16+x3[x1];
                        }
                        for (int i=0; i<20; i++){
                            screen[i+1360].Attributes=x2[x1]*16+x3[x1];
                            screen[i+1440].Attributes=x2[x1]*16+x3[x1];
                        }
                    }
                }
                if (o == 2){
                    if (x3[x1] < 16 && x3[x1] != 0){
                        for (int i=0; i<4; i++){
                            screen[1120+i+x3[x1]*4].Char.AsciiChar=32;
                        }
                        x3[x1]--;
                        for (int i=0; i<4; i++){
                            screen[1120+i+x3[x1]*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<4; i++){
                            screen[i+160+x1*4].Attributes=x2[x1]*16+x3[x1];
                            screen[i+240+x1*4].Attributes=x2[x1]*16+x3[x1];
                        }
                        for (int i=0; i<20; i++){
                            screen[i+1360].Attributes=x2[x1]*16+x3[x1];
                            screen[i+1440].Attributes=x2[x1]*16+x3[x1];
                        }
                    }
                }
                if (o == 3){
                    if (x4[x1] < 5 && x4[x1] != 0){
                        for (int i=0; i<4; i++){
                            screen[1520+i+x4[x1]*4].Char.AsciiChar=32;
                        }
                        x4[x1]--;
                        for (int i=0; i<4; i++){
                            screen[1520+i+x4[x1]*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<4; i++){
                            screen[i+160+x1*4].Char.AsciiChar=grays[x4[x1]];
                            screen[i+240+x1*4].Char.AsciiChar=grays[x4[x1]];
                        }
                    }
                }
            }
        }
        if ((uint16_t(GetAsyncKeyState(39)) / 32768) != keys[3]){
            keys[3] ^= 1;
            if (keys[3] == 1){
                if (o == 0){
                    if (x1 < 6){
                        for (int i=0; i<4; i++){
                            screen[320+i+x1*4].Char.AsciiChar=32;
                        }
                        for (int i=0; i<4; i++){
                            screen[720+i+x2[x1]*4].Char.AsciiChar=32;
                        }
                        for (int i=0; i<4; i++){
                            screen[1120+i+x3[x1]*4].Char.AsciiChar=32;
                        }
                        for (int i=0; i<4; i++){
                            screen[1520+i+x4[x1]*4].Char.AsciiChar=32;
                        }
                        x1++;
                        for (int i=0; i<4; i++){
                            screen[320+i+x1*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<4; i++){
                            screen[720+i+x2[x1]*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<4; i++){
                            screen[1120+i+x3[x1]*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<4; i++){
                            screen[1520+i+x4[x1]*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<20; i++){
                            screen[i+1360].Attributes=x2[x1]*16+x3[x1];
                            screen[i+1440].Attributes=x2[x1]*16+x3[x1];
                        }
                    }
                }
                if (o == 1){
                    if (x2[x1] < 15){
                        for (int i=0; i<4; i++){
                            screen[720+i+x2[x1]*4].Char.AsciiChar=32;
                        }
                        x2[x1]++;
                        for (int i=0; i<4; i++){
                            screen[720+i+x2[x1]*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<4; i++){
                            screen[i+160+x1*4].Attributes=x2[x1]*16+x3[x1];
                            screen[i+240+x1*4].Attributes=x2[x1]*16+x3[x1];
                        }
                        for (int i=0; i<20; i++){
                            screen[i+1360].Attributes=x2[x1]*16+x3[x1];
                            screen[i+1440].Attributes=x2[x1]*16+x3[x1];
                        }
                    }
                }
                if (o == 2){
                    if (x3[x1] < 15){
                        for (int i=0; i<4; i++){
                            screen[1120+i+x3[x1]*4].Char.AsciiChar=32;
                        }
                        x3[x1]++;
                        for (int i=0; i<4; i++){
                            screen[1120+i+x3[x1]*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<4; i++){
                            screen[i+160+x1*4].Attributes=x2[x1]*16+x3[x1];
                            screen[i+240+x1*4].Attributes=x2[x1]*16+x3[x1];
                        }
                        for (int i=0; i<20; i++){
                            screen[i+1360].Attributes=x2[x1]*16+x3[x1];
                            screen[i+1440].Attributes=x2[x1]*16+x3[x1];
                        }
                    }
                }
                if (o == 3){
                    if (x4[x1] < 4){
                        for (int i=0; i<4; i++){
                            screen[1520+i+x4[x1]*4].Char.AsciiChar=32;
                        }
                        x4[x1]++;
                        for (int i=0; i<4; i++){
                            screen[1520+i+x4[x1]*4].Char.AsciiChar=94;
                        }
                        for (int i=0; i<4; i++){
                            screen[i+160+x1*4].Char.AsciiChar=grays[x4[x1]];
                            screen[i+240+x1*4].Char.AsciiChar=grays[x4[x1]];
                        }
                    }
                }
            }
        }
        WriteConsoleOutput(hConsole, screen, testscreensize, teststart, &testscreenrect);
        fr.sleep();  // sleeps the remaining part of 1/60 second
    }
    //long double t = 1;
    //t /= 3;
    //std::cout << std::fixed << std::setprecision(std::numeric_limits<long double>::digits10 + 1) << t;
    return 0;
}
