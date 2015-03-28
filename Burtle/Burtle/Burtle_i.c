

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun Mar 29 00:56:58 2015
 */
/* Compiler settings for Burtle.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IBugTraqProvider,0x298B927C,0x7220,0x423C,0xB7,0xB4,0x6E,0x24,0x1F,0x00,0xCD,0x93);


MIDL_DEFINE_GUID(IID, IID_IBugTraqProvider2,0xC5C85E31,0x2F9B,0x4916,0xA7,0xBA,0x8E,0x27,0xD4,0x81,0xEE,0x83);


MIDL_DEFINE_GUID(IID, LIBID_BurtleLib,0x7C1367DD,0x1E76,0x490B,0xAB,0x07,0xFD,0x4A,0xE2,0xD8,0x99,0xDE);


MIDL_DEFINE_GUID(CLSID, CLSID_BurtleProvider,0xFB586010,0xA6BA,0x4921,0xB9,0x21,0xA4,0xA1,0x8E,0xD8,0xA8,0xC1);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



