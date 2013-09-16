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

#ifndef PJCONV_PJ_CONVERTER_H_
#define PJCONV_PJ_CONVERTER_H_

#include <string>
#include <google/protobuf/message.h>
#include <json/json.h>

namespace pjconv {

/**
 * Protobuf and Json Converter
 */
class PJConverter {
 public:
  PJConverter();
  virtual ~PJConverter();

  /**
   * Convert a protobuf message to a JSON object
   *
   * @param message the input protobuf message
   * @param json the output JSON object
   * @param whether to convert the unset fields in the protobuf message
   * @return true if convert successfully
   */
  bool Convert(
      const google::protobuf::Message& message,
      Json::Value* json,
      bool convert_unset_fields = true) const;

  /**
   * Convert a protobuf message to a JSON string
   *
   * @param message the input protobuf message
   * @param json the output JSON string
   * @param whether to convert the unset fields in the protobuf message
   * @param styled whether to format the output string in a human friendly way
   * @return true if convert successfully
   */
  bool Convert(
      const google::protobuf::Message& message,
      std::string* json,
      bool styled = true,
      bool convert_unset_fields = true) const;

  /**
   * Convert a JSON object to a protobuf message
   *
   * @param json the input JSON object
   * @param message the output protobuf message
   * @return true if convert successfully
   */
  bool Convert(const Json::Value& json, google::protobuf::Message* message) const;

  /**
   * Convert a JSON string to a protobuf message
   *
   * @param json the input JSON string
   * @param message the output protobuf message
   * @return true if convert successfully
   */
  bool Convert(const std::string& json, google::protobuf::Message* message) const;

 private:
  void ConvertFromMessage(const google::protobuf::Message& message, Json::Value* json) const;

  void ConvertFromSingelField(
      const google::protobuf::Message& message,
      const google::protobuf::Reflection *ref,
      const google::protobuf::FieldDescriptor* field,
      Json::Value* json) const;

  void ConvertFromRepeatedField(
      const google::protobuf::Message& message,
      const google::protobuf::Reflection *ref,
      const google::protobuf::FieldDescriptor* field,
      Json::Value* json) const;

  void ConvertToMessage(const Json::Value& json, google::protobuf::Message* message) const;

  void ConvertToSingleField(
      const Json::Value& json,
      google::protobuf::Message* message,
      const google::protobuf::Descriptor* desc,
      const google::protobuf::Reflection* ref,
      const google::protobuf::FieldDescriptor* field) const;

  void ConvertToRepeatedField(
      const Json::Value& json,
      google::protobuf::Message* message,
      const google::protobuf::Descriptor* desc,
      const google::protobuf::Reflection* ref,
      const google::protobuf::FieldDescriptor* field) const;

  template<typename Checker, typename Getter, typename Setter>
  void SetField(
      const Json::Value& json,
      google::protobuf::Message* message,
      const google::protobuf::Reflection* ref,
      const google::protobuf::FieldDescriptor* field,
      Checker checker,
      Getter getter,
      Setter setter) const;

  template<typename Setter>
  void SetEnumField(
      const Json::Value& json,
      google::protobuf::Message* message,
      const google::protobuf::Descriptor* desc,
      const google::protobuf::Reflection* ref,
      const google::protobuf::FieldDescriptor* field,
      Setter setter) const;

  mutable bool convert_unset_fields_;
};

}  // namespace pjconv
#endif  // PJCONV_PJ_CONVERTER_H_
