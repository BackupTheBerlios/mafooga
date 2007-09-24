#ifndef	___CErisoConstants_H_
#define	___CErisoConstants_H_
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// メニュー定数
const uint32	BUTTON_BROWSE_DEST_CLICKED = 'brwd';
const uint32	BUTTON_BROWSE_SRC_CLICKED = 'brws';
const uint32	MITEM_GO_CLICKED = 'go  ';
const uint32	LVIEW_SRC_IMAGES_SELECTED = 'lsis';
const uint32	RANGE_ID_REQUESTED = 'rgir';
const uint32	MITEM_RANGE_ID_CLICKED = 'rngi';
const uint32	SET_DEST_FILE_NAME_REQUESTED = 'sdfr';
const uint32	SET_SOURCE_FILE_NAME_REQUESTED = 'ssfr';
const uint32	TXT_CTRL_DEST_FILE_NAME_CHANGED = 'tdfc';
const uint32	TXT_CTRL_SRC_FILE_NAME_CHANGED = 'tsfc';
//------------------------------------------------------------------------------
// 終了時の状態
const char		ARC_NAME_PANEL_BROWSE_DEST[] = "PBrD";
const char		ARC_NAME_PANEL_BROWSE_SRC[] = "PBrS";
const char		ARC_NAME_PANEL_PROJECT[] = "PPrj";
//------------------------------------------------------------------------------
// 環境設定
//------------------------------------------------------------------------------
// プロジェクトアーカイブ
const char		ARC_NAME_DEST_FILE_NAME[] = "DsFN";
const char		ARC_NAME_RESOURCE_ID[] = "RsID";
const char		ARC_NAME_RESOURCE_NAME[] = "RsNm";
const char		ARC_NAME_SOURCE_FILE_NAME[] = "SrFN";
const char		ARC_NAME_SOURCE_ITEM[] = "SrIt";
//------------------------------------------------------------------------------
// リソース
const char		RSRC_NAME_CERISO_VIEW[] = "CErisoViewRsrc";
//------------------------------------------------------------------------------
// ビュー
const char		VIEW_NAME_BOX_SEP_1[] = "BoxSep1";
const char		VIEW_NAME_BOX_SEP_2[] = "BoxSep2";
const char		VIEW_NAME_BUTTON_ADD[] = "ButtonAdd";
const char		VIEW_NAME_BUTTON_BROWSE_DEST[] = "ButtonBrowseDest";
const char		VIEW_NAME_BUTTON_BROWSE_SRC[] = "ButtonBrowseSrc";
const char		VIEW_NAME_BUTTON_DOWN[] = "ButtonDown";
const char		VIEW_NAME_BUTTON_GO[] = "ButtonGo";
const char		VIEW_NAME_BUTTON_REMOVE[] = "ButtonRemove";
const char		VIEW_NAME_BUTTON_UP[] = "ButtonUp";
const char		VIEW_NAME_STAT_BAR_PROGRESS[] = "StatBarProgress";
const char		VIEW_NAME_STR_VIEW_SRC_IMAGES[] = "StrViewSrcImages";
const char		VIEW_NAME_TXT_CTRL_DEST_FILE_NAME[] = "TxtCtrlDestFileName";
const char		VIEW_NAME_TXT_CTRL_RSRC_ID[] = "TxtCtrlRsrcID";
const char		VIEW_NAME_TXT_CTRL_RSRC_NAME[] = "TxtCtrlRsrcName";
const char		VIEW_NAME_TXT_CTRL_SRC_FILE_NAME[] = "TxtCtrlSrcFileName";
//------------------------------------------------------------------------------
// char[]
const char		EXT_MAFOOGA_PROJ[] = "mfg";								// プロジェクトファイルの拡張子
const char		EXT_RSRC_FILE[] = "rsrc";
const char		FILE_NAME_DEFAULT_HELP[] = "ReadMe_enUS.txt";
const char		FILE_NAME_EXIT_PROJECT[] = "MafooGa_ExitProject.arc";	// 終了時のプロジェクトを記録するファイル名
const char		FILE_NAME_EXIT_STATES[] = "MafooGa_ExitStates.arc";		// 終了時の状態記録ファイル名
const char		FILE_NAME_MRUFS[] = "MafooGa_MRUFs.arc";				// 履歴
const char		FILE_NAME_PREFS[] = "MafooGa_Prefs.arc";				// 環境設定記録ファイル名
const char		FOLDER_NAME_MAFOOGA_SETTINGS[] = "MafooGa";				// 設定保存パス
const char		SIGNATURE_MAFOOGA_PROJ[] = "application/x-vnd.SHINTA-MafooGa-Project";
// int32
const int32		MAX_MRUFS = 10;
// float
const float		MARGIN_VV = 10.0;
const float		MARGIN_WV = 10.0;
//==============================================================================
#endif	// ___CErisoConstants_H_
