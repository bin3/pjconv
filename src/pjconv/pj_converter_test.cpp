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

#include "pjconv/pj_converter.h"
#include "pjconv/proto/addressbook.pb.h"

namespace pjconv {

TEST(PJConverter, Convert) {
  PJConverter conv;
  tutorial::AddressBook ab;
  Json::Value json;

  tutorial::Person* person = ab.add_person();
  person->set_name("bin3");
  person->set_id(0);
  person->set_email("bin3@gmail.com");
  tutorial::Person::PhoneNumber* number = person->add_phone();
  number->set_number("10000");
  number->set_type(tutorial::Person::HOME);
  number = person->add_phone();
  number->set_number("10001");
  number->set_type(tutorial::Person::WORK);

  EXPECT_TRUE(conv.Convert(ab, &json));
  Json::FastWriter writer;
  EXPECT_EQ("{\"person\":[{\"email\":\"bin3@gmail.com\",\"id\":0,\"name\":\"bin3\",\"phone\":[{\"number\":\"10000\",\"type\":1},{\"number\":\"10001\",\"type\":2}]}]}\n",
            writer.write(json));
}

}
  // namespace pjconv
