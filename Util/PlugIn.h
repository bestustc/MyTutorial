#ifndef __UTIL__PLUG_IN__H__
#define __UTIL__PLUG_IN__H__

#pragma once

#include "UtilHelper.h"

#ifndef PLUGIN_FUNCALL
#define PLUGIN_FUNCALL __stdcall
#endif

// ����Ļ�����Ϣ
class CPlugInInfo
{
public:

	// ����
	TCHAR name[256];

	// ����
	TCHAR function[256];

	// ����,ʵ����Ա
	TCHAR author[256];

	// ������Ϣ
	TCHAR description[1024];

	// �汾��
	TCHAR version[16];

	// GUID��ʶ��
	TCHAR guid[64];

public:

	CPlugInInfo();

	CPlugInInfo(const CPlugInInfo & other);

	CPlugInInfo & operator =(const CPlugInInfo & other);

public:

	// ��������
	void SetName(const TCHAR * name);	

	// ���ù���
	void SetFunction(const TCHAR * name);	

	// ��������,ʵ����Ա
	void SetAuthor(const TCHAR * name);

	// ����������Ϣ
	void SetDescription(const TCHAR * name);

	// ���ð汾��
	void SetVersion(const TCHAR * name);

	// ����GUID��ʶ��
	void SetGuid(const TCHAR * name);
};

// ���ڴ���PlugIn�е��ַ���
struct IPlugInString
{
	// ��ȡ�ַ���
	virtual const TCHAR * PLUGIN_FUNCALL GetValue() const = 0;

	// �����ַ���
	virtual void PLUGIN_FUNCALL SetValue(const TCHAR * value) = 0;
};


// �������
struct IPlugIn
{
	// ��������,ʹ�ø÷������������Լ���ռ����Դ
	virtual void PLUGIN_FUNCALL Dispose() = 0;	
};

// �豸���Ӵ�������
struct IPlugInFactory
{
	// ��ȡһ������
	virtual IPlugIn * PLUGIN_FUNCALL CreateObject() = 0;

	// ��ȡ�������Ϣ
	virtual void PLUGIN_FUNCALL GetInfo(CPlugInInfo & info) = 0;

	// ��������,ʹ�ø÷������������Լ���ռ����Դ
	virtual void PLUGIN_FUNCALL Dispose() = 0;	

	// ��ȡ�Ƿ�Ϊһ�����͵�����
	virtual BOOL PLUGIN_FUNCALL IsTypeOf(const type_info & typeInfo) = 0;

	// ��ȡ�Ƿ�Ϊһ�����͵�����
	template<typename T>
	BOOL PLUGIN_FUNCALL IsTypeOf() const
	{
		return IsTypeOf(typeid(T));
	}
};

extern "C"
{
	// �������ӹ����ĵ�������
	_declspec(dllimport) IPlugInFactory * CreatePlugInFactory();
}

typedef IPlugInFactory * (* CreatePlugInFactoryProc)();

#define CREATE_PLUG_IN_FACTORY_PROC_NAME _T("CreatePlugInFactory")

// PlugInString��һ��ʵ��
class CPlugInString : public IPlugInString
{
	void * m_buffer;

public:

	CPlugInString();

	~CPlugInString();

	virtual const TCHAR * PLUGIN_FUNCALL GetValue() const ;

	virtual void PLUGIN_FUNCALL SetValue(const TCHAR * value) ;

private:

	CPlugInString(const CPlugInString &);

	CPlugInString &operator = (const CPlugInString &);
};

#endif 
