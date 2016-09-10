#ifndef __YZBankMessage_h__
#define __YZBankMessage_h__


#include "YZBaseType.h"
#include "YZCommon/YZCommonMarco.h"

//////////////////////////////////////////////////////////////////////////
#define MDM_GP_BANK							121

//////////////////////////////////////////////////////////////////////////
//打开银行
#define ASS_GP_BANK_OPEN					21      //打开银行获取钱包信息


#define HC_GP_BANK_OPEN_SUC					1		//打开钱柜成功
#define HC_GP_BANK_OPEN_ERR					2		//打开钱柜错误
#define HC_GP_BANK_OPEN_ERR_PSW				3		//打开钱框错误

//转账返利结构体 20140314 wzd
typedef struct tag_TRANMONBACK
{
	LLONG	i64FetchTotal;         //累计领取返现金币数
	INT     nFetcYZum;             //领取次数
	LLONG	i64NoFetchTotal;       //未领取返现金币数
	bool    bFetch;                //账号是否具有转账返利特权
	LLONG	i64FetchLimit;         //领取返现金币的限制值(必须大于该值才能领取，默认是100万)
	LLONG	i64TranMaxTax;         //转账手续费上限，默认是10000000
	DOUBLE  nCommToComTax;         //普通玩家之间转账手续费费率
	DOUBLE  nCommToParTax;         //普通玩家向合伙人转账手续费费率
	DOUBLE  nParToComTax;          //合伙人向普通玩家转账手续费费率
	DOUBLE  nParToParTax;          //合伙人之间转账手续费费率
	DOUBLE  nParToComBackTax;      //合伙人向普通玩家转账返利利率
	DOUBLE  nParToParBackTax;      //合伙人之间转账返利利率

	tag_TRANMONBACK()
	{
		i64FetchTotal = 0;
		nFetcYZum = 0;
		i64NoFetchTotal = 0;
		bFetch = 0;
		i64FetchLimit = 1000000;
		i64TranMaxTax = 10000000;
		nCommToComTax = 0;
		nCommToParTax = 0;
		nParToComTax = 0;
		nParToParTax = 0;
		nParToComBackTax = 0;
		nParToParBackTax = 0;
	}

	tag_TRANMONBACK& operator=(const tag_TRANMONBACK &TranBack)
	{
		i64FetchTotal = TranBack.i64FetchTotal;
		nFetcYZum = TranBack.nFetcYZum;
		i64NoFetchTotal = TranBack.i64NoFetchTotal;
		bFetch = TranBack.bFetch;
		i64FetchLimit = TranBack.i64FetchLimit;
		i64TranMaxTax = TranBack.i64TranMaxTax;
		nCommToComTax = TranBack.nCommToComTax;
		nCommToParTax = TranBack.nCommToParTax;
		nParToComTax = TranBack.nParToComTax;
		nParToParTax = TranBack.nParToParTax;
		nParToComBackTax = TranBack.nParToComBackTax;
		nParToParBackTax = TranBack.nParToParBackTax;
		return *this;
	}
}TRANMONBACK;

// 客户端发送请求
typedef struct TMSG_GP_BankOpenReq
{
	char szMD5Pass[50]; //加密密码
} GNUC_PACKED TMSG_GP_BankOpenReq;


// 服务端返回
typedef struct TMSG_GP_BankInfo
{
	UINT			dwUserID;
	LLONG			i64Bank;							//钱柜
	LLONG			i64Ingot;                           //元宝数量
	LLONG			i64TLessMoney;                      //单次赠送最低值
	INT             iTMultiple;                         //赠送金额倍率
	CHAR			szTwoPassword[50];					//银行密码
	INT				nVer;								//银行版本
	INT             iIngotToCoinRate;                   //元宝对金币的汇率
	TRANMONBACK     TranBack;                           //转账返利结构体 20140314 wzd
	bool            IsOpenCommonTran;					//是否打开普通用户转账显示。
} GNUC_PACKED TMSG_GP_BankInfo;

//////////////////////////////////////////////////////////////////////////
// 修改密码
#define ASS_GP_BANK_CHPWD					28		//修改银行密码

#define HC_GP_BANK_CHPWD_SUC				1		//修改成功
#define HC_GP_BANK_CHPWD_ERR_PWD			2		//修改失败，密码错误

//客户端发送请求
typedef struct TMSG_GP_BankChPwd
{
	UINT			user_id;							///用户ID
	CHAR			MD5Pass_old[50];					///银行密码旧
	CHAR			MD5Pass_new[50];					///银行密码新
} GNUC_PACKED TMSG_GP_BankChPwd;


