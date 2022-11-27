#!/bin/bash
adb root
adb remount
adb shell mkdir /data/hidl/
adb push led_client /data/hidl/
adb push android.led_hidl.hardware.led@1.0-service /data/hidl/
adb push android.hardware.led@1.0-impl.so /vendor/lib64/hw/
adb push android.led_hidl.hardware.led@1.0-impl.so /system/lib64/hw/
adb push android.led_hidl.hardware.led@1.0-impl.so /vendor/lib64/hw/
adb push manifest_android.hardware.led@1.0-service.xml /vendor/
adb shell chmod 755 /vendor/bin/hw/android.led_hidl.hardware.led@1.0-service
adb shell export LD_LIBRARY_PATH=/vendor/lib64/hw:$LD_LIBRARY_PATH
