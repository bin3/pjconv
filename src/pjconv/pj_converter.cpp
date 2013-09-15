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

bool PJConverter::Convert(const pb::Message& message, Json::Value* json) const {
  if (!json) return false;
  json->clear();
  ConvertFromMessage(message, json);
  return true;
}

bool PJConverter::Convert(const pb::Message& message, std::string* json, bool styled) const {
  if (!json) return false;
  Json::Value value;
  bool ret = Convert(message, &value);
  if (ret) {
    if (styled) {
      Json::StyledWriter writer;
      *json = writer.write(value);
    } else {
      Json::FastWriter writer;
      *json = writer.write(value);
    }
  }
  return ret;
}

bool PJConverter::Convert(const Json::Value& json, pb::Message* message) const {
  if (!message) return false;
  message->Clear();
  ConvertToMessage(json, message);
  return true;
}

bool PJConverter::Convert(const std::string& json, pb::Message* message) const {
  if (!message) return false;
  Json::Reader reader;
  Json::Value value;
  bool ret = reader.parse(json, value, false);
  if (ret) {
    ret = Convert(value, message);
  }
  return ret;
}

void PJConverter::ConvertFromMessage(const pb::Message& message, Json::Value* json) const {
  const pb::Descriptor* desc = message.GetDescriptor();
  const pb::Reflection *ref = message.GetReflection();

  Json::Value& out = *json;
  int n = desc->field_count();
  for (int i = 0; i < n; ++i) {
    const pb::FieldDescriptor* field = desc->field(i);
    const std::string& name = field->name();
    if (field->is_repeated()) {
      ConvertFromRepeatedField(message, ref, field, &out[name]);
    } else {
      ConvertFromSingelField(message, ref, field, &out[name]);
    }
  }
}

void PJConverter::ConvertFromSingelField(
    const pb::Message& message,
    const pb::Reflection* ref,
    const pb::FieldDescriptor* field,
    Json::Value* json) const {
  switch (field->cpp_type()) {
    case pb::FieldDescriptor::CPPTYPE_INT32:
      *json = ref->GetInt32(message, field);
      break;
    case pb::FieldDescriptor::CPPTYPE_INT64:
      *json = ref->GetInt64(message, field);
      break;
    case pb::FieldDescriptor::CPPTYPE_UINT32:
      *json = ref->GetUInt32(message, field);
      break;
    case pb::FieldDescriptor::CPPTYPE_UINT64:
      *json = ref->GetUInt64(message, field);
      break;
    case pb::FieldDescriptor::CPPTYPE_DOUBLE:
      *json = ref->GetDouble(message, field);
      break;
    case pb::FieldDescriptor::CPPTYPE_FLOAT:
      *json = ref->GetFloat(message, field);
      break;
    case pb::FieldDescriptor::CPPTYPE_BOOL:
      *json = ref->GetBool(message, field);
      break;
    case pb::FieldDescriptor::CPPTYPE_ENUM:
      *json = ref->GetEnum(message, field)->name();
      break;
    case pb::FieldDescriptor::CPPTYPE_STRING:
      *json = ref->GetString(message, field);
      break;
    case pb::FieldDescriptor::CPPTYPE_MESSAGE:
      const pb::Message& m = ref->GetMessage(message, field);
      ConvertFromMessage(m, json);
      break;
  }
}

void PJConverter::ConvertFromRepeatedField(
    const pb::Message& message,
    const pb::Reflection* ref,
    const pb::FieldDescriptor* field,
    Json::Value* json) const {
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
        ConvertFromMessage(m, &value);
        json->append(value);
      }
      break;
  }
}

void PJConverter::ConvertToMessage(const Json::Value& json, pb::Message* message) const {
  const pb::Descriptor* desc = message->GetDescriptor();
  const pb::Reflection *ref = message->GetReflection();
  for (Json::ValueIterator iter = json.begin(); iter != json.end(); ++iter) {
    const pb::FieldDescriptor* field = desc->FindFieldByName(iter.memberName());
    if (!field) continue;
    if (field->label() == pb::FieldDescriptor::LABEL_REPEATED) {
      ConvertToRepeatedField(*iter, message, desc, ref, field);
    } else {
      ConvertToSingleField(*iter, message, desc, ref, field);
    }
  }
}

