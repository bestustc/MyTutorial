#ifndef __UTIL__PLUG_IN__H__
#define __UTIL__PLUG_IN__H__

#pragma once

#include "UtilHelper.h"

#ifndef PLUGIN_FUNCALL
#define PLUGIN_FUNCALL __stdcall
#endif

// 插件的基本信息
class CPlugInInfo
{
public:

	// 名称
	TCHAR name[256];

	// 功能
	TCHAR function[256];

	// 作者,实现人员
	TCHAR author[256];

	// 描述信息
	TCHAR description[1024];

	// 版本号
	TCHAR version[16];

	// GUID标识符
	TCHAR guid[64];

public:

	CPlugInInfo();

	CPlugInInfo(const CPlugInInfo & other);

	CPlugInInfo & operator =(const CPlugInInfo & other);

public:

	// 设置名称
	void SetName(const TCHAR * name);	

	// 设置功能
	void SetFunction(const TCHAR * name);	

	// 设置作者,实现人员
	void SetAuthor(const TCHAR * name);

	// 设置描述信息
	void SetDescription(const TCHAR * name);

	// 设置版本号
	void SetVersion(const TCHAR * name);

	// 设置GUID标识符
	void SetGuid(const TCHAR * name);
};

// 用于处理PlugIn中的字符串
struct IPlugInString
{
	// 获取字符串
	virtual const TCHAR * PLUGIN_FUNCALL GetValue() const = 0;

	// 设置字符串
	virtual void PLUGIN_FUNCALL SetValue(const TCHAR * value) = 0;
};


// 插件对象
struct IPlugIn
{
	// 析构方法,使用该方法析构自身，以及用占用资源
	virtual void PLUGIN_FUNCALL Dispose() = 0;	
};

// 设备连接创建工厂
struct IPlugInFactory
{
	// 获取一个对象
	virtual IPlugIn * PLUGIN_FUNCALL CreateObject() = 0;

	// 获取插件的信息
	virtual void PLUGIN_FUNCALL GetInfo(CPlugInInfo & info) = 0;

	// 析构方法,使用该方法析构自身，以及用占用资源
	virtual void PLUGIN_FUNCALL Dispose() = 0;	

	// 获取是否为一个类型的子类
	virtual BOOL PLUGIN_FUNCALL IsTypeOf(const type_info & typeInfo) = 0;

	// 获取是否为一个类型的子类
	template<typename T>
	BOOL PLUGIN_FUNCALL IsTypeOf() const
	{
		return IsTypeOf(typeid(T));
	}
};

extern "C"
{
	// 创建连接工厂的导出函数
	_declspec(dllimport) IPlugInFactory * CreatePlugInFactory();
}

typedef IPlugInFactory * (* CreatePlugInFactoryProc)();

#define CREATE_PLUG_IN_FACTORY_PROC_NAME _T("CreatePlugInFactory")

// PlugInString的一个实现
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
