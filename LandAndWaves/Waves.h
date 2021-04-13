
#ifndef WAVES_H
#define WAVES_H

#include <vector>
#include "../math/Vec3.h"

class Waves
{
public:
    Waves(int m, int n, float dx, float dt, float speed, float damping);
    Waves(const Waves& rhs) = delete;
    Waves& operator=(const Waves& rhs) = delete;
    ~Waves();

	int RowCount()const;
	int ColumnCount()const;
	int VertexCount()const;
	int TriangleCount()const;
	float Width()const;
	float Depth()const;

	// Returns the solution at the ith grid point.
    const Math::Vec3& Position(int i)const { return mCurrSolution[i]; }

	// Returns the solution normal at the ith grid point.
    const Math::Vec3& Normal(int i)const { return mNormals[i]; }

	// Returns the unit tangent vector at the ith grid point in the local x-axis direction.
    const Math::Vec3& TangentX(int i)const { return mTangentX[i]; }

	void Update(float dt);
	void Disturb(int i, int j, float magnitude);

private:
    int mNumRows = 0;
    int mNumCols = 0;

    int mVertexCount = 0;
    int mTriangleCount = 0;

    // Simulation constants we can precompute.
    float mK1 = 0.0f;
    float mK2 = 0.0f;
    float mK3 = 0.0f;

    float mTimeStep = 0.0f;
    float mSpatialStep = 0.0f;

    std::vector<Math::Vec3> mPrevSolution;
    std::vector<Math::Vec3> mCurrSolution;
    std::vector<Math::Vec3> mNormals;
    std::vector<Math::Vec3> mTangentX;
};

#endif // WAVES_H