#!/bin/bash

case $OS in
  linux)
  {
    sudo apt-get update
    sudo apt-get install -y build-essential qtbase5-dev qt5-qmake qt5-default
  } ;;
  macOS)
  {
    brew update
    brew install qt5
    brew link --force qt5
  } ;;
esac