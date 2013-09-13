#include "LayoutGather.h"


CLayoutGather::CLayoutGather(SimpleProfile::DataNode * pNode)
{
	m_pNode = pNode;
}

CLayoutGather::~CLayoutGather(void)
{
}


DataGatherResult CLayoutGather::Text(MetaDataType dataType, 
									 const String & name, 
									 const String & caption,
									 TextEditorStyle style , 
									 const String & regex,
									 const String & defaultText,
									 const String & subProfile,
									 const String & description )
{
	DataGatherResult result;
	result.pGather = this;
	result.pMeta = NULL;
	result.pNode = NULL;

	if(m_pNode != NULL)
	{
		SimpleProfile::DataMeta meta;

		meta.dataType = dataType;
		meta.name = name;
		meta.caption = caption;

		meta.regex = regex;
		meta.defaultText = defaultText;
		meta.description = description;

		SimpleProfile::MetaEditor editor;
		editor.type = SimpleProfile::MetaEditor::TEXT;
		editor.editorStyle = (int)style;
		editor.subProfile = subProfile;

		meta.editor = editor;

		m_pNode->subMetas.push_back(meta);

		result.pMeta = &m_pNode->subMetas[ m_pNode->subMetas.size()-1];
	}

	return result;
}

DataGatherResult CLayoutGather::Select(MetaDataType dataType, 
									   const String & name, 
									   const String & caption,
									   SelectEditorStyle style,
									   const String & selectList ,
									   const String & separator ,
									   const String & subProfiles,
									   const String & defaultText,
									   const String & description )
{
	DataGatherResult result;
	result.pGather = this;
	result.pMeta = NULL;
	result.pNode = NULL;

	if(m_pNode != NULL)
	{
		SimpleProfile::DataMeta meta;

		meta.dataType = dataType;
		meta.name = name;
		meta.caption = caption;

		meta.defaultText = defaultText;
		meta.description = description;

		SimpleProfile::MetaEditor editor;
		editor.type = SimpleProfile::MetaEditor::SELECT;
		editor.editorStyle = (int)style;
		editor.selectList = selectList;
		editor.selectSeparator = separator;
		editor.subProfile = subProfiles;

		meta.editor = editor;

		m_pNode->subMetas.push_back(meta);

		result.pMeta = &m_pNode->subMetas[ m_pNode->subMetas.size()-1];
	}

	return result;
}

DataGatherResult CLayoutGather::Vector(MetaDataType dataType, 
									   const String & name, 		
									   const String & caption,
									   const String & subProfile,
									   int maxLength,
									   int minLength,
									   const String & defaultText,
									   const String & description )
{
	DataGatherResult result;
	result.pGather = this;
	result.pMeta = NULL;
	result.pNode = NULL;

	if(m_pNode != NULL)
	{
		SimpleProfile::DataMeta meta;

		meta.dataType = dataType;
		meta.name = name;
		meta.caption = caption;

		meta.defaultText = defaultText;
		meta.description = description;

		SimpleProfile::MetaEditor editor;
		editor.type = SimpleProfile::MetaEditor::VECTOR;
		editor.vecMaxLength = maxLength;
		editor.vecMinLength = minLength;
		editor.subProfile = subProfile;

		meta.editor = editor;

		m_pNode->subMetas.push_back(meta);

		result.pMeta = &m_pNode->subMetas[ m_pNode->subMetas.size()-1];
	}

	return result;
}



DataGatherResult CLayoutGather::Object(GatherLayoutProc proc,
									   const String & name,
									   const String & caption,
									   const String & description)
{
	DataGatherResult result;
	result.pGather = this;
	result.pMeta = NULL;
	result.pNode = NULL;

	if(m_pNode != NULL)
	{
		SimpleProfile::DataNode node;

		node.name = name;
		node.caption = caption;
		node.description = description;		

		m_pNode->subNodes.push_back(node);

		result.pNode = &m_pNode->subNodes[ m_pNode->subNodes.size()-1];

		CLayoutGather gather(result.pNode);

		if(proc != NULL) proc(&gather);
	}

	return result;
}

DataGatherResult CLayoutGather::Map(const String & name, 		
									const String & caption,
									const String & description)
{
	return Object(NULL, name, caption, description);
}



