
# 1. 以控制LED为例进行demo的编写
## 1.1 编写led.hal 和types.hal
- 新建defalut文件夹
`mkdir -p android_hidl/led_hidl/hardware/interfaces/led/1.0/default`
- 使用hidl-gen生成代码
```bash
PACKAGE=android.led_hidl.hardware.led@1.0
LOC=vendor/android_hidl/led_hidl/hardware/interfaces/led/1.0/default/
hidl-gen -o $LOC -Lc++-impl -randroid.led_hidl.hardware:vendor/android_hidl/led_hidl/hardware/interfaces -randroid.hidl:system/libhidl/transport $PACKAGE
hidl-gen -o $LOC -Landroidbp-impl -randroid.led_hidl.hardware:vendor/android_hidl/led_hidl/hardware/interfaces -randroid.hidl:system/libhidl/transport $PACKAGE
```
在default路径下面就会生成Led.cpp,Led.h.
## 1.2 执行update-makefiles.sh 编译文件

- hardware/interfaces/update-makefiles.sh
需要将update-makefiles.sh中的路径改为当前interfaces的路径，可以从其他地方拷贝一个过来

## 1.3 实现android.hardware.led@1.0-impl和android.hardware.led@1.0-service
	编写android.bp
```
		cc_library_shared {
    name: "android.led_hidl.hardware.led@1.0-impl",
    defaults: ["hidl_defaults"],
    srcs:["Led.cpp"],
   // proprietary: true,
    relative_install_path: "hw",
    shared_libs: [
        "libhidlbase",
        "liblog",
        "libbase",
        "libutils",
        "libcutils",
        "android.led_hidl.hardware.led@1.0",
    ]
}

cc_binary {
    name: "android.led_hidl.hardware.led@1.0-service",
    srcs: [
        "Service.cpp",
        "Led.cpp",
    ],
    //proprietary: true,
    relative_install_path: "hw",
    init_rc: ["android.hardware.led@1.0-service.rc"],
    shared_libs: [
        "liblog",
        "libhidlbase",
        "libbase",
        "libutils",
        "libcutils",
        "android.led_hidl.hardware.led@1.0",
    ],
    vintf_fragments: ["manifest_android.hardware.led@1.0-service.xml"],
    cflags: [
        "-Werror",
        "-Wall",
        "-Wno-error=deprecated-declarations",
    ],
}
```
		接下来，使用相应功能填写存根并设置守护进程。守护进程代码（支持直通）示例：
```c
		#include <hidl/LegacySupport.h>

		int main(int /* argc */, char* /* argv */ []) {
			return defaultPassthroughServiceImplementation<INfc>("nfc");
		}
		defaultPassthroughServiceImplementation 将对提供的 -impl 库执行 dlopen() 操作，并将其作为绑定式服务提供。守护进程代码（对于纯绑定式服务）示例：

		int main(int /* argc */, char* /* argv */ []) {
			sp nfc = new Nfc();
			const status_t status = nfc->registerAsService();
			if (status != ::android::OK) {
				return 1; // or handle error
			}
			// join pool or do other things
		}
```
add the code to the manifest.xml 以至于hwservicemanager 查找到指定的hidl service
manifest.xml路径为:`device/rockchip/rk3399/manifest.xml`
```xml
    <hal format="hidl">
        <name>android_hidl.led_hidl.hardware.led</name>
        <transport>hwbinder</transport>
        <version>1.0</version>
        <interface>
            <name>ILed</name>
            <instance>default</instance>
        </interface>
    </hal>

```
# 1.4 添加selinux
system/sepolicy/vendor/hal_led_default.te:
```
type hal_led_default, domain;
hal_server_domain(hal_led_default, hal_light)

type hal_led_default_exec, exec_type, vendor_file_type, file_type;
init_daemon_domain(hal_led_default)
```

system/sepolicy/vendor/file_contexts:
```
/(vendor|system/vendor)/bin/hw/android\.led_hidl\.hardware\.led@1\.0-service          u:object_r:hal_led_default_exec:s0
```
# 1.5 运行log
service端log打印如下:
```log
# ./android.led_hidl.hardware.led@1.0-service                                    <
11-28 17:16:23.742   157   157 E SELinux : avc:  denied  { add } for interface=android.led_hidl.hardware.led::ILed sid=u:r:sudaemon:s0 pid=2709 scontext=u:r:sudaemon:s0 tcontext=u:object_r:default_android_hwservice:s0 tclass=hwservice_manager permissive=1
11-28 17:16:23.745  2709  2709 I ServiceManagement: Registered android.led_hidl.hardware.led@1.0::ILed/Led (start delay of 27ms)
11-28 17:16:23.745  2709  2709 I ServiceManagement: Removing namespace from process name android.led_hidl.hardware.led@1.0-service to led@1.0-service.
11-28 17:16:23.746  2709  2709 I android.led_hidl.hardware.led@1.0-service: Led service is ready

:/ # start led-hal-1-0
[   34.740239] init: Received control message 'start' for 'led-hal-1-0' from pid: 1620 (start led-hal-1-0)
[   34.741402] init: starting service 'led-hal-1-0'...
:/ #
```
client端打印如下:
```log

130|:/vendor/bin/hw # logcat |grep led_client &
[1] 1935 1936
:/vendor/bin/hw # [  110.009249] logd: logdr: UID=0 GID=0 PID=1935 b tail=0 logMask=19 pid=0 start=0ns timeout=0ns
11-28 18:19:04.984  1682  1682 E led_client: ILed ON
11-28 18:19:04.984  1682  1682 E led_client: ILed OFF
11-28 18:19:04.984  1682  1682 E led_client: ILed set
11-28 18:19:04.984  1682  1682 E led_client: ILed get
11-28 18:19:04.984  1682  1682 E led_client: ILed get: 0
11-28 18:19:04.984  1682  1682 E led_client: ILed getBrightnessRange ret: 1
11-28 18:19:04.984  1682  1682 E led_client: ILed getBrightnessRange Max: 100
11-28 18:19:04.984  1682  1682 E led_client: ILed getBrightnessRange Min: 1
11-28 18:19:04.985  1682  1682 E led_client: ILed getBrightnessValue bool: 0
```

# 2. 实现java调用hidl service的例子
##	2.1 添加库：
##	2.2 将以下内容添加到您的 Java 文件中：
		import android.hardware.foo.V1_0.IFoo;
		...
		// retry to wait until the service starts up if it is in the manifest
		IFoo server = IFoo.getService(true /* retry */); // throws NoSuchElementException if not available
		IFoo anotherServer = IFoo.getService("second_impl", true /* retry */);
		server.doSomething(…);


# 3. 实现C++ 调用hidl service的例子
android.led_hidl.hardware.led@1.0-service
android.led_hidl.hardware.led@1.0-impl.so
android.led_hidl.hardware.led@1.0.so
android.hardware.led@1.0-service.rc
manifest_android.hardware.led@1.0-service.xml

led_client

# 4. 最后生成的文件


# 5. 编写LED驱动