void PJConverter::ConvertToSingleField(
    const Json::Value& json,
    pb::Message* message,
    const pb::Descriptor* desc,
    const pb::Reflection* ref,
    const pb::FieldDescriptor* field) const {
  switch (field->cpp_type()) {
    case pb::FieldDescriptor::CPPTYPE_INT32:
      SetField(json, message, ref, field, &Json::Value::isIntegral, &Json::Value::asInt,
               &pb::Reflection::SetInt32);
      break;
    case pb::FieldDescriptor::CPPTYPE_INT64:
      SetField(json, message, ref, field, &Json::Value::isIntegral, &Json::Value::asInt64,
               &pb::Reflection::SetInt64);
      break;
    case pb::FieldDescriptor::CPPTYPE_UINT32:
      SetField(json, message, ref, field, &Json::Value::isIntegral, &Json::Value::asUInt,
               &pb::Reflection::SetUInt32);
      break;
    case pb::FieldDescriptor::CPPTYPE_UINT64:
      SetField(json, message, ref, field, &Json::Value::isIntegral, &Json::Value::asUInt64,
               &pb::Reflection::SetUInt64);
      break;
    case pb::FieldDescriptor::CPPTYPE_DOUBLE:
      SetField(json, message, ref, field, &Json::Value::isNumeric, &Json::Value::asDouble,
               &pb::Reflection::SetDouble);
      break;
    case pb::FieldDescriptor::CPPTYPE_FLOAT:
      SetField(json, message, ref, field, &Json::Value::isNumeric, &Json::Value::asFloat,
               &pb::Reflection::SetFloat);
      break;
    case pb::FieldDescriptor::CPPTYPE_BOOL:
      SetField(json, message, ref, field, &Json::Value::isBool, &Json::Value::asBool,
               &pb::Reflection::SetBool);
      break;
    case pb::FieldDescriptor::CPPTYPE_ENUM:
      SetEnumField(json, message, desc, ref, field, &pb::Reflection::SetEnum);
      break;
    case pb::FieldDescriptor::CPPTYPE_STRING:
      SetField(json, message, ref, field, &Json::Value::isString, &Json::Value::asString,
               &pb::Reflection::SetString);
      break;
    case pb::FieldDescriptor::CPPTYPE_MESSAGE:
      ConvertToMessage(json, ref->MutableMessage(message, field));
      break;
  }
}

void PJConverter::ConvertToRepeatedField(
    const Json::Value& json,
    pb::Message* message,
    const pb::Descriptor* desc,
    const pb::Reflection* ref,
    const pb::FieldDescriptor* field) const {
  switch (field->cpp_type()) {
    case pb::FieldDescriptor::CPPTYPE_INT32:
      for (Json::ValueIterator iter = json.begin(); iter != json.end(); ++iter) {
        SetField(*iter, message, ref, field, &Json::Value::isIntegral, &Json::Value::asInt,
                 &pb::Reflection::AddInt32);
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_INT64:
      for (Json::ValueIterator iter = json.begin(); iter != json.end(); ++iter) {
        SetField(*iter, message, ref, field, &Json::Value::isIntegral, &Json::Value::asInt64,
                 &pb::Reflection::AddInt64);
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_UINT32:
      for (Json::ValueIterator iter = json.begin(); iter != json.end(); ++iter) {
        SetField(*iter, message, ref, field, &Json::Value::isIntegral, &Json::Value::asUInt,
                 &pb::Reflection::AddUInt32);
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_UINT64:
      for (Json::ValueIterator iter = json.begin(); iter != json.end(); ++iter) {
        SetField(*iter, message, ref, field, &Json::Value::isIntegral, &Json::Value::asUInt64,
                 &pb::Reflection::AddUInt64);
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_DOUBLE:
      for (Json::ValueIterator iter = json.begin(); iter != json.end(); ++iter) {
        SetField(*iter, message, ref, field, &Json::Value::isNumeric, &Json::Value::asDouble,
                 &pb::Reflection::AddDouble);
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_FLOAT:
      for (Json::ValueIterator iter = json.begin(); iter != json.end(); ++iter) {
        SetField(*iter, message, ref, field, &Json::Value::isNumeric, &Json::Value::asFloat,
                 &pb::Reflection::AddFloat);
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_BOOL:
      for (Json::ValueIterator iter = json.begin(); iter != json.end(); ++iter) {
        SetField(*iter, message, ref, field, &Json::Value::isBool, &Json::Value::asBool,
                 &pb::Reflection::AddBool);
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_ENUM:
      for (Json::ValueIterator iter = json.begin(); iter != json.end(); ++iter) {
        SetEnumField(*iter, message, desc, ref, field, &pb::Reflection::AddEnum);
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_STRING:
      for (Json::ValueIterator iter = json.begin(); iter != json.end(); ++iter) {
        SetField(*iter, message, ref, field, &Json::Value::isString, &Json::Value::asString,
                 &pb::Reflection::AddString);
      }
      break;
    case pb::FieldDescriptor::CPPTYPE_MESSAGE:
      for (Json::ValueIterator iter = json.begin(); iter != json.end(); ++iter) {
        ConvertToMessage(*iter, ref->AddMessage(message, field));
      }
      break;
  }
}

template<typename Checker, typename Getter, typename Setter>
void PJConverter::SetField(
    const Json::Value& json,
    pb::Message* message,
    const pb::Reflection* ref,
    const pb::FieldDescriptor* field,
    Checker checker,
    Getter getter,
    Setter setter) const {
  if ((json.*checker)()) {
    (ref->*setter)(message, field, (json.*getter)());
  }
}

template<typename Setter>
void PJConverter::SetEnumField(
    const Json::Value& json,
    pb::Message* message,
    const pb::Descriptor* desc,
    const pb::Reflection* ref,
    const pb::FieldDescriptor* field,
    Setter setter) const {
  const pb::EnumDescriptor* enum_desc = desc->FindEnumTypeByName(field->full_name());
  if (enum_desc) {
    if (json.isString()) {
      const pb::EnumValueDescriptor* value = enum_desc->FindValueByName(json.asString());
      if (value) (ref->*setter)(message, field, value);
    } else if (json.isIntegral()) {
      const pb::EnumValueDescriptor* value = enum_desc->FindValueByNumber(json.asInt());
      if (value) (ref->*setter)(message, field, value);
    }
  }
}

}  // namespace pjconv
