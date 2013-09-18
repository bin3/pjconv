# pjconv


pjconv is an easy to use converter between Protobuf and Json, written in C++.

## Build
pjconv uses cmake as the build tool.

```
mkdir -p build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<install-prefix> ..
make
make install
```
## Dependencies
* [protobuf](http://code.google.com/p/protobuf/)
* [jsoncpp](https://github.com/mrtazz/json-cpp)


## Usage

The API is very easy to use:

```
bool Convert(const google::protobuf::Message& message, Json::Value* json) const;
bool Convert(const google::protobuf::Message& message, std::string* json, bool styled = true) const;

bool Convert(const Json::Value& json, google::protobuf::Message* message) const;
bool Convert(const std::string& json, google::protobuf::Message* message) const;

```
## References
* [pb2json](https://github.com/renenglish/pb2json)
* [protobuf-to-jsoncpp](https://code.google.com/p/protobuf-to-jsoncpp/)