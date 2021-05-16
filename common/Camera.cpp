
#include "Camera.h"

Camera::Camera()
{
	SetLens(0.25f*M_PI, 1.0f, 1.0f, 1000.0f);
}

Camera::~Camera()
{
}

Math::Vec3 Camera::GetPosition()const
{
	return mPosition;
}

void Camera::SetPosition(float x, float y, float z)
{
	mPosition = Math::Vec3(x, -y, z);
	mViewDirty = true;
}

void Camera::SetPosition(const Math::Vec3& v)
{
	mPosition = v;
	mViewDirty = true;
}

Math::Vec3 Camera::GetRight()const
{
	return mRight;
}

Math::Vec3 Camera::GetUp()const
{
	return mUp;
}

Math::Vec3 Camera::GetLook()const
{
	return mLook;
}

float Camera::GetNearZ()const
{
	return mNearZ;
}

float Camera::GetFarZ()const
{
	return mFarZ;
}

float Camera::GetAspect()const
{
	return mAspect;
}

float Camera::GetFovY()const
{
	return mFovY;
}

float Camera::GetFovX()const
{
	float halfWidth = 0.5f*GetNearWindowWidth();
	return 2.0f*atan(halfWidth / mNearZ);
}

float Camera::GetNearWindowWidth()const
{
	return mAspect * mNearWindowHeight;
}

float Camera::GetNearWindowHeight()const
{
	return mNearWindowHeight;
}

float Camera::GetFarWindowWidth()const
{
	return mAspect * mFarWindowHeight;
}

float Camera::GetFarWindowHeight()const
{
	return mFarWindowHeight;
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf( 0.5f*mFovY );
	mFarWindowHeight  = 2.0f * mFarZ * tanf( 0.5f*mFovY );

	Math::Mat4::createPerspective(MATH_RAD_TO_DEG(mFovY), mAspect, mNearZ, mFarZ, &mProj);
	mProj.transpose();
}

void Camera::LookAt(const Math::Vec3& pos, const Math::Vec3& target, const Math::Vec3& up)
{
	Math::Vec3 L = (target - pos).getNormalized();
	Math::Vec3 R = Math::crossProduct(L, up).getNormalized();
	Math::Vec3 U = Math::crossProduct(R, L);

	mPosition = pos;
	mLook = L;
	mRight = R;
	mUp = U;

	mViewDirty = true;
}

Math::Mat4 Camera::GetView()const
{
	assert(!mViewDirty);
	return mView;
}

Math::Mat4 Camera::GetProj()const
{
	return mProj;
}

void Camera::Strafe(float d)
{
	mPosition += mRight * d;

	mViewDirty = true;
}

void Camera::Walk(float d)
{
	mPosition += mLook * d;

	mViewDirty = true;
}

void Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	Math::Mat4 R;
	R.rotate(mRight, -angle);

	mUp *= R;
	mLook *= R;

	mViewDirty = true;
}

void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	Math::Mat4 R;
	R.rotateY(-angle);

	mRight *= R;
	mUp *= R;
	mLook *= R;

	mViewDirty = true;
}

void Camera::UpdateViewMatrix()
{
	if(mViewDirty)
	{
		Math::Vec3 R = mRight;
		Math::Vec3 U = mUp;
		Math::Vec3 L = mLook;
		Math::Vec3 P = mPosition;

		// Keep camera's axes orthogonal to each other and of unit length.
		L.normalize();
		U = Math::crossProduct(R, L).getNormalized();

		// U, L already ortho-normal, so no need to normalize cross product.
		R = Math::crossProduct(L, U);

		// Fill in the view matrix entries.
		float x = -Math::dot(R, P);
		float y = -Math::dot(U, P);
		float z = -Math::dot(L, P);

		mRight = R;
		mUp = -U;
		mLook = L;

		mView.m[0] = mRight.x;
		mView.m[1] = mRight.y;
		mView.m[2] = mRight.z;
		mView.m[3] = x;

		mView.m[4] = mUp.x;
		mView.m[5] = mUp.y;
		mView.m[6] = mUp.z;
		mView.m[7] = y;

		mView.m[8] = mLook.x;
		mView.m[9] = mLook.y;
		mView.m[10] = mLook.z;
		mView.m[11] = z;

		mView.m[12] = 0.0f;
		mView.m[13] = 0.0f;
		mView.m[14] = 0.0f;
		mView.m[15] = 1.0f;

		//mView.transpose();

		mViewDirty = false;
	}
}


