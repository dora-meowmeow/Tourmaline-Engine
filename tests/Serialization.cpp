/*
 * SPDX-FileCopyrightText: Dora "cat" <cat@thenight.club>
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Corrade/Containers/Optional.h"
#include "Corrade/Containers/String.h"
#include "Corrade/TestSuite/Compare/String.h"
#include "Corrade/TestSuite/Tester.h"
#include "Corrade/Utility/Path.h"

#include "Tests.hpp"
#include "Tourmaline/Systems/Serialization.hpp"

using namespace Tourmaline;
using namespace Systems;

Corrade::Containers::String Tests::serialDataFileName = "Test_SerialData.data";

struct dummierData : public Serialization::Serializable<dummierData> {
  int16_t a;
  int8_t b;

  static inline SerialList serialList{&dummierData::b};
};

struct dummyData : public Serialization::Serializable<dummyData> {
  uint32_t x;
  int64_t y;
  dummierData z;
  static inline SerialList serialList{&dummyData::x, &dummyData::z};
};

void Tests::Serialization_CanSerialize() {
  dummyData data;
  data.x = 5;
  data.z.b = 'c';

  // These should be ignored in serialization
  data.y = 2408;
  data.z.a = 1234;

  dummyData sameData(data);

  // Does the same data serialize the same way?
  CORRADE_COMPARE_AS(data.Serialize(), sameData.Serialize(),
                     Corrade::TestSuite::Compare::String);

  // For Deserialization
  Corrade::Utility::Path::write(serialDataFileName,
                                Corrade::Containers::String(data.Serialize()));
}

void Tests::Serialization_CanDeserialize() {
  auto x = Corrade::Utility::Path::readString(serialDataFileName);
  dummyData data;
  data.x = 1;
  data.z.b = 'y';

  // These should be untouched in deserialization
  data.y = 2;
  data.z.a = 4321;
  data.Deserialize(x->data());

  CORRADE_VERIFY(data.x == 5 && data.y == 2 && data.z.a == 4321 &&
                 data.z.b == 'c');
}
