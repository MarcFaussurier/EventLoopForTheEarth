#!/usr/bin/env bash
source utils.sh
#======================
#   DEBIAN DEPS
#======================
sudo apt install  inotify-tools

#======================
#   CPP DEPS
#======================
cd ./libs;

## ---| lib name    branch          git repo |-------------------------- ##


#rang (Terminal colour utility)
install rang        master          https://github.com/agauniyal/rang.git;

# Catch2 (Unit testing framework)
install catch2      master          https://github.com/catchorg/Catch2.git;

# DotEnv (.env file parser)
install cpp-dotenv  master          https://github.com/adeharo9/cpp-dotenv.git;

# Yaml (.yml parser)
install yaml-cpp    master          https://github.com/jbeder/yaml-cpp.git;

# Json (A .json parser)
install json        master          https://github.com/kazuho/picojson.git;

# Serializer (Save / Load objects)
install serializer  master          https://github.com/eyalz800/serializer.git;


cd ./../
