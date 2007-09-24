#define	GLOBAL_DEF

//------------------------------------------------------------------------------
#include "Common.h"
//------------------------------------------------------------------------------
// BeOS
#include <Locale.h>
#include <Path.h>
// Add2
#include <add2/FUConstants.h>
#include <add2/MiscUtils.h>
#include <add2/SHINTAUtils.h>
#include <add2/StringUtils.h>
// Proj
#include "Constants.h"
#include "LKKConstants.h"
//==============================================================================
TCErisoAppPrivacy::TCErisoAppPrivacy()
{
}
//------------------------------------------------------------------------------
TCErisoAppPrivacy::~TCErisoAppPrivacy()
{
}
//------------------------------------------------------------------------------
void	TCErisoAppPrivacy::UpdateAccFormat()
{
	gIAPs.GetString(IAP_NAME_ACC_KEY_FORMAT, &mAccFormat);
}
//------------------------------------------------------------------------------
//==============================================================================
TExitStates::TExitStates()
		: SUPER()
{
	// その他
	mFrame.Set(50, 50, 350, 300);
}
//------------------------------------------------------------------------------
TExitStates::~TExitStates()
{
}
//------------------------------------------------------------------------------
status_t	TExitStates::Archive(BMessage* oArc, bool oDeep) const
{
	status_t	aResult = B_OK;

	aResult |= SUPER::Archive(oArc);
	// string
	aResult |= oArc->AddString(FUC_ARC_NAME_APP_VER, gAppVer);
	aResult |= oArc->AddString(ARC_NAME_PANEL_BROWSE_DEST, mFolderNamePanelBrowseDest.c_str());
	aResult |= oArc->AddString(ARC_NAME_PANEL_BROWSE_SRC, mFolderNamePanelBrowseSrc.c_str());
	aResult |= oArc->AddString(ARC_NAME_PANEL_PROJECT, mFolderNamePanelProj.c_str());
	// BRect
	aResult |= oArc->AddRect(FUC_ARC_NAME_FRAME, mFrame);
	return aResult;
}
//------------------------------------------------------------------------------
status_t	TExitStates::LoadFromFile()
{
	return LoadByName(FOLDER_NAME_MAFOOGA_SETTINGS, FILE_NAME_EXIT_STATES);
}
//------------------------------------------------------------------------------
status_t	TExitStates::SaveToFile()
{
	return SaveByName(FOLDER_NAME_MAFOOGA_SETTINGS, FILE_NAME_EXIT_STATES);
}
//------------------------------------------------------------------------------
status_t	TExitStates::Unarchive(const BMessage& oMsg)
{
	status_t	aResult = B_OK;

	aResult |= SUPER::Unarchive(oMsg);
	// string
	aResult |= FindString(oMsg, FUC_ARC_NAME_APP_VER, &mAppVer);
	aResult |= FindString(oMsg, ARC_NAME_PANEL_BROWSE_DEST, &mFolderNamePanelBrowseDest);
	aResult |= FindString(oMsg, ARC_NAME_PANEL_BROWSE_SRC, &mFolderNamePanelBrowseSrc);
	aResult |= FindString(oMsg, ARC_NAME_PANEL_PROJECT, &mFolderNamePanelProj);
	// BRect
	aResult |= FindRect(oMsg, FUC_ARC_NAME_FRAME, &mFrame);
	return aResult;
}
//==============================================================================
TPrefs::TPrefs()
		: SUPER()
{
}
//------------------------------------------------------------------------------
TPrefs::~TPrefs()
{
}
//------------------------------------------------------------------------------
status_t	TPrefs::Archive(BMessage* oArc, bool oDeep) const
{
	status_t	aResult = B_OK;

	aResult |= SUPER::Archive(oArc);
	return aResult;
}
//------------------------------------------------------------------------------
status_t	TPrefs::LoadFromFile()
{
	return LoadByName(FOLDER_NAME_MAFOOGA_SETTINGS, FILE_NAME_PREFS);
}
//------------------------------------------------------------------------------
status_t	TPrefs::SaveToFile()
{
	return SaveByName(FOLDER_NAME_MAFOOGA_SETTINGS, FILE_NAME_PREFS);
}
//------------------------------------------------------------------------------
status_t	TPrefs::Unarchive(const BMessage& oMsg)
{
	status_t	aResult = B_OK;

	aResult |= SUPER::Unarchive(oMsg);
	return aResult;
}
//==============================================================================
string	acc_label(const string& oAcc, const string& oStr)
{
	return merge_accelerator_key(gAppPrivacy.mAccFormat, oAcc, oStr);
}
//------------------------------------------------------------------------------
string	alert_caption()
{
	return BString(_T(LKK_ALERT_CAPTION)).ReplaceAll(LKR_APP_NAME, _T(gAppNameEnUs)).String();
}
//------------------------------------------------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
