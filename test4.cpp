#include <windows.h>
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <fstream>
#include <cstdint> // use the C++ header
#include <chrono>  // std::chrono::time_point
#include <ratio>   // std::ratio
#include <thread>  // std::this_thread::sleep_until

//..............................................................................

/* This part of code credits to Ted Lyngmo from stack overflow:
https://stackoverflow.com/a/59442868/5483294
License: "Anything published on Stackoverflow better be totally free and
everything I've ever written definitely is."
vvvvv */

template<uint64_t FPS>
class frame_rater {
public:
    frame_rater() :
        // initialize the object keeping the pace - casting needed
        tp{std::chrono::time_point_cast<decltype(time_between_frames)>(
            std::chrono::steady_clock::now())}
    {}

    void sleep() {
        // add to time point
        tp += time_between_frames;

        // and sleep until that time point
        std::this_thread::sleep_until(tp);
    }

private:
    // a duration with a length of 1/FPS seconds
    static std::chrono::duration<uint64_t, std::ratio<1, FPS>>
        time_between_frames;

    // the time point we'll add to in every loop
    std::chrono::time_point<std::chrono::steady_clock, decltype(time_between_frames)> tp;
};

template<uint64_t FPS>
std::chrono::duration<uint64_t, std::ratio<1, FPS>>
    frame_rater<FPS>::time_between_frames{1};

