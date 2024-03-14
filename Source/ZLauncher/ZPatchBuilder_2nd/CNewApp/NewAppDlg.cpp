
#include "stdafx.h"
#include "NewAppDlg.h"
#include "Log.h"
#include "MZip.h"
#include "FileInfo.h"
#include <stack>
#include <string>
using namespace std;


// DoDataExchange
void CNewAppDlg::DoDataExchange( CDataExchange* pDX)
{
	CDDBDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAppDlg)
	DDX_Control(pDX, IDC_STARTGAME, m_cStartGame);
	//}}AFX_DATA_MAP
}


// Message map
BEGIN_MESSAGE_MAP( CNewAppDlg, CDDBDialog)
	//{{AFX_MSG_MAP(CNewAppDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STARTGAME, OnBnClickedStartgame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/************************************************************************************
  Dialog
*************************************************************************************/
// Ŭ���� ������
CNewAppDlg::CNewAppDlg( CWnd* pParent) : CDDBDialog( IDD_DIALOG_MAINDLG, pParent)
{
	// �̰����� ������ ���̾�α��� ������ �Է��Ѵ�
	// �� �� �ڼ��� ������ ���ϸ� CDDBDialog.h ȭ���� Structure of CDDBDIALOGINFO ������ �����Ѵ�
	CDDBDLGINFO DlgInfo;
	DlgInfo.nWidth  = 640;
	DlgInfo.nHeight = 240;
	DlgInfo.IDSkinResource = IDR_BMP_MAINDLG;
	DlgInfo.IDIconResource = IDR_ICO_MAINFRAME;
	DlgInfo.IDWaveResource = IDR_WAV_PUSHBUTTON;
	DlgInfo.bUseQuitButton = true;
	DlgInfo.bEnableMoveDlg = true;
	DlgInfo.bUseTitlebar = true;
	DlgInfo.nTextColor = RGB( 38, 38, 38);
	DlgInfo.nTextBkColor = RGB( 182, 182, 182);
	SetDialogInfo( DlgInfo);
}


CNewAppDlg::~CNewAppDlg()
{
	// clear list
	while(m_PatchList.begin() != m_PatchList.end())
	{
		delete (*m_PatchList.begin());
		m_PatchList.pop_front();
	}
}


/************************************************************************************
  Ŭ���� �޼��� �ڵ鷯
*************************************************************************************/
// OnInitDialog : ���̾�α׸� �ʱ�ȭ�� �Լ�
BOOL CNewAppDlg::OnInitDialog() 
{
	CDDBDialog::OnInitDialog();


	// ��Ʈ�� �ʱ�ȭ
	((CComboBox*)GetDlgItem( IDC_COUNTRY))->SetCurSel( 0);
	

	// ���̾�α� Ÿ��Ʋ ����
	SetWindowText( "Gunz Patch Builder");


	// ���̾�α� ��ġ �̵� �� ũ�� ����
	CRect rect; 
	GetWindowRect( &rect);
	int nWidth = rect.Width(), nHeight = rect.Height();
	rect.left = AfxGetApp()->GetProfileInt( "Window Position", "x", 50);
	rect.top  = AfxGetApp()->GetProfileInt( "Window Position", "y", 50);
	rect.right  = rect.left + nWidth;
	rect.bottom = rect.top  + nHeight;
	MoveWindow( rect, true);


	return true;
}


/************************************************************************************
  ������Ʈ ����
*************************************************************************************/
void CNewAppDlg::OnBnClickedStartgame()
{
	GetDlgItem( IDC_STARTGAME)->EnableWindow( FALSE);

	if ( ((CComboBox*)GetDlgItem( IDC_COUNTRY))->GetCurSel() == 3)		// 0=Korea, 1=Japan, 2=USA, 3=Brazil, 4=India, 5=International
	{
		CopyFile( "Gunz.exe.manifest", "theduel.exe.manifest", FALSE);
		DeleteFile( "Gunz.exe.manifest");
	}


	ScanDir( ".");


	Build();
}


