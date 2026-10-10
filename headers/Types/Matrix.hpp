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
 * @brief Magnum::Math::Matrix4 with QoL features.
 *
 * This class mainly adds functions to make working with Magnum::Math::Matrix4
 * more concise and easy.
 */
class Matrix : public Magnum::Math::Matrix4<float> {
public:
  static constexpr uint8_t Rows = Matrix4<float>::Rows;
  static constexpr uint8_t Columns = Matrix4<float>::Cols;
  static constexpr uint8_t Elements = Rows * Columns;

  Matrix() : Matrix(Magnum::Math::Vector3<float>{0.0f}) {}
  Matrix(float fillWith) : Magnum::Math::Matrix4<float>(fillWith) {};
  Matrix(const Magnum::Math::Matrix4<float> &copy)
      : Magnum::Math::Matrix4<float>(copy) {};

  Matrix &operator=(const Magnum::Math::Matrix4<float> &copy) {
    std::copy(copy.data(), copy.data() + Elements, this->data());
    return *this;
  }

  static Matrix
  FromTranslation(const Magnum::Math::Vector3<float> &translation) {
    return Matrix(translation);
  }

  // Translation
  Matrix &translate(const Magnum::Math::Vector3<float> &position,
                    Apply order = Apply::Locally);
  Matrix &setTranslation(const Magnum::Math::Vector3<float> &position);

  // Rotate
  Matrix &rotate(float angle, Magnum::Math::Vector3<float> rotationAxis,
                 Apply order = Apply::Locally,
                 AngleUnit anglesIn = AngleUnit::Radiants);
  Matrix &rotateQuaternion(Magnum::Math::Quaternion<float> quaternion,
                           Apply order = Apply::Locally);
  Matrix &rotateEuler(const Magnum::Math::Vector3<float> &angels,
                      Apply order = Apply::Locally,
                      AngleUnit anglesIn = AngleUnit::Radiants);
  // Rotation Get
  Magnum::Math::Vector3<float>
  getEulerAngels(AngleUnit anglesIn = AngleUnit::Radiants) const;
  Magnum::Math::Quaternion<float> getQuaternion() const;

  // Scaling
  Matrix &scale(const Magnum::Math::Vector3<float> &scaling,
                Apply order = Apply::Locally);
  Matrix &scale(float scaleBy, Apply order = Apply::Locally);

  Matrix &setScaling(const Magnum::Math::Vector3<float> &scaling);
  Matrix &setScaling(float scaleBy);

  Magnum::Math::Matrix4<float> &asMagnumMatrix();

private:
  Matrix(const Magnum::Math::Vector3<float> &translation)
      : Magnum::Math::Matrix4<float>(
            Magnum::Math::Matrix4<float>::translation(translation)) {};
};
} // namespace Tourmaline::Type

#endif
