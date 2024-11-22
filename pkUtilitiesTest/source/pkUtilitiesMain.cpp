#define CATCH_CONFIG_MAIN
#include "externals/catch.hpp"
#include "pkMatrix4.h"
#include "pkMath.h"
#include "pkPrerequisitesUtilities.h"
#include "pkVector2.h"
#include "pkVector3.h"

using namespace pkEngineSDK;

/**************************************************************/
/**
 * Platform Types Tests
**/
/**************************************************************/

TEST_CASE("Platform Types") {
  REQUIRE(sizeof(uint8) == 1);
  REQUIRE(sizeof(uint16) == 2);
  REQUIRE(sizeof(uint32) == 4);
  REQUIRE(sizeof(uint64) == 8);

  REQUIRE(sizeof(int8) == 1);
  REQUIRE(sizeof(int16) == 2);
  REQUIRE(sizeof(int32) == 4);
  REQUIRE(sizeof(int64) == 8);

  REQUIRE(sizeof(TYPE_OF_NULL) == 4);

#if PK_ARCH_TYPE == PK_ARCHITECTURE_X86_32
  REQUIRE(sizeof(pkEngineSDK::SIZE_T) == 4);
#elif PK_ARCH_TYPE == PK_ARCHITECTURE_X86_64
  REQUIRE(sizeof(pkEngineSDK::SIZE_T) == 8);
#endif
}

/**************************************************************/
/**
 * Platform Math Tests
**/
/**************************************************************/

TEST_CASE("Platform Math") {
  REQUIRE(Math::PI == Approx(3.14159f));
  REQUIRE(Math::EULER == Approx(2.71828f));
  REQUIRE(Math::SMALL_NUMBER == Approx(1e-6f));
  REQUIRE(Math::RAD2DEG == Approx(180.0f / Math::PI));
  REQUIRE(Math::DEG2RAD == Approx(Math::PI / 180.0f));

  REQUIRE(Math::abs(-1.0f) == 1.0f);
  REQUIRE(Math::pow(2.0f, 2.0f) == 4.0f);
  REQUIRE(Math::sqrt(6.0f) == Approx(2.44949f));
  REQUIRE(Math::exp(2.0f) == Approx(7.38906f));

  REQUIRE(Math::min(6.0f, 3.0f) == 3.0f);
  REQUIRE(Math::max(6.0f, 3.0f) == 6.0f);

  REQUIRE(Math::sin(1.0f) == Approx(0.841471f));
  REQUIRE(Math::asin(1.0f) == Approx(1.5708f));

  REQUIRE(Math::cos(1.0f) == Approx(0.540302f));
  REQUIRE(Math::acos(1.0f) == 0.0f);

  REQUIRE(Math::tan(1.0f) == Approx(1.55741f));
  REQUIRE(Math::atan(1.0f) == Approx(0.785398f));

  REQUIRE(Math::clamp(-1.0f, 0.0f, 1.0f) == 0.0f);
  REQUIRE(Math::clamp(2.0f, 0.0f, 1.0f) == 1.0f);
}

/**************************************************************/
/**
 * Vector2 Tests
**/
/**************************************************************/

TEST_CASE("Vector2 Test") {
  /**
   * Size verification bytes
  **/
  REQUIRE(sizeof(Vector2) == 8.0f);

  Vector2 vecTest0(2.0f, 3.0f);
  Vector2 vecTest1(4.0f, 5.0f);

  /**
   * Dot Product test
  **/
  REQUIRE(vecTest0.x == 2.0f);
  REQUIRE(vecTest0.y == 3.0f);
  REQUIRE(vecTest1.x == 4.0f);
  REQUIRE(vecTest1.y == 5.0f);
  REQUIRE(vecTest0.dotProd(vecTest1) == 23.0f);

  /**
   * Magnitude test
  **/
  REQUIRE(vecTest0.magnitudeSquare() == 13.0f);

  /**
   * Magnitude test
  **/
  REQUIRE(vecTest0.magnitude() == Approx(3.60555f));

  /**
   * Normalize test
  **/
  vecTest0.normalize();
  REQUIRE(vecTest0.x == Approx(0.5547f));
  REQUIRE(vecTest0.y == Approx(0.83205f));

  /**
   * Clamp test
  **/
  vecTest0 = Vector2(3.0f, -0.5f);
  vecTest0.clamp(0.0f, 1.0f);
  REQUIRE(vecTest0.x == 1.0f);
  REQUIRE(vecTest0.y == 0.0f);

  /**
   * Distance Test
  **/
  vecTest0 = Vector2(1.0f, 0.0f);
  Vector2 vecTest2 = Vector2(-5.0f, 0.0f);
  REQUIRE(vecTest0.distanceTo(vecTest2) == 6.0f);
}

