/** @file
  Copyright (C) 2018, vit9696. All rights reserved.

  All rights reserved.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef OPEN_CORE_H
#define OPEN_CORE_H

#include <Library/OcBootManagementLib.h>
#include <Library/OcConfigurationLib.h>
#include <Library/OcCpuLib.h>
#include <Library/OcCryptoLib.h>
#include <Library/OcGuardLib.h>
#include <Library/OcMiscLib.h>
#include <Library/OcStringLib.h>
#include <Library/OcStorageLib.h>

#include <Protocol/OcBootstrap.h>

/**
  OpenCORE version 注册到 log 和 NVRAM.
  OPEN_CORE_VERSION 必须遵循 X.Y.Z 格式, 其中 X.Y.Z 每位为单个数字.
**/
#define OPEN_CORE_VERSION          "0.5.3"

/**
  注册到 log 和 NVRAM 的 OpenCore 生成类型.
**/
#if defined (OC_TARGET_RELEASE)
#define OPEN_CORE_TARGET           "REL" ///< 版本.
#elif defined (OC_TARGET_DEBUG)
#define OPEN_CORE_TARGET           "DBG" ///< 使用编译器优化进行调试.
#elif defined (OC_TARGET_NOOPT)
#define OPEN_CORE_TARGET           "NPT" ///< 无编译器优化的调试.
#else
#error "Unknown target definition"
#endif

#define OPEN_CORE_IMAGE_PATH       L"EFI\\OC\\OpenCore.efi"

/**
  多个板，即华硕P8H61-M和P8H61-M LX2 不会打开带有尾随斜杠的目录。前斜线对他们来说是否存在无关紧要.
  这意味着 L"EFI\\OC\\" and L"\\EFI\\OC\\" 都不能打开,
  而 L"EFI\\OC" and L"\\EFI\\OC" 可以打开.

  除了root path和dmg，我们不打开任何目录，所以配置在这里.
**/
#define OPEN_CORE_ROOT_PATH        L"EFI\\OC"

#define OPEN_CORE_CONFIG_PATH      L"config.plist"

#define OPEN_CORE_LOG_PREFIX_PATH  L"opencore"

#define OPEN_CORE_NVRAM_PATH       L"nvram.plist"

#define OPEN_CORE_ACPI_PATH        L"ACPI\\"

#define OPEN_CORE_UEFI_DRIVER_PATH L"Drivers\\"

#define OPEN_CORE_KEXT_PATH        L"Kexts\\"

#define OPEN_CORE_TOOL_PATH        L"Tools\\"

#define OPEN_CORE_NVRAM_ATTR       (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS)

#define OPEN_CORE_INT_NVRAM_ATTR   EFI_VARIABLE_BOOTSERVICE_ACCESS

/**
  如果安装了密码密钥.

  @param[in]  Bootstrap  bootstrap protocol.

  @return public key or NULL.
**/
OC_RSA_PUBLIC_KEY *
OcGetVaultKey (
  IN  OC_BOOTSTRAP_PROTOCOL *Bootstrap
  );

/**
  像自定义表一样加载ACPI兼容性支持.

  @param[in]  Storage   OpenCore storage.
  @param[in]  Config    OpenCore configuration.
**/
VOID
OcLoadAcpiSupport (
  IN OC_STORAGE_CONTEXT  *Storage,
  IN OC_GLOBAL_CONFIG    *Config
  );

/**
  加载设备属性兼容性支持.

  @param[in]  Config    OpenCore configuration.
**/
VOID
OcLoadDevPropsSupport (
  IN OC_GLOBAL_CONFIG    *Config
  );

/**
  加载内核兼容性支持，如kexts.

  @param[in]  Storage   OpenCore storage.
  @param[in]  Config    OpenCore configuration.
  @param[in]  CpuInfo   CPU information.
**/
VOID
OcLoadKernelSupport (
  IN OC_STORAGE_CONTEXT  *Storage,
  IN OC_GLOBAL_CONFIG    *Config,
  IN OC_CPU_INFO         *CpuInfo
  );

/**
  失败时清除内核兼容性支持.
**/
VOID
OcUnloadKernelSupport (
  VOID
  );

