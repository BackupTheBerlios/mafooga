#include <InterfaceDefs.h>

/*============================================================================*/
const char*	gAppNameJaJp   = "魔封画";
const char*	gAppNameEnUs   = "Mafoo-Ga";
const char*	gAppVer        = "Ver 3.66";
const char*	gCright        = "Copyright "B_UTF8_COPYRIGHT" 2000-2007 by %AppAuthor%";
const char*	gAppAuthorJaJp = "ＳＨＩＮＴＡ";
const char*	gAppAuthorEnUs = "SHINTA";
const char*	gAppSignature  = "application/x-vnd.SHINTA-MafooGa";
/*============================================================================*/

/*=== Memo =====================================================================
Dest file 存在チェック
重複 ID 警告
ソート
Up ボタンを押したときに、アイテムが画面外に出たらスクロール
言語設定が変わったとき、ファイルパネルのボタンの UI 言語が変わらない

2000/09/05 プロジェクトソース総行数 : 1653

OK ＳＨＩＮＴＡ共通環境設定動的対応
OK Locale Kit 動的対応
OK Locale Kit 対応
OK 追加ボタンのラベルは、追加・置換と適宜変わる
OK TxtCtrlSrc と LViewSrc の選択状態を常に同期させる
OK ファイルパネルフォルダの終了時保存
OK プロジェクトファイルに MIME を設定
OK 新名称：All-in-one Graphics、魔封画
OK 環境設定は廃止（ドロップしたら ID などは自動決定）
OK Ctrlキーを押しながらプロジェクト切換でプロジェクト削除
OK ヘルプ
OK ドロップ受け付け
OK 実行中はボタンを灰色に
OK 環境設定（言語）
OK TLLMとTArchivable(FindBool安全版とか)をSHIMix.hに
OK ＧＵＩの「リソースファイル名」の欄を１行でまとめる

不要 プロジェクト切替メニューに「直前のプロジェクト」（まだ名無しのプロジェクトを作成中に他のプロジェクトに切り替えてしまったときに悲惨）
不要 ID 整列ダイアログを専用ダイアログにして、「選択されたアイテムのみ整列」「全てのアイテム」を選べるラジオボタンをつける
不要 環境設定・プロジェクトは、変化したときに保存
不要 「実行」時、同名プロジェクトが存在する場合は警告するか選択できる
不要 ID整列の終了時保存
==============================================================================*/