/**************************************************************/
/**
 * Vector3 Tests
**/
/**************************************************************/

TEST_CASE("Vector3 Test") {
  /**
   * Size verification bytes
  **/
  REQUIRE(sizeof(Vector3) == 12.0f);

  Vector3 vecTest0(2.0f, 3.0f, 4.0f);
  Vector3 vecTest1(5.0f, 6.0f, 7.0f);

  /**
   * Dot product test
  **/
  REQUIRE(vecTest0.x == 2.0f);
  REQUIRE(vecTest0.y == 3.0f);
  REQUIRE(vecTest0.z == 4.0f);
  REQUIRE(vecTest1.x == 5.0f);
  REQUIRE(vecTest1.y == 6.0f);
  REQUIRE(vecTest1.z == 7.0f);
  REQUIRE(vecTest0.dotProd(vecTest1) == 56);

  /**
   * Magnitude square test
  **/
  REQUIRE(vecTest0.magnitudeSquare() == 29);

  /**
   * Magnitude Test
  **/
  REQUIRE(vecTest0.magnitude() == Approx(5.38516f));

  /**
   * Normalize test
  **/
  vecTest0.normalize();
  REQUIRE(vecTest0.x == Approx(0.371391f));
  REQUIRE(vecTest0.y == Approx(0.557086f));
  REQUIRE(vecTest0.z == Approx(0.742781f));

  /**
   * Clamp test
  **/
  vecTest0 = Vector3(3.0f, 0.5f, 0.1f);
  vecTest0.clamp(0.0f, 1.0f);
  REQUIRE(vecTest0.x == 1.0f);
  REQUIRE(vecTest0.y == 0.5f);
  REQUIRE(vecTest0.z == 0.1f);

  /**
   * Distance Test
  **/
  vecTest0 = Vector3(1.0f, 0.0f, 0.0f);
  Vector3 vecTest2 = Vector3(-1.0f, 0.0f, 0.0f);
  REQUIRE(vecTest0.distanceTo(vecTest2) == 2.0f);
}

/**************************************************************/
/**
 * Vector4 Tests
**/
/**************************************************************/

