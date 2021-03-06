// eLua platform configuration

#ifndef __PLATFORM_CONF_H__
#define __PLATFORM_CONF_H__

#include "auxmods.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "stacks.h"
#include "sysctl.h"

// *****************************************************************************
// Define here what components you want for this platform

$build_xmodem #define BUILD_XMODEM
$build_shell #define BUILD_SHELL
$build_romfs #define BUILD_ROMFS
$build_mmcfs #define BUILD_MMCFS
$build_term #define BUILD_TERM
$build_uip #define BUILD_UIP
$build_dhcpc #define BUILD_DHCPC
$build_dns #define BUILD_DNS
$build_con_generic #define BUILD_CON_GENERIC
$build_adc #define BUILD_ADC
$build_rpc #define BUILD_RPC
$build_con_tcp #define BUILD_CON_TCP

// *****************************************************************************
// UART/Timer IDs configuration data (used in main.c)

#define CON_UART_ID           0
#define CON_UART_SPEED        115200
#define CON_TIMER_ID          0
#define TERM_LINES            25
#define TERM_COLS             80

// *****************************************************************************
// Auxiliary libraries that will be compiled for this platform

// The name of the platform specific libs table
// FIXME: should handle partial or no inclusion of platform specific modules per conf.py
#ifdef ENABLE_DISP
#define PS_LIB_TABLE_NAME   "lm3s"
#endif

#ifdef FORLM3S6918
#define PWMLINE
#else
#define PWMLINE  _ROM( AUXLIB_PWM, luaopen_pwm, pwm_map )
#endif

#ifdef BUILD_UIP
#define NETLINE  _ROM( AUXLIB_NET, luaopen_net, net_map )
#else
#define NETLINE
#endif

#ifdef BUILD_ADC
#define ADCLINE _ROM( AUXLIB_ADC, luaopen_adc, adc_map )
#else
#define ADCLINE
#endif

#ifdef BUILD_RPC
#define RPCLINE _ROM( AUXLIB_RPC, luaopen_rpc, rpc_map )
#else
#define RPCLINE
#endif

#ifdef PS_LIB_TABLE_NAME
#define PLATLINE _ROM( PS_LIB_TABLE_NAME, luaopen_platform, platform_map )
#else
#define PLATLINE
#endif

#define LUA_PLATFORM_LIBS_ROM\
  _ROM( AUXLIB_PIO, luaopen_pio, pio_map )\
  _ROM( AUXLIB_SPI, luaopen_spi, spi_map )\
  _ROM( AUXLIB_TMR, luaopen_tmr, tmr_map )\
  _ROM( AUXLIB_PD, luaopen_pd, pd_map )\
  _ROM( AUXLIB_UART, luaopen_uart, uart_map )\
  PWMLINE\
  _ROM( AUXLIB_TERM, luaopen_term, term_map )\
  _ROM( AUXLIB_PACK, luaopen_pack, pack_map )\
  _ROM( AUXLIB_BIT, luaopen_bit, bit_map )\
  _ROM( AUXLIB_BITARRAY, luaopen_bitarray, bitarray_map )\
  NETLINE\
  _ROM( AUXLIB_CPU, luaopen_cpu, cpu_map )\
  ADCLINE\
  RPCLINE\
  _ROM( LUA_MATHLIBNAME, luaopen_math, math_map )\
  PLATLINE

// *****************************************************************************
// Configuration data

// Static TCP/IP configuration
#define ELUA_CONF_IPADDR0     $ip0
#define ELUA_CONF_IPADDR1     $ip1
#define ELUA_CONF_IPADDR2     $ip2
#define ELUA_CONF_IPADDR3     $ip3

#define ELUA_CONF_NETMASK0    $mask0
#define ELUA_CONF_NETMASK1    $mask1
#define ELUA_CONF_NETMASK2    $mask2
#define ELUA_CONF_NETMASK3    $mask3

#define ELUA_CONF_DEFGW0      $gateway0
#define ELUA_CONF_DEFGW1      $gateway1
#define ELUA_CONF_DEFGW2      $gateway2
#define ELUA_CONF_DEFGW3      $gateway3

#define ELUA_CONF_DNS0        $dns0
#define ELUA_CONF_DNS1        $dns1
#define ELUA_CONF_DNS2        $dns2
#define ELUA_CONF_DNS3        $dns3


// *****************************************************************************
// Configuration data

// Virtual timers (0 if not used)
#define VTMR_NUM_TIMERS       4
#define VTMR_FREQ_HZ          4

// Number of resources (0 if not available/not implemented)
#ifdef FORLM3S9B92
  #define NUM_PIO             7
#else
  #define NUM_PIO             7
#endif
#define NUM_SPI               1
#ifdef FORLM3S6965
  #define NUM_UART            3
#elif FORLM3S9B92
  #define NUM_UART            3
#else
  #define NUM_UART            2
#endif
#define NUM_TIMER             4
#ifndef FORLM3S6918
  #define NUM_PWM             6
#else
  #define NUM_PWM             0
#endif  
#define NUM_ADC               4
#define NUM_CAN               0

// Enable RX buffering on UART
//#define BUF_ENABLE_UART
//#define CON_BUF_SIZE          BUF_SIZE_128

