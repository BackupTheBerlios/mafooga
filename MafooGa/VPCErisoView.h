#ifndef	___VarPackCErisoView_H_
#define	___VarPackCErisoView_H_
//------------------------------------------------------------------------------
// BeOS
#include <ListItem.h>
#include <SupportDefs.h>
// C++
#include <string>
//==============================================================================
class	TCErisoSItem : public BStringItem
		// ソースリストビューのアイテム
{
public:
	TCErisoSItem(const string& oSrcFileName, int32 oRsrcID, const string& cRsrcName);
	TCErisoSItem(BMessage* oArc);

	virtual status_t	Archive(BMessage* oArc, bool oDeep = true) const;
	status_t	Unarchive(const BMessage& oArc);
	void		UpdateText();							//文字列の更新

	int32		mRsrcID;		// リソース ID
	string		mRsrcName;		// リソース名
	string		mSrcFileName;	// 画像ファイル名

private:
	typedef	BStringItem	SUPER;
};
//==============================================================================
#endif	// ___VarPackCErisoView_H_
