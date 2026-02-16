#ifndef RESOLUTION_H
#define RESOLUTION_H

#include "globals.h"
#include <string>
#include <vector>

enum class AspectRatio
{
    Ratio_16_9,
    Ratio_16_10,
    Ratio_4_3,
    Ratio_21_9,
    Ratio_1_1
};

struct Resolution
{
    unsigned int width;
    unsigned int height;
    AspectRatio aspectRatio;
    std::string displayName;

    Resolution(unsigned int w, unsigned int h, AspectRatio ar, const std::string& name)
        : width(w), height(h), aspectRatio(ar), displayName(name)
    {
    }

    float getAspectRatioValue() const
    {
        return static_cast<float>(width) / static_cast<float>(height);
    }
};

class ResolutionManager
{
    private:
        std::vector<Resolution> m_availableResolutions;
        size_t m_currentResolutionIndex;

    public:
        ResolutionManager()
            : m_currentResolutionIndex(0)
        {
            // 16:9 resolutions
            m_availableResolutions.emplace_back(1920, 1080, AspectRatio::Ratio_16_9, "1920x1080");

            // 16:10 resolutions
            // m_availableResolutions.emplace_back(1920, 1200, AspectRatio::Ratio_16_10, "1920x1200");
            // m_availableResolutions.emplace_back(1680, 1050, AspectRatio::Ratio_16_10, "1680x1050");
            m_availableResolutions.emplace_back(1440, 900, AspectRatio::Ratio_16_10, "1440x900");

            // 4:3 resolutions
            // m_availableResolutions.emplace_back(1600, 1200, AspectRatio::Ratio_4_3, "1600x1200");
            // m_availableResolutions.emplace_back(1280, 1024, AspectRatio::Ratio_4_3, "1280x1024");
            m_availableResolutions.emplace_back(1024, 768, AspectRatio::Ratio_4_3, "1024x768");

            // 21:9 ultrawide
            // m_availableResolutions.emplace_back(2560, 1080, AspectRatio::Ratio_21_9, "2560x1080");
            // m_availableResolutions.emplace_back(3440, 1440, AspectRatio::Ratio_21_9, "3440x1440");

            // 1:1 Square
            m_availableResolutions.emplace_back(900, 900, AspectRatio::Ratio_1_1, "900x900");
            m_availableResolutions.emplace_back(500, 500, AspectRatio::Ratio_1_1, "500x500");
        }

        const Resolution& getCurrentResolution() const
        {
            return m_availableResolutions[m_currentResolutionIndex];
        }

        const std::vector<Resolution>& getAllResolutions() const
        {
            return m_availableResolutions;
        }

        void setResolution(size_t index)
        {
            if(index < m_availableResolutions.size())
            {
                m_currentResolutionIndex = index;
                windowWidth = getCurrentResolution().width;
                windowHeight = getCurrentResolution().height;
                viewWidth = windowWidth;
                viewHeight = windowHeight;
            }
        }

        void setResolution(unsigned int width, unsigned int height)
        {
            for(size_t i = 0; i < m_availableResolutions.size(); i++)
            {
                if(m_availableResolutions[i].width == width && m_availableResolutions[i].height == height)
                {
                    m_currentResolutionIndex = i;
                    return;
                }
            }
        }

        size_t getCurrentIndex() const
        {
            return m_currentResolutionIndex;
        }
};

#endif
