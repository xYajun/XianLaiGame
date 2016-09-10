#ifndef __YZ_BaseType_H__
#define __YZ_BaseType_H__

typedef		char				CHAR;
typedef		unsigned char       BYTE;

typedef		unsigned short      WORD;

typedef		short               SHORT;
typedef     unsigned short      USHORT;

typedef		int					INT;
typedef		unsigned int		UINT;

typedef		long long			LLONG;

typedef		double              DOUBLE;
typedef		float               FLOAT;

#define		INVALID_VALUE			UINT(-1)

#define		INVALID_DESKNO			BYTE(-1)
#define		INVALID_DESKSTATION		BYTE(-1)
#define		INVALID_USER_ID         INT(-1)

#include <cstring>

#include <cassert>

enum emSocketStatus
{
	SocketStatus_UNKNOW = 0,
	SocketStatus_CONNECT,
	SocketStatus_OUTTIME,
	SocketStatus_STCCESS,
	SocketStatus_RECV,
	SocketStatus_DISCONNECT,
	SocketStatus_ERROR
};

#endif	//__YZ_BaseType_H__
