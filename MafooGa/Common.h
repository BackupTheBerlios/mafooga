#ifndef	___CErisoCommon_H_
#define	___CErisoCommon_H_
//------------------------------------------------------------------------------
#ifdef	GLOBAL_DEF
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif
//------------------------------------------------------------------------------
// Add2
#include <add2/AppPrivacy.h>
#include <add2/SHINTAArchivable.h>
#include <add2/SHINTAIAPs.h>
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================
class	TCErisoAppPrivacy : public TAppPrivacy
{
public:
	TCErisoAppPrivacy();
	virtual	~TCErisoAppPrivacy();

	void		UpdateAccFormat();

	string						mAccFormat;

private:
	typedef	TAppPrivacy	SUPER;
};
//==============================================================================
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
終了時の状態
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
class	TExitStates : public TSHINTAArchivable
{
public:
	TExitStates();
	~TExitStates();

	virtual status_t	Archive(BMessage* oArc, bool oDeep = true) const;
	status_t			LoadFromFile();
	status_t			SaveToFile();
	virtual status_t	Unarchive(const BMessage& oMsg);	// メッセージから情報を得る

	string			mAppVer;								// App: バージョンチェック用
	string			mFolderNamePanelBrowseDest;				// View: デストフォルダ
	string			mFolderNamePanelBrowseSrc;				// View: ソースフォルダ
	string			mFolderNamePanelProj;					// View: プロジェクトフォルダ
	BRect			mFrame;									// Win: ウィンドウの位置

private:
	typedef	TSHINTAArchivable	SUPER;	// スーパークラス
};
//==============================================================================
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
ユーザーが設定できる項目
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
class	TPrefs : public TSHINTAArchivable
{
public:
	TPrefs();
	~TPrefs();
	virtual status_t	Archive(BMessage* oArc, bool oDeep = true) const;
	status_t			LoadFromFile();
	status_t			SaveToFile();
	virtual status_t	Unarchive(const BMessage& oMsg);	// メッセージから情報を得る

private:
	typedef	TSHINTAArchivable	SUPER;	// スーパークラス
};
//==============================================================================
string		acc_label(const string& oAcc, const string& oStr);
string		alert_caption();
//==============================================================================
// グローバル変数
extern const char*			gAppAuthorEnUs;
extern const char*			gAppAuthorJaJp;
extern const char*			gAppNameEnUs;
extern const char*			gAppNameJaJp;
extern const char*			gAppSignature;
extern const char*			gAppVer;
extern const char*			gCright;
GLOBAL TExitStates			gExitStates;
GLOBAL TInterAppPrefs		gIAPs;
GLOBAL TPrefs				gPrefs;
GLOBAL TCErisoAppPrivacy	gAppPrivacy;
//==============================================================================
#endif	// ___CErisoCommon_H_
