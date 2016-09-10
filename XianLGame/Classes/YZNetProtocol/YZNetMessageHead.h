#ifndef _YZ_MessageHead_H_
#define _YZ_MessageHead_H_

#include "YZBaseType.h"
#include "YZCommon/YZCommonMarco.h"
#include "YZComStruct.h"

namespace YZ 
{
	// network packet header
	typedef struct tagNetMessageHead
	{		
		UINT						uMessageSize;						/// 数据包大小
		UINT						bMainID;							/// 处理主类型
		UINT						bAssistantID;						/// 辅助处理类型 ID
		UINT						bHandleCode;						/// 数据包处理代码   1 表示分包结束 . 0 //表示有分包，并且没有接收完成
		UINT						bReserve;							/// 保留字段
	} GNUC_PACKED NetMessageHead;
}

#endif // !_YZ_NetMessageHead_H_