TEST_CASE("Vector4 Test") {
  /**
   * Size verification bytes
  **/
  REQUIRE(sizeof(Vector4) == 16.0f);

  Vector4 vecTest0(1.0f, 2.0f, 3.0f, 4.0f);
  Vector4 vecTest1(5.0f, 6.0f, 7.0f, 8.0f);

  /**
   * Dot product test
  **/
  REQUIRE(vecTest0.x == 1.0f);
  REQUIRE(vecTest0.y == 2.0f);
  REQUIRE(vecTest0.z == 3.0f);
  REQUIRE(vecTest0.w == 4.0f);
  REQUIRE(vecTest1.x == 5.0f);
  REQUIRE(vecTest1.y == 6.0f);
  REQUIRE(vecTest1.z == 7.0f);
  REQUIRE(vecTest1.w == 8.0f);
  REQUIRE(vecTest0.dotProd(vecTest1) == 70.0f);

  /**
   * Dot Product 3 test
  **/
  REQUIRE(vecTest0.dotProd3(vecTest1) == 38.0f);

  /**
   * Vector 3 Cross
  **/
  Vector4 crossTest = vecTest0.vector3Cross(vecTest1);
  REQUIRE(crossTest.x == -4.0f);
  REQUIRE(crossTest.y == 8.0f);
  REQUIRE(crossTest.z == -4.0f);
  REQUIRE(crossTest.w == 0.0f);

  /**
   * Magnitude square test
  **/
  REQUIRE(vecTest0.magnitudeSquare() == 30.0f);

  /**
   * Magnitude test
  **/
  REQUIRE(vecTest0.magnitude() == Approx(5.47723f));

  /**
   * Normalize test
  **/
  vecTest0.normalize();
  REQUIRE(vecTest0.x == Approx(0.182574f));
  REQUIRE(vecTest0.y == Approx(0.365148f));
  REQUIRE(vecTest0.z == Approx(0.547723f));
  REQUIRE(vecTest0.w == Approx(0.730297f));

  /**
   * Clamp test
  **/
  vecTest0 = Vector4(3.0f, 0.5f, 0.1f, -2.0f);
  vecTest0.clamp(0.0f, 1.0f);
  REQUIRE(vecTest0.x == 1.0f);
  REQUIRE(vecTest0.y == 0.5f);
  REQUIRE(vecTest0.z == 0.1f);
  REQUIRE(vecTest0.w == 0.0f);

  /**
   * Distance Test
  **/
  vecTest0 = Vector4(1.0f, 0.0f, 0.0f, 5.0f);
  Vector4 vecTest2 = Vector4(-1.0f, 0.0f, 0.0f, 8.0f);
  REQUIRE(vecTest0.distanceTo(vecTest2) == Approx(3.60555f));

  /**
   * Is different test
  **/
  vecTest2 = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
  Vector4 vecTest3(1.0f, 1.0f, 1.0f, 1.0f);
  REQUIRE(vecTest2.x == 1.0f);
  REQUIRE(vecTest2.y == 1.0f);
  REQUIRE(vecTest2.z == 1.0f);
  REQUIRE(vecTest2.w == 1.0f);
  REQUIRE(vecTest3.x == 1.0f);
  REQUIRE(vecTest3.y == 1.0f);
  REQUIRE(vecTest3.z == 1.0f);
  REQUIRE(vecTest3.w == 1.0f);
  REQUIRE(vecTest2.isDifferent(vecTest3) == false);

  /**
   * Vector splat value
  **/
  Vector4 vsvt = Vector4::vectorSplatValue(5.0f);
  REQUIRE(vsvt.x == 5.0f);
  REQUIRE(vsvt.y == 5.0f);
  REQUIRE(vsvt.z == 5.0f);
  REQUIRE(vsvt.w == 5.0f);
}

/**************************************************************/
/**
 * Matrix4 Tests
**/
/**************************************************************/

