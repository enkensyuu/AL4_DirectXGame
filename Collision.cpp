#include "Collision.h"

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, XMVECTOR* inter)
{
    // ���W�n�̌��_���狅�̒��S���W�ւ̋���
    XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
    // ���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
    float dist = distV.m128_f32[0] - plane.distance;
    // �����̐�Βl�����a���傫����Γ����Ă��Ȃ�
    if (fabsf(dist) > sphere.radius) return false;

    // �[����_���v�Z
    if (inter)
    {
        *inter = -dist * plane.normal + sphere.center;
    }

    return true;
}
