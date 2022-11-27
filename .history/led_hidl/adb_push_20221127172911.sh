#!/bin/bash
adb root
adb remount
adb shell mkdir /data/hidl/
adb push led_client /data/hidl/
adb push android.hardware.led@1.0-service /data/hidl/
adb push hidl_java.java /data/hidl/
adb push android.hardware.led@1.0-impl.so /vendor/lib64/hw/