TEST_CASE("Matrix4 test") {
  /**
   * Size verification bytes
  **/
  REQUIRE(sizeof(Matrix4) == 64.0f);

  /**
   * Get transposed test
  **/
  Matrix4 matTest0 = Matrix4::IDENTITY;
  matTest0.matrix[1][0] = 5.0f;
  matTest0.matrix[2][1] = 5.0f;
  matTest0.matrix[3][2] = 5.0f;
  const Matrix4 compare0 = matTest0;
  REQUIRE(matTest0.matrix[0][0] == 1.0f);
  REQUIRE(matTest0.matrix[1][0] == 5.0f);
  REQUIRE(matTest0.matrix[2][0] == 0.0f);
  REQUIRE(matTest0.matrix[3][0] == 0.0f);

  REQUIRE(matTest0.matrix[0][1] == 0.0f);
  REQUIRE(matTest0.matrix[1][1] == 1.0f);
  REQUIRE(matTest0.matrix[2][1] == 5.0f);
  REQUIRE(matTest0.matrix[3][1] == 0.0f);

  REQUIRE(matTest0.matrix[0][2] == 0.0f);
  REQUIRE(matTest0.matrix[1][2] == 0.0f);
  REQUIRE(matTest0.matrix[2][2] == 1.0f);
  REQUIRE(matTest0.matrix[3][2] == 5.0f);

  REQUIRE(matTest0.matrix[0][3] == 0.0f);
  REQUIRE(matTest0.matrix[1][3] == 0.0f);
  REQUIRE(matTest0.matrix[2][3] == 0.0f);
  REQUIRE(matTest0.matrix[3][3] == 1.0f);

  matTest0 = matTest0.getTransposed();
  REQUIRE(matTest0.matrix[0][0] == 1.0f);
  REQUIRE(matTest0.matrix[1][0] == 0.0f);
  REQUIRE(matTest0.matrix[2][0] == 0.0f);
  REQUIRE(matTest0.matrix[3][0] == 0.0f);

  REQUIRE(matTest0.matrix[0][1] == 5.0f);
  REQUIRE(matTest0.matrix[1][1] == 1.0f);
  REQUIRE(matTest0.matrix[2][1] == 0.0f);
  REQUIRE(matTest0.matrix[3][1] == 0.0f);

  REQUIRE(matTest0.matrix[0][2] == 0.0f);
  REQUIRE(matTest0.matrix[1][2] == 5.0f);
  REQUIRE(matTest0.matrix[2][2] == 1.0f);
  REQUIRE(matTest0.matrix[3][2] == 0.0f);

  REQUIRE(matTest0.matrix[0][3] == 0.0f);
  REQUIRE(matTest0.matrix[1][3] == 0.0f);
  REQUIRE(matTest0.matrix[2][3] == 5.0f);
  REQUIRE(matTest0.matrix[3][3] == 1.0f);

  /**
   * Matrix scaling test
  **/
  float scale = 5.0f;
  matTest0 = Matrix4::matrixScaling(scale, scale, scale);
  REQUIRE(matTest0.matrix[0][0] == 5.0f);
  REQUIRE(matTest0.matrix[1][0] == 0.0f);
  REQUIRE(matTest0.matrix[2][0] == 0.0f);
  REQUIRE(matTest0.matrix[3][0] == 0.0f);

  REQUIRE(matTest0.matrix[0][1] == 0.0f);
  REQUIRE(matTest0.matrix[1][1] == 5.0f);
  REQUIRE(matTest0.matrix[2][1] == 0.0f);
  REQUIRE(matTest0.matrix[3][1] == 0.0f);

  REQUIRE(matTest0.matrix[0][2] == 0.0f);
  REQUIRE(matTest0.matrix[1][2] == 0.0f);
  REQUIRE(matTest0.matrix[2][2] == 5.0f);
  REQUIRE(matTest0.matrix[3][2] == 0.0f);

  REQUIRE(matTest0.matrix[0][3] == 0.0f);
  REQUIRE(matTest0.matrix[1][3] == 0.0f);
  REQUIRE(matTest0.matrix[2][3] == 0.0f);
  REQUIRE(matTest0.matrix[3][3] == 1.0f);

  /**
   * Translation test
  **/
  float translation = 8.0f;
  matTest0 = Matrix4::translation(translation, translation, translation);
  REQUIRE(matTest0.matrix[0][0] == 1.0f);
  REQUIRE(matTest0.matrix[1][0] == 0.0f);
  REQUIRE(matTest0.matrix[2][0] == 0.0f);
  REQUIRE(matTest0.matrix[3][0] == 0.0f);

  REQUIRE(matTest0.matrix[0][1] == 0.0f);
  REQUIRE(matTest0.matrix[1][1] == 1.0f);
  REQUIRE(matTest0.matrix[2][1] == 0.0f);
  REQUIRE(matTest0.matrix[3][1] == 0.0f);

  REQUIRE(matTest0.matrix[0][2] == 0.0f);
  REQUIRE(matTest0.matrix[1][2] == 0.0f);
  REQUIRE(matTest0.matrix[2][2] == 1.0f);
  REQUIRE(matTest0.matrix[3][2] == 0.0f);

  REQUIRE(matTest0.matrix[0][3] == 8.0f);
  REQUIRE(matTest0.matrix[1][3] == 8.0f);
  REQUIRE(matTest0.matrix[2][3] == 8.0f);
  REQUIRE(matTest0.matrix[3][3] == 1.0f);

  Vector3 translationVector(translation);
  matTest0 = Matrix4::translation(translationVector);
  REQUIRE(matTest0.matrix[0][0] == 1.0f);
  REQUIRE(matTest0.matrix[1][0] == 0.0f);
  REQUIRE(matTest0.matrix[2][0] == 0.0f);
  REQUIRE(matTest0.matrix[3][0] == 0.0f);

  REQUIRE(matTest0.matrix[0][1] == 0.0f);
  REQUIRE(matTest0.matrix[1][1] == 1.0f);
  REQUIRE(matTest0.matrix[2][1] == 0.0f);
  REQUIRE(matTest0.matrix[3][1] == 0.0f);

  REQUIRE(matTest0.matrix[0][2] == 0.0f);
  REQUIRE(matTest0.matrix[1][2] == 0.0f);
  REQUIRE(matTest0.matrix[2][2] == 1.0f);
  REQUIRE(matTest0.matrix[3][2] == 0.0f);

  REQUIRE(matTest0.matrix[0][3] == 8.0f);
  REQUIRE(matTest0.matrix[1][3] == 8.0f);
  REQUIRE(matTest0.matrix[2][3] == 8.0f);
  REQUIRE(matTest0.matrix[3][3] == 1.0f);

  /**
   * scale test
  **/
  scale = 8.0f;
  Vector3 scaleVector(scale);
  matTest0 = Matrix4::scale(scaleVector);
  REQUIRE(matTest0.matrix[0][0] == 1.0f);
  REQUIRE(matTest0.matrix[1][0] == 0.0f);
  REQUIRE(matTest0.matrix[2][0] == 0.0f);
  REQUIRE(matTest0.matrix[3][0] == 8.0f);

  REQUIRE(matTest0.matrix[0][1] == 0.0f);
  REQUIRE(matTest0.matrix[1][1] == 1.0f);
  REQUIRE(matTest0.matrix[2][1] == 0.0f);
  REQUIRE(matTest0.matrix[3][1] == 8.0f);

  REQUIRE(matTest0.matrix[0][2] == 0.0f);
  REQUIRE(matTest0.matrix[1][2] == 0.0f);
  REQUIRE(matTest0.matrix[2][2] == 1.0f);
  REQUIRE(matTest0.matrix[3][2] == 8.0f);

  REQUIRE(matTest0.matrix[0][3] == 0.0f);
  REQUIRE(matTest0.matrix[1][3] == 0.0f);
  REQUIRE(matTest0.matrix[2][3] == 0.0f);
  REQUIRE(matTest0.matrix[3][3] == 1.0f);

  /**
   * rotation test
  **/
  float rotation = 8.0f;
  matTest0 = Matrix4::rotation(rotation, rotation, rotation);
  REQUIRE(matTest0.matrix[0][0] == Approx(0.0211703f));
  REQUIRE(matTest0.matrix[1][0] == Approx(0.286371f));
  REQUIRE(matTest0.matrix[2][0] == Approx(-0.957885f));
  REQUIRE(matTest0.matrix[3][0] == 0.0f);

  REQUIRE(matTest0.matrix[0][1] == Approx(-0.143952f));
  REQUIRE(matTest0.matrix[1][1] == Approx(-0.947243f));
  REQUIRE(matTest0.matrix[2][1] == Approx(-0.286371f));
  REQUIRE(matTest0.matrix[3][1] == 0.0f);

  REQUIRE(matTest0.matrix[0][2] == Approx(-0.989358f));
  REQUIRE(matTest0.matrix[1][2] == Approx(0.143952f));
  REQUIRE(matTest0.matrix[2][2] == Approx(0.0211703f));
  REQUIRE(matTest0.matrix[3][2] == 0.0f);

  REQUIRE(matTest0.matrix[0][3] == 0.0f);
  REQUIRE(matTest0.matrix[1][3] == 0.0f);
  REQUIRE(matTest0.matrix[2][3] == 0.0f);
  REQUIRE(matTest0.matrix[3][3] == 1.0f);

  Vector3 rotationVecton(rotation);
  matTest0 = Matrix4::rotation(rotationVecton);
  REQUIRE(matTest0.matrix[0][0] == Approx(0.0211703f));
  REQUIRE(matTest0.matrix[1][0] == Approx(0.286371f));
  REQUIRE(matTest0.matrix[2][0] == Approx(-0.957885f));
  REQUIRE(matTest0.matrix[3][0] == 0.0f);

  REQUIRE(matTest0.matrix[0][1] == Approx(-0.143952f));
  REQUIRE(matTest0.matrix[1][1] == Approx(-0.947243f));
  REQUIRE(matTest0.matrix[2][1] == Approx(-0.286371f));
  REQUIRE(matTest0.matrix[3][1] == 0.0f);

  REQUIRE(matTest0.matrix[0][2] == Approx(-0.989358f));
  REQUIRE(matTest0.matrix[1][2] == Approx(0.143952f));
  REQUIRE(matTest0.matrix[2][2] == Approx(0.0211703f));
  REQUIRE(matTest0.matrix[3][2] == 0.0f);

  REQUIRE(matTest0.matrix[0][3] == 0.0f);
  REQUIRE(matTest0.matrix[1][3] == 0.0f);
  REQUIRE(matTest0.matrix[2][3] == 0.0f);
  REQUIRE(matTest0.matrix[3][3] == 1.0f);
}