// ADC Configuration Params
#define ADC_BIT_RESOLUTION    10
#define BUF_ENABLE_ADC
#define ADC_BUF_SIZE          BUF_SIZE_2

// These should be adjusted to support multiple ADC devices
#define ADC_TIMER_FIRST_ID    0
#define ADC_NUM_TIMERS        NUM_TIMER  

// RPC boot options
#define RPC_UART_ID           CON_UART_ID
#define RPC_TIMER_ID          CON_TIMER_ID
#define RPC_UART_SPEED        CON_UART_SPEED

// SD/MMC Filesystem Setup
#define MMCFS_TICK_HZ     4
#define MMCFS_TICK_MS     ( 1000 / MMCFS_TICK_HZ )

#if defined( ELUA_BOARD_EKLM3S6965 )
  // EK-LM3S6965
  #define MMCFS_CS_PORT                3
  #define MMCFS_CS_PIN                 0
  #define MMCFS_SPI_NUM                0
#elif defined( ELUA_BOARD_EKLM3S8962 )
  // EK-LM3S8962
  #define MMCFS_CS_PORT                6
  #define MMCFS_CS_PIN                 0
  #define MMCFS_SPI_NUM                0
#elif defined( ELUA_BOARD_EAGLE100 )
  // Eagle-100
  #define MMCFS_CS_PORT                6
  #define MMCFS_CS_PIN                 1
  #define MMCFS_SPI_NUM                0
#elif defined( BUILD_MMCFS ) && !defined( MMCFS_SPI_NUM )
  #warning "MMCFS was enabled, but required SPI & CS data are undefined, disabling MMCFS"
  #undef BUILD_MMCFS
#endif


// CPU frequency (needed by the CPU module, 0 if not used)
#define CPU_FREQUENCY         SysCtlClockGet()

// PIO prefix ('0' for P0, P1, ... or 'A' for PA, PB, ...)
#define PIO_PREFIX            'A'
// Pins per port configuration:
// #define PIO_PINS_PER_PORT (n) if each port has the same number of pins, or
// #define PIO_PIN_ARRAY { n1, n2, ... } to define pins per port in an array
// Use #define PIO_PINS_PER_PORT 0 if this isn't needed
#ifdef FORLM3S9B92
  #define PIO_PIN_ARRAY         { 8, 8, 8, 8, 8, 6, 8, 8, 8 }
#else
  #define PIO_PIN_ARRAY         { 8, 8, 8, 8, 4, 4, 2 }
#endif
//                                A, B, C, D, E, F, G, H, J

#ifdef FORLM3S9B92
  #define SRAM_SIZE ( 0x18000 )
#else
  #define SRAM_SIZE ( 0x10000 )
#endif

// Allocator data: define your free memory zones here in two arrays
// (start address and end address)
#define MEM_START_ADDRESS     { ( void* )end }
#define MEM_END_ADDRESS       { ( void* )( SRAM_BASE + SRAM_SIZE - STACK_SIZE_TOTAL - 1 ) }

// *****************************************************************************
// CPU constants that should be exposed to the eLua "cpu" module

#include "hw_ints.h"

#define PLATFORM_CPU_CONSTANTS\
  _C( INT_GPIOA ),\
  _C( INT_GPIOB ),\
  _C( INT_GPIOC ),\
  _C( INT_GPIOD ),\
  _C( INT_GPIOE ),\
  _C( INT_UART0 ),\
  _C( INT_UART1 ),\
  _C( INT_SSI0 ),\
  _C( INT_I2C0 ),\
  _C( INT_PWM_FAULT ),\
  _C( INT_PWM0 ),\
  _C( INT_PWM1 ),\
  _C( INT_PWM2 ),\
  _C( INT_QEI0 ),\
  _C( INT_ADC0 ),\
  _C( INT_ADC1 ),\
  _C( INT_ADC2 ),\
  _C( INT_ADC3 ),\
  _C( INT_WATCHDOG ),\
  _C( INT_TIMER0A ),\
  _C( INT_TIMER0B ),\
  _C( INT_TIMER1A ),\
  _C( INT_TIMER1B ),\
  _C( INT_TIMER2A ),\
  _C( INT_TIMER2B ),\
  _C( INT_COMP0 ),\
  _C( INT_COMP1 ),\
  _C( INT_COMP2 ),\
  _C( INT_SYSCTL ),\
  _C( INT_FLASH ),\
  _C( INT_GPIOF ),\
  _C( INT_GPIOG ),\
  _C( INT_GPIOH ),\
  _C( INT_UART2 ),\
  _C( INT_SSI1 ),\
  _C( INT_TIMER3A ),\
  _C( INT_TIMER3B ),\
  _C( INT_I2C1 ),\
  _C( INT_QEI1 ),\
  _C( INT_CAN0 ),\
  _C( INT_CAN1 ),\
  _C( INT_CAN2 ),\
  _C( INT_ETH ),\
  _C( INT_HIBERNATE ),\
  _C( INT_USB0 ),\
  _C( INT_PWM3 ),\
  _C( INT_UDMA ),\
  _C( INT_UDMAERR )

#endif // #ifndef __PLATFORM_CONF_H__
