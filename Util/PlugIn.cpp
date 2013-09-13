#include "PlugIn.h"

CPlugInInfo::CPlugInInfo()
{
	memset(this,0,sizeof(*this));
}

#define SET_PLUGIN_STRING_VALUE(dest, src) _tcscpy_s(dest, sizeof(dest)/sizeof(TCHAR), src);

CPlugInInfo::CPlugInInfo(const CPlugInInfo & other)
{

	SET_PLUGIN_STRING_VALUE(this->author, other.author);	
	SET_PLUGIN_STRING_VALUE(this->description, other.description);
	SET_PLUGIN_STRING_VALUE(this->function, other.function);

	SET_PLUGIN_STRING_VALUE(this->guid, other.guid);
	SET_PLUGIN_STRING_VALUE(this->name, other.name);
	SET_PLUGIN_STRING_VALUE(this->version, other.version);

}

void CPlugInInfo::SetName(const TCHAR * name)
{
	SET_PLUGIN_STRING_VALUE(this->name, name);
}

void CPlugInInfo::SetFunction(const TCHAR * fuction)
{
	SET_PLUGIN_STRING_VALUE(this->function, function);
}

void CPlugInInfo::SetAuthor(const TCHAR * author)
{
	SET_PLUGIN_STRING_VALUE(this->author, author);
}

void CPlugInInfo::SetDescription(const TCHAR * description)
{
	SET_PLUGIN_STRING_VALUE(this->description, description);
}

void CPlugInInfo::SetVersion(const TCHAR * version)
{
	SET_PLUGIN_STRING_VALUE(this->version, version);
}

void CPlugInInfo::SetGuid(const TCHAR * guid)
{
	SET_PLUGIN_STRING_VALUE(this->guid, guid);
}



CPlugInInfo & CPlugInInfo::operator =(const CPlugInInfo & other)
{
	if(&other == this) return *this;

	SET_PLUGIN_STRING_VALUE(this->author, other.author);	
	SET_PLUGIN_STRING_VALUE(this->description, other.description);
	SET_PLUGIN_STRING_VALUE(this->function, other.function);

	SET_PLUGIN_STRING_VALUE(this->guid, other.guid);
	SET_PLUGIN_STRING_VALUE(this->name, other.name);
	SET_PLUGIN_STRING_VALUE(this->version, other.version);

	return *this;
}



CPlugInString::CPlugInString()
{
	m_buffer = NULL;
}

CPlugInString::~CPlugInString()
{
	if(m_buffer != NULL)
	{
		delete [] reinterpret_cast<TCHAR *>(m_buffer);
	}
}

const TCHAR * __stdcall CPlugInString::GetValue() const 
{
	return reinterpret_cast<TCHAR *>(m_buffer);
}

void __stdcall CPlugInString::SetValue(const TCHAR * value)
{
	if(m_buffer != NULL)
	{
		delete [] reinterpret_cast<TCHAR *>(m_buffer);
		m_buffer = NULL;
	}

	size_t len = STRLEN(value);

	TCHAR * temp = new TCHAR[len+1];

	memset(temp,0,sizeof(TCHAR)*(len +1) );

	_tcscpy_s(temp, len+1, value);

	m_buffer = temp;

}


