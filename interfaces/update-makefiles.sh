#!/bin/bash

source $ANDROID_BUILD_TOP/system/tools/hidl/update-makefiles-helper.sh

do_makefiles_update \
  "vendor.egistec:hardware/sony/interfaces/egistec"

do_makefiles_update \
  "vendor.semc:hardware/sony/interfaces/semc"
