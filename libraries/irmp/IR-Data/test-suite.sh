#! /bin/sh
#----------------------------------------------------------------------------
# test suite for IRMP
#
# usage:
#
#        ./test-suite.sh
#
# Copyright (c) 2010 Frank Meyer - frank(at)fli4l.de
#
# $Id: test-suite.sh,v 1.10 2010/06/09 12:04:04 fm Exp $
#----------------------------------------------------------------------------

set -e                              # exit on error
cd `dirname $0`
mkdir -p tmpsrc
cp ../irmp.[ch] ../irmpconfig.h ../irsnd.[ch] ../irsndconfig.h ../irmpsystem.h ../irmpprotocols.h ../makefile.lnx tmpsrc
cd tmpsrc
sed 's/#define \(IRMP_SUPPORT_[A-Z_0-9]*  *\)[01]/#define \1 1/g' <irmpconfig.h >irmpconfig.new
mv irmpconfig.new irmpconfig.h
make -f makefile.lnx clean
make -f makefile.lnx all
cd ..

for j in                            \
    3xNEC3xAPPLE.log.txt            \
    Dbox.txt                        \
    DK_Digital.txt                  \
    Grundig_TP715.txt               \
    Grundig_TP715_SatTV.txt         \
    Grundig_TP715_Video.txt         \
    Kathrein-UFS-912-Remote.txt     \
    Matsushita.txt                  \
    Nokia.txt                       \
    Panasonic-Blue-Ray.txt          \
    RC5-Taste.txt                   \
    Samsung_DVD_Rec_00062C.txt      \
    Samsung_TV.txt                  \
    Sony-RM-S-310.txt               \
    sony-rm-s311.txt                \
    Sony-RM-U305C.txt               \
    Sony-RMT-D142P-DVD.txt          \
    Sony-RMT-V406.txt               \
    Sony_RM-S315_lange.txt          \
    Sony_Bravia_RM-ED0009_new.txt   \
    Yamaha-RAV388.txt               \
    apple.txt                       \
    apple-unibody-remote.txt        \
    bo_beolink1000-10kHz.txt        \
    denon.txt                       \
    elta_radio.txt                  \
    fdc.txt                         \
    jvc.txt                         \
    jvc-rm-rk250-10kHz.txt          \
    nec-repetition.txt              \
    nec-skymaster-dt500.txt         \
    nec.txt                         \
    nec-non-std-rep.txt             \
    nikon.txt                       \
    nubert-subwoofer.txt            \
    orion_vcr_07660BM070.txt        \
    panasonic-scan.txt              \
    rc-car.txt                      \
    rc5.txt                         \
    rc5x-79.txt                     \
    rc5x.txt                        \
    rc6-hold.txt                    \
    rc6.txt                         \
    sharp_kurz_10khz.txt            \
    sharp_lang_10khz.txt            \
    xbox360-10kHz.txt
do
    echo -n "testing $j ... "
    if tmpsrc/irmp-10kHz -v < $j | grep -q error
    then
        tmpsrc/irmp-10kHz -v < $j | grep error
        echo "test failed"
        exit 1
    else
        if tmpsrc/irmp-10kHz -v < $j | grep -q checked
        then
            echo "checked!"
        else
            echo "successful"
        fi
    fi
done

# t-home-mediareceiver-15kHz.txt (RUWIDO) conflicts with Denon
# a1tvbox-15kHz.txt (A1TVBOX) conflicts with MERLIN
# Siemens-Gigaset-M740AV-15kHz.txt (SIEMENS) conflicts with MERLIN

for j in                                \
    bo_beolink1000-15kHz.txt            \
    bose_wave_system_15khz.txt          \
    denon-15kHz.txt                     \
    denon-rc-176-15kHz.txt              \
    denon-rc-176-repeat-15kHz.txt       \
    irc-15kHz.txt                       \
    kathrein-15kHz.txt                  \
    lg-air-15kHz.txt                    \
    merlin-15kHz.txt                    \
    panasonic-vcr-15kHz.txt             \
    pentax-15kHz.txt                    \
    rc5-philipps-15kHz.txt              \
    rc6a-siemens-15kHz.txt              \
    recs80-15kHz.txt                    \
    saa3004-15kHz.txt                   \
    samsung-br-15kHz.txt                \
    samsung32-tv-15kHz.txt              \
    samsung32-15kHz.txt                 \
    samsung48-15kHz.txt                 \
    sharp_15khz.txt                     \
    technics-15kHz.txt                  \
    thomson-mb100-15kHz.txt             \
    tp400vt-15kHz.txt                   \
    universal-15kHz.txt                 \
    vincent-15kHz.txt                   \
    vincent-flash-15kHz.txt             \
    xbox360-15kHz.txt
do
    echo -n "testing $j ... "
    if tmpsrc/irmp-15kHz -v < $j | grep -q error
    then
        tmpsrc/irmp-15kHz -v < $j | grep error
        echo "test failed"
        exit 1
    else
        if tmpsrc/irmp-15kHz -v < $j | grep -q checked
        then
            echo "checked!"
        else
            echo "successful"
        fi
    fi
done

# a1tvbox-15kHz.txt (A1TVBOX) conflicts with MERLIN

for j in                                \
    rc-car-20kHz.txt                    \
    fdc-20kHz.txt                       \
    fdc2-20kHz.txt                      \
    rcmm-20kHz.txt                      \
    saa3004-20kHz.txt                   \
    telefunken-1560-20kHz.txt
do
    echo -n "testing $j ... "
    if tmpsrc/irmp-20kHz -v < $j | grep -q error
    then
        tmpsrc/irmp-20kHz -v < $j | grep error
        echo "test failed"
        exit 1
    else
        if tmpsrc/irmp-20kHz -v < $j | grep -q checked
        then
            echo "checked!"
        else
            echo "successful"
        fi
    fi
done

# rm -rf tmpsrc

echo "all tests successful"
exit 0
