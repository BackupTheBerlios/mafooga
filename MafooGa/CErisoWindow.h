#ifndef	___CErisoWindow_H_
#define	___CErisoWindow_H_
//------------------------------------------------------------------------------
// BeOS
#include <MenuBar.h>
// C++
#include <string>
// Add
#include <add2/GUIBase.h>
#include <add2/MRUF.h>
// Proj
//------------------------------------------------------------------------------
class	TCErisoView;
//==============================================================================
class	TCErisoWindow : public TDialogWindow
{
public:
	TCErisoWindow(BRect oFrame);
	virtual	~TCErisoWindow();

	virtual void	Quit();
	virtual bool	QuitRequested();
	string			StartID() const;			// ビューの同名関数呼び出し
protected:
private:
	typedef	TDialogWindow	SUPER;				// スーパークラス

	void			AddHistoryRequested(BMessage* oMsg);
	void			CreateMenu();				// メニューの作成
	void			DestroyMenu();				// メニューの破棄
	void			GenerateMenuMRUF();
	virtual void	IAPChanged(BMessage* oMsg);
	virtual void	LanguageChanged();
	virtual void	MenusBeginning();
	virtual void	MessageReceived(BMessage* oMsg);
	void			MItemDelAllHistoryClicked();
	void			SetWinTitle();				// タイトルの設定

	status_t		mInitCheck;
	BMenu*			mMenuOpen;
	BMenuBar*		mMBar;						// メニューバー
	BMenuItem*		mMItemGo;
	TCErisoView*	mCErisoView;				// メインビュー
	TMRUF			mMRUF;
};
//==============================================================================
#endif	// ___CErisoWindow_H_