void CNewAppDlg::ScanDir( const char* szDir)
{
	// clear list
	while(m_PatchList.begin() != m_PatchList.end())
	{
		delete (*m_PatchList.begin());
		m_PatchList.pop_front();
	}

	WIN32_FIND_DATA FindData;
	HANDLE hFind;

	stack<string>	ScanDir;
	ScanDir.push(szDir);

	while( !ScanDir.empty())
	{
		string strDir = ScanDir.top();
		ScanDir.pop();

		char szFind[_MAX_DIR];
		sprintf(szFind, "%s/*", strDir.c_str());

		// Find first .c file in current directory
		if( (hFind = FindFirstFile(szFind, &FindData )) == INVALID_HANDLE_VALUE)
		{
			_ASSERT(FALSE);
			continue;
		}
		
		else
		{
			do
			{
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if ( (strcmp(FindData.cFileName, ".")==0) || (strcmp(FindData.cFileName, "..")==0) )
					{
						// Skip
					}
					else
					{
						char szPath[_MAX_DIR];
						sprintf(szPath, "%s/%s", strDir.c_str(), FindData.cFileName);
						ScanDir.push(szPath);
					}
				}
				else
				{
					char szFilePath[_MAX_DIR];
					sprintf(szFilePath, "%s/%s", strDir.c_str(), FindData.cFileName);


					// ���� Ȯ���
					DWORD dwCRC = 0;
					char szFileExtName[25];
					strcpy( szFileExtName, FindData.cFileName + ( strlen( FindData.cFileName) - 3));


					// *.mrs������ ���� ����� �о CheckSum�� ���Ѵ�
					if ( !stricmp( szFileExtName, "mrs"))
					{
						FILE* fp = fopen( szFilePath, "rb");
						if ( fp != NULL)
						{
							MZip zMRSFile;
							unsigned long dwReadMode = MZIPREADFLAG_MRS2;
							if ( zMRSFile.Initialize( fp , dwReadMode ))
							{
								for ( int i = 0;  i < zMRSFile.GetFileCount();  i++)
									dwCRC += zMRSFile.GetFileCRC32( i);
							}
	
							fclose( fp);
						}
					}

					// �׿� ������ ���� ��ü�� �о CheckSum�� ���Ѵ�
					else
						dwCRC = GetFileCheckSum( szFilePath);


					// ��� �߰�
					m_PatchList.push_back( new MPatchNode( szFilePath, FindData.nFileSizeLow, FindData.ftLastWriteTime, dwCRC));
				}

			} while( FindNextFile( hFind, &FindData ) );	// Find the rest files 

			FindClose( hFind );
		}
	}
}


// Build
void CNewAppDlg::Build()
{
	CString str;
	((CListBox*)GetDlgItem( IDC_OUTPUT))->ResetContent();


	m_Itr = m_PatchList.begin();

	SetTimer( 0, 10, NULL);
}


