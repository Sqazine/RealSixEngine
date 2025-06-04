#include "Camera.hpp"
#include "Renderer.hpp"
namespace RealSix
{
    Camera::Camera()
    {
        InitializeDefault(Vector3f::UNIT_Z, Vector3f::ZERO);
    }

    Camera::Camera(const Vector3f &position, const Vector3f &target)
    {
        InitializeDefault(position, target);
    }

    Camera *Camera::SetPosition(const Vector3f &position)
    {
        mPosition = position;
        UpdateCoordinateSystem();
        UpdateViewMatrix();
        return this;
    }

    Camera *Camera::SetTarget(const Vector3f &target)
    {
        mTarget = target;
        UpdateCoordinateSystem();
        UpdateViewMatrix();
        return this;
    }

    Camera *Camera::SetFovByRadian(float fov)
    {
        mFov = fov;
        UpdateProjectionMatrix();
        return this;
    }

    Camera *Camera::SetFovByDegree(float fov)
    {
        mFov = Math::ToRadian(fov);
        UpdateProjectionMatrix();
        return this;
    }

    Camera *Camera::SetAspect(float aspect)
    {
        mAspect = aspect;
        UpdateProjectionMatrix();
        return this;
    }

    Camera *Camera::SetZNear(float zNear)
    {
        mZNear = zNear;
        UpdateProjectionMatrix();
        return this;
    }

    Camera *Camera::SetZFar(float zFar)
    {
        mZFar = zFar;
        UpdateProjectionMatrix();
        return this;
    }

    void Camera::InitializeDefault(const Vector3f &position, const Vector3f &target)
    {
        CreateRenderDataBuffer();
        SetPosition(position);
        SetTarget(target);
        SetFovByDegree(60.0f);
        SetAspect(Renderer::GetGfxDevice()->GetSwapChain()->GetAspect());
        SetZNear(0.1f);
        SetZFar(1000.0f);
    }

    void Camera::CreateRenderDataBuffer()
    {
        GfxBufferDesc desc{};
        desc.bufferSize = sizeof(CameraRenderData);
        desc.elementSize = sizeof(CameraRenderData);
        desc.data = &mRenderData;

        mRenderDataBuffer.reset(GfxUniformBuffer::Create(Renderer::GetGfxDevice(), desc));
    }

    void Camera::UpdateViewMatrix()
    {
        mRenderData.viewMatrix = Matrix4f::LookAt(mPosition, mTarget, mUp);
        UpdateRenderData();
    }

    void Camera::UpdateProjectionMatrix()
    {
        mRenderData.projectionMatrix = Matrix4f::Perspective(mFov, mAspect, mZNear, mZFar);
        UpdateRenderData();
    }

    void Camera::UpdateCoordinateSystem()
    {
        mFront = Vector3f::Normalize(mTarget - mPosition);
        mRight = Vector3f::Normalize(Vector3f::Cross(mFront, Vector3f(0.0f, 1.0f, 0.0f)));
        mUp = Vector3f::Normalize(Vector3f::Cross(mRight, mFront));
    }

    void Camera::UpdateRenderData()
    {
        GfxBufferDesc desc{};
        desc.bufferSize = sizeof(CameraRenderData);
        desc.elementSize = sizeof(CameraRenderData);
        desc.data = &mRenderData;
        mRenderDataBuffer->SetData(desc);
    }
}