#pragma once
#include <iostream>
#include <chrono>
//credit for fps counter. generated through chat gpt
class FPSCounter
{
private:
    std::chrono::steady_clock::time_point m_startTime;
    int m_frameCount;
    float fps = 0;

public:
    FPSCounter() : m_frameCount(0) {
        m_startTime = std::chrono::steady_clock::now();
    }

    void FrameRendered();

    float getFps() const;
};