/**
  加载NVRAM兼容性支持.

  @param[in]  Storage   OpenCore storage.
  @param[in]  Config    OpenCore configuration.
**/
VOID
OcLoadNvramSupport (
  IN OC_STORAGE_CONTEXT  *Storage,
  IN OC_GLOBAL_CONFIG    *Config
  );

/**
  加载平台兼容性支持，如DataHub或SMBIOS.

  @param[in]  Config    OpenCore configuration.
  @param[in]  CpuInfo   CPU information.
**/
VOID
OcLoadPlatformSupport (
  IN OC_GLOBAL_CONFIG    *Config,
  IN OC_CPU_INFO         *CpuInfo
  );

/**
  加载UEFI兼容性支持，如驱动程序.

  @param[in]  Storage   OpenCore storage.
  @param[in]  Config    OpenCore configuration.
  @param[in]  CpuInfo   CPU information.
**/
VOID
OcLoadUefiSupport (
  IN OC_STORAGE_CONTEXT  *Storage,
  IN OC_GLOBAL_CONFIG    *Config,
  IN OC_CPU_INFO         *CpuInfo
  );

/**
  Decide whether console controller reconnection is required
  upon changing screen resolution.

  @param[in]  Config    OpenCore configuration.

  @retval TRUE when required.
**/
BOOLEAN
OcShouldReconnectConsoleOnResolutionChange (
  IN OC_GLOBAL_CONFIG  *Config
  );

/**
  Get human readable version string.

  @retval null-terminated 7-bit ASCII version string.
**/
CONST CHAR8 *
OcMiscGetVersionString (
  VOID
  );

/**
  Get ballooning handler for memory allocation protections.

  @param[in]  Config    OpenCore configuration.

  @retval Handler address or NULL.
**/
OC_BALLOON_ALLOC
OcGetBallooningHandler (
  IN  OC_GLOBAL_CONFIG  *Config
  );

/**
  Load early miscellaneous support like configuration.

  @param[in]  Storage   OpenCore storage.
  @param[out] Config    OpenCore configuration.
  @param[in]  VaultKey  Vault key.

  @retval EFI_SUCCESS when allowed to continue.
**/
EFI_STATUS
OcMiscEarlyInit (
  IN  OC_STORAGE_CONTEXT *Storage,
  OUT OC_GLOBAL_CONFIG   *Config,
  IN  OC_RSA_PUBLIC_KEY  *VaultKey  OPTIONAL
  );

/**
  Load late miscellaneous support like boot screen config.

  @param[in]  Config     OpenCore configuration.
  @param[in]  LoadPath   OpenCore loading path.
  @param[out] LoadHandle OpenCore loading handle.

  @retval EFI_SUCCESS on success, informational.
**/
EFI_STATUS
OcMiscLateInit (
  IN  OC_GLOBAL_CONFIG          *Config,
  IN  EFI_DEVICE_PATH_PROTOCOL  *LoadPath  OPTIONAL,
  OUT EFI_HANDLE                *LoadHandle OPTIONAL
  );

/**
  Load late miscellaneous support like boot screen config.

  @param[in]  Storage         OpenCore storage.
  @param[in]  Config          OpenCore configuration.
  @param[in]  Privilege       OpenCore privilege context.
  @param[in]  StartImage      Image starting routine used.
  @param[in]  LoadHandle      OpenCore loading handle.
  @param[in]  CustomBootGuid  Use custom (gOcVendorVariableGuid) for Boot#### variables.
**/
VOID
OcMiscBoot (
  IN  OC_STORAGE_CONTEXT        *Storage,
  IN  OC_GLOBAL_CONFIG          *Config,
  IN  OC_PRIVILEGE_CONTEXT      *Privilege OPTIONAL,
  IN  OC_IMAGE_START            StartImage,
  IN  BOOLEAN                   CustomBootGuid,
  IN  EFI_HANDLE                LoadHandle OPTIONAL
  );

/**
  Load miscellaneous support after UEFI quirks.

  @param[in]  Config     OpenCore configuration.
**/
VOID
OcMiscUefiQuirksLoaded (
  IN OC_GLOBAL_CONFIG   *Config
  );

#endif // OPEN_CORE_H
