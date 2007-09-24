#include "CErisoWindow.h"
//------------------------------------------------------------------------------
// BeOS
#include <Application.h>
#include <Locale.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Path.h>
#include <Resources.h>
// Add2
#include <add2/BeConstants.h>
#include <add2/FUConstants.h>
#include <add2/MiscDebug.h>
#include <add2/MiscUtils.h>
#include <add2/SHINTAConstants.h>
#include <add2/SHINTAUtils.h>
// Proj
#include "Common.h"
#include "Constants.h"
#include "LKKConstants.h"
#include "CErisoView.h"
//==============================================================================
TCErisoWindow::TCErisoWindow(BRect oFrame)
		: SUPER(oFrame, "")
{
	const char*	aViewObj;
	size_t		aSize;
	BEntry		aEntry;
	BMessage	aViewArc;

	// メンバ変数初期化
	mInitCheck = B_NO_INIT;
	mMBar = NULL;
	// ビューの作成
	aViewObj = reinterpret_cast<const char*>(BApplication::AppResources()->LoadResource(B_ARCHIVED_OBJECT, RSRC_NAME_CERISO_VIEW, &aSize));
	if ( aViewObj == NULL )
		return;
	aViewArc.Unflatten(aViewObj);
	mCErisoView = new TCErisoView(&aViewArc);
	AddChild(mCErisoView);	// 最初に追加しなくてはならない
	// メニューの作成
	mMBar = new BMenuBar(BRect(0, 0, 0, 0), "MenuBar");
	AddChild(mMBar);
	// 履歴の設定
	mMRUF.SetShowSC(true);
	mMRUF.SetMaxMRUF(MAX_MRUFS);
	mMRUF.SetTgtHandler(mCErisoView);
	mMRUF.SetAccFormat(gAppPrivacy.mAccFormat);
	get_settings_entry(FOLDER_NAME_MAFOOGA_SETTINGS, FILE_NAME_MRUFS, &aEntry);
	if ( (modifiers() & FUC_CLEAN_LAUNCH_KEY) == FUC_CLEAN_LAUNCH_KEY ) {
		aEntry.Remove();
	} else {	
		mMRUF.Load(aEntry);
	}
	// 調整
	mCErisoView->MoveTo(0, mMBar->Frame().bottom+1);
	ResizeTo(mCErisoView->Frame().right, mCErisoView->Frame().bottom);
	SetWinTitle();
	CreateMenu();	// 履歴設定より後、GenerateMenuMRUF() より前
	GenerateMenuMRUF();
	mInitCheck = B_OK;
}
//------------------------------------------------------------------------------
TCErisoWindow::~TCErisoWindow()
{
}
//------------------------------------------------------------------------------
void	TCErisoWindow::AddHistoryRequested(BMessage* oMsg)
{
	entry_ref	aERef;

	if ( oMsg->FindRef(B_MSG_NAME_REFS, &aERef) != B_OK )
		return;
	mMRUF.AddMRUF(BPath(&aERef).Path());
	GenerateMenuMRUF();
}
//------------------------------------------------------------------------------
void	TCErisoWindow::CreateMenu()
{
	BMenu*			aMenu;
	BMenuItem*		aMItem;
	BMenuItem*		aMItemDelAllMRUFs;

	// プロジェクト
	aMenu = new BMenu(acc_label("P", _T(LKK_MENU_PROJECT)).c_str());
		// 新規作成
		aMItem = new BMenuItem(acc_label("N", _T(LKK_MITEM_NEW)).c_str(), new BMessage(FUC_MITEM_NEW_CLICKED), 'N');
		aMenu->AddItem(aMItem);
		// 開く
		mMenuOpen = new BMenu((acc_label("O", _T(LKK_MENU_OPEN))+FUC_ELLIPSIS).c_str());
			mMenuOpen->AddSeparatorItem();
			aMItemDelAllMRUFs = new BMenuItem(acc_label("D", _T(LKK_MITEM_DEL_ALL_MRUPS)).c_str(), new BMessage(FUC_MITEM_DEL_ALL_HISTORY_CLICKED));
			mMenuOpen->AddItem(aMItemDelAllMRUFs);
		aMenu->AddItem(mMenuOpen);
		mMenuOpen->Superitem()->SetMessage(new BMessage(FUC_MITEM_OPEN_CLICKED));
		mMenuOpen->Superitem()->SetShortcut('O', B_COMMAND_KEY);
		aMenu->AddSeparatorItem();
		// 名前を付けて保存
		aMItem = new BMenuItem((acc_label("A", _T(LKK_MITEM_SAVE_AS))+FUC_ELLIPSIS).c_str(), new BMessage(FUC_MITEM_SAVE_AS_CLICKED), '2');
		aMenu->AddItem(aMItem);
		aMenu->AddSeparatorItem();
		// 実行
		mMItemGo = new BMenuItem(acc_label("G", _T(LKK_MITEM_GO)).c_str(), new BMessage(MITEM_GO_CLICKED), 'G');
		aMenu->AddItem(mMItemGo);
		aMenu->AddSeparatorItem();
		aMenu->SetTargetForItems(mCErisoView);	// 終了以外はビューへ
		aMItemDelAllMRUFs->SetTarget(this);		// でも履歴削除はウィンドウへ
		// 終了
		aMItem = new BMenuItem(acc_label("Q", _T(LKK_MITEM_QUIT)).c_str(), new BMessage(B_QUIT_REQUESTED), 'Q');
		aMenu->AddItem(aMItem);
	mMBar->AddItem(aMenu);
	// ツール
	aMenu = new BMenu(acc_label("T", _T(LKK_MENU_TOOL)).c_str());
		// ID 整列
		aMItem = new BMenuItem((acc_label("R", _T(LKK_MITEM_RANGE_ID))+FUC_ELLIPSIS).c_str(), new BMessage(MITEM_RANGE_ID_CLICKED), 'R');
		aMenu->AddItem(aMItem);
		aMenu->AddSeparatorItem();
		// ＳＨＩＮＴＡ共通環境設定
		aMItem = new BMenuItem(acc_label("S", _T(LKK_MITEM_SHINTA_COMMON_PREFS)+FUC_ELLIPSIS).c_str(), new BMessage(SHINTA_MITEM_SHINTA_COMMON_PREFS_CLICKED));
		aMenu->AddItem(aMItem);
		aMenu->SetTargetForItems(be_app);
	mMBar->AddItem(aMenu);
	// ヘルプ
	aMenu = new BMenu(acc_label("H", _T(LKK_MENU_HELP)).c_str());
		// ヘルプ
		aMItem = new BMenuItem(acc_label("H", _T(LKK_MITEM_HELP)).c_str(), new BMessage(FUC_MITEM_HELP_CLICKED));
		aMenu->AddItem(aMItem);
		aMenu->AddSeparatorItem();
		// ＳＨＩＮＴＡのホームページを開く
		aMItem = new BMenuItem(acc_label("J", BString(_T(LKK_MITEM_AUTHOR_WEB)).ReplaceAll(LKR_APP_AUTHOR, _T(gAppAuthorEnUs)).String()).c_str(),
				new BMessage(FUC_MITEM_AUTHOR_WEB_CLICKED));
		aMenu->AddItem(aMItem);
		aMenu->AddSeparatorItem();
		// バージョン情報
		aMItem = new BMenuItem(acc_label("A", _T(LKK_MITEM_ABOUT)+FUC_ELLIPSIS).c_str(), new BMessage(B_ABOUT_REQUESTED));
		aMenu->AddItem(aMItem);
		aMenu->SetTargetForItems(be_app);
	mMBar->AddItem(aMenu);
}
//------------------------------------------------------------------------------
void	TCErisoWindow::DestroyMenu()
{
	BMenu*	aMenu;
	
	while  ( (aMenu = mMBar->SubmenuAt(0)) != NULL ) {
		mMBar->RemoveItem(aMenu);
		delete aMenu;
	}
}
//------------------------------------------------------------------------------
void	TCErisoWindow::GenerateMenuMRUF()
{
	BEntry	aEntry;

	// メニューの生成
	mMRUF.SetTgtMenu(mMenuOpen);
	mMRUF.GenerateMenu();
	mMenuOpen->ItemAt(mMRUF.CountMRUFs()+1)->SetEnabled(mMRUF.CountMRUFs() > 0);
	// 保存
	get_settings_entry(FOLDER_NAME_MAFOOGA_SETTINGS, FILE_NAME_MRUFS, &aEntry);
	mMRUF.Save(aEntry);
}
//------------------------------------------------------------------------------
void	TCErisoWindow::IAPChanged(BMessage* oMsg)
{
	DestroyMenu();
	CreateMenu();
	mMRUF.SetAccFormat(gAppPrivacy.mAccFormat);
	GenerateMenuMRUF();
}
//------------------------------------------------------------------------------
void	TCErisoWindow::LanguageChanged()
{
	SetWinTitle();
	DestroyMenu();
	CreateMenu();
	GenerateMenuMRUF();
}
//------------------------------------------------------------------------------
void	TCErisoWindow::MenusBeginning()
{
	mMItemGo->SetEnabled(mCErisoView->IsButtonGoEnabled());
}
//------------------------------------------------------------------------------
void	TCErisoWindow::MessageReceived(BMessage* oMsg)
{
	switch( oMsg->what ) {
	// 転送
	case RANGE_ID_REQUESTED:
		BMessenger(mCErisoView).SendMessage(oMsg);
		break;
	// 自前処理
	case FUC_ADD_HISTORY_REQUESTED:
		AddHistoryRequested(oMsg);
		break;
	case FUC_MITEM_DEL_ALL_HISTORY_CLICKED:
		MItemDelAllHistoryClicked();
		break;
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
void	TCErisoWindow::MItemDelAllHistoryClicked()
{
	if ( show_alert3(alert_caption(), _T(LKK_ASK_DEL_ALL_MRUPS), acc_label("N", _T(LKK_BUTTON_NO)),
			acc_label("Y", _T(LKK_BUTTON_YES))) == 0 )
		return;
	mMRUF.ClearMRUF();
	GenerateMenuMRUF();
}
//------------------------------------------------------------------------------
void	TCErisoWindow::Quit()
{
	gExitStates.mFrame = Frame();
	SUPER::Quit();
}
//------------------------------------------------------------------------------
bool	TCErisoWindow::QuitRequested()
{
	mCErisoView->SaveExitProject();
	be_app_messenger.SendMessage(B_QUIT_REQUESTED);
	return SUPER::QuitRequested();
}
//------------------------------------------------------------------------------
void	TCErisoWindow::SetWinTitle()
{
#ifdef DEBUG
	char		aTitle[256];
	strcpy(aTitle, "［デバッグ］ ");
	strcat(aTitle, _T(gAppNameEnUs));
	SetTitle(aTitle);
#else
	SetTitle(_T(gAppNameEnUs));
#endif
}
//------------------------------------------------------------------------------
string	TCErisoWindow::StartID() const
{
	return mCErisoView->StartID();
}
//------------------------------------------------------------------------------
//==============================================================================
