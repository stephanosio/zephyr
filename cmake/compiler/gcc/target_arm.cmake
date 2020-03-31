# SPDX-License-Identifier: Apache-2.0
if(CONFIG_ARM64)
  list(APPEND TOOLCHAIN_C_FLAGS
    -mcpu=${GCC_M_CPU}
    )
  list(APPEND TOOLCHAIN_LD_FLAGS
    -mcpu=${GCC_M_CPU}
    )
else()
  list(APPEND TOOLCHAIN_C_FLAGS   -mcpu=${GCC_M_CPU})
  list(APPEND TOOLCHAIN_LD_FLAGS  -mcpu=${GCC_M_CPU})

  if(CONFIG_CPU_HAS_VFP)
    list(APPEND TOOLCHAIN_C_FLAGS   -mfpu=${GCC_M_FPU})
    list(APPEND TOOLCHAIN_LD_FLAGS  -mfpu=${GCC_M_FPU})

    if(CONFIG_CPU_HAS_DCLS AND NOT CONFIG_FP_HARDABI)
      # If the processor is equipped with VFP and configured in DCLS topology,
      # the FP "hard" ABI must be used in order to facilitate the FP register
      # initialisation and synchronisation.
      set(FORCE_FP_HARDABI TRUE)
    endif()
  endif()

  if(CONFIG_COMPILER_ISA_THUMB2)
    list(APPEND TOOLCHAIN_C_FLAGS   -mthumb)
    list(APPEND TOOLCHAIN_LD_FLAGS  -mthumb)
  endif()

  if    (CONFIG_FP_HARDABI OR FORCE_FP_HARDABI)
    list(APPEND TOOLCHAIN_C_FLAGS   -mfloat-abi=hard)
    list(APPEND TOOLCHAIN_LD_FLAGS  -mfloat-abi=hard)
  elseif(CONFIG_FP_SOFTABI)
    list(APPEND TOOLCHAIN_C_FLAGS   -mfloat-abi=softfp)
    list(APPEND TOOLCHAIN_LD_FLAGS  -mfloat-abi=softfp)
  endif()
endif()
