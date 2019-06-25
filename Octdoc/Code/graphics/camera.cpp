#include "camera.h"

namespace octdoc
{
	namespace gfx
	{
		void Camera::SetProjectionMatrix()
		{
			m_projectionMatrix = mth::float4x4::PerspectiveFOV(m_fieldOfView, m_screenAspect, m_screenNear, m_screenDepth);
		}
		Camera::Camera(float screenAspect, float fov, float screenNear, float screenDepth) :
			m_screenAspect(screenAspect),
			m_fieldOfView(fov),
			m_screenNear(screenNear),
			m_screenDepth(screenDepth)
		{
			SetProjectionMatrix();
		}
		void Camera::SetFOV(float fov)
		{
			m_fieldOfView = fov;
			SetProjectionMatrix();
		}
		float Camera::getFOV()
		{
			return m_fieldOfView;
		}
		void Camera::SetScreenAspect(float screenAspect)
		{
			m_screenAspect = screenAspect;
			SetProjectionMatrix();
		}
		void Camera::SetParams(float screenAspect, float fov, float screenNear, float screenDepth)
		{
			m_screenAspect = screenAspect;
			m_fieldOfView = fov;
			m_screenNear = screenNear;
			m_screenDepth = screenDepth;
			SetProjectionMatrix();
		}
		void Camera::Update()
		{
			m_viewMatrix = mth::float4x4::RotationCamera(position, rotation);
		}
		mth::float4x4 Camera::getViewMatrix()
		{
			return m_viewMatrix;
		}
		mth::float4x4 Camera::getProjectionMatrix()
		{
			return m_projectionMatrix;
		}
		mth::float4x4 Camera::GetCameraMatrix()
		{
			return m_projectionMatrix * m_viewMatrix;
		}
	}
}