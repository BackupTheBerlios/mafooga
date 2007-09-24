#ifndef	___CErisoView_H_
#define	___CErisoView_H_
//------------------------------------------------------------------------------
// BeOS
#include <Box.h>
#include <Button.h>
#include <ListView.h>
#include <ScrollView.h>
#include <StatusBar.h>
#include <interface/StringView.h>
#include <TextControl.h>
// C++
#include <string>
// Add2
#include <add2/GUIBase.h>
//==============================================================================
class	TCErisoView : public TDialogView
{
public:
	TCErisoView(BMessage* oArc);
	virtual	~TCErisoView();

	bool			IsButtonGoEnabled() const;
	status_t		SaveExitProject() const;
	string			StartID() const;
private:
	typedef	TDialogView	SUPER;	// スーパークラス

	void			AddSrc();
	virtual void	AllAttached();
	virtual void	AttachedToWindow();
	int32			AutoResourceID() const;
	string			AutoResourceName() const;
	void			ButtonAddClicked();
	void			ButtonBrowseDestClicked();
	void			ButtonBrowseSrcClicked();
	void			ButtonDownClicked();
	void			ButtonRemoveClicked();
	void			ButtonUpClicked();
	void			Cancel(BMessage* oMsg);
	void			DestDropped(BMessage* oMsg);
	void			DisableCompos();
	int32			FindSrc() const;		// テキストコントロールに入力されているファイル名のインデックスを検索
	void			GetChildPointers();
	virtual void	IAPChanged(BMessage* oMsg);
	virtual void	LanguageChanged();
	void			LViewSrcImagesSelected();
	status_t		MakeRsrc() const;
	virtual void	MessageReceived(BMessage* oMsg);
	void			MItemGoClicked();
	void			MItemNewClicked();
	void			MItemOpenClicked();
	void			MItemSaveAsClicked();
	void			MoveSrc(int32 oBefore, int32 oAfter);
	status_t		OpenExitProject();
	status_t		OpenProject(const entry_ref& oERef);	// プロジェクトをコンポーネントに格納
	void			RangeIDRequested(BMessage* oMsg);
	void			RefsReceived(BMessage* oMsg);
	void			ReplaceSrc(int32 oIdx);
	status_t		SaveProject(const entry_ref& oERef) const;
	void			SaveRequested(BMessage* oMsg);
	void			SendAddHistoryRequested(const entry_ref& oERef) const;
	void			SetCompoEnables(bool oEnabled);			// すべてのコントロールの Enabled を一括設定
	void			SetDestFileNameRequested(BMessage* oMsg);
	void			SetPanelBrowseFolder(BFilePanel* oFilePanel) const;	// 終了時の状態から設定
	void			SetPanelBrowseLabels(BFilePanel* oFilePanel) const;
	void			SetPanelProjFolder(BFilePanel* oSrc, BFilePanel* oFilePanel) const;	// 終了時の状態から設定
	void			SetPanelProjLabels(BFilePanel* oFilePanel) const;
	void			SetSourceFileNameRequested(BMessage* oMsg);
	void			SetViewLabels();
	virtual void	ShortCutDown(int32 oChar, int32 oFunc);
	void			ShowPanelBrowse(BFilePanel** oFilePanel) const;
	void			ShowPanelProj(BFilePanel** oFilePanel) const;
	void			SimpleData(BMessage* oMsg);
	void			SrcListDropped(BMessage* oMsg);
	void			TxtCtrlDestFileNameChanged();
	void			TxtCtrlSrcFileNameChanged();
	void			UpdateButtonAdd();
	void			UpdateButtonAddEnabled();
	void			UpdateButtonGo();
	void			UpdateComposEnabled();
	void			UpdateCompos();
	void			UpdateLViewSrcImages();
	void			UpdateSourceButtonsEnabled();
	void			UpdateStrViewSrcImages();

	BBox*			mBoxSep1;
	BBox*			mBoxSep2;
	BButton*		mButtonAdd;
	BButton*		mButtonBrowseDest;
	BButton*		mButtonBrowseSrc;
	BButton*		mButtonDown;
	BButton*		mButtonGo;
	BButton*		mButtonRemove;
	BButton*		mButtonUp;
	BFilePanel*		mPanelBrowseDest;			// 出力先参照
	BFilePanel*		mPanelBrowseSrc;			// ソース参照
	BFilePanel*		mPanelProjOpen;				// プロジェクトを開く
	BFilePanel*		mPanelProjSave;				// プロジェクトに名前を付けて保存
	BListView*		mLViewSrcImages;
	BScrollView*	mScrlViewSrcImages;
	BStatusBar*		mStatBarProgress;
	BStringView*	mStrViewSrcImages;
	BTextControl*	mTxtCtrlDestFileName;
	BTextControl*	mTxtCtrlRsrcID;
	BTextControl*	mTxtCtrlRsrcName;
	BTextControl*	mTxtCtrlSrcFileName;
};
//==============================================================================
#endif	// ___CErisoView_H_
