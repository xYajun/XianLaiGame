#ifndef _YZErrorCode_H__
#define _YZErrorCode_H__

/// 错误代码
#define ERR_GP_ERROR_UNKNOW				0									///未知错误
#define ERR_GP_LOGON_SUCCESS			1									///登陆成功
#define ERR_GP_USER_NO_FIND				2									///登陆名字错误
#define ERR_GP_USER_PASS_ERROR			3									///用户密码错误
#define ERR_GP_USER_VALIDATA			4									///用户帐号禁用
#define ERR_GP_USER_IP_LIMITED			5									///登陆 IP 禁止
#define ERR_GP_USER_EXIST				6									///用户已经存在
#define ERR_GP_PASS_LIMITED				7									///密码禁止效验
#define ERR_GP_IP_NO_ORDER				8									///不是指定地址 
#define ERR_GP_LIST_PART				9									///部分游戏列表
#define ERR_GP_LIST_FINISH				10									///全部游戏列表
#define ERR_GP_USER_LOGON				11									///此帐号已经登录
#define ERR_GP_USERNICK_EXIST			12									///此昵称已经存在
#define ERR_GP_USER_BAD					13									///未法字符
#define ERR_GP_IP_FULL					14									///IP已满
#define ERR_GP_LOCK_SUCCESS				15									///锁定机器成功	2009-11-12
#define	ERR_GP_ACCOUNT_HAS_LOCK			16									///机器已经处于锁定状态	2009-11-12
#define	ERR_GP_UNLOCK_SUCCESS			17									///解除锁定成功 2009-11-12
#define	ERR_GP_NO_LOCK					18									///机器根本就没有锁定，所以解锁失败 2009-11-12
#define	ERR_GP_CODE_DISMATCH			19									///机器码不匹配，解锁失败。	2009-11-12
#define ERR_GP_ACCOUNT_LOCKED			20									///本账号锁定了某台机器，登录失败 2009-11-12 zxj

#define ERR_GP_MATHINE_LOCKED			21

// PengJiLin, 2010-6-7, 第三方认证返回的错误码。鸥朴
#define ERR_GP_USER_NOT_EXIST           30      // 用户不存在
#define ERR_GP_USER_OVERDATE            31      // 用户已过期

//JianGuankun 20111110
#define EXP_GP_ALLO_PARTY				50								///本账号异地登陆

#endif	//_YZErrorCode_H__