//////////////////////////////////////////////////////////////////////////
//存取钱
//服务端返回
//没有数据，通过handlecode判断修改成功或失败

#define ASS_GP_BANK_CHECK					23		//银行存取钱

#define HC_GP_BANK_CHECK_SUC				0		//操作成功
#define HC_GP_BANK_CHECK_NO_USER			1		//没有找到用户
#define HC_GP_BANK_CHECK_ERR_PWD			2		//错误密码
#define HC_GP_BANK_CHECK_NOT_MONEY			4		//余额不足

//客户端发送请求
typedef struct TMSG_GP_BankCheck
{
	INT				operate_type;	//1-取钱，2-存钱
	INT				user_id;
	INT				game_id;
	CHAR			szMD5Pass[50];  //银行密码
	LLONG			money;
} GNUC_PACKED TMSG_GP_BankCheck;
//服务端返回与发送一致
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//银行转账
#define ASS_GP_BANK_TRANSFER					24		//银行转账

#define HC_GP_BANK_TRANSFER_ERROR				1		//转帐错误
#define HC_GP_BANK_TRANSFER_SUC					2		//转帐成功
#define HC_GP_BANK_TRANSFER_NO_DEST				3		//转帐目标不存在
#define HC_GP_BANK_TRANSFER_PASSWORD_ERROR		4		//转帐密码错误
#define HC_GP_BANK_TRANSFER_TOTAL_LESS			5		//银行总额太小，不够资格
#define HC_GP_BANK_TRANSFER_TOO_LESS			6		//单笔转帐数目太少
#define HC_GP_BANK_TRANSFER_MULTIPLE			7		//单笔转帐数目必须是某数的倍数
#define HC_GP_BANK_TRANSFER_NOT_ENOUGH			8		//银行金额不够本次转帐
#define HC_GP_BANK_TRANSFER_TOO_MANY_TIME		9		//当天转账的次数太多了
#define HC_GP_BANK_TRANSFER_UPDATE_REBATE		10		//更新返利数据
#define HC_GP_BANK_TRANSFER_UPDATE_USER_REBATE	11		//更新玩家返利数据
#define HC_GP_BANK_TRANSFER_SET_REBATE_RES		12		//修改返利数据

#define MDM_GP_NOTICE_MSG         111
#define ASS_GR_TRANSFER_MONEY     6

// 转帐
typedef struct TMSG_GP_BankTransfer
{
	INT			UserID;					//用户ID
	INT			destUserID;				//转给谁
	LLONG		i64Money;				//欲转帐金额
	LLONG		i64ActualTransfer;		//实际到帐金额
	INT			bTranTax;               //是否扣税，非0不扣税
	CHAR		szMD5Pass[50];			//银行密码
	CHAR		szNickName[50];			//用户昵称		add by lxl 2010-11-10
	CHAR		szDestNickName[50];		//被转账用户昵称
	bool		bUseDestID;
	UINT        uHandleCode;			// PengJiLin, 2010-8-23, 用来区分是否是“全转”功能
	LLONG       i64FetchBack;           //返回转账成功后，玩家未领取的返利金币总值 add by wzd 20140220

	TMSG_GP_BankTransfer()
	{
		memset(this, 0x0, sizeof(TMSG_GP_BankTransfer));
	}
} GNUC_PACKED TMSG_GP_BankTransfer;

//////////////////////////////////////////////////////////////////////////
#define ASS_GP_BANK_TRANS_RECORD			25		//银行转账记录

#define HC_GP_BANK_TRANS_RECORD_SUC			1		//银行转账记录正常获取



/// 转帐记录查询请求
typedef struct TMSG_GP_BankTranRecordReq
{
	INT		dwUserID;	///用户 ID
} GNUC_PACKED TMSG_GP_BankTranRecordReq;


//用户转帐记录结果
typedef struct TMSG_GP_BankTranRecord
{
	INT				nSeriNo;				///< 序号
	UINT			dwUserID;				///< 用户 ID
	UINT			destUserID;				///< 转给谁
	LLONG			i64Money;				///< 欲转帐金额
	LLONG			i64ActualTransfer;		///< 实际到帐金额
	LLONG	oleDateTime;			///< 时间

	TMSG_GP_BankTranRecord()
	{
		memset(this, 0x0, sizeof(TMSG_GP_BankTranRecord));
	}
} GNUC_PACKED TMSG_GP_BankTranRecord;

#endif // __YZBankMessage_h__
