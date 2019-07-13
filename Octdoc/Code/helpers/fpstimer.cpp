#include "fpstimer.h"

namespace octdoc
{
	namespace hlp
	{
		FPSTimer::FPSTimer() :
			m_frameTimes(nullptr),
			m_maxFrameCount(0),
			m_frameCount(0),
			m_lastIndex(0) {}

		FPSTimer::~FPSTimer()
		{
			if (m_frameTimes)
				delete[] m_frameTimes;
		}

		void FPSTimer::SetMaxFrameCount(int maxFrameCount)
		{
			if (m_frameTimes)
				delete[] m_frameTimes;
			m_frameTimes = maxFrameCount == 0 ? nullptr : new double[maxFrameCount];
			m_maxFrameCount = maxFrameCount;
			m_frameCount = 0;
			m_lastIndex = 0;
		}

		void FPSTimer::Update(double frameTime)
		{
			if (m_maxFrameCount > 0)
			{
				if (m_frameCount < m_maxFrameCount)
				{
					m_frameTimes[m_frameCount++] = frameTime;
				}
				else
				{
					m_frameTimes[m_lastIndex] = frameTime;
					m_lastIndex = (m_lastIndex + 1) % m_maxFrameCount;
				}
			}
		}
		double FPSTimer::GetFPS()
		{
			double timeSum = 0.0;
			for (int i = 0; i < m_frameCount; i++)
				timeSum += m_frameTimes[i];
			return m_frameCount / timeSum;
		}
	}
}