# android_hidl
Some android hidl examples.

# led hidl examples








# hidl-gen help

- 参数说明：

    - -L： 语言类型，包括c++, c++-headers, c++-sources, export-header, c++-impl, java, java-constants, vts, makefile, androidbp, androidbp-impl, hash等。hidl-gen可根据传入的语言类型产生不同的文件。
    - fqname： 完全限定名称的输入文件。比如本例中android.hardware.gunder@1.0，要求在源码目录下必须有hardware/interfaces/ gunder /1.0/目录。对于单个文件来说，格式如下：package@version::fileName，比如android.hardware. gunder @1.0::types.Feature。对于目录来说。格式如下package@version，比如android.hardware. gunder @1.0。
    - -r： 格式package:path，可选，对fqname对应的文件来说，用来指定包名和文件所在的目录到Android系统源码根目录的路径。如果没有制定，前缀默认是：android.hardware，目录是Android源码的根目录。
    - -o：存放hidl-gen产生的中间文件的路径。


# 参考链接
- https://blog.xzr.moe/archives/99/