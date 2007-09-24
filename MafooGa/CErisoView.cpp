#include "CErisoView.h"
//------------------------------------------------------------------------------
// BeOS
#include <Application.h>
#include <File.h>
#include <Locale.h>
#include <NodeInfo.h>
#include <Path.h>
#include <Resources.h>
// Add2
#include <add2/BeConstants.h>
#include <add2/FUConstants.h>
#include <add2/MiscDebug.h>
#include <add2/MiscUtils.h>
#include <add2/SHINTAUtils.h>
#include <add2/SimpleInput.h>
#include <add2/StringUtils.h>
// Proj
#include "Common.h"
#include "Constants.h"
#include "LKKConstants.h"
#include "VPCErisoView.h"
//==============================================================================
TCErisoView::TCErisoView(BMessage* oArc)
		: SUPER(oArc)
{
	BRect	aFrameTmp;

	// メンバ変数初期化
	mPanelBrowseDest = NULL;
	mPanelBrowseSrc = NULL;
	mPanelProjOpen = NULL;
	mPanelProjSave = NULL;
	GetChildPointers();
	// ビュー作成
	aFrameTmp.left = MARGIN_WV;
	aFrameTmp.top = mButtonUp->Frame().top;
	aFrameTmp.right = mButtonUp->Frame().left-(MARGIN_VV+B_V_SCROLL_BAR_WIDTH);
	aFrameTmp.bottom = mButtonRemove->Frame().bottom-B_H_SCROLL_BAR_HEIGHT;
			mLViewSrcImages = new BListView(aFrameTmp, "LViewSrcImages", B_MULTIPLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES);
		mScrlViewSrcImages = new BScrollView("ScrlViewSrcImages", mLViewSrcImages, B_FOLLOW_NONE, 0, true, true);
	AddChild(mScrlViewSrcImages);
	// ビュー設定
	mTxtCtrlSrcFileName->SetModificationMessage(new BMessage(TXT_CTRL_SRC_FILE_NAME_CHANGED));
	mButtonBrowseSrc->SetMessage(new BMessage(BUTTON_BROWSE_SRC_CLICKED));
	mButtonAdd->SetMessage(new BMessage(FUC_BUTTON_ADD_CLICKED));
	mLViewSrcImages->SetSelectionMessage(new BMessage(LVIEW_SRC_IMAGES_SELECTED));
	mButtonUp->SetMessage(new BMessage(FUC_BUTTON_UP_CLICKED));
	mButtonDown->SetMessage(new BMessage(FUC_BUTTON_DOWN_CLICKED));
	mButtonRemove->SetMessage(new BMessage(FUC_BUTTON_REMOVE_CLICKED));
	mTxtCtrlDestFileName->SetModificationMessage(new BMessage(TXT_CTRL_DEST_FILE_NAME_CHANGED));
	mButtonBrowseDest->SetMessage(new BMessage(BUTTON_BROWSE_DEST_CLICKED));
	mStatBarProgress->Hide();
	mButtonGo->SetMessage(new BMessage(MITEM_GO_CLICKED));
	mButtonGo->MakeDefault(true);
}
//------------------------------------------------------------------------------
TCErisoView::~TCErisoView()
{
	entry_ref	aERef;

	// 終了時の状態を保存
		// ソース
	if ( mPanelBrowseSrc != NULL ) {
		mPanelBrowseSrc->GetPanelDirectory(&aERef);
		gExitStates.mFolderNamePanelBrowseSrc = BPath(&aERef).Path();
	}
		// デスト
	if ( mPanelBrowseDest != NULL ) {
		mPanelBrowseDest->GetPanelDirectory(&aERef);
		gExitStates.mFolderNamePanelBrowseDest = BPath(&aERef).Path();
	}
		// プロジェクトを開く・保存
	if ( mPanelProjOpen != NULL ) {
		mPanelProjOpen->GetPanelDirectory(&aERef);
		gExitStates.mFolderNamePanelProj = BPath(&aERef).Path();
	}
	if ( mPanelProjSave != NULL ) {
		mPanelProjSave->GetPanelDirectory(&aERef);
		gExitStates.mFolderNamePanelProj = BPath(&aERef).Path();
	}
	// 解放
	delete mPanelBrowseSrc;
	delete mPanelBrowseDest;
}
//------------------------------------------------------------------------------
void	TCErisoView::AddSrc()
{
	TCErisoSItem*	aSItem;

	aSItem = new TCErisoSItem(mTxtCtrlSrcFileName->Text(), atoi(mTxtCtrlRsrcID->Text()), mTxtCtrlRsrcName->Text());
	mLViewSrcImages->AddItem(aSItem);
}
//------------------------------------------------------------------------------
void	TCErisoView::AllAttached()
{
	BEntry	aEntry;

	// ターゲットの設定
	SetTargetToThis(this);
	mLViewSrcImages->SetTarget(this);
	// 終了時のプロジェクト
	if ( (modifiers() & FUC_CLEAN_LAUNCH_KEY) == FUC_CLEAN_LAUNCH_KEY ) {
		get_settings_entry(FOLDER_NAME_MAFOOGA_SETTINGS, FILE_NAME_EXIT_PROJECT, &aEntry);
		aEntry.Remove();
	} else {
		OpenExitProject();
	}
	// 表示設定
	SetViewLabels();
	UpdateCompos();
	UpdateComposEnabled();
	mTxtCtrlSrcFileName->MakeFocus(true);
}
//------------------------------------------------------------------------------
void	TCErisoView::AttachedToWindow()
{
}
//------------------------------------------------------------------------------
int32	TCErisoView::AutoResourceID() const
{
	int32			aAutoRsrcID;	// 自動計算したリソース ID（返値）
	int32			aIdx;			//ターゲットソース（テキストコントロール入力値）の登録リスト中でのインデックス
	TCErisoSItem*	aSItem;

	aIdx = FindSrc();
	if ( aIdx >= 0 ) {	// ターゲットソースは既に登録済
		aSItem = dynamic_cast<TCErisoSItem*>(mLViewSrcImages->ItemAt(aIdx));
		aAutoRsrcID = aSItem->mRsrcID;
	} else {	// ターゲットソースは未登録
		if ( mLViewSrcImages->CountItems() == 0 ) {
			aAutoRsrcID = 0;
		} else {
			aSItem = dynamic_cast<TCErisoSItem*>(mLViewSrcImages->ItemAt(mLViewSrcImages->CountItems()-1));
			aAutoRsrcID = aSItem->mRsrcID+1;
		}
	}
	return aAutoRsrcID;
}
//------------------------------------------------------------------------------
string	TCErisoView::AutoResourceName() const
{
	int32			aIdx;			// ターゲットソース（テキストコントロール入力値）の登録リスト中でのインデックス
	string			aAutoRsrcName;	// 算出したリソース名（返値）
	TCErisoSItem*	aSItem;

	aIdx = FindSrc();
	if ( aIdx >= 0 ) {	// ターゲットソースは既に登録済
		aSItem = dynamic_cast<TCErisoSItem*>(mLViewSrcImages->ItemAt(aIdx));
		aAutoRsrcName = aSItem->mRsrcName;
	} else {	// ターゲットソースは未登録
		aAutoRsrcName = extract_file_body(mTxtCtrlSrcFileName->Text()).c_str();
	}
	return aAutoRsrcName;
}
//------------------------------------------------------------------------------
void	TCErisoView::ButtonAddClicked()
{
	int		aIdx;

	ASSERT(mTxtCtrlSrcFileName->Text()[0] == '\0', "TCErisoView::ButtonAddClicked()");
	aIdx = FindSrc();
	if ( aIdx >= 0 ) {
		ReplaceSrc(aIdx);
	} else {
		AddSrc();
	}
	UpdateStrViewSrcImages();
	UpdateButtonGo();
}
//------------------------------------------------------------------------------
void	TCErisoView::ButtonBrowseDestClicked()
{
	ShowPanelBrowse(&mPanelBrowseDest);
}
//------------------------------------------------------------------------------
void	TCErisoView::ButtonBrowseSrcClicked()
{
	ShowPanelBrowse(&mPanelBrowseSrc);
}
//------------------------------------------------------------------------------
void	TCErisoView::ButtonDownClicked()
{
	if ( mLViewSrcImages->CountItems() <= 1 || mLViewSrcImages->IsItemSelected(mLViewSrcImages->CountItems()-1) )
		return;
	for ( int32 i = mLViewSrcImages->CountItems()-2 ; i >= 0 ; i-- ) {
		if ( mLViewSrcImages->IsItemSelected(i) ) {
			MoveSrc(i, i+1);
		}
	}
}
//------------------------------------------------------------------------------
void	TCErisoView::ButtonRemoveClicked()
{
	for ( int32 i = mLViewSrcImages->CountItems()-1 ; i >= 0 ; i-- ) {
		if ( mLViewSrcImages->IsItemSelected(i) ) {
			delete mLViewSrcImages->RemoveItem(i);
		}
	}
	UpdateStrViewSrcImages();
	UpdateButtonGo();
}
//------------------------------------------------------------------------------
void	TCErisoView::ButtonUpClicked()
{
	if ( mLViewSrcImages->CountItems() <= 1 || mLViewSrcImages->IsItemSelected(0) )
		return;
	for ( int32 i = 1 ; i < mLViewSrcImages->CountItems() ; i++ ) {
		if ( mLViewSrcImages->IsItemSelected(i) ) {
			MoveSrc(i, i-1);
		}
	}
}
//------------------------------------------------------------------------------
void	TCErisoView::Cancel(BMessage* oMsg)
{
	void*	aSrc;	// メッセージを送って来たファイルパネル

	if ( oMsg->FindPointer(B_MSG_NAME_SOURCE, &aSrc) != B_OK )
		return;
	set_file_panel_priority(reinterpret_cast<BFilePanel*>(aSrc), B_LOW_PRIORITY);
}
//------------------------------------------------------------------------------
void	TCErisoView::DestDropped(BMessage* oMsg)
{
	entry_ref	aERef;

	if ( oMsg->FindRef(B_MSG_NAME_REFS, &aERef) != B_OK )
		return;
	mTxtCtrlDestFileName->SetText(BPath(&aERef).Path());
}
//------------------------------------------------------------------------------
void	TCErisoView::DisableCompos()
{
	SetCompoEnables(false);
}
//------------------------------------------------------------------------------
int32	TCErisoView::FindSrc() const
{
	int32			aIdx = -1;	// ソースファイルの、リストビューの中におけるインデックス（返値）
	TCErisoSItem*	aSItem;

	for ( int32 i = 0 ; i < mLViewSrcImages->CountItems() ; i++ ) {
		aSItem = dynamic_cast<TCErisoSItem*>(mLViewSrcImages->ItemAt(i));
		ASSERT(aSItem == NULL, "TCErisoView::FindSrc() - not aSItem");
		if ( aSItem->mSrcFileName == mTxtCtrlSrcFileName->Text() ) {
			aIdx = i;
			break;
		}
	}
	return aIdx;
}
//------------------------------------------------------------------------------
void	TCErisoView::GetChildPointers()
{
	// 格納したい画像ファイル名
	mTxtCtrlSrcFileName = dynamic_cast<BTextControl*>(FindView(VIEW_NAME_TXT_CTRL_SRC_FILE_NAME));
	mButtonBrowseSrc = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_BROWSE_SRC));
	// リソース情報
	mTxtCtrlRsrcID = dynamic_cast<BTextControl*>(FindView(VIEW_NAME_TXT_CTRL_RSRC_ID));
	mTxtCtrlRsrcName = dynamic_cast<BTextControl*>(FindView(VIEW_NAME_TXT_CTRL_RSRC_NAME));
	// 取り込む画像
	mStrViewSrcImages = dynamic_cast<BStringView*>(FindView(VIEW_NAME_STR_VIEW_SRC_IMAGES));
	// 追加
	mButtonAdd = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_ADD));
	// ソース操作ボタン
	mButtonUp = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_UP));
	mButtonDown = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_DOWN));
	mButtonRemove = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_REMOVE));
	// セパレータ 1
	mBoxSep1 = dynamic_cast<BBox*>(FindView(VIEW_NAME_BOX_SEP_1));
	// 出力先リソースファイル名
	mTxtCtrlDestFileName = dynamic_cast<BTextControl*>(FindView(VIEW_NAME_TXT_CTRL_DEST_FILE_NAME));
	mButtonBrowseDest = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_BROWSE_DEST));
	// セパレータ 2
	mBoxSep2 = dynamic_cast<BBox*>(FindView(VIEW_NAME_BOX_SEP_2));
	// 進捗状況
	mStatBarProgress = dynamic_cast<BStatusBar*>(FindView(VIEW_NAME_STAT_BAR_PROGRESS));
	// 実行
	mButtonGo = dynamic_cast<BButton*>(FindView(VIEW_NAME_BUTTON_GO));
}
//------------------------------------------------------------------------------
void	TCErisoView::IAPChanged(BMessage* oMsg)
{
	SetViewLabels();
}
//------------------------------------------------------------------------------
bool	TCErisoView::IsButtonGoEnabled() const
{
	return mButtonGo->IsEnabled();
}
//------------------------------------------------------------------------------
void	TCErisoView::LanguageChanged()
{
	SetViewLabels();
	SetPanelBrowseLabels(mPanelBrowseSrc);
	SetPanelBrowseLabels(mPanelBrowseDest);
	SetPanelProjLabels(mPanelProjOpen);
	SetPanelProjLabels(mPanelProjSave);
}
//------------------------------------------------------------------------------
void	TCErisoView::LViewSrcImagesSelected()
{
	int32			aSelIdx;
	BHandler*		aTgtBak;
	TCErisoSItem*	aSItem;

	UpdateSourceButtonsEnabled();
	aSelIdx = mLViewSrcImages->CurrentSelection();
	if ( aSelIdx < 0 )
		return;
	aSItem = dynamic_cast<TCErisoSItem*>(mLViewSrcImages->ItemAt(aSelIdx));
	aTgtBak = mTxtCtrlSrcFileName->Target();
	mTxtCtrlSrcFileName->SetTarget(NULL);	// テキストを変更するとリストビューの選択状態が変更されるのを抑止（リストアイテムが複数選択されているときの不都合を回避）
	mTxtCtrlSrcFileName->SetText(aSItem->mSrcFileName.c_str());
	mTxtCtrlSrcFileName->SetTarget(aTgtBak);
	mTxtCtrlRsrcID->SetText((BString() << aSItem->mRsrcID).String());
	mTxtCtrlRsrcName->SetText(aSItem->mRsrcName.c_str());
	UpdateButtonAdd();
}
//------------------------------------------------------------------------------
status_t	TCErisoView::MakeRsrc() const
{
	char*			aBuf;		//画像ファイルをまるごと格納
	off_t			aSize;
	BFile			aDestFile;
	BFile			aSrcFile;
	BNodeInfo		aNodeInfo;
	BResources		aRsrc;
	TCErisoSItem*	aSItem;

	mStatBarProgress->Reset();
	mStatBarProgress->SetMaxValue(1.0);
	// 出力ファイルの初期化
	aDestFile.SetTo(mTxtCtrlDestFileName->Text(), B_READ_WRITE|B_ERASE_FILE|B_CREATE_FILE);
	aRsrc.SetTo(&aDestFile, true);
	// リソースを吐く
	for ( int32 i = 0 ; i < mLViewSrcImages->CountItems() ; i++ ) {
		mStatBarProgress->Update(1.0/mLViewSrcImages->CountItems());
#ifdef	DEBUG
		snooze(100*1000);
#endif
		aSItem = dynamic_cast<TCErisoSItem*>(mLViewSrcImages->ItemAt(i));
		if ( aSrcFile.SetTo(aSItem->mSrcFileName.c_str(), B_READ_ONLY) != B_OK )
			continue;
		aSrcFile.GetSize(&aSize);
		aBuf = new char [aSize];
		aSrcFile.Read(aBuf, aSize);
		aRsrc.AddResource(B_RAW_TYPE, aSItem->mRsrcID, aBuf, aSize,aSItem->mRsrcName.c_str());
		delete [] aBuf;
	}
	// リソースファイルの MIME Type 設定
	aNodeInfo.SetTo(&aDestFile);
	aNodeInfo.SetType(FUC_MIME_TYPE_RESOURCE_FILE);
	return B_OK;
}
//------------------------------------------------------------------------------
void	TCErisoView::MessageReceived(BMessage* oMsg)
{
	switch ( oMsg->what ) {
	// 自前処理
	case B_CANCEL:
		Cancel(oMsg);
		break;
	case B_REFS_RECEIVED:
		RefsReceived(oMsg);
		break;
	case B_SAVE_REQUESTED:
		SaveRequested(oMsg);
		break;
	case B_SIMPLE_DATA:
		SimpleData(oMsg);
		break;
	case FUC_BUTTON_ADD_CLICKED:
		ButtonAddClicked();
		break;
	case FUC_BUTTON_DOWN_CLICKED:
		ButtonDownClicked();
		break;
	case FUC_BUTTON_REMOVE_CLICKED:
		ButtonRemoveClicked();
		break;
	case FUC_BUTTON_UP_CLICKED:
		ButtonUpClicked();
		break;
	case FUC_MITEM_NEW_CLICKED:
		MItemNewClicked();
		break;
	case FUC_MITEM_OPEN_CLICKED:
		MItemOpenClicked();
		break;
	case FUC_MITEM_SAVE_AS_CLICKED:
		MItemSaveAsClicked();
		break;
	case BUTTON_BROWSE_DEST_CLICKED:
		ButtonBrowseDestClicked();
		break;
	case BUTTON_BROWSE_SRC_CLICKED:
		ButtonBrowseSrcClicked();
		break;
	case LVIEW_SRC_IMAGES_SELECTED:
		LViewSrcImagesSelected();
		break;
	case MITEM_GO_CLICKED:
		MItemGoClicked();
		break;
	case RANGE_ID_REQUESTED:
		RangeIDRequested(oMsg);
		break;
	case SET_DEST_FILE_NAME_REQUESTED:
		SetDestFileNameRequested(oMsg);
		break;
	case SET_SOURCE_FILE_NAME_REQUESTED:
		SetSourceFileNameRequested(oMsg);
		break;
	case TXT_CTRL_DEST_FILE_NAME_CHANGED:
		TxtCtrlDestFileNameChanged();
		break;
	case TXT_CTRL_SRC_FILE_NAME_CHANGED:
		TxtCtrlSrcFileNameChanged();
		break;
	default:
		SUPER::MessageReceived(oMsg);
	}
}
//------------------------------------------------------------------------------
void	TCErisoView::MItemGoClicked()
{
	ASSERT(!mButtonGo->IsEnabled(), "TCErisoView::MItemGoClicked()");
	DisableCompos();
	mStatBarProgress->Show();
	if ( MakeRsrc() == B_OK ) {
		show_alert3(alert_caption(), _T(LKK_DONE_MAKE_RESOURCE), _T(LKK_BUTTON_OK));
	} else {
		show_alert3(alert_caption(), _T(LKK_ERR_CANNOT_MAKE_RESOURCE), _T(LKK_BUTTON_OK), string(),
				string(), -1, B_WARNING_ALERT);
	}
	mStatBarProgress->Hide();
	UpdateComposEnabled();
}
//------------------------------------------------------------------------------
void	TCErisoView::MItemNewClicked()
{
	mTxtCtrlSrcFileName->SetText("");
	mTxtCtrlRsrcID->SetText("");
	mTxtCtrlRsrcName->SetText("");
	for ( int32 i = mLViewSrcImages->CountItems()-1 ; i >= 0 ; i-- ) {
		delete mLViewSrcImages->RemoveItem(i);
	}
	mTxtCtrlDestFileName->SetText("");
	UpdateCompos();
}
//------------------------------------------------------------------------------
void	TCErisoView::MItemOpenClicked()
{
	ShowPanelProj(&mPanelProjOpen);
}
//------------------------------------------------------------------------------
void	TCErisoView::MItemSaveAsClicked()
{
	ShowPanelProj(&mPanelProjSave);
}
//------------------------------------------------------------------------------
void	TCErisoView::MoveSrc(int32 oBefore, int32 oAfter)
{
	TCErisoSItem*	aItem;

	ASSERT(oBefore < 0 || oBefore >= mLViewSrcImages->CountItems() || oAfter < 0
			|| oAfter >= mLViewSrcImages->CountItems() || oBefore == oAfter, "TCErisoView::MoveSrc()");
	aItem = dynamic_cast<TCErisoSItem*>(mLViewSrcImages->RemoveItem(oBefore));
	mLViewSrcImages->AddItem(aItem, oAfter);
	mLViewSrcImages->Select(oAfter, true);
}
//------------------------------------------------------------------------------
status_t	TCErisoView::OpenExitProject()
{
	entry_ref	aERef;
	BEntry		aEntry;

	if ( get_settings_entry(FOLDER_NAME_MAFOOGA_SETTINGS, FILE_NAME_EXIT_PROJECT, &aEntry) != B_OK )
		return B_ERROR;
	aEntry.GetRef(&aERef);
	return OpenProject(aERef);
}
//------------------------------------------------------------------------------
status_t	TCErisoView::OpenProject(const entry_ref& oERef)
{
	int32			aNumItems;
	type_code		aType;
	BFile			aFile(&oERef, B_READ_ONLY);
	BMessage		aItemArc;
	BMessage		aProjArc;
	BString			aString;
	TCErisoSItem*	aSItem;

	if ( aFile.InitCheck() != B_OK )
		return B_ERROR;
	if ( aProjArc.Unflatten(&aFile) != B_OK )
		return B_ERROR;
	// ソース
	aProjArc.FindString(ARC_NAME_SOURCE_FILE_NAME, &aString);
	mTxtCtrlSrcFileName->SetText(aString.String());
	aProjArc.FindString(ARC_NAME_RESOURCE_ID, &aString);
	mTxtCtrlRsrcID->SetText(aString.String());
	aProjArc.FindString(ARC_NAME_RESOURCE_NAME, &aString);
	mTxtCtrlRsrcName->SetText(aString.String());
	// アイテム削除
	for ( int32 i = mLViewSrcImages->CountItems()-1 ; i >= 0 ; i-- ) {
		delete mLViewSrcImages->RemoveItem(i);
	}
	// アイテム追加
	aProjArc.GetInfo(ARC_NAME_SOURCE_ITEM, &aType, &aNumItems);
	for ( int32 i = 0 ; i < aNumItems ; i++ ) {
		aProjArc.FindMessage(ARC_NAME_SOURCE_ITEM, i, &aItemArc);
		aSItem = new TCErisoSItem(&aItemArc);
		mLViewSrcImages->AddItem(aSItem);
	}
	// デスト
	aProjArc.FindString(ARC_NAME_DEST_FILE_NAME, &aString);
	mTxtCtrlDestFileName->SetText(aString.String());
	return B_ERROR;
}
//------------------------------------------------------------------------------
void	TCErisoView::RangeIDRequested(BMessage* oMsg)
{
	bool			aForce;
	int32			aID;
	BString			aText;
	TCErisoSItem*	aSItem;

	if ( oMsg->FindString(SID_MSG_NAME_SIMPLE_INPUT_TEXT, &aText) != B_OK )
		return;
	aID = atoi(aText.String());
	aForce = (mLViewSrcImages->CurrentSelection(1) < 0);	// 選択されているアイテムが無いまたは 1 つだったらすべて整列
	for ( int32 i = 0 ; i < mLViewSrcImages->CountItems() ; i++ ) {
		if ( aForce || mLViewSrcImages->IsItemSelected(i) ) {
			aSItem = dynamic_cast<TCErisoSItem*>(mLViewSrcImages->ItemAt(i));
			aSItem->mRsrcID = aID;
			aSItem->UpdateText();
			aID++;
		}
	}
	mLViewSrcImages->Invalidate();
}
//------------------------------------------------------------------------------
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
プロジェクトを開く
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
void	TCErisoView::RefsReceived(BMessage* oMsg)
{
	entry_ref	aERef;

	if ( oMsg->FindRef(B_MSG_NAME_REFS, &aERef) != B_OK )
		return;
	OpenProject(aERef);
	SendAddHistoryRequested(aERef);
}
//------------------------------------------------------------------------------
void	TCErisoView::ReplaceSrc(int32 oIdx)
{
	TCErisoSItem*	aSItem;

	aSItem = dynamic_cast<TCErisoSItem*>(mLViewSrcImages->ItemAt(oIdx));
	aSItem->mRsrcID = atoi(mTxtCtrlRsrcID->Text());
	aSItem->mRsrcName = mTxtCtrlRsrcName->Text();
	aSItem->UpdateText();
	mLViewSrcImages->Invalidate();
}
//------------------------------------------------------------------------------
status_t	TCErisoView::SaveExitProject() const
{
	entry_ref	aERef;
	BEntry		aEntry;

	if ( get_settings_entry(FOLDER_NAME_MAFOOGA_SETTINGS, FILE_NAME_EXIT_PROJECT, &aEntry) != B_OK )
		return B_ERROR;
	aEntry.GetRef(&aERef);
	return SaveProject(aERef);
}
//------------------------------------------------------------------------------
status_t	TCErisoView::SaveProject(const entry_ref& oERef) const
{
	BFile		aFile(&oERef, B_WRITE_ONLY|B_CREATE_FILE|B_ERASE_FILE);
	BMessage	aItemArc;
	BMessage	aProjArc;
	BNodeInfo	aNodeInfo;

	if ( aFile.InitCheck() != B_OK )
		return B_ERROR;
	// ソース
	aProjArc.AddString(ARC_NAME_SOURCE_FILE_NAME, mTxtCtrlSrcFileName->Text());
	aProjArc.AddString(ARC_NAME_RESOURCE_ID, mTxtCtrlRsrcID->Text());
	aProjArc.AddString(ARC_NAME_RESOURCE_NAME, mTxtCtrlRsrcName->Text());
	// アイテム
	for ( int32 i = 0 ; i < mLViewSrcImages->CountItems() ; i++ ) {
		aItemArc = BMessage();
		dynamic_cast<TCErisoSItem*>(mLViewSrcImages->ItemAt(i))->Archive(&aItemArc);
		aProjArc.AddMessage(ARC_NAME_SOURCE_ITEM, &aItemArc);
	}
	// デスト
	aProjArc.AddString(ARC_NAME_DEST_FILE_NAME, mTxtCtrlDestFileName->Text());
	// 保存
	if ( aProjArc.Flatten(&aFile) != B_OK )
		return B_ERROR;
	// MIME
	aNodeInfo.SetTo(&aFile);
	aNodeInfo.SetType(SIGNATURE_MAFOOGA_PROJ);
	return B_OK;
}
//------------------------------------------------------------------------------
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
プロジェクトの保存
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
void	TCErisoView::SaveRequested(BMessage* oMsg)
{
	entry_ref	aERef;
	entry_ref	aERef2;

	if ( get_save_ref(oMsg, &aERef) != B_OK )
		return;
	get_ref_for_path(add_ext(BPath(&aERef).Path(), EXT_MAFOOGA_PROJ).c_str(), &aERef2);
	SaveProject(aERef2);
	SendAddHistoryRequested(aERef2);
}
//------------------------------------------------------------------------------
void	TCErisoView::SendAddHistoryRequested(const entry_ref& oERef) const
{
	BMessage	aSendMsg(FUC_ADD_HISTORY_REQUESTED);

	aSendMsg.AddRef(B_MSG_NAME_REFS, &oERef);
	Window()->PostMessage(&aSendMsg);
}
//------------------------------------------------------------------------------
void	TCErisoView::SetCompoEnables(bool oEnabled)
{
	BControl*	aControl;

	for ( int32 i = 0 ; i < CountChildren() ; i++ ) {
		aControl = dynamic_cast<BControl*>(ChildAt(i));
		if ( aControl != NULL ) {
			aControl->SetEnabled(oEnabled);
		}
	}
}
//------------------------------------------------------------------------------
void	TCErisoView::SetDestFileNameRequested(BMessage* oMsg)
{
	entry_ref	aERef;

	if ( get_save_ref(oMsg, &aERef) != B_OK )
		return;
	mTxtCtrlDestFileName->SetText(add_ext(BPath(&aERef).Path(), EXT_RSRC_FILE).c_str());
}
//------------------------------------------------------------------------------
void	TCErisoView::SetPanelBrowseFolder(BFilePanel* oFilePanel) const
{
	string	aFolderName;

	if ( oFilePanel == mPanelBrowseSrc ) {
		aFolderName = gExitStates.mFolderNamePanelBrowseSrc;
	} else if ( oFilePanel == mPanelBrowseDest ) {
		aFolderName = gExitStates.mFolderNamePanelBrowseDest;
	} else {
		ASSERT(1, "TCErisoWindow::SetPanelFolder() - bad target");
	}
	if ( !aFolderName.empty() ) {
		oFilePanel->SetPanelDirectory(BPath(aFolderName.c_str()).Path());
	}
}
//------------------------------------------------------------------------------
void	TCErisoView::SetPanelBrowseLabels(BFilePanel* oFilePanel) const
{
	const char*	aTitle;

	if ( oFilePanel == NULL )
		return;
	if ( oFilePanel == mPanelBrowseSrc ) {
		aTitle = _T(LKK_PANEL_BROWSE_SRC_TITLE);
	} else {
		aTitle = _T(LKK_PANEL_BROWSE_DEST_TITLE);
	}
	oFilePanel->Window()->SetTitle(BString(aTitle).ReplaceAll(LKR_APP_NAME, _T(gAppNameEnUs))
			.ReplaceAll(LKR_COLON_SPACE, _T(LKK_COLON_SPACE)).String());
	oFilePanel->SetButtonLabel(B_DEFAULT_BUTTON, _T(LKK_BUTTON_OK));
}
//------------------------------------------------------------------------------
void	TCErisoView::SetPanelProjFolder(BFilePanel* oSrc, BFilePanel* oFilePanel) const
{
	entry_ref	aERef;
	string		aFolderName;

	if ( oSrc == NULL ) {
		aFolderName = gExitStates.mFolderNamePanelProj;
	} else {
		oSrc->GetPanelDirectory(&aERef);
		aFolderName = BPath(&aERef).Path();
	}
	if ( !aFolderName.empty() ) {
		oFilePanel->SetPanelDirectory(BPath(aFolderName.c_str()).Path());
	}
}
//------------------------------------------------------------------------------
void	TCErisoView::SetPanelProjLabels(BFilePanel* oFilePanel) const
{
	const char*	aTitle;

	if ( oFilePanel == NULL )
		return;
	if ( oFilePanel == mPanelProjOpen ) {
		aTitle = _T(LKK_PANEL_PROJ_OPEN_TITLE);
	} else {
		aTitle = _T(LKK_PANEL_PROJ_SAVE_TITLE);
	}
	oFilePanel->Window()->SetTitle(BString(aTitle).ReplaceAll(LKR_APP_NAME, _T(gAppNameEnUs))
			.ReplaceAll(LKR_COLON_SPACE, _T(LKK_COLON_SPACE)).String());
}
//------------------------------------------------------------------------------
/* ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
ソース参照パネルからのメッセージ
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' */
void	TCErisoView::SetSourceFileNameRequested(BMessage* oMsg)
{
	entry_ref		aERef;

	if ( oMsg->FindRef(B_MSG_NAME_REFS, &aERef) != B_OK )
		return;
	mTxtCtrlSrcFileName->SetText(BPath(&aERef).Path());
}
//------------------------------------------------------------------------------
void	TCErisoView::SetViewLabels()
{
	// 格納したい画像ファイル名
	mTxtCtrlSrcFileName->SetLabel((acc_label("S", _T(LKK_TXT_CTRL_SRC_FILE_NAME))+_T(LKK_COLON_SPACE)).c_str());
	adjust_txt_ctrl_divider(mTxtCtrlSrcFileName);
	mButtonBrowseSrc->SetLabel((acc_label("X", _T(LKK_BUTTON_BROWSE))+FUC_ELLIPSIS).c_str());
	// リソース情報
	mTxtCtrlRsrcID->SetLabel((acc_label("I", _T(LKK_TXT_CTRL_RSRC_ID))+_T(LKK_COLON_SPACE)).c_str());
	adjust_txt_ctrl_divider(mTxtCtrlRsrcID);
	mTxtCtrlRsrcName->SetLabel((acc_label("N", _T(LKK_TXT_CTRL_RSRC_NAME))+_T(LKK_COLON_SPACE)).c_str());
	adjust_txt_ctrl_divider(mTxtCtrlRsrcName);
	// （ファイル数表示は Update 系で行う）
	UpdateStrViewSrcImages();
	// （追加・更新は Update 系で行う）
	UpdateButtonAdd();
	// ソース操作ボタン
	mButtonUp->SetLabel(_T(LKK_BUTTON_UP));
	mButtonDown->SetLabel(_T(LKK_BUTTON_DOWN));
	mButtonRemove->SetLabel(acc_label("R", _T(LKK_BUTTON_REMOVE)).c_str());
	// 出力先リソースファイル名
	mTxtCtrlDestFileName->SetLabel((acc_label("D", _T(LKK_TXT_CTRL_DEST_FILE_NAME))+_T(LKK_COLON_SPACE)).c_str());
	adjust_txt_ctrl_divider(mTxtCtrlDestFileName);
	mButtonBrowseDest->SetLabel((acc_label("Y", _T(LKK_BUTTON_BROWSE))+FUC_ELLIPSIS).c_str());
	// 実行
	mButtonGo->SetLabel(acc_label("G", _T(LKK_BUTTON_GO)).c_str());
}
//------------------------------------------------------------------------------
void	TCErisoView::ShortCutDown(int32 oChar, int32 oFunc)
{
	BMessage	aSendMsg;

	switch ( oChar ) {
	case B_DOWN_ARROW:
		click_button(mButtonDown);
		break;
	case B_FUNCTION_KEY:
		if ( oFunc == B_F1_KEY ) {
			aSendMsg.what = FUC_MITEM_HELP_CLICKED;
			be_app_messenger.SendMessage(&aSendMsg);
		}
		break;
	case B_UP_ARROW:
		click_button(mButtonUp);
		break;
	case 'l':
		mLViewSrcImages->MakeFocus(true);
		break;
	default:
		SUPER::ShortCutDown(oChar, oFunc);
	}
}
//------------------------------------------------------------------------------
void	TCErisoView::ShowPanelBrowse(BFilePanel** oFilePanel) const
{
	file_panel_mode	aMode;
	BMessenger	aMsger(this);
	BMessage	aMsg;

	if ( *oFilePanel == NULL ) {
		if ( oFilePanel == &mPanelBrowseSrc ) {
			aMode = B_OPEN_PANEL;
			aMsg.what = SET_SOURCE_FILE_NAME_REQUESTED;
		} else {
			aMode = B_SAVE_PANEL;
			aMsg.what = SET_DEST_FILE_NAME_REQUESTED;
		}
		*oFilePanel = new BFilePanel(aMode, &aMsger, NULL, 0, false);
		(*oFilePanel)->SetMessage(&aMsg);
		SetPanelBrowseFolder(*oFilePanel);
		SetPanelBrowseLabels(*oFilePanel);
	} else {
		set_file_panel_priority(*oFilePanel, B_DISPLAY_PRIORITY);
	}
	(*oFilePanel)->Show();
}
//------------------------------------------------------------------------------
void	TCErisoView::ShowPanelProj(BFilePanel** oFilePanel) const
{
	file_panel_mode	aMode;
	BFilePanel*	aSrc;
	BMessenger	aMsger(this);

	if ( *oFilePanel == NULL ) {
		if ( oFilePanel == &mPanelProjOpen ) {
			aMode = B_OPEN_PANEL;
			aSrc = mPanelProjSave;
		} else {
			aMode = B_SAVE_PANEL;
			aSrc = mPanelProjOpen;
		}
		*oFilePanel = new BFilePanel(aMode, &aMsger, NULL, 0, false);
		SetPanelProjFolder(aSrc, *oFilePanel);
		SetPanelProjLabels(*oFilePanel);
	} else {
		set_file_panel_priority(*oFilePanel, B_DISPLAY_PRIORITY);
	}
	(*oFilePanel)->Show();
}
//------------------------------------------------------------------------------
void	TCErisoView::SimpleData(BMessage* oMsg)
{
	entry_ref	aERef;
	BPoint		aDropPoint;

	if ( !oMsg->WasDropped() )
		return;
	if ( !oMsg->IsSourceRemote() )
		return;
	if ( oMsg->FindRef(B_MSG_NAME_REFS, &aERef) != B_OK )
		return;
	aDropPoint = ConvertFromScreen(oMsg->DropPoint());
	// ソースファイル名領域か？
	if ( BRect(0, 0, Bounds().right, mButtonBrowseSrc->Frame().bottom).Contains(aDropPoint) ) {
		SetSourceFileNameRequested(oMsg);
		return;
	}
	// ソースリスト領域か？
	if ( BRect(mStrViewSrcImages->Frame().LeftTop(), mScrlViewSrcImages->Frame().RightBottom()).Contains(aDropPoint) ) {
		SrcListDropped(oMsg);
		return;
	}
	// デスト領域か？
	if ( BRect(mBoxSep1->Frame().LeftBottom(), mBoxSep2->Frame().RightTop()).Contains(aDropPoint) ) {
		DestDropped(oMsg);
		return;
	}
}
//------------------------------------------------------------------------------
void	TCErisoView::SrcListDropped(BMessage* oMsg)
{
	int32		aNumERefs;
	entry_ref	aERef;
	type_code	aType;
	BString		xAutoRsrcID;

	oMsg->GetInfo(B_MSG_NAME_REFS, &aType, &aNumERefs);
	for ( int32 i = 0 ; i < aNumERefs ; i++ ) {
		if ( oMsg->FindRef(B_MSG_NAME_REFS, i, &aERef) != B_OK )
			continue;
		mTxtCtrlSrcFileName->SetText(BPath(&aERef).Path());
		mTxtCtrlRsrcID->SetText((BString() << AutoResourceID()).String());
		mTxtCtrlRsrcName->SetText(AutoResourceName().c_str());
		ButtonAddClicked();	// メッセージ経由ではダメ
	}
}
//------------------------------------------------------------------------------
string	TCErisoView::StartID() const
{
	return mTxtCtrlRsrcID->Text();
}
//------------------------------------------------------------------------------
void	TCErisoView::TxtCtrlDestFileNameChanged()
{
	UpdateButtonGo();
}
//------------------------------------------------------------------------------
void	TCErisoView::TxtCtrlSrcFileNameChanged()
{
	UpdateButtonAddEnabled();
	UpdateLViewSrcImages();
	UpdateButtonAdd();
}
//------------------------------------------------------------------------------
void	TCErisoView::UpdateButtonAdd()
{
	if ( FindSrc() < 0 ) {
		mButtonAdd->SetLabel(acc_label("A", _T(LKK_BUTTON_ADD_ADD)).c_str());
	} else {
		mButtonAdd->SetLabel(acc_label("A", _T(LKK_BUTTON_ADD_REPLACE)).c_str());
	}
}
//------------------------------------------------------------------------------
void	TCErisoView::UpdateButtonAddEnabled()
{
	mButtonAdd->SetEnabled(mTxtCtrlSrcFileName->Text()[0] != '\0');
}
//------------------------------------------------------------------------------
void	TCErisoView::UpdateButtonGo()
{
	mButtonGo->SetEnabled(mLViewSrcImages->CountItems() > 0 && mTxtCtrlDestFileName->Text()[0] != '\0');
}
//------------------------------------------------------------------------------
void	TCErisoView::UpdateComposEnabled()
{
	SetCompoEnables(true);
	UpdateButtonAddEnabled();
	UpdateSourceButtonsEnabled();
	UpdateButtonGo();
}
//------------------------------------------------------------------------------
void	TCErisoView::UpdateCompos()
{
	UpdateStrViewSrcImages();
	UpdateLViewSrcImages();
}
//------------------------------------------------------------------------------
void	TCErisoView::UpdateLViewSrcImages()
{
	int32		aIdx = FindSrc();
	BHandler*	aTgtBak = mLViewSrcImages->Target();

	mLViewSrcImages->SetTarget(NULL);	// 選択したときにメッセージが飛ぶのを抑止
	mLViewSrcImages->DeselectAll();
	if ( aIdx >= 0 ) {
		mLViewSrcImages->Select(aIdx);
	}
	mLViewSrcImages->SetTarget(aTgtBak);
	UpdateSourceButtonsEnabled();
}
//------------------------------------------------------------------------------
void	TCErisoView::UpdateSourceButtonsEnabled()
{
	bool	aEnabled = mLViewSrcImages->CurrentSelection() >= 0;

	mButtonUp->SetEnabled(aEnabled);
	mButtonDown->SetEnabled(aEnabled);
	mButtonRemove->SetEnabled(aEnabled);
}
//------------------------------------------------------------------------------
void	TCErisoView::UpdateStrViewSrcImages()
{
	BString	aStr = acc_label("L", _T(LKK_STR_VIEW_SRC_IMAGES)).c_str();

	aStr.ReplaceAll(LKR_NUM, (BString() << mLViewSrcImages->CountItems()).String());
	mStrViewSrcImages->SetText(aStr.String());
}
//------------------------------------------------------------------------------
//==============================================================================
