#pragma once
#include <memory>
#include "Math/Vector3.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Math.hpp"
#include "Gfx/IGfxBuffer.hpp"

namespace RealSix
{
    class Camera
    {
    public:
        Camera();
        Camera(const Vector3f &position, const Vector3f &target);
        virtual ~Camera() = default;

        Camera *SetPosition(const Vector3f &position);
        Vector3f GetPosition() const { return mPosition; }

        Camera *SetTarget(const Vector3f &target);
        Vector3f GetTarget() const { return mTarget; }

        Camera *SetFovByRadian(float fov);
        Camera *SetFovByDegree(float fov);
        Camera *SetAspect(float aspect);
        Camera *SetZNear(float zNear);
        Camera *SetZFar(float zFar);

        const Matrix4f &GetViewMatrix() const { return mRenderData.viewMatrix; }
        const Matrix4f &GetProjectionMatrix() const { return mRenderData.projectionMatrix; }

        const GfxUniformBuffer *GetRenderDataBuffer() const { return mRenderDataBuffer.get(); }
    protected:
        void InitializeDefault(const Vector3f &position, const Vector3f &target);

        struct CameraRenderData
        {
            alignas(16) Matrix4f viewMatrix;
            alignas(16) Matrix4f projectionMatrix;
        };
        CameraRenderData mRenderData;

        std::unique_ptr<GfxUniformBuffer> mRenderDataBuffer;

        // ViewMatrix Data
        Vector3f mPosition;
        Vector3f mTarget;

        // ProjectionMatrix Data
        float mFov;
        float mAspect;
        float mZNear;
        float mZFar;

        // Coordinate system
        Vector3f mFront;
        Vector3f mUp;
        Vector3f mRight;

        // Rotation Data
        float mYaw, mPitch, mRoll;

    private:
        void CreateRenderDataBuffer();
        void UpdateViewMatrix();
        void UpdateProjectionMatrix();
        void UpdateCoordinateSystem();
        void UpdateRenderData();
    };
}