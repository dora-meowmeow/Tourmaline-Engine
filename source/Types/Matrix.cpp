/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Magnum/Math/Angle.h"
#include "Magnum/Math/Math.h"
#include "Magnum/Math/Matrix4.h"
#include "Magnum/Math/Quaternion.h"

#include "Magnum/Math/Vector3.h"
#include "Types/Matrix.hpp"
#include "Types/Units.hpp"

using namespace Tourmaline;
using namespace Magnum::Math;

Vector3<float>
Type::Matrix::getEulerAngels(Tourmaline::Type::AngleUnit anglesIn) const {
  Vector3<Rad<float>> result =
      Magnum::Math::Quaternion<float>::fromMatrix(this->rotation()).toEuler();

  if (anglesIn == AngleUnit::Radiants) {
    return Vector3<float>(result);
  }
  return Vector3<float>(Vector3<Deg<float>>(result));
}

Type::Matrix &Type::Matrix::rotateEuler(const Vector3<float> &angels,
                                        Apply order, AngleUnit anglesIn) {
  Matrix4<float> rotation;
  if (anglesIn == AngleUnit::Radiants) [[likely]] {
    rotation = Matrix4::rotationX(Rad(angels.x())) *
               Matrix4::rotationY(Rad(angels.y())) *
               Matrix4::rotationZ(Rad(angels.z()));
  } else {
    rotation = Matrix4::rotationX(Deg(angels.x())) *
               Matrix4::rotationY(Deg(angels.y())) *
               Matrix4::rotationZ(Deg(angels.z()));
  }

  if (order == Apply::Locally) {
    *this = *this * rotation;
    return *this;
  }

  *this = rotation * *this;
  return *this;
}

Magnum::Math::Quaternion<float> Type::Matrix::getQuaternion() const {
  return Magnum::Math::Quaternion<float>::fromMatrix(this->rotation());
}

Type::Matrix &Type::Matrix::rotate(float angle,
                                   Magnum::Math::Vector3<float> rotationAxis,
                                   Apply order, AngleUnit anglesIn) {
  Quaternion<float> rotation;
  if (anglesIn == AngleUnit::Radiants) [[likely]] {
    rotation = Quaternion<float>::rotation(Rad<float>(angle),
                                           rotationAxis.normalized());
  } else {
    rotation = Quaternion<float>::rotation(Rad<float>(Deg<float>(angle)),
                                           rotationAxis.normalized());
  }

  return rotateQuaternion(rotation, order);
}

Type::Matrix &
Type::Matrix::rotateQuaternion(Magnum::Math::Quaternion<float> quaternion,
                               Apply order) {
  static Vector3<float> center{0.0f};
  Matrix4<float> rotation = Matrix4<float>::from(quaternion.toMatrix(), center);

  if (order == Apply::Locally) {
    *this = *this * rotation;
    return *this;
  }
  *this = rotation * *this;

  return *this;
}

Type::Matrix &
Type::Matrix::translate(const Magnum::Math::Vector3<float> &position,
                        Apply order) {
  if (order == Apply::Locally) {
    *this = *this * Matrix::translation(position);
    return *this;
  }
  *this = Matrix::translation(position) * *this;
  return *this;
}

Type::Matrix &
Type::Matrix::setTranslation(const Magnum::Math::Vector3<float> &position) {
  this->translation() = position;
  return *this;
}

Type::Matrix &Type::Matrix::scale(const Magnum::Math::Vector3<float> &scaling,
                                  Apply order) {
  if (order == Apply::Locally) {
    *this = *this * Matrix::scaling(scaling);
    return *this;
  }
  *this = Matrix::scaling(scaling) * *this;
  return *this;
}

Type::Matrix &Type::Matrix::scale(float scaleBy, Apply order) {
  return scale(Vector3<float>{scaleBy}, order);
}

Type::Matrix &
Type::Matrix::setScaling(const Magnum::Math::Vector3<float> &scaling) {
  Matrix3x3<float> top = this->rotation();
  // Trying best to preserve rotation
  for (uint8_t index = 0; index < 3; index++) {
    (*this)[index][index] = scaling[index] * top[index][index];
  }
  return *this;
}

Type::Matrix &Type::Matrix::setScaling(float scaleBy) {
  return setScaling(Vector3<float>{scaleBy});
}

Magnum::Math::Matrix4<float> &Type::Matrix::asMagnumMatrix() {
  return *static_cast<Magnum::Math::Matrix4<float> *>(this);
}
