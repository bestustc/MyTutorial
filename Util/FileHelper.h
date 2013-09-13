#ifndef __UTIL__FILE_HELPER__H__
#define __UTIL__FILE_HELPER__H__

#pragma once

#include "UtilHelper.h"

struct IDataSerializable;

class CFileHelper
{
public:

	static HRESULT WriteBytes(LPCTSTR path, int count,const BYTE * bytes);
	
	static HRESULT WriteText(LPCTSTR path, LPCTSTR text);

	static HRESULT Contains(LPCTSTR path, BOOL & b);

	static HRESULT GetCurrentForder(String & folder);

	static HRESULT ReadAllBytes(LPCTSTR path, vector<BYTE> & bytes) ;

	static HRESULT ReadAllText(LPCTSTR path, String & text);

	static HRESULT ReadLines(LPCTSTR path, vector<String> & lines);


	static HRESULT ListChildren(LPCTSTR path, vector<String> & children);

	static HRESULT IsDirectory(LPCTSTR path, BOOL & b);


	static HRESULT RemoveFile(LPCTSTR path);

	static HRESULT RemoveFolder(LPCTSTR path, BOOL removeChildren);


	static HRESULT AppendQuotes(LPCTSTR path, String & newPath);

	static HRESULT RemoveQuotes(LPCTSTR path, String & newPath);


	static HRESULT GetParentFolder(LPCTSTR path, String & parent);

	static HRESULT CreateFolder(LPCTSTR path);


	static HRESULT WriteObject(LPCTSTR path, IDataSerializable & data);

	static HRESULT ReadObject(LPCTSTR path, IDataSerializable & data);

};


#endif