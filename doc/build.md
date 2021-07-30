# Linephone SDK Build

用于记录编译linphone库时的问题和细节

- [Linephone SDK Build](#linephone-sdk-build)
  - [准备工作](#准备工作)
  - [编译选项](#编译选项)
    - [编译选项说明](#编译选项说明)
  - [Windows](#windows)
    - [准备环境](#准备环境)
    - [修改错误脚本](#修改错误脚本)
    - [找不到sh.exe错误(Cannot find the rxvt.exe or sh.exe binary -- aborting.)](#找不到shexe错误cannot-find-the-rxvtexe-or-shexe-binary----aborting)
    - [vpx编译的相关错误](#vpx编译的相关错误)
    - [openh264编译的相关错误](#openh264编译的相关错误)
    - [ffmpeg编译的相关错误](#ffmpeg编译的相关错误)
    - [ms2编译的相关错误](#ms2编译的相关错误)
    - [msopenh264的相关错误](#msopenh264的相关错误)
    - [linphone的相关错误](#linphone的相关错误)
  - [Linux](#linux)
    - [QT GL](#qt-gl)

## 准备工作
仓库地址[linphone-sdk](https://github.com/BelledonneCommunications/linphone-sdk)  
相关依赖和工具版本都可在reademe中看到,下面说明一下各个平台编译的坑  

## 编译选项
可以使用目录下的linphone_build.sh脚本进行cmake,然后打开xxx.sln,调用visual studio来编译  
有一些模块我并不需要用到(聊天系统),所以我只选择编译了一部分  
* cmake \\  
-DENABLE_NON_FREE_CODECS=true \\  
-DENABLE_OPENH264=true \\  
-DENABLE_MKV=true \\  
-DENABLE_VIDEO=true \\  
-DENABLE_FFMPEG=ture \\  
-DENABLE_QT_GL=true \\  
-DENABLE_BV16=false \\  
-DENABLE_GSM=false \\  
-DENABLE_ILBC=false \\  
-DENABLE_LIME_X3DH=false \\  
-DENABLE_ADVANCED_IM=false \\  
-DENABLE_DB_STORAGE=false \\  
-DENABLE_VCARD=false \\  
-DENABLE_UNIT_TESTS=false \\  
-DENABLE_DEBUG_LOGS=false  \\  
-DENABLE_DOC=false  \\  
../linphone-sdk

### 编译选项说明
|  选项     |  作用    | 
|   :----   |  :----  | 
| NON_FREE_CODECS	    | 这个和H264绑定,使用h264编解码器则需要添加该选
| OPENH264	          | 这个不需要多解释,视频编解码器
| MKV	                | 这个用来录像用的,保存为MKV格式(demo中未实现录像功能,我暂时用不到)
| VIDEO	             | 视频通话,这个好像不需要显示设置,默认为tru
| FFMPEG	             | 这个用于缩放图片,在渲染自己的摄像头时,会使用到缩放,所以如果不加这个则会出现预览自己摄像头时崩溃
| QT_GL               | qt渲染视频使用,不添加这个则无法使用MSQOGL,无法在qt的窗口上渲染视频
| BV16,GSM,ILBC,LIME  | 音频编解码器,默认带的有点多,去掉几个,压缩体积
| ADVANCED_IM         | 聊天模块,不需要
| DB_STORAGE          | 用数据库保存聊天信息,不需要
| VCARD               | 这个库好像是名片吧?,使用这个之后会导致一个路径问题,正好,我不需要
| V4L                 | Linux使用,用于捕捉视频
| 剩下的比较好认就不说明了

************************


## Windows
这个库在windows下编译真的是问题巨多,编译完成后linphone++只有三个头文件,应该是doxygen生成文档时出错,导致C++头文件生成失败  

### 准备环境
+ 根据官网准备好环境,需要visual studio和msys  
+ 由于直接使用mingw编译会出现各种头文件错误,所以只支持msvc编译  
+ msys只是提供一个sh环境,如果已经有sh.exe,则不需要msys也可以  
+ 以下编译都在visual studio上,不推荐使用其他方式编译,因为visual studio带有整个编译套件的环境,路径等  
+ 我自己编译的环境是win10 + vs2015 + msys,要注意visual studio需要update3和window10 SDK 10586  

### 修改错误脚本
cmake执行过后,实际编译时可能会出现各种错误,下面我根据我遇到的错误进行纠正  

### 找不到sh.exe错误(Cannot find the rxvt.exe or sh.exe binary -- aborting.)  
> 该错误是由于脚本路径写死了C:\MinGW\msys\1.0\bin\,有两种解决方案,一种是将自己的msys放到该目录,这个可能会对原来的编译环境造成破坏  
下面推荐另一种做法,这个要提前将sh.exe放到环境变量中
> 1. 首先进到脚本目录,在根目录下的cmake-builder\cmake
> 2. 脚本文件是mingw_***.bat.cmake的三个文件
> 3. 打开脚本文件,将最下面两行(:startsh下面的两行),删掉第一行,将第二行的%WD%sh改为sh.exe  
> if NOT EXIST %WD%sh.exe goto notfound  
> start /W %WD%sh --login -i @CMAKE_CURRENT_BINARY_DIR@/EP_@MSVC_PROJNAME@_install.sh  
> 改成  
> start /W sh.exe --login -i @CMAKE_CURRENT_BINARY_DIR@/EP_@MSVC_PROJNAME@_install.sh

### vpx编译的相关错误
> vpx的相关编译脚本在desktop目录下,错误日志在EP_vpx.log  
> 为了方便,自己编译vpx,然后屏蔽构建系统脚本中对vpx的编译  
> 1) 先将linphone-sdk/external/libvpx的文件拷贝到构建目录中的./WORK/desktop/Build/vpx
>     * build_path=C:/msys64/home/qida/build
>     * src_path=C:/msys64/home/qida/linphone-sdk
> 2) 将linphone-sdk/external/libvpx的文件拷贝到构建目录中的./WORK/desktop/Build/vpx
>     * cd ${build_path}/WORK/desktop/Build/vpx
>     * cp -rf ${src_path}/external/libvpx/* .
> 3) 参考项目中构建vpx的参数,输入下面的命令(在linphone-sdk/cmake-builder/builders/vpx/windows_configure.sh.cmake)
>     * ./configure  "--enable-error-concealment" "--enable-multithread" "--enable-realtime-only" "--enable-spatial-resampling" "--enable-vp8" "--disable-vp9" "--enable-libs" "--disable-install-docs" "--disable-debug-libs" "--disable-examples" "--disable-unit-tests" "--disable-tools" "--as=auto" "--prefix=${build_path}/linphone-sdk/desktop" "--target=x86-win32-vs14"
> 4) 导入MSBuild.exe的路径变量(需要注意一点,如果先编译openh264,可能会导致路径污染,重启msys即可)
>     * export PATH="$PATH:/c/Program Files (x86)/MSBuild/14.0/Bin"
> 5) 可能存在的一个问题,需要修改MakeFile(执行make命令时报340行文件找不到)
>     * 第67行VPATH=$(SRC_PATH_BARE)上面添加下面这一行(vpx的构建完整路径)
>     * SRC_PATH_BARE=C:/msys64/home/qida/build/WORK/desktop/Build/vpx
> 6) 执行make,编译成功后,执行make install
> 7) 使用这个方法则需要注意要将vpx的build脚本(linphone-sdk/cmake-builder/builders/vpx/)注释掉执行命令,因为这个我们手动编译了,不要让错误中断整体的cmake

### openh264编译的相关错误
> openh264的编译和vpx很类似,相关编译脚本在desktop目录下,错误日志在EP_openh264.log  
> 之前走了一个误区,采用python的ninja系统构建,这个默认编译的是mingw版本,和msvc不兼容,所以采用make编译  
> 使用make编译需要注意环境变量的问题,需要导入msvc所需的exe,lib和include路径
> 先找到openh264的脚本构建路径,在linphone-sdk/cmake-builder/builders/openh264
> 1) 先修改build.sh.cmake,在cd前面添加下面三行环境变量导入(具体路径看自己环境)  
>    - export PATH="$PATH:/c/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin:/c/Program Files (x86)/Microsoft Visual Studio 14.0/Common7/IDE;C:\Program Files (x86)\Windows Kits\10\bin\x86"
export INCLUDE="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include;C:\Program Files (x86)\Windows Kits\10\Include\10.0.10586.0\um;C:\Program Files (x86)\Windows Kits\10\Include\10.0.10586.0\shared;C:\Program Files (x86)\Windows Kits\10\Include\10.0.10586.0\ucrt"
export LIB="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10586.0\um\x86;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10586.0\ucrt\x86;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib"
> 2) 在cd后面添加一行,用于清空临时文件  
>    - rm -rf ./*
> 3) 在最后一行的@EP_openh264_ADDITIONAL_OPTIONS@后面添加一个参数  
>    - ARCH="i386"
> 4) 修改install.sh.cmake文件,和步骤3一样添加一个参数

### ffmpeg编译的相关错误
> ffmpeg的编译和vpx很类似,相关编译脚本在desktop目录下,错误日志在EP_ffmpeg.log  
> 解决思路和vpx差不多,需要注意,ffmpeg的脚本生成在linphone-sdk/cmake-builder/cmake下  
> 不过该脚本应该是通用脚本,修改起来需要额外小心,目前我们用到的功能只有ffmpeg使用到该脚本,所以不需要担心  
> 1) 修改configure.sh.cmake文件,把后面的逻辑代码注释掉,并添加如下代码(注意路径修改,可以提前编译一次即可拿到生成之后的路径,直接复制)  
>    + cd "@ep_build@"  
>    + build_path=C:/msys64/home/qida/build
>    + cp -r "C:/msys64/home/qida/linphone-sdk/external/ffmpeg" ../
>    + if [ ! -f "config.h" ] 
>    + then
>	  + ./configure  "--prefix=${build_path}/linphone-sdk/desktop" "--libdir=${build_path}/linphone-sdk/desktop/lib" "--shlibdir=${build_path}/linphone-sdk/desktop/bin" "--arch=i386" "--target-os=mingw32" "--disable-static" "--enable-shared" "--disable-doc" "--disable-zlib" "--disable-bzlib" "--disable-ffplay" "--disable-ffprobe" "--disable-ffserver" "--disable-avdevice" "--disable-avfilter" "--disable-network" "--disable-avformat" "--disable-everything" "--enable-decoder=mjpeg" "--enable-encoder=mjpeg" "--disable-vda" "--disable-vaapi" "--disable-vdpau" "--ar=$AR" "--nm=$NM" "--extra-cflags=$CFLAGS -w" "--extra-cxxflags=$CXXFLAGS" "--extra-ldflags=$LDFLAGS" "--cc=gcc" "--enable-runtime-cpudetect" 2>&1 >> "${build_path}/desktop/EP_ffmpeg.log"
>    + fi
> 2) 修改build.sh.cmake文件,把V=xxxx这一串删除,添加-j4
> 3) 修改install.sh.cmake文件,把V=xxxx这一串删除
   

### ms2编译的相关错误
> + 找不到QTANGLE
>    - 这个简单,直接修改mediastreamer2/cmake/FindQtAngle.cmake,添加下面这句 
>    - set(QTANGLE_INCLUDE_DIRS "C:/Qt/Qt5.9.3/5.9.3/msvc2015/include/")
> * 警告被视为错误
>    - 进到源码mediastreamer2文件夹,将CMakeLists.txt的/WX注释了,大概在645行
> * The following variables are used in this project, but they are set to NOTFOUND.
>    + 将WORK\desktop\Build下的ms2文件夹删了或者将该文件夹里的CMakeCache删除
> * 出现H265未定义的错误
>    - 将出错的地方注释掉,用不上265
> * 添加QT GL选项后的qt依赖库问题
>    + 分系统来说明一下<br>
>        1) Windows
>           * 大部分步骤和Linux差不多
>           * 有一个GLsizeiptr重定义的问题
>           * 将ms2的头文件定义改成qt的头文件定义
>           * Windows下也有qt路径的问题,参考Linux方案
>        2) Linux
>           * 这个处理方案和fcitx方案差不多,都是添加一个qt库查找路径即可
>           * 先打开mediastreamer2的CMakeLists.txt,查找if(ENABLE_QT_GL)
>           * 在find_package前添加set(CMAKE_PREFIX_PATH /home/zhr/Qt5.9.3/5.9.3/gcc_64/lib/cmake)
>           * 改完后,可能会出现错误,则把find_package的OPTIONAL去掉和QmlModels模块去掉(5.9没有QmlModels模块)
>           * 如果gcc版本在8.0以上,还可能会报错误,需要添加-Wclass-memaccess编译选项
>           * 在STAICT_OPTIONS_CPP定义的那段代码中最后添加编译选项list(APPEND STAICT_OPTIONS_CPP "-Wclass-memaccess")

### msopenh264的相关错误
> + Could NOT find OpenH264
>    + 和找不到QTANGLE一样,脚本在msopenh264/cmake/FindOpenH264.cmake
>    + 在第47行,lib路径判断时加上openh264_dll即可

### linphone的相关错误
> 这个比较麻烦,需要直接编译,sln在build/WORK/desktop/Build/linphone
>    * 报C3431错误,作用域问题
>        * 是因为该语法msvc不支持,需要做一些小调整
>        * 直接从vs错误导向源代码,在src/chat/chat-room/chat-room-params.h
>        * 注释掉这两行代码并添加如下两行代码,分别在38和45
>        * using ChatRoomBackend = Backend;
>        * using ChatRoomEncryptionBackend = EncryptionBackend;
>    * 警告被视为错误
>        * 这个和ms2的错误差别不大
>        * 进到源码linphone文件夹,将CMakeLists.txt的/WX注释了,大概在288行
>    * LINK : fatal error LNK1181: 无法打开输入文件“LIBGCC-NOTFOUND.lib”
>        * 这个比较麻烦,是因为cmake无法找到gcc和mingw的路径,添加这两个路径即可,CMakeLists文件在linphone/src
>        * set(LIBGCC c:/msys64/usr/bin)
>        * set(LIBMINGWEX c:/msys64)
>    * 还有一个doxygen路径错误的问题,简单点就是直接把linphone-doc项目去掉即可,不管他也没什么问题

####
在所有步骤完成后,编译INSTALL项目即可


## Linux
这个库在Linux环境下编译就比较顺利了,基本上不存在什么编译问题,有也是库没装  
这里需要说明一下需要QT GL的情况

### QT GL
在Linux环境下添加qt gl需要注意qt路径,参考Windows下的[ms2库编译](#ms2编译的相关错误)