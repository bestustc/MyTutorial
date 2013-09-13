#include "FileHelper.h"
#include <direct.h>
#include "DataSerialize.h"
#include "StringHelper.h"
#include "ComHelper.h"

HRESULT CFileHelper::Contains(LPCTSTR path, BOOL & b)
{
#ifdef UNICODE
	b = _waccess(path,0)>=0 ;
#else
	b= access(path,0)>=0 ;
#endif

	return S_OK;
}


HRESULT CFileHelper::GetCurrentForder(String & folder)
{
	TCHAR w[MAX_PATH]={0};

#ifdef UNICODE
	_wgetcwd(w,522);
#else
	getcwd(w,522);
#endif

	folder=w;		

	return S_OK;
}

HRESULT CFileHelper::WriteBytes(LPCTSTR path, int count,const BYTE * bytes)
{
	BOOL exists = FALSE;

	HRESULT hr = Contains(path, exists);
	if(FAILED(hr)) return hr;

	if( !exists)
	{
		String folder ;
		if(SUCCEEDED(GetParentFolder(path, folder)))
		{
			hr = CreateFolder(folder.c_str());
			if(FAILED(hr)) return hr;
		}
	}

	ofstream ofs;

	ofs.open(path);

	if(!ofs.is_open()) return E_FAIL;

	ofs.clear();

	if(bytes) 	
		ofs.write((const char *)bytes,(int)count);

	ofs.close();

	return S_OK;

}

HRESULT CFileHelper::WriteText(LPCTSTR path, LPCTSTR text)
{
	BOOL exists = FALSE;

	HRESULT hr = Contains(path, exists);
	if(FAILED(hr)) return hr;

	if( !exists)
	{
		String folder ;
		if(SUCCEEDED( GetParentFolder(path, folder)))
		{
			hr = CreateFolder(folder.c_str());	
			if(FAILED(hr)) return hr;
		}
	}	

	FileOutputStream ofs;

	ofs.imbue(locale("chs"));

	ofs.open(path);

	if(!ofs.is_open()) return E_FAIL;

	ofs.clear();

	ofs.write(text, STRLEN(text)/* *sizeof(TCHAR)*/);

	ofs.close();

	return S_OK;
}


HRESULT CFileHelper::ReadAllBytes(LPCTSTR path, vector<BYTE> & bytes) 
{
	basic_ifstream<BYTE, std::char_traits<BYTE> >  ifs;	

	ifs.open(path,std::ifstream::in);

	if(!ifs.is_open()) return  E_FAIL;

	ifs.seekg(0,ifs.end);

	int size=ifs.tellg();

	ifs.seekg(0);	

	BYTE * pBytes = (BYTE *)malloc(sizeof(BYTE)*(size+1));
	memset(pBytes,0, sizeof(BYTE) *(size +1));

	ifs._Read_s(pBytes,size+1,size);
	ifs.close();

	for(int i=0;i<size;i++)
	{
		bytes.push_back(pBytes[i]);
	}

	free(pBytes);

	return S_OK;
}

HRESULT CFileHelper::ReadAllText(LPCTSTR path, String & text)
{
	FileInputStream ifs;

	ifs.imbue(locale("chs"));

	ifs.open(path,ifs.in);

	if(!ifs.is_open()) return E_FAIL;

	ifs.seekg(0,ifs.end);

	int size=ifs.tellg();

	ifs.seekg(0);	

	TCHAR * s=(TCHAR *)malloc(sizeof(TCHAR) * (size+1));

	memset(s,0,sizeof(TCHAR)*(size+1));

	ifs._Read_s(s,size ,sizeof(TCHAR) * size);

	ifs.close();

	text = s;

	free(s);

	return S_OK;
}

HRESULT CFileHelper::ReadLines(LPCTSTR path, vector<String> & lines)
{
	String text;
	HRESULT hr = ReadAllText(path, text);
	if(FAILED(hr)) return hr;

	vector<String> temp;

	BOOL b = CStringHelper::Split(text.c_str(), _T('\n'), temp);
	if(!b) return E_FAIL;

	for(vector<String>::iterator it = temp.begin(), end = temp.end();
		it != end ;it++)
	{
		String line = CStringHelper::Trim(it->c_str() ,_T('\r'));
		lines.push_back(line);
	}

	return S_OK;
}


static HRESULT _ListChildren(LPCTSTR path, vector<WIN32_FIND_DATA> & children)
{
	WIN32_FIND_DATA fileData;

	String szPath = path;
	szPath += _T("\\*");

	HANDLE  handle = ::FindFirstFile(szPath.c_str(),&fileData);

	while(handle != INVALID_HANDLE_VALUE)
	{
		children.push_back( fileData);
		if( !FindNextFile(handle,&fileData) )break;
	}

	::FindClose(handle);

	return S_OK;
}


HRESULT CFileHelper::ListChildren(LPCTSTR path, vector<String> & children)
{
	vector<WIN32_FIND_DATA> fileDatas;
	HRESULT hr = _ListChildren(path, fileDatas);

	if(SUCCEEDED(hr))
	{
		for(int i=0,size = fileDatas.size() ; i<size;i++)
		{
			children.push_back( LPTSTR2STRING(fileDatas[i].cFileName) );
		}

		return S_OK;
	}

	return hr;

}

