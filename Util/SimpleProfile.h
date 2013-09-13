#ifndef __UTIL__SIMPLE_PROFILE__H__
#define __UTIL__SIMPLE_PROFILE__H__

#pragma once

#include "UtilHelper.h"
#include "DataProfile.h"

namespace SimpleProfile
{
	struct Scripts
	{
		String type;

		String receivedAction;

		String sendingAction;
		String sendedAction;

		String nodeChangingAction;
		String nodeChangedAction;
	};

	struct MetaEditor
	{
		enum Type
		{
			NONE,
			TEXT,
			SELECT,
			VECTOR,

		} type;

		int editorStyle;
		int textDigits;
		int textStep;

		String selectList;
		String selectSeparator;

		String subProfile;
		int vecMaxLength;
		int vecMinLength;

		MetaEditor()
		{
			type = NONE;

			editorStyle = 0;
			textDigits = 0;
			textStep = 0;

			vecMaxLength = INT_MAX;
			vecMinLength = 0;
		}

	};

	struct DataMeta
	{
		String name;
		String caption;
		String description;
		Scripts scripts;
		map<String, String> attrs;

		int dataType;
		String text;
		String regex;
		String defaultText;

		MetaEditor editor;
		DataMeta()
		{
			dataType = 0;
		}

	};

	struct DataNode
	{
		String name;
		String caption;
		String description;

		Scripts scripts;
		map<String, String> attrs;

		vector<DataNode> subNodes;
		vector<DataMeta> subMetas;
	};

	BOOL Convert(IDataNode * srcNode, 
		SimpleProfile::DataNode & destNode);

	HRESULT Convert(SimpleProfile::DataNode & srcNode, 
		IDataParser * parser, 
		IDataNode * destNode);

	IDataNode *  Convert(SimpleProfile::DataNode & srcNode,
		IDataParser * parser);


	String ToString(IDataParser * parser,
		SimpleProfile::DataNode & srcNode,
		BOOL styled);

	BOOL Parse(IDataParser * parser, 
		LPCWSTR text, 
		SimpleProfile::DataNode & destNode);


	String ToString(SimpleProfile::DataNode & srcNode,
		BOOL styled);

	BOOL Parse(LPCWSTR text, 
		SimpleProfile::DataNode & destNode);

}

#endif 