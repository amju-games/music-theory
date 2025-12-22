# Set up env vars for make -- SOURCE ME with . ./setup_make_vars.sh
export AMJU_BUILD_ARCH=Mac
export AMJU_MACHDEP='-DMACOSX -DXP_MACOSX -DAMJU_USE_BASS -DAMJU_USE_CURL -DUSE_SHADOW_MAP_OPENGL_2'

export AMJULIB_MAC_SCRIPT_PATH=`pwd`/../../../amjulib/Script/MacOSX/

