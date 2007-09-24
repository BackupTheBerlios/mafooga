#ifndef ___CErisoApp_H_
#define ___CErisoApp_H_
//------------------------------------------------------------------------------
// BeOS
#include <Application.h>
#include <SupportDefs.h>
// Add2
#include <add2/SHINTAAbout.h>
#include <add2/SimpleInput.h>
// Proj
//------------------------------------------------------------------------------
class	TCErisoWindow;
//==============================================================================
class	TCErisoApp : public BApplication
{
public:
	TCErisoApp();
	virtual	~TCErisoApp();

protected:
private:
	typedef	BApplication	SUPER;						// スーパークラス

	virtual void	AboutRequested();
	void			IAPChanged(BMessage* oMsg);
	void			LanguageChanged();
	virtual void	MessageReceived(BMessage* oMsg);
	void			MItemAuthorWebClicked();
	void			MItemHelpClicked();
	void			MItemRangeIDClicked();
	void			MItemSHINTACommonPrefsClicked();
	virtual void	Quit();
	virtual bool	QuitRequested();
	void			RegisterProjectMime() const;
	void			SetRangeIDWindowLabels();

	TCErisoWindow*			mCErisoWin;
	TSHINTAAboutWindow*		mAboutWin;			// バージョン情報ダイアログ
	TSimpleInputWindow*		mRangeIDWin;
};
//==============================================================================
int		main(int oArgC, char* oArgV[]);			// メインルーチン
//==============================================================================
#endif	// ___CErisoApp_H_
