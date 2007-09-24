#include "VPCErisoView.h"
//------------------------------------------------------------------------------
// BeOS
#include <Message.h>
// Add2
// Proj
#include "Constants.h"
//==============================================================================
TCErisoSItem::TCErisoSItem(const string& oSrcFileName, int32 oRsrcID, const string& oRsrcName)
		: SUPER("", 0, true)
{
	mSrcFileName = oSrcFileName;
	mRsrcID = oRsrcID;
	mRsrcName = oRsrcName;
	UpdateText();
}
//------------------------------------------------------------------------------
TCErisoSItem::TCErisoSItem(BMessage* oArc)
		: SUPER(oArc)
{
	Unarchive(*oArc);
}
//------------------------------------------------------------------------------
status_t	TCErisoSItem::Archive(BMessage* oArc, bool oDeep) const
{
	status_t	aResult = B_OK;

	aResult |= SUPER::Archive(oArc, oDeep);
	// int32
	aResult |= oArc->AddInt32(ARC_NAME_RESOURCE_ID, mRsrcID);
	// string
	aResult |= oArc->AddString(ARC_NAME_RESOURCE_NAME, mRsrcName.c_str());
	aResult |= oArc->AddString(ARC_NAME_SOURCE_FILE_NAME, mSrcFileName.c_str());
	return aResult;
}
//------------------------------------------------------------------------------
status_t	TCErisoSItem::Unarchive(const BMessage& oArc)
{
	status_t	aResult = B_OK;
	BString		aString;

	// int32
	aResult |= oArc.FindInt32(ARC_NAME_RESOURCE_ID, &mRsrcID);
	// string
	aResult |= oArc.FindString(ARC_NAME_RESOURCE_NAME, &aString);
	mRsrcName = aString.String();
	aResult |= oArc.FindString(ARC_NAME_SOURCE_FILE_NAME, &aString);
	mSrcFileName = aString.String();
	return aResult;
}
//------------------------------------------------------------------------------
void	TCErisoSItem::UpdateText()
{
	SetText((BString() << mSrcFileName.c_str() << ", " << mRsrcID << ", " << mRsrcName.c_str()).String());
}
//------------------------------------------------------------------------------
//==============================================================================