/************************************************************************************
  Ÿ�̸�
*************************************************************************************/
void CNewAppDlg::OnTimer(UINT nIDEvent)
{
	// ������Ʈ Ÿ�̸�
	if ( nIDEvent == 0)
	{
		FILE* pFP = fopen( "patch.xml", "a");
		if ( pFP != NULL)
		{
			MPatchNode* pNode = *m_Itr;

			if ( m_Itr == m_PatchList.begin())
			{
				fprintf( pFP, "<?xml version=\"1.0\"?>\n");
				fprintf( pFP, "<XML>\n");
				fprintf( pFP, "<PATCHINFO>\n");
			}


			if ( !CheckFilteredFile( pNode->GetName()))
			{
				CString str;
				str.Format( "Get file info : %s (%u byte,  %u CRC)", pNode->GetName(), pNode->GetSize(), pNode->GetChecksum());
				((CListBox*)GetDlgItem( IDC_OUTPUT))->AddString( str);
				((CListBox*)GetDlgItem( IDC_OUTPUT))->SetCaretIndex( ((CListBox*)GetDlgItem( IDC_OUTPUT))->GetCount());

				fprintf( pFP, "	<PATCHNODE file=\"%s\">\n",		pNode->GetName());
				fprintf( pFP, "		<SIZE>%u</SIZE>\n",			pNode->GetSize());
				fprintf( pFP, "		<CHECKSUM>%u</CHECKSUM>\n",	pNode->GetChecksum());
				fprintf( pFP, "	</PATCHNODE>\n");
			}


			m_Itr++;


			if ( m_Itr == m_PatchList.end())
			{
				fprintf( pFP, "</PATCHINFO>\n");
				fprintf( pFP, "</XML>\n");

				((CListBox*)GetDlgItem( IDC_OUTPUT))->AddString( "Complete.");
				((CListBox*)GetDlgItem( IDC_OUTPUT))->SetCaretIndex( ((CListBox*)GetDlgItem( IDC_OUTPUT))->GetCount());

				GetDlgItem( IDC_STARTGAME)->EnableWindow( TRUE);

				this->SetFocus();

				KillTimer( 0);
			}

			fclose( pFP);
		}
	}


	CDDBDialog::OnTimer(nIDEvent);
}


// CheckFilteredFile
bool CNewAppDlg::CheckFilteredFile( const char* szFileName)
{
	// ���� �̸��� �ҹ��ڷ� ��ȯ
	char *pFileName = _strlwr( _strdup( szFileName));

	bool bSkip = false;


	char drive[ _MAX_PATH], dir[ _MAX_PATH], fname[ _MAX_PATH], ext[ _MAX_PATH];
	_splitpath( pFileName, drive, dir, fname, ext);


	// Ȯ���� ���͸�
	if ( strcmp( ext, ".txt") == 0)				bSkip = true;
	else if ( strcmp( ext, ".html") == 0)		bSkip = true;
	else if ( strcmp( ext, ".htm") == 0)		bSkip = true;
	else if ( strcmp( ext, ".dat") == 0)		bSkip = true;
	else if ( strcmp( ext, ".xml") == 0)		bSkip = true;
	else if ( strcmp( ext, ".nsi") == 0)		bSkip = true;
	else if ( strcmp( ext, ".bak") == 0)		bSkip = true;
	else if ( strcmp( ext, ".map") == 0)		bSkip = true;
	else if ( strcmp( ext, ".pdb") == 0)		bSkip = true;
	else if ( strcmp( ext, ".bk_") == 0)		bSkip = true;
	else if ( strcmp( ext, ".cs3") == 0)		bSkip = true;
	else if ( strcmp( ext, ".des") == 0)		bSkip = true;
	

    // Ư�� ���� ���͸�
	else if ( strstr( pFileName, "zpatchbuilder.exe") != NULL)	bSkip = true;
	else if ( strstr( pFileName, "xtrap") != NULL)				bSkip = true;
	else if ( strstr( pFileName, "xpva") != NULL)				bSkip = true;



	// ������ ���͸�
	switch ( ((CComboBox*)GetDlgItem( IDC_COUNTRY))->GetCurSel())		// 0=Korea, 1=Japan, 2=USA, 3=Brazil, 4=India, 5=International
	{
		case 0:		// Korea
			break;

		case 1:		// Japan
			break;

		case 2:		// USA
			if ( strstr( pFileName, "gunz.exe.manifest") != NULL)	bSkip = true;
			break;

		case 3:		// Brazil
			if ( strstr( pFileName, "license.htm") != NULL)			bSkip = false;
			break;

		case 4:		// India
			break;

		case 5:		// International
			break;

		default:
			break;
	}


	// ������ ����
	delete pFileName;


	if ( bSkip)
		return true;

	return false;
}