/* ^^^^^ This part of code credits to Ted Lyngmo from stack overflow:
https://stackoverflow.com/a/59442868/5483294
License: "Anything published on Stackoverflow better be totally free and
everything I've ever written definitely is."
*/

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
    for (int i=0; i<10; i++){
        screen[i+162].Char.AsciiChar="Foreground"[i];
    }
    for (int i=0; i<23; i++){
        screen[i+560].Char.AsciiChar="Select background color"[i];
    }
    for (int i=0; i<32; i++){
        screen[i+640].Char.AsciiChar=219;
        screen[i+960].Char.AsciiChar=219;
    }
    for (int i=0; i<32; i++){
        screen[i+640].Attributes=i/2;
        screen[i+960].Attributes=i/2;
    }
    for (int i=0; i<23; i++){
        screen[i+880].Char.AsciiChar="Select foreground color"[i];
    }
    for (int i=0; i<12; i++){
        screen[i+1200].Char.AsciiChar="Select shade"[i];
    }
    for (int i=0; i<11; i++){
        screen[i+1680].Char.AsciiChar="Save & Quit"[i];
    }
    char grays[5];
    grays[0]=32; grays[1]=176; grays[2]=177; grays[3]=178; grays[4]=219;
    for (int i=0; i<10; i++){
        screen[i+1280].Char.AsciiChar=grays[i/2];
    }
    for (int i=0; i<80; i++){
        screen[i+400].Attributes=31;
    }
    uint8_t row = 0;
    uint8_t flag = 0;
    uint8_t o = 0;
    uint8_t x1 = 0;
    uint8_t x2[7] ;
    uint8_t x3[7] ;
    uint8_t x4[7] ;
    std::fstream save;
    save.open("savefile", std::ios::in|std::ios::binary);
    save.seekg (0, save.end);
    uint64_t length = save.tellg();
    save.seekg (0, save.beg);
    char load[length];
    save.read(load,length);
    save.close();
    CHAR_INFO pieces[7];
    for (int i=0; i<7; i++){
        pieces[i].Char.AsciiChar = uint8_t(load[i*2+1]);
        pieces[i].Attributes = uint8_t(load[i*2]);
    }
    for (int i=0; i<7; i++){
        x2[i]=pieces[i].Attributes / 16;
        x3[i]=pieces[i].Attributes % 16;
        x4[i]=0;
        for (int q=0; q<5; q++){
            if (pieces[i].Char.AsciiChar == grays[q]){
                x4[i]=q;
            }
        }
    }
    for (int i=0; i<2; i++){
        screen[400+i+x1*2].Char.AsciiChar=94;
    }
    for (int i=0; i<2; i++){
        screen[720+i+x2[x1]*2].Char.AsciiChar=94;
    }
    for (int i=0; i<2; i++){
        screen[1040+i+x3[x1]*2].Char.AsciiChar=94;
    }
    for (int i=0; i<2; i++){
        screen[1360+i+x4[x1]*2].Char.AsciiChar=94;
    }
    for (int i=0; i<14; i++){
        screen[i+320].Attributes=x2[i/2]*16+x3[i/2];
        screen[i+320].Char.AsciiChar=grays[x4[i/2]];
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
    uint8_t run = -1;
        frame_rater<60> fr;
    while (run) {
        testscreenrect.Left = 0;
        testscreenrect.Top = 0;
        testscreenrect.Right = 79;
        testscreenrect.Bottom = 24;
        teststart = {0, 0};
        if ((uint16_t(GetAsyncKeyState(38)) / 32768) != keys[0]){
            keys[0] ^= 1;
            if (keys[0] == 1){
                if (o < 5 && o != 0){
                    for (int i=0; i<80; i++){
                        screen[i+400+(o*320)].Attributes=7;
                    }
                    o--;
                    for (int i=0; i<80; i++){
                        screen[i+400+(o*320)].Attributes=31;
                    }
                }
            }
        }
        if ((uint16_t(GetAsyncKeyState(40)) / 32768) != keys[1]){
            keys[1] ^= 1;
            if (keys[1] == 1){
                if (o < 4){
                    for (int i=0; i<80; i++){
                        screen[i+400+(o*320)].Attributes=7;
                    }
                    o++;
                    for (int i=0; i<80; i++){
                        screen[i+400+(o*320)].Attributes=31;
                    }
                }
            }
        }
        if ((uint16_t(GetAsyncKeyState(37)) / 32768) != keys[2]){
            keys[2] ^= 1;
            if (keys[2] == 1){
                if (o == 0){
                    if (x1 < 7 && x1 != 0){
                        for (int i=0; i<2; i++){
                            screen[400+i+x1*2].Char.AsciiChar=32;
                        }
                        for (int i=0; i<2; i++){
                            screen[720+i+x2[x1]*2].Char.AsciiChar=32;
                        }
                        for (int i=0; i<2; i++){
                            screen[1040+i+x3[x1]*2].Char.AsciiChar=32;
                        }
                        for (int i=0; i<2; i++){
                            screen[1360+i+x4[x1]*2].Char.AsciiChar=32;
                        }
                        x1--;
                        for (int i=0; i<2; i++){
                            screen[400+i+x1*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<2; i++){
                            screen[720+i+x2[x1]*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<2; i++){
                            screen[1040+i+x3[x1]*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<2; i++){
                            screen[1360+i+x4[x1]*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<10; i++){
                            screen[i+1280].Attributes=x2[x1]*16+x3[x1];
                        }
                    }
                }
                if (o == 1){
                    if (x2[x1] < 16 && x2[x1] != 0){
                        for (int i=0; i<2; i++){
                            screen[720+i+x2[x1]*2].Char.AsciiChar=32;
                        }
                        x2[x1]--;
                        for (int i=0; i<2; i++){
                            screen[720+i+x2[x1]*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<2; i++){
                            screen[i+320+x1*2].Attributes=x2[x1]*16+x3[x1];
                        }
                        for (int i=0; i<10; i++){
                            screen[i+1280].Attributes=x2[x1]*16+x3[x1];
                        }
                    }
                }
                if (o == 2){
                    if (x3[x1] < 16 && x3[x1] != 0){
                        for (int i=0; i<2; i++){
                            screen[1040+i+x3[x1]*2].Char.AsciiChar=32;
                        }
                        x3[x1]--;
                        for (int i=0; i<2; i++){
                            screen[1040+i+x3[x1]*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<2; i++){
                            screen[i+320+x1*2].Attributes=x2[x1]*16+x3[x1];
                        }
                        for (int i=0; i<10; i++){
                            screen[i+1280].Attributes=x2[x1]*16+x3[x1];
                        }
                    }
                }
                if (o == 3){
                    if (x4[x1] < 5 && x4[x1] != 0){
                        for (int i=0; i<2; i++){
                            screen[1360+i+x4[x1]*2].Char.AsciiChar=32;
                        }
                        x4[x1]--;
                        for (int i=0; i<2; i++){
                            screen[1360+i+x4[x1]*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<2; i++){
                            screen[i+320+x1*2].Char.AsciiChar=grays[x4[x1]];
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
                        for (int i=0; i<2; i++){
                            screen[400+i+x1*2].Char.AsciiChar=32;
                        }
                        for (int i=0; i<2; i++){
                            screen[720+i+x2[x1]*2].Char.AsciiChar=32;
                        }
                        for (int i=0; i<2; i++){
                            screen[1040+i+x3[x1]*2].Char.AsciiChar=32;
                        }
                        for (int i=0; i<2; i++){
                            screen[1360+i+x4[x1]*2].Char.AsciiChar=32;
                        }
                        x1++;
                        for (int i=0; i<2; i++){
                            screen[400+i+x1*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<2; i++){
                            screen[720+i+x2[x1]*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<2; i++){
                            screen[1040+i+x3[x1]*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<2; i++){
                            screen[1360+i+x4[x1]*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<10; i++){
                            screen[i+1280].Attributes=x2[x1]*16+x3[x1];
                        }
                    }
                }
                if (o == 1){
                    if (x2[x1] < 15){
                        for (int i=0; i<2; i++){
                            screen[720+i+x2[x1]*2].Char.AsciiChar=32;
                        }
                        x2[x1]++;
                        for (int i=0; i<2; i++){
                            screen[720+i+x2[x1]*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<2; i++){
                            screen[i+320+x1*2].Attributes=x2[x1]*16+x3[x1];
                        }
                        for (int i=0; i<10; i++){
                            screen[i+1280].Attributes=x2[x1]*16+x3[x1];
                        }
                    }
                }
                if (o == 2){
                    if (x3[x1] < 15){
                        for (int i=0; i<2; i++){
                            screen[1040+i+x3[x1]*2].Char.AsciiChar=32;
                        }
                        x3[x1]++;
                        for (int i=0; i<2; i++){
                            screen[1040+i+x3[x1]*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<2; i++){
                            screen[i+320+x1*2].Attributes=x2[x1]*16+x3[x1];
                        }
                        for (int i=0; i<10; i++){
                            screen[i+1280].Attributes=x2[x1]*16+x3[x1];
                        }
                    }
                }
                if (o == 3){
                    if (x4[x1] < 4){
                        for (int i=0; i<2; i++){
                            screen[1360+i+x4[x1]*2].Char.AsciiChar=32;
                        }
                        x4[x1]++;
                        for (int i=0; i<2; i++){
                            screen[1360+i+x4[x1]*2].Char.AsciiChar=94;
                        }
                        for (int i=0; i<2; i++){
                            screen[i+320+x1*2].Char.AsciiChar=grays[x4[x1]];
                        }
                    }
                }
            }
        }
        if ((uint16_t(GetAsyncKeyState(13)) / 32768) != keys[7]){
            keys[7] ^= 1;
            if (keys[7] == 1){
                if (o == 4){
                    char saving[14];
                    for (int i = 0; i < 7; i++){
                        saving[2*i+1] = grays[x4[i]];
                        saving[2*i] = x2[i]*16+x3[i];
                    }
                    save.open("savefile", std::ios::out|std::ios::binary);
                    save.write(saving, 14);
                    save.close();
                    run = 0;
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
