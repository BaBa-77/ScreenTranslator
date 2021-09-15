cmake_minimum_required(VERSION 3.13)

project("brewemulator")

set(CMAKE_CXX_STANDARD 17)

add_library(brew STATIC IMPORTED)
set_property(TARGET brew PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/brew.fixed.lib)

add_library(aee STATIC IMPORTED)
set_property(TARGET aee PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aee.fixed.lib)

add_library(aeedialog STATIC IMPORTED)
set_property(TARGET aeedialog PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeedialog.fixed.lib)

add_library(aeepdiparsers STATIC IMPORTED)
set_property(TARGET aeepdiparsers PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeepdiparsers.fixed.lib)

add_library(aeevcalstore STATIC IMPORTED)
set_property(TARGET aeevcalstore PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeevcalstore.fixed.lib)

add_library(aeefont STATIC IMPORTED)
set_property(TARGET aeefont PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeefont.fixed.lib)

add_library(aeesecurity STATIC IMPORTED)
set_property(TARGET aeesecurity PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeesecurity.fixed.lib)

add_library(aeersa STATIC IMPORTED)
set_property(TARGET aeersa PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeersa.fixed.lib)

add_library(aeeweb STATIC IMPORTED)
set_property(TARGET aeeweb PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeeweb.fixed.lib)

add_library(aeenet STATIC IMPORTED)
set_property(TARGET aeenet PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeenet.fixed.lib)

add_library(aeecontrols STATIC IMPORTED)
set_property(TARGET aeecontrols PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeecontrols.fixed.lib)

add_library(aeemenu STATIC IMPORTED)
set_property(TARGET aeemenu PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeemenu.fixed.lib)

add_library(aeeijg STATIC IMPORTED)
set_property(TARGET aeeijg PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeeijg.fixed.lib)

add_library(aeepng STATIC IMPORTED)
set_property(TARGET aeepng PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeepng.fixed.lib)

add_library(png STATIC IMPORTED)
set_property(TARGET png PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/png.fixed.lib)

add_library(aeeseed STATIC IMPORTED)
set_property(TARGET aeeseed PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeeseed.fixed.lib)

add_library(aeemm STATIC IMPORTED)
set_property(TARGET aeemm PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeemm.fixed.lib)

add_library(aeetapi STATIC IMPORTED)
set_property(TARGET aeetapi PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeetapi.fixed.lib)

add_library(aeeunzip STATIC IMPORTED)
set_property(TARGET aeeunzip PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeeunzip.fixed.lib)

add_library(aeebci STATIC IMPORTED)
set_property(TARGET aeebci PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeebci.fixed.lib)

add_library(z STATIC IMPORTED)
set_property(TARGET z PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/z.fixed.lib)

add_library( brewemu SHARED
        brewemu.cpp
        brewemu.h
        breABI.c

        bre/OEMRan.c
        bre/OEMOS.c
        bre/OEMMutex.c
        bre/OEMHeap.c
        bre/OEMAppFuncs.c
        bre/OEMFSPath.cpp
        bre/OEMFS.cpp
        bre/OEMSound.c
        bre/md5c.c
        bre/OEMMD5.c
        bre/OEMDebug.c
        bre/OEMBN.c
        bre/atof.c
        bre/DummyMIFs.c
        bre/Shims.c
        bre/OEMMemCacheArch.c
        bre/OEMDisplayDev.c
        bre/time_jul.c
        bre/OEMCriticalSection.c
        bre/OEMTAPI.c
        bre/OEMSysClock.c
        bre/ARMLegacyABI.S

        bre2/breStartup.c

        basic11.h
        basic11b.h
        basic14.h
        OEMAppFrame.c
        OEMAppx5.c
        OEMAuth.c
        OEMBitmap_common.c
        OEMBitmap_generic.h
        OEMBitmap_priv.h
        OEMBitmap24.c
        OEMBitmapFuncs_priv.h
        OEMCallHistory.c
        OEMConstFiles.c
        OEMDB.c
        OEMDefaults.c
        OEMDeviceNotifier.c
        OEMDisp.c
        OEMFlip.c
        OEMFont.c
        OEMIMM.c
        OEMIMUI.c
        oemitextctl.c
        OEMKeysMapping.c
        OEMLogger.c
        OEMLoggerBTIL.c
        OEMLoggerFile.c
        OEMMCFMIF_10888.c
        OEMMCFMIF_10889.c
        OEMMCFMIF_10890.c
        OEMMCFMIF_10891.c
        OEMMCFMIF_10892.c
        OEMMCFMIF_16225.c
        OEMMCFMIF_18067.c
        OEMMCFMIF_19240.c
        OEMMCFMIF_19917.c
        OEMMCFMIF_19918.c
        OEMMCFMIF_19919.c
        OEMMCFMIF_19920.c
        OEMMCFMIF_19922.c
        OEMMCFMIF_19923.c
        OEMMCFMIF_19924.c
        OEMMedia.c
        OEMMediaFormats.c
        OEMMemCache.c
        OEMModTable.c
        OEMNativeNotifier.c
        OEMObjectMgr.c
        OEMPrivacy.c
        OEMRegistry.c
        OEMResArbiter.c
        OEMRinger.c
        OEMStockFonts.c
        OEMStringFuncs.c
        OEMText.c
        OEMTransform_generic.h
        OEMUSBHMSMIF_26108.c
        OEMUSBHMSMIF_26109.c
        samplefont.bmp.h
        )

set_property(SOURCE bre/ARMLegacyABI.S PROPERTY LANGUAGE C)

add_definitions(-DAEE_STATIC)

target_include_directories(brewemu PRIVATE "D:\\src\\BREW\\BREWPKMSM315SP02\\inc" "D:\\src\\BREW\\BREWPKMSM315SP02\\pk\\inc" "D:\\src\\BREW\\BREWPKMSM315SP02\\sdk\\inc")
target_link_options(brewemu PRIVATE -Wl,-z,notext -Wl,-z,muldefs -Wl,-error-limit=0)

find_library( log-lib log )
find_library( android-lib android )
find_package(ZLIB)

target_link_libraries( brewemu
        brew
        aee
        aeebci
        aeedialog
        aeepdiparsers
        aeevcalstore
        aeefont
        aeesecurity
        aeersa
        aeeweb
        #aeenet
        aeecontrols
        aeemenu
        aeeijg
        aeepng
        aeeseed
        aeemm
        aeetapi
        aeeunzip
        z
        png
        ${log-lib}
        ${android-lib}
        )