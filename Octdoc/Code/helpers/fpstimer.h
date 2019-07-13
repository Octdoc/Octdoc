#pragma once

namespace octdoc
{
	namespace hlp
	{
		class FPSTimer
		{
			double *m_frameTimes;
			int m_maxFrameCount;
			int m_frameCount;
			int m_lastIndex;

		public:
			FPSTimer();
			~FPSTimer();
			void SetMaxFrameCount(int maxFrameCount);
			void Update(double frameTime);
			double GetFPS();
		};
	}
}