//------------------------------------------------------------------------------
#include "CErisoApp.h"
//------------------------------------------------------------------------------
// BeOS
#include <Locale.h>
#include <Mime.h>
// C++
// Add2
#include <add2/BeConstants.h>
#include <add2/FUConstants.h>
#include <add2/MiscDebug.h>
#include <add2/MiscUtils.h>
#include <add2/SHINTAConstants.h>
// Proj
#include "Common.h"
#include "Constants.h"
#include "LKKConstants.h"
#include "CErisoWindow.h"
//==============================================================================
TCErisoApp::TCErisoApp()
		: SUPER(gAppSignature)
{
	// メンバ変数初期化
	mRangeIDWin = NULL;
	mAboutWin = NULL;
	// MIME 登録
	if ( gExitStates.mAppVer != gAppVer ) {
		RegisterProjectMime();
	}
	// ウィンドウ作成・実行
	mCErisoWin = new TCErisoWindow(gExitStates.mFrame);
	mCErisoWin->Show();
}
//------------------------------------------------------------------------------
TCErisoApp::~TCErisoApp()
{
}
//------------------------------------------------------------------------------
void	TCErisoApp::AboutRequested()
{
	BRect		aWinFrame;

	if ( mAboutWin == NULL ) {
		aWinFrame = mCErisoWin->Frame();
		aWinFrame.top += FUC_HEIGHT_WIN_TAB;
		mAboutWin = new TSHINTAAboutWindow(aWinFrame);
		mAboutWin->IntelligentMoveTo(aWinFrame.LeftTop());
		mAboutWin->SetAppInfo(gAppNameEnUs, gAppVer, gCright);
		mAboutWin->SetTranslatorInfo(LKK_ABOUT_TRANSLATOR_INFO);
		mAboutWin->SetIAPRef(&gIAPs);
	}
	mAboutWin->Appear();
}
//------------------------------------------------------------------------------
void	TCErisoApp::IAPChanged(BMessage* oMsg)
{
	BString	aItemName;

	if ( oMsg->FindString(MSG_NAME_IAP_SOURCE_IAP_ITEM, &aItemName) != B_OK )
		return;
	gIAPs.SetValue(*oMsg);
	if ( aItemName == IAP_NAME_ACC_KEY_FORMAT ) {
		gAppPrivacy.UpdateAccFormat();
		SetRangeIDWindowLabels();
	}
}
//------------------------------------------------------------------------------
void	TCErisoApp::LanguageChanged()
{
	SetRangeIDWindowLabels();
}
//------------------------------------------------------------------------------
void	TCErisoApp::MessageReceived(BMessage* oMsg)
{
	switch ( oMsg->what ) {
	// 転送＆処理
	case FUC_IAP_CHANGED:
		IAPChanged(oMsg);
		BMessenger(mCErisoWin).SendMessage(oMsg);
		break;
	// 自前処理
	case B_LANGUAGE_CHANGED:
		LanguageChanged();
		break;
	case SHINTA_MITEM_SHINTA_COMMON_PREFS_CLICKED:
		MItemSHINTACommonPrefsClicked();
		break;
	case FUC_MITEM_AUTHOR_WEB_CLICKED:
		MItemAuthorWebClicked();
		break;
	case FUC_MITEM_HELP_CLICKED:
		MItemHelpClicked();
		break;
	case MITEM_RANGE_ID_CLICKED:
		MItemRangeIDClicked();
		break;
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
void	TCErisoApp::MItemAuthorWebClicked()
{
	show_url(SHINTA_URL_AUTHOR_WEB);
}
//------------------------------------------------------------------------------
void	TCErisoApp::MItemHelpClicked()
{
	string	aHelpFileName;

	aHelpFileName = locale_text_with_default(LKV_FILE_NAME_HELP, FILE_NAME_DEFAULT_HELP);
	if ( show_help((gAppPrivacy.ExePath()+aHelpFileName).c_str()) != B_OK ) {
		show_alert3(alert_caption(), BString(_T(LKK_ERR_CANNOT_SHOW_HELP)).ReplaceAll(LKR_FILE_NAME, aHelpFileName.c_str()).String(),
				acc_label("U", _T(LKK_BUTTON_OK)), "", "", -1, B_WARNING_ALERT);
	}
}
//------------------------------------------------------------------------------
void	TCErisoApp::MItemRangeIDClicked()
{
	BRect		aWinFrame;

	if ( mRangeIDWin == NULL ) {
		aWinFrame = mCErisoWin->Frame();
		aWinFrame.top += FUC_HEIGHT_WIN_TAB;
		mRangeIDWin = new TSimpleInputWindow(aWinFrame);
		mRangeIDWin->IntelligentMoveTo(aWinFrame.LeftTop());
		mRangeIDWin->SetMessage(new BMessage(RANGE_ID_REQUESTED));
		mRangeIDWin->SetTarget(mCErisoWin);
		SetRangeIDWindowLabels();
	}
	mRangeIDWin->SetText(mCErisoWin->StartID().c_str());
	mRangeIDWin->Appear();
}
//------------------------------------------------------------------------------
void	TCErisoApp::MItemSHINTACommonPrefsClicked()
{
	BRect	aWinFrame;

	aWinFrame = mCErisoWin->Frame();
	aWinFrame.top += FUC_HEIGHT_WIN_TAB;
	gIAPs.ShowWindow(aWinFrame);
}
//------------------------------------------------------------------------------
void	TCErisoApp::Quit()
{
	// 見かけの終了を速めるため、先に隠す
	mCErisoWin->Hide();
	// ダイアログを終了
	close_window(reinterpret_cast<BWindow**>(&mAboutWin));
	close_window(reinterpret_cast<BWindow**>(&mRangeIDWin));
	// メインウィンドウを終了
	close_window(reinterpret_cast<BWindow**>(&mCErisoWin));
	// 自身を終了
	SUPER::Quit();
}
//------------------------------------------------------------------------------
bool	TCErisoApp::QuitRequested()
{
	return mCErisoWin->QuitRequested();
}
//------------------------------------------------------------------------------
void	TCErisoApp::RegisterProjectMime() const
{
	BMessage		aExts;
	BMimeType		aProjMime(SIGNATURE_MAFOOGA_PROJ);

	DBEXP("TCErisoApp::RegisterProjectMime()", "MIME 登録");
	// 拡張子
	aExts.AddString(B_MSG_NAME_EXTS, EXT_MAFOOGA_PROJ);
	aExts.AddString(B_MSG_NAME_EXTS, BString(EXT_MAFOOGA_PROJ).ToUpper());
	aProjMime.SetFileExtensions(&aExts);
	// 説明
	aProjMime.SetShortDescription(_T(LKK_MIME_MAFOOGA_PROJ_SHORT_DESCRIPTION));
	aProjMime.SetLongDescription(_T(LKK_MIME_MAFOOGA_PROJ_LONG_DESCRIPTION));
	// 関連づけ
	aProjMime.SetPreferredApp(gAppSignature);
	aProjMime.Install();
}
//------------------------------------------------------------------------------
void	TCErisoApp::SetRangeIDWindowLabels()
{
	if ( mRangeIDWin == NULL )
		return;
	mRangeIDWin->SetTitle(_T(LKK_RANGE_ID_WINDOW));
	mRangeIDWin->SetLabel((acc_label("S", _T(LKK_RANGE_ID_WINDOW_TEXT))+_T(LKK_COLON_SPACE)).c_str());
	mRangeIDWin->SetButtonLabel(B_CANCEL_BUTTON, _T(LKK_BUTTON_CANCEL));
	mRangeIDWin->SetButtonLabel(B_DEFAULT_BUTTON, acc_label("G", _T(LKK_BUTTON_GO)).c_str());
}
//------------------------------------------------------------------------------
//==============================================================================
int		main(int oArgC,char* oArgV[])
{
	TCErisoApp*	aCErisoApp;

	gAppPrivacy.AnalyzeExePath(oArgV[0]);		// 最初にやる
	gExitStates.LoadFromFile();					// 2 番目にやる
	gPrefs.LoadFromFile();
	gIAPs.SetAppSignatureRef(gAppSignature);	// IAP: 3 番目にやる
	gIAPs.AddItem(new TIAPItemAccKeyFormat());
//	gIAPs.AddItem(new TIAPItemFlatToolBar());
	gIAPs.AddItem(new TIAPItemShouseiUserName());
	gIAPs.AddItem(new TIAPItemShouseiUserNumber());
	gAppPrivacy.UpdateAccFormat();
	aCErisoApp = new TCErisoApp();
	aCErisoApp->Run();
	delete aCErisoApp;
	gExitStates.SaveToFile();
	return B_OK;
}
//==============================================================================
#ifdef	DEBUG
extern "C" _EXPORT	const char	gDebugMark[] = "DEBUG/debug";
#endif
//==============================================================================
