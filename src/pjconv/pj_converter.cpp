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

#include <google/protobuf/descriptor.h>

#include "pj_converter.h"

namespace pjconv {

namespace pb = google::protobuf;

PJConverter::PJConverter() {
}

PJConverter::~PJConverter() {
}

bool PJConverter::Convert(const google::protobuf::Message& message,
                          Json::Value* json) const {
  if (!json)
    return false;
  json->clear();
  return ConvertMessage(message, json);
}

bool PJConverter::Convert(const Json::Value& json,
                          google::protobuf::Message* pb) const {
  return true;
}

bool PJConverter::ConvertMessage(const google::protobuf::Message& message,
                                 Json::Value* json) const {
  const pb::Descriptor* desc = message.GetDescriptor();
  const pb::Reflection *ref = message.GetReflection();
  if (!desc || !ref)
    return false;

  Json::Value& out = *json;
  int n = desc->field_count();
  for (int i = 0; i < n; ++i) {
    const pb::FieldDescriptor* field = desc->field(i);
    const std::string& name = field->name();
    if (field->is_repeated()) {
      ConvertRepeated(message, ref, field, &out[name]);
    } else {
      switch (field->cpp_type()) {
        case pb::FieldDescriptor::CPPTYPE_INT32:
          out[name] = ref->GetInt32(message, field);
          break;
        case pb::FieldDescriptor::CPPTYPE_INT64:
          out[name] = ref->GetInt64(message, field);
          break;
        case pb::FieldDescriptor::CPPTYPE_UINT32:
          out[name] = ref->GetUInt32(message, field);
          break;
        case pb::FieldDescriptor::CPPTYPE_UINT64:
          out[name] = ref->GetUInt64(message, field);
          break;
        case pb::FieldDescriptor::CPPTYPE_DOUBLE:
          out[name] = ref->GetDouble(message, field);
          break;
        case pb::FieldDescriptor::CPPTYPE_FLOAT:
          out[name] = ref->GetFloat(message, field);
          break;
        case pb::FieldDescriptor::CPPTYPE_BOOL:
          out[name] = ref->GetBool(message, field);
          break;
        case pb::FieldDescriptor::CPPTYPE_ENUM:
          out[name] = ref->GetEnum(message, field)->number();
          break;
        case pb::FieldDescriptor::CPPTYPE_STRING:
          out[name] = ref->GetString(message, field);
          break;
        case pb::FieldDescriptor::CPPTYPE_MESSAGE:
          const pb::Message& m = ref->GetMessage(message, field);
          ConvertMessage(m, &(out[name]));
          break;
      }
    }
  }
  return true;
}

void PJConverter::ConvertRepeated(
    const google::protobuf::Message& message,
    const google::protobuf::Reflection* ref,
    const google::protobuf::FieldDescriptor* field, Json::Value* json) const {
  int n = ref->FieldSize(message, field);
  switch (field->cpp_type()) {
    case pb::FieldDescriptor::CPPTYPE_INT32:
      for (int i = 0; i < n; ++i) {
        json->append(ref->GetRepeatedInt32(message, field, i));
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_INT64:
      for (int i = 0; i < n; ++i) {
        json->append(ref->GetRepeatedInt64(message, field, i));
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_UINT32:
      for (int i = 0; i < n; ++i) {
        json->append(ref->GetRepeatedUInt32(message, field, i));
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_UINT64:
      for (int i = 0; i < n; ++i) {
        json->append(ref->GetRepeatedUInt64(message, field, i));
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_DOUBLE:
      for (int i = 0; i < n; ++i) {
        json->append(ref->GetRepeatedDouble(message, field, i));
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_FLOAT:
      for (int i = 0; i < n; ++i) {
        json->append(ref->GetRepeatedFloat(message, field, i));
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_BOOL:
      for (int i = 0; i < n; ++i) {
        json->append(ref->GetRepeatedBool(message, field, i));
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_ENUM:
      for (int i = 0; i < n; ++i) {
        json->append(ref->GetRepeatedEnum(message, field, i)->number());
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_STRING:
      for (int i = 0; i < n; ++i) {
        json->append(ref->GetRepeatedString(message, field, i));
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_MESSAGE:
      for (int i = 0; i < n; ++i) {
        const pb::Message& m = ref->GetRepeatedMessage(message, field, i);
        Json::Value value;
        ConvertMessage(m, &value);
        json->append(value);
      }
      break;
  }
}

}  // namespace pjconv
