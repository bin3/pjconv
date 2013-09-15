/*
 * Copyright (c) 2013 Binson Zhang.
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @author	Binson Zhang <bin183cs@gmail.com>
 * @date		2013-9-15
 */

#include <gtest/gtest.h>

#include "pjconv/pjconv.h"
#include "pjconv/proto/addressbook.pb.h"

namespace pjconv {

static const std::string kJsonString = "{\"person\":[{\"email\":\"bin3@gmail.com\",\"id\":0,"
    "\"name\":\"bin3\",\"phone\":[{\"number\":\"10000\",\"type\":\"HOME\"},{\"number\":\"10001\","
    "\"type\":\"WORK\"}]}]}\n";

void Build(tutorial::AddressBook* ab) {
  tutorial::Person* person = ab->add_person();
  person->set_name("bin3");
  person->set_id(0);
  person->set_email("bin3@gmail.com");
  tutorial::Person::PhoneNumber* number = person->add_phone();
  number->set_number("10000");
  number->set_type(tutorial::Person::HOME);
  number = person->add_phone();
  number->set_number("10001");
  number->set_type(tutorial::Person::WORK);
}

void Check(const tutorial::AddressBook& ab) {
  ASSERT_EQ(1, ab.person_size());
  const tutorial::Person& person2 = ab.person(0);
  EXPECT_EQ("bin3", person2.name());
  ASSERT_EQ(2, person2.phone_size());
  EXPECT_EQ("10000", person2.phone(0).number());
  EXPECT_EQ("10001", person2.phone(1).number());
}

TEST(PJConverter, ConvertBetweenPbAndJson) {
  PJConverter conv;

  tutorial::AddressBook ab;
  Build(&ab);

  Json::Value json;
  ASSERT_TRUE(conv.Convert(ab, &json));
  Json::FastWriter writer;
  EXPECT_EQ(kJsonString, writer.write(json));

  tutorial::AddressBook ab2;
  ASSERT_TRUE(conv.Convert(json, &ab2));
  Check(ab2);
}

TEST(PJConverter, ConvertBetweenPbAndString) {
  PJConverter conv;

  tutorial::AddressBook ab;
  Build(&ab);

  std::string json;
  ASSERT_TRUE(conv.Convert(ab, &json, false));
  ASSERT_EQ(kJsonString, json);

  tutorial::AddressBook ab2;
  ASSERT_TRUE(conv.Convert(json, &ab2));
  Check(ab2);
}

}
  // namespace pjconv
