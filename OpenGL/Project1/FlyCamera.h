#pragma once
#include <SDL.h>
#include "Math.h"
#include "Input.h"
#include "mouse.h"

class FlyCamera
{
public:
    FlyCamera(Vector3& cameraPos);
    ~FlyCamera() {};
    void        SetZoom(float zoom) { mZoom = zoom; }
    void        SetCameraMoveSpeed(float speed) { mCameraMoveSpeed = speed; }
    void        SetSensitivity(float sensitibity) { mMouseWheelSensitibity = sensitibity; }
    void        SetScreenWidth(float screenw) { mScreenWidth = screenw; }
    void        SetScreenHeight(float screenh) { mScreenHeight = screenh; }
    const       Matrix4& GetViewMatrix() const { return mViewMatrix; } // �J�����i�r���[�s��)�̎擾
    const       Matrix4& GetProjectionMatrix() const { return mProjectionMatrix; }     // �v���W�F�N�V�����s��̎擾
    const       Vector3& GetFrontVec() const { return mCameraFront; }
    const       Vector3& GetPositionVec() const { return mCameraPos; }
    void        UpdateCamera(float frameDeltaTime); // �J�����̈ړ��v�Z

private:
    Vector3     mCameraPos;              // �J�����ʒu
    Vector3     mCameraFront;            // �J�����O������
    Vector3     mCameraUp;               // �J���������

    Matrix4     mViewMatrix;             // �J�����s��
    Matrix4     mProjectionMatrix;       // �v���W�F�N�V�����s��
    float       mZoom;                   // �Y�[�����x���i��p)
    float       mCameraMoveSpeed;        // �J�����ړ����x (/ sec)
    float       mMouseWheelSensitibity;  // �}�E�X�z�C�[���W�� (/ frame)
    float       mYaw;                    // ���[(�I�C���[�p�j
    float       mPitch;                  // �s�b�`(�I�C���[�p�j
    float       mScreenWidth;            // �X�N���[����
    float       mScreenHeight;           // �X�N���[������
    void        UpdateMouse(float frameDeltaTime);
    void        UpdateKey(float frameDeltaTime);
};