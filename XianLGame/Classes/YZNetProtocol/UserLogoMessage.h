#pragma once

#ifndef AFC_USER_LOGO_MESSAGE_HEAD_FILE
#define AFC_USER_LOGO_MESSAGE_HEAD_FILE

/*
 文件说明：本文件定义了与用户头像上传下载相关的消息和结构体
 整合平台时，需要在GameRoomLogonDT中包含本文件
 文件创建：Fred Huang 2008-03-17
 消息格式：MDM_		表示主消息
 ASS_UL	表示与用户头像相关子消息
 ASS_ULC	表示由客户端向服务器端发送的消息
 ASS_ULS	表示由服务器端向客户端发送的消息
 结构格式：MSG_		表示是消息结构体
 MSG_UL	与头像相关的消息结构
 MSG_UL_C_ 由客户端发给服务器端的消息结构体
 MSG_UL_S_ 由服务器端发给客户端的消息结构体
 */

//主消息
#define MDM_GR_USER_LOGO				120					//头像相关


//子消息
#define ASS_ULC_UPLOAD					0x01					//上传头像数据
#define ASS_ULC_INFOREQUEST				0x02					//请求头像文件消息
#define ASS_ULC_DOWNREQUEST				0x03					//请求下载头像文件
#define ASS_ULC_DOWNRESULT				0x04					//返回下载的结果，实际上用于分包下载时请求下一数据包

#define ASS_ULS_UPLOADSUCCEED			0x11					//上传上头像成功
#define ASS_ULS_UPLOADFAILED			0x12					//上传头像失败
#define ASS_ULS_LOGOINFORMATION			0x13					//用户头像文件信息
#define ASS_ULS_DOWN					0x14					//下载头像数据


#define MAX_BLOCK_SIZE					512						//每个消息中最大可带头像数据大小
/*
 结构：上传头像数据结构
 内容：文件名、文件大小、折包大小、折包数、折包序号、本次图片数据有效大小、图片数据
 */
typedef struct
{
    int			dwUserID;										//用户ID
    int			dwUserSourceLogo;								//原始的LOGOid号
    int			updown_type;									//上传下载的类型 （0-自定义头像  1-个人相册图片1  2-图片2  3-图片3）
    int 		nFileSize;										//文件大小，一般不超过20K，即20480Byte
    int 		nBlockSize;										//折包大小
    int			nBlockCount;									//折包数
    int			nBlockIndex;									//折包序号，从0-nPackageCount
    int			nPackageSize;									//本此消息中所附带数据大小
    char		szData[MAX_BLOCK_SIZE];							//图像数据
}MSG_UL_C_UPLOAD,MSG_UL_S_DOWN;//下载头像数据包

//上传下载结果
typedef struct
{
    int			dwUserID;										//上传或下载的用户ID
    int			nBlockCount;									//上传或下载的数据块总数
    int			nBlockIndex;									//上传或下载的数据块索引
    int			updown_type;									//上传下载的类型
    bool		bNeedCheck;
}MSG_UL_C_DOWNRESULT,MSG_UL_S_UPLOADRESULT;

//请求用户的头像数据
typedef struct
{
    int		dwUserID;										//请求者的用户ID
    int		dwRequestUserID;								//被请求者的用户ID
    int		updown_type;									//上传下载的类型 （0-自定义头像  1-个人相册图片1  2-图片2  3-图片3）
}MSG_UL_C_INFORREQUEST,MSG_UL_C_DOWNREQUEST;//请求下载头像

//返回的用户头像信息
typedef struct
{
    int			dwUserID;										//用户ID号
    int			dwUserLogoID;									//头像ID号，如果=0xFF，才表示有自定义头像，此处看起来多此一举，但为了规范和防止出错，特意加上
    char		szFileMD5[33];									//头像文件的MD5值
}MSG_UL_S_INFORMATION;

#endif