#include "FlyCamera.h"

const float initZoomDegree = 45.0f; // �����Y�[����p(degree)
const float initMoveSpeed = 2.5f; // �����ړ����x ( / sec�P��)
const float initSensitibity = 0.25f; // �����z�C�[���X�N���[�����x( / frame )
const float initYawDegree = -90.0f; // ������]�ʒu (x������-90�x�ړ� z�����Α�)
const float pitchMaxDegree = 89.0f; // �J�����s�b�`�ō��p�x(degree)
const float pitchMinDegree = -89.0f; // �J�����s�b�`�Œ�p�x(degree)
const float zoomMaxDegree = 90.0f; // �Y�[����p�ő�p�x(�Y�[���A�E�g)
const float zoomMinDegree = 1.0f; // �Y�[����p�ŏ��p�x(�Y�[���C��)

FlyCamera::FlyCamera(Vector3& cameraPos)
    : mCameraPos(cameraPos),
    mCameraFront(Vector3(0.0f, 0.0f, -1.0f)),
    mCameraUp(Vector3(0.0f, 1.0f, 0.0f)),
    mZoom(Math::ToRadians(initZoomDegree)),
    mCameraMoveSpeed(initMoveSpeed),
    mMouseWheelSensitibity(initSensitibity),
    mYaw(Math::ToRadians(initYawDegree)),
    mPitch(0.0f),
    mScreenWidth(1024.0f),
    mScreenHeight(768.0f)
{
    mViewMatrix = Matrix4::CreateLookAt(mCameraPos, mCameraPos + mCameraFront, mCameraUp);
    mProjectionMatrix = Matrix4::CreatePerspectiveFOV(mZoom, mScreenWidth, mScreenHeight, 0.1, 1000.0f);
}

// �J�����̍X�V����
void FlyCamera::UpdateCamera(float frameDeltaTime)
{
    UpdateMouse(frameDeltaTime); // ���[�s�b�`�E�Y�[���̕ύX
    UpdateKey(frameDeltaTime);   // �J�����ʒu
    mProjectionMatrix = Matrix4::CreatePerspectiveFOV(mZoom, mScreenWidth, mScreenHeight, 0.1f, 1000.0f);
    mViewMatrix = Matrix4::CreateLookAt(mCameraPos, mCameraPos + mCameraFront, mCameraUp);
}

// ���[�s�b�`�E�Y�[�����x���̕ύX
void FlyCamera::UpdateMouse(float frameDeltaTime)
{
    // �J�����̃��[�s�b�`��ύX
    Vector2 mouse = MOUSE_INSTANCE.GetPosition();
    static float sensitivity = 0.2f;
    float xoffset = mouse.x * sensitivity * frameDeltaTime;
    float yoffset = mouse.y * sensitivity * frameDeltaTime;
    mYaw -= xoffset;
    mPitch -= yoffset;
    if (mPitch > Math::ToRadians(pitchMaxDegree))
    {
        mPitch = Math::ToRadians(pitchMaxDegree);
    }
    if (mPitch < Math::ToRadians(pitchMinDegree))
    {
        mPitch = Math::ToRadians(pitchMinDegree);
    }
    // �J�����̃��[�s�b�`����J�����̑O���x�N�g�������߂�
    Vector3 direction;
    direction.x = cosf(mYaw) * cos(mPitch);
    direction.y = sinf(mPitch);
    direction.z = sinf(mYaw) * cosf(mPitch);
    direction.Normalize();

    // �J�����O���x�N�g��
    mCameraFront = direction;

    // Zoom�W��
    Vector2 wheel = MOUSE_INSTANCE.GetWheel();
    mZoom += wheel.y * frameDeltaTime;

    // �ő�E�ŏ��Y�[���p�ɃN�����v����
    if (Math::ToDegrees(mZoom) < zoomMinDegree)
    {
        mZoom = Math::ToRadians(zoomMinDegree);
    }
    if (Math::ToDegrees(mZoom) > zoomMaxDegree)
    {
        mZoom = Math::ToRadians(zoomMaxDegree);
    }
}

// �L�[�{�[�h���쏈��
void FlyCamera::UpdateKey(float frameDeltaTime)
{
    // �J�����̑O���ړ����x�ƉE�����x�N�g���̎擾
    const float cameraSpeed = mCameraMoveSpeed * frameDeltaTime;
    Vector3 cameraRightVec = Vector3::Cross(mCameraUp, mCameraFront);
    // �L�[���͂���J�����̈ʒu�̌v�Z����
    if (INPUT_INSTANCE.GetInput(SDL_SCANCODE_W) == Input::KEY_STATE_PRESSED)
    {
        mCameraPos += cameraSpeed * mCameraFront;
    }
    if (INPUT_INSTANCE.GetInput(SDL_SCANCODE_S) == Input::KEY_STATE_PRESSED)
    {
        mCameraPos -= cameraSpeed * mCameraFront;
    }
    if (INPUT_INSTANCE.GetInput(SDL_SCANCODE_D) == Input::KEY_STATE_PRESSED)
    {
        mCameraPos += cameraSpeed * cameraRightVec;
    }
    if (INPUT_INSTANCE.GetInput(SDL_SCANCODE_A) == Input::KEY_STATE_PRESSED)
    {
        mCameraPos -= cameraSpeed * cameraRightVec;
    }
}