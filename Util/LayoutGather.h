#pragma once

#include "SimpleProfile.h"

class CLayoutGather;

struct DataGatherResult
{
	CLayoutGather * pGather;

	SimpleProfile::DataMeta * pMeta;
	SimpleProfile::DataNode * pNode;

	HRESULT hResult;

	DataGatherResult()
	{
		pGather = NULL;

		pMeta = NULL;
		pNode = NULL;

		hResult = E_FAIL;
	}

};

template<typename T>
struct LayoutGatherOwner
{
	static DataGatherResult GatherLayout(CLayoutGather * pGather)
	{
		DataGatherResult result;
		result.hResult = E_NOTIMPL;
		result.pGather = NULL;
		result.pMeta = NULL;
		result.pNode = NULL;

		return result;
	}

	static String ToLayoutConfig()
	{
		SimpleProfile::DataNode node;
		CLayoutGather gather(&node);

		T::GatherLayout(&gather);	

		String json = SimpleProfile::ToString(node, TRUE);

		return json;
	}
};

class CLayoutGather
{
public:

	SimpleProfile::DataNode * m_pNode;	

	typedef DataGatherResult ( *GatherLayoutProc)(CLayoutGather * pGather); 

public:

	CLayoutGather(SimpleProfile::DataNode * pNode);
	~CLayoutGather(void);

public:

	DataGatherResult Text(MetaDataType dataType, 
		const String & name, 
		const String & caption = _T(""),
		TextEditorStyle style = TES_Default, 
		const String & regex = _T(""),
		const String & defaultText = _T(""),
		const String & subProfile = _T(""),
		const String & description = _T(""));

	DataGatherResult Select(MetaDataType dataType, 
		const String & name, 
		const String & caption = _T(""),
		SelectEditorStyle style = SES_Default,
		const String & selectList = _T(""),
		const String & separator = _T(","),
		const String & subProfiles = _T(""),
		const String & defaultText = _T(""),
		const String & description = _T(""));

	DataGatherResult Vector(MetaDataType dataType, 
		const String & name, 		
		const String & caption = _T(""),
		const String & subProfile = _T(""),
		int maxLength = INT_MAX,
		int minLength = 0,
		const String & defaultText = _T(""),
		const String & description = _T(""));

	DataGatherResult Object(
		GatherLayoutProc proc,
		const String & name,
		const String & caption = _T(""),
		const String & description = _T(""));	
	
	DataGatherResult Map(const String & name, 		
		const String & caption = _T(""),
		const String & description = _T(""));

	template<typename T>
	DataGatherResult Object(const String & name,
		const String & caption = _T(""),
		const String & description = _T(""))
	{
		return Object(T::GatherLayout, name,caption, description);
	}


};
