#ifndef S_ENDIAN_H
#define S_ENDIAN_H

#ifdef __linux__
#include <endian.h>
#include <byteswap.h>
#endif


#ifdef _WIN32
#define __BIG_ENDIAN 0
#define __LITTLE_ENDIAN 1
#define __BYTE_ORDER  __LITTLE_ENDIAN
#define __bswap_16(x) _byteswap_ushort(x)
#define __bswap_32(x) _byteswap_ulong(x)
#define __bswap_64(x) _byteswap_uint64(x)
#endif



# if __BYTE_ORDER == __BIG_ENDIAN
#   define uint16_big_endian_to_local(x) (x)
#   define uint32_big_endian_to_local(x) (x)
#   define uint64_big_endian_to_local(x) (x)

#   define uint16_little_endian_to_local(x) __bswap_16 (x)
#   define uint32_little_endian_to_local(x) __bswap_32 (x)
#   define uint64_little_endian_to_local(x) __bswap_64 (x)

#   define uint16_local_to_big_endian(x) (x)
#   define uint32_local_to_big_endian(x) (x)
#   define uint64_local_to_big_endian(x) (x)

#   define uint16_local_to_little_endian(x) __bswap_16(x)
#   define uint32_local_to_little_endian(x) __bswap_32(x)
#   define uint64_local_to_little_endian(x) __bswap_64(x)
#else
#  if __BYTE_ORDER == __LITTLE_ENDIAN
#   define uint16_big_endian_to_local(x) __bswap_16 (x)
#   define uint32_big_endian_to_local(x) __bswap_32 (x)
#   define uint64_big_endian_to_local(x) __bswap_64 (x)

#   define uint16_little_endian_to_local(x) (x)
#   define uint32_little_endian_to_local(x) (x)
#   define uint64_little_endian_to_local(x) (x)

#   define uint16_local_to_big_endian(x) __bswap_16 (x)
#   define uint32_local_to_big_endian(x) __bswap_32 (x)
#   define uint64_local_to_big_endian(x) __bswap_64 (x)

#   define uint16_local_to_little_endian(x) (x)
#   define uint32_local_to_little_endian(x) (x)
#   define uint64_local_to_little_endian(x) (x)
#  endif
#endif

#endif // S_ENDIAN_H
