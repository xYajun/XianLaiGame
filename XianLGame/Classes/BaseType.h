#ifndef ___BaseType_H__
#define ___BaseType_H__

typedef		char				CHAR;
typedef		unsigned char       BYTE;

typedef		unsigned short      WORD;

typedef		short               SHORT;

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

#endif	//__BaseType_H__