HRESULT CFileHelper::IsDirectory(LPCTSTR path, BOOL & b)
{
	DWORD attrs = ::GetFileAttributes(path);
	b = BIT_IS_TRUE(attrs, FILE_ATTRIBUTE_DIRECTORY);
	return S_OK;
}


HRESULT CFileHelper::RemoveFile(LPCTSTR path)
{
#ifdef UNICODE
	return _wremove(path) ==0 ? S_OK : E_FAIL;
#else
	return remove(path)==0 ? S_OK : E_FAIL;
#endif

}

HRESULT CFileHelper::RemoveFolder(LPCTSTR path, BOOL removeChildren)
{
	vector<WIN32_FIND_DATA> fileDatas;

	if(_ListChildren(path, fileDatas))
	{
		for(int i=0,size = fileDatas.size() ; i<size;i++)
		{				
			LPCTSTR path = fileDatas[i].cFileName;

			if(BIT_IS_TRUE( fileDatas[i].dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) && removeChildren)
			{
				HRESULT hr = RemoveFolder(path, removeChildren);
				if(FAILED(hr)) return hr;
			}
			else
			{
				HRESULT hr = RemoveFile(path);
				if(FAILED(hr)) return hr;
			}
		}

		return S_OK;
	}

	return E_FAIL;
}



HRESULT CFileHelper::AppendQuotes(LPCTSTR path, String & newPath)
{
	TCHAR szPath[MAX_PATH + _ATL_QUOTES_SPACE] = { TZERO };

	int index = 0;
	for(index =0;index < MAX_PATH && path[index] != TZERO; index++)
	{
		szPath [index +1 ] = path [index];
	}

	szPath[0] = TQUOTES;
	szPath[index + 1 ] = TQUOTES;
	szPath[index + 2 ] = TZERO;

	newPath = LPTSTR2STRING(szPath);

	return S_OK;
}

HRESULT CFileHelper::RemoveQuotes(LPCTSTR path, String & newPath)
{
	TCHAR szPath[MAX_PATH ] = { TZERO };

	int index = 1;
	for(index =1;index < MAX_PATH + _ATL_QUOTES_SPACE -1 && path[index] != TZERO; index++)
	{
		szPath [index - 1 ] = path [index];
	}

	szPath[index - 1 ] = TZERO;

	newPath = LPTSTR2STRING(szPath);

	return S_OK;
}


HRESULT CFileHelper::GetParentFolder(LPCTSTR path, String & parent)
{
	TCHAR szPath[MAX_PATH + _ATL_QUOTES_SPACE] = { TZERO };

	int lastIndex = 0;
	for(int index =0;index < MAX_PATH + _ATL_QUOTES_SPACE && path[index] != TZERO; index++)
	{
		TCHAR c = path[index];
		if(c == TBACKSLASH) c = TSLASH;
		szPath [index] =  c;

		if(c == TSLASH) lastIndex = index;
	}

	if(path[0] == TQUOTES) 
	{
		szPath[lastIndex ]  =  TQUOTES;
		szPath[lastIndex + 1] = TZERO;
	}
	else
	{
		szPath[lastIndex] = TZERO;
	}

	parent = LPTSTR2STRING( szPath);

	return S_OK;
}

HRESULT CFileHelper::CreateFolder(LPCTSTR path)
{
	BOOL b= FALSE;
	HRESULT hr = Contains(path, b);
	if(FAILED(hr)) return hr;

	if(b) return S_FALSE;

	String parent;
	hr = GetParentFolder(path, parent);
	if(FAILED(hr)) return hr;

	hr = CreateFolder(parent.c_str());
	if(FAILED(hr)) return hr;

#ifdef UNICODE
	return _wmkdir(path) == 0 ? S_OK : E_FAIL;
#else
	return _mkdir(path) == 0 ? S_OK : E_FAIL;
#endif
}


HRESULT CFileHelper::WriteObject(LPCTSTR path, IDataSerializable & data)
{
	CDataSerialize serialize(TRUE) ;

	HRESULT hr = data.Write(serialize);
	if(FAILED(hr)) return hr;

	IDataNode * pNode;
	hr = serialize.GetNode(&pNode);
	if(FAILED(hr)) return hr;

	BSTR text;
	hr = PROFILE_PARSER()->ToFullJson(pNode, COM_BOOL(TRUE), &text);
	pNode->Release();

	if(FAILED(hr)) return hr;

	hr = WriteText(path, text);
	FREE_BSTR(text);

	return hr;

}

HRESULT CFileHelper::ReadObject(LPCTSTR path, IDataSerializable & data)
{
	String text ;
	HRESULT hr = ReadAllText(path, text);
	if(FAILED(hr)) return hr;

	IDataNode * pNode = NULL;
	hr = PROFILE_PARSER()->ParseJson(VARIANT_BSTR(text.c_str()), NULL, &pNode);
	if(FAILED(hr)) return hr;


	CDataSerialize serialize(FALSE, pNode) ;
	pNode->Release();

	return data.Read(serialize);

}