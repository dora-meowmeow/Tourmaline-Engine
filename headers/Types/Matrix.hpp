/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GUARD_TOURMALINE_TYPE_MATRIX_H
#define GUARD_TOURMALINE_TYPE_MATRIX_H
/**
 * @file
 * @brief Matrix type definition
 */
#include "Magnum/Math/Matrix4.h"
#include "Magnum/Math/Quaternion.h"
#include "Magnum/Math/Vector3.h"

#include "Units.hpp"
#include <algorithm>
#include <cstdint>

namespace Tourmaline::Type {
/**
 * @brief Magnum::Math::Matrix4 with Quality of Life features.
 *
 * This class mainly adds functions to make working with Magnum::Math::Matrix4
 * more concise and easy.
 *
 * @see Magnum::Math::Matrix4
 * @see [What is a
 * matrix?](https://simple.wikipedia.org/wiki/Matrix_(mathematics))
 * @see [What is a transformation
 * matrix?](https://www.geeksforgeeks.org/maths/transformation-matrix/)
 */
class Matrix : public Magnum::Math::Matrix4<float> {
public:
  /**
   * @brief Rows on the matrix. This value is equal to 4.
   */
  static constexpr uint8_t Rows = Matrix4<float>::Rows;

  /**
   * @brief Columns on the matrix. This value is equal to 4.
   */
  static constexpr uint8_t Columns = Matrix4<float>::Cols;

  /**
   * @brief Rows * Columns. Basically every cell/element of the matrix.
   */
  static constexpr uint8_t Elements = Rows * Columns;

  /**
   * @brief Default constructor for the Matrix class.
   *
   * If you want to set position/translation of your matrix, you can use
   * Tourmaline::Type::Matrix::FromTranslation.
   *
   * @see Tourmaline::Type::Matrix::FromTranslation
   */
  Matrix() : Matrix(Magnum::Math::Vector3<float>{0.0f}) {}

  /**
   * @brief Constructs a matrix with same value on every cell.
   * @param fillWith This value will be used to fill every cell inside the
   * matrix.
   */
  Matrix(float fillWith) : Magnum::Math::Matrix4<float>(fillWith) {};

  /**
   * @brief Copy constructor using Magnum::Math::Matrix4.
   * @see Magnum::Math::Matrix4
   */
  Matrix(const Magnum::Math::Matrix4<float> &copy)
      : Magnum::Math::Matrix4<float>(copy) {};

  /**
   * @brief Copy data (using Magnum::Math::Matrix4).
   * @see Magnum::Math::Matrix4
   */
  Matrix &operator=(const Magnum::Math::Matrix4<float> &copy) {
    std::copy(copy.data(), copy.data() + Elements, this->data());
    return *this;
  }

  /**
   * @brief Constructs a matrix at a specified position/translation.
   */
  static Matrix
  FromTranslation(const Magnum::Math::Vector3<float> &translation) {
    return Matrix(translation);
  }

  // Translation
  /**
   * @brief Adds offset to the position/translation of a matrix.
   *
   * @param offset You can think of this as how much you want to move the
   * matrix.
   * @param order Wheter or not the offset is applied locally or from a global
   * axis.
   *
   * @return A reference to the matrix itself for method chaining.
   *
   * @see Tourmaline::Type::Apply
   */
  Matrix &translate(const Magnum::Math::Vector3<float> &offset,
                    Apply order = Apply::Locally);

  /**
   * @brief Sets the position/translation for the matrix.
   *
   * @param position New position/translation to put the matrix on.
   *
   * @return A reference to the matrix itself for method chaining.
   */
  Matrix &setTranslation(const Magnum::Math::Vector3<float> &position);

  // Rotate

  /**
   * @brief Rotate a matrix on a given arbitrary axis using quaternions.
   *
   * @param angle Rotation angle on given axis. Positive means
   * counter-clockwise.
   * @param rotationAxis This can be thought as a normal of a 2D plane where the
   * rotation will occur. This value cannot have magnitude of zero.
   * @param order Wheter or not the rotation should be applied locally or from
   * a global axis.
   * @param anglesIn Specifies if angles are in radiants or degrees.
   *
   * @return A reference to the matrix itself for method chaining.
   *
   * @see Tourmaline::Type::AngleUnit
   * @see Tourmaline::Type::Apply
   */
  Matrix &rotate(float angle, Magnum::Math::Vector3<float> rotationAxis,
                 Apply order = Apply::Locally,
                 AngleUnit anglesIn = AngleUnit::Radiants);

  /**
   * @brief Rotate a matrix using quaternions.
   *
   * @param quaternion Quaternion to use for rotation.
   * @param order Wheter or not the rotation should be applied locally or from
   * a global axis.
   *
   * @return A reference to the matrix itself for method chaining.
   *
   * @see [What are quaternions?](https://www.youtube.com/watch?v=d4EgbgTm0Bg)
   * @see [Please simplify me what quaternions
   * are!](https://marctenbosch.com/quaternions/)
   * @see Magnum::Math::Quaternion
   * @see Tourmaline::Type::Apply
   */
  Matrix &rotateQuaternion(Magnum::Math::Quaternion<float> quaternion,
                           Apply order = Apply::Locally);
  /**
   * @brief Rotates the matrix using Euler angles.
   *
   * @param angles Rotation angles on XY, XZ, and YZ planes. Positive means
   * counter-clockwise.
   * @param order Wheter or not the rotation should be applied locally or from
   * a global axis.
   * @param anglesIn Specifies if angles are in radiants or degrees.
   *
   * @return A reference to the matrix itself for method chaining.
   *
   * @warning It is strongly discouraged to use Euler angles for rotation. Euler
   * angles are susceptible to gimbal lock! Please try to use
   * Tourmaline::Type::Matrix::rotate or
   * Tourmaline::Type::Matrix::rotateQuaternion
   *
   * @see [What is gimbal lock?](https://www.youtube.com/watch?v=zc8b2Jo7mno)
   * @see Tourmaline::Type::Matrix::rotate
   * @see Tourmaline::Type::AngleUnit
   * @see Tourmaline::Type::Apply
   */
  Matrix &rotateEuler(const Magnum::Math::Vector3<float> &angles,
                      Apply order = Apply::Locally,
                      AngleUnit anglesIn = AngleUnit::Radiants);
  // Rotation Get
  /**
   * @brief Returns the current rotation on the matrix as Euler angles.
   *
   * @param anglesIn Which unit should the angles be in.
   *
   * @return The Euler angles in XY, XZ, and YZ planes.
   *
   * @see Tourmaline::Type::AngleUnit
   */
  Magnum::Math::Vector3<float>
  getEulerAngles(AngleUnit anglesIn = AngleUnit::Radiants) const;

  /**
   * @brief Returns the current rotation on the matrix as a quaternion.
   *
   * @return The rotation as a quaternion.
   * @see [What are quaternions?](https://www.youtube.com/watch?v=d4EgbgTm0Bg)
   * @see [Please simplify me what quaternions
   * are!](https://marctenbosch.com/quaternions/)
   */
  Magnum::Math::Quaternion<float> getQuaternion() const;

  // Scaling
  /**
   * @brief Scales the matrix by given amount.
   *
   * @param scaleBy How much on X, Y, and Z axis (individually) the matrix
   * should scale.
   * @param order Wheter or not if the offset is applied locally or from global
   * axis.
   *
   * @return A reference to the matrix itself for method chaining.
   *
   * @see Tourmaline::Type::Apply
   */
  Matrix &scale(const Magnum::Math::Vector3<float> &scaleBy,
                Apply order = Apply::Locally);

  /**
   * @brief Scales the matrix by given amount.
   *
   * @param scaleBy How much on X, Y, and Z axis (equally) the matrix should
   * scale.
   * @param order Wheter or not if the offset is applied locally or from global
   * axis.
   *
   * @return A reference to the matrix itself for method chaining.
   *
   * @see Tourmaline::Type::Apply
   */
  Matrix &scale(float scaleBy, Apply order = Apply::Locally);

  /**
   * @brief Sets the scale of the matrix.
   *
   * @param scale on X, Y, and Z axis (individually) what the matrix's scale
   * should be equal to.
   *
   * @return A reference to the matrix itself for method chaining.
   *
   * @see Tourmaline::Type::Apply
   */
  Matrix &setScaling(const Magnum::Math::Vector3<float> &scale);

  /**
   * @brief Sets the scale of the matrix.
   *
   * @param scale on X, Y, and Z axis (equally) what the matrix's scale
   * should be equal to.
   *
   * @return A reference to the matrix itself for method chaining.
   *
   * @see Tourmaline::Type::Apply
   */
  Matrix &setScaling(float scale);

  Magnum::Math::Matrix4<float> &asMagnumMatrix();

private:
  Matrix(const Magnum::Math::Vector3<float> &translation)
      : Magnum::Math::Matrix4<float>(
            Magnum::Math::Matrix4<float>::translation(translation)) {};
};
} // namespace Tourmaline::Type

#endif
