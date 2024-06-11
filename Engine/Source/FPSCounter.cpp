#include "../Include/Jett Engine/FPSCounter.h"

void FPSCounter::FrameRendered()
{
    
        m_frameCount++;

        // Calculate time elapsed since the start
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_startTime).count();

        // If 1 second has passed, calculate FPS and reset frame count
        if (elapsedTime >= 1000) {
            this->fps = static_cast<float>(m_frameCount) / (elapsedTime / 1000.0f);
            m_frameCount = 0;
            m_startTime = currentTime;
        }
    
}

float FPSCounter::getFps() const
{
    return this->fps;
}
