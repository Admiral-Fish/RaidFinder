#!/bin/bash

case $OS in
  linux)
  {
    mv build/RaidFinder .
    zip -r RaidFinder-linux.zip RaidFinder
    sha256sum RaidFinder-linux.zip > RaidFinder-linux.zip.sha256
  } ;;  
  macOS)
  {
    mv build/RaidFinder.app .
    PATH=$PATH:$HOME/Qt/6.1/macos/bin macdeployqt RaidFinder.app -dmg -verbose=2
    zip -r RaidFinder-macOS.zip RaidFinder.dmg
    shasum -a 256 RaidFinder-macOS.zip > RaidFinder-macOS.zip.sha256
  } ;;
esac