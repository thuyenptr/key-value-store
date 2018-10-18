#!/usr/bin/env bash
protoc -I=protoc --cpp_out=build/gen protoc/Communication.proto