#include "DataProfileHelper.h"
#include "StringHelper.h"
#include "ComHelper.h"

#define COPY_STRING_NODE2NODE(propertyName, src, dest)\
	do\
	{\
	BSTR __str = NULL;\
	if(SUCCEEDED(src->get_##propertyName(&__str)))\
		{\
		if(SysStringLen(__str) != 0) dest->put_##propertyName(__str);\
		SysFreeString(__str);\
		__str = NULL;\
		}\
	}while(0);

#define COPY_BASE_NODE2NODE(propertyName, src, dest, type)\
	do\
	{\
	type __val = type();\
	if(SUCCEEDED(src->get_##propertyName(&__val)))\
		{\
		dest->put_##propertyName(__val);\
		}\
	}while(0);


#define COPY_STRING_JSON2NODE(name, propertyName, json, node)\
	do\
{\
	BSTR __str = NULL;\
	if(SUCCEEDED(GetBSTR(json,name,&__str)))\
	{\
	node->put_##propertyName(__str);\
	SysFreeString(__str);\
	__str = NULL;\
	}\
}while(0);

#define COPY_BASE_JSON2NODE( name, propertyName, json,  node, type)\
	do\
{\
	LONG __val = NULL;\
	if(SUCCEEDED(GetLong(json,name,&__val)))\
	{\
	node->put_##propertyName((type)__val);\
	}\
}while(0);


#define COPY_STRING_NODE2JSON(propertyName, name, node,  json)\
	do\
{\
	BSTR __str = NULL;\
	if(SUCCEEDED(node->get_##propertyName(&__str)) && __str!=NULL)\
	{\
	if(SysStringLen(__str)!=0) json->SetByName(name, CComVariant(__str)); \
	SysFreeString(__str);\
	__str=NULL;\
	}\
}while(0);

#define COPY_BASE_NODE2JSON(propertyName, name, node,  json, type)\
	do\
{\
	type __val = type() ;\
	if(SUCCEEDED(node->get_##propertyName(&__val)))\
	{\
	json->SetByName(name, CComVariant((LONG) __val)); \
	}\
}while(0);


HRESULT CDataProfileHelper::Clone(IDataNode * srcNode, bool full, IDataNode * destNode, IDataParser * parser)
{	
	if(full)
	{
		// 复制属性
		IDataAttributes * srcAttrs = NULL;
		IDataAttributes * destAttrs = NULL;		

		if(SUCCEEDED( srcNode->get_Attributes(&srcAttrs))
			&& SUCCEEDED( destNode->get_Attributes(&destAttrs)))
		{
			CopyAttributes(srcAttrs,destAttrs);			
		}

		if( srcAttrs != NULL) srcAttrs->Release();
		if( destAttrs != NULL) destAttrs->Release();

		// 复制脚本
		IDataScripts * srcScripts = NULL;
		IDataScripts * destScripts = NULL;

		if(SUCCEEDED(srcNode->get_Scripts(&srcScripts))
			&&srcScripts != NULL 			
			&& SUCCEEDED(destNode->get_Scripts(&destScripts))
			&& destScripts != NULL)
		{
			COPY_STRING_NODE2NODE(Type,srcScripts, destScripts);
			COPY_STRING_NODE2NODE(ReceivedAction,srcScripts, destScripts);

			COPY_STRING_NODE2NODE(SendingAction,srcScripts, destScripts);
			COPY_STRING_NODE2NODE(SendedAction,srcScripts, destScripts);

			COPY_STRING_NODE2NODE(NodeChangingAction,srcScripts, destScripts);
			COPY_STRING_NODE2NODE(NodeChangedAction,srcScripts, destScripts);
		}

		if(srcScripts != NULL) srcScripts->Release();
		if(destScripts != NULL) destScripts->Release();
	}

	// 复制基本量
	if(full)
	{
		COPY_STRING_NODE2NODE(Caption, srcNode , destNode);
		COPY_STRING_NODE2NODE(Description, srcNode , destNode);
	}

	IDataMeta * srcMeta = NULL;
	IDataMeta * destMeta = NULL;

	if(SUCCEEDED(srcNode->QueryInterface(&srcMeta))) //  如果是数据元，复制数据元
	{
		if(SUCCEEDED(destNode->QueryInterface(&destMeta)))
		{		
			COPY_STRING_NODE2NODE(Text,srcMeta,destMeta);	

			if(full)
			{
				COPY_BASE_NODE2NODE(DataType,srcMeta,destMeta, MetaDataType);			
				COPY_STRING_NODE2NODE(Default,srcMeta,destMeta);
				COPY_STRING_NODE2NODE(Regex,srcMeta,destMeta);	

				IDataMetaEditor * srcEditor = NULL;
				if(SUCCEEDED(srcMeta->get_Editor(&srcEditor)) && srcEditor != NULL)
				{
					IDataMetaSelectEditor * srcSelectEditor = NULL;
					IDataMetaTextEditor * srcTextEditor = NULL;
					IDataMetaVectorEditor * srcVectorEditor = NULL;

					IDataMetaSelectEditor * destSelectEditor = NULL;
					IDataMetaTextEditor * destTextEditor = NULL;
					IDataMetaVectorEditor * destVectorEditor = NULL;

					if(SUCCEEDED(srcEditor->QueryInterface(&srcSelectEditor))
						&& SUCCEEDED(parser->CreateSelectEditor(&destSelectEditor))
						&& SUCCEEDED(destMeta->put_Editor(destSelectEditor)))
					{
						COPY_STRING_NODE2NODE(SelectList,srcSelectEditor,destSelectEditor);
						COPY_STRING_NODE2NODE(Separator,srcSelectEditor,destSelectEditor);	
						COPY_STRING_NODE2NODE(SubProfile,srcSelectEditor,destSelectEditor);

						COPY_BASE_NODE2NODE(Style,srcSelectEditor,destSelectEditor, SelectEditorStyle);
					}

					if(SUCCEEDED(srcEditor->QueryInterface(&srcTextEditor))
						&& SUCCEEDED(parser->CreateTextEditor(&destTextEditor))
						&& SUCCEEDED(destMeta->put_Editor(destTextEditor)))
					{
						COPY_BASE_NODE2NODE(Style,srcTextEditor,destTextEditor, TextEditorStyle);
						COPY_BASE_NODE2NODE(Digits,srcTextEditor,destTextEditor, LONG);
						COPY_BASE_NODE2NODE(Step,srcTextEditor,destTextEditor, LONG);
					}

					if(SUCCEEDED(srcEditor->QueryInterface(&srcVectorEditor))
						&& SUCCEEDED(parser->CreateVectorEditor(&destVectorEditor))
						&& SUCCEEDED(destMeta->put_Editor(destVectorEditor)))
					{						
						COPY_STRING_NODE2NODE(SubProfile,srcVectorEditor,destVectorEditor);
						COPY_BASE_NODE2NODE(MaxLength,srcVectorEditor,destVectorEditor,LONG);
						COPY_BASE_NODE2NODE(MinLength,srcVectorEditor,destVectorEditor,LONG);
					}

					if(srcSelectEditor != NULL) srcSelectEditor->Release();
					if(srcTextEditor != NULL) srcTextEditor->Release();
					if(srcVectorEditor != NULL) srcVectorEditor->Release();

					if(destSelectEditor != NULL) destSelectEditor->Release();
					if(destTextEditor != NULL) destTextEditor->Release();
					if(destVectorEditor != NULL) destVectorEditor->Release();
				}

				if(srcEditor != NULL) srcEditor->Release();
			}
		}

		if(srcMeta != NULL) srcMeta->Release();
		if(destMeta != NULL) destMeta->Release();

	}
	else // 复制子元素
	{
		IDataChildren * srcChildren = NULL;
		IDataChildren * destChildren = NULL;
		IReadonlyStringList * srcNameList = NULL;
		LONG srcChildCount = 0;

		if(SUCCEEDED(srcNode->get_Children(&srcChildren))
			&& SUCCEEDED(destNode->get_Children(&destChildren))
			&& SUCCEEDED( srcChildren->get_Names(&srcNameList))
			&& SUCCEEDED(srcChildren->get_Count(&srcChildCount))
			&& srcChildCount != 0)
		{

			for(LONG i=0;i<srcChildCount;i++)
			{	
				IDataNode * srcChildNode = NULL;
				IDataMeta * srcChildMeta = NULL;

				IDataNode * destChildNode = NULL;
				IDataMeta * destChildMeta = NULL;

				IUnknown * destUnk = NULL;

				BSTR name = NULL;
				if(SUCCEEDED(srcNameList->GetAt(i,&name))
					&& SUCCEEDED(srcChildren->GetByName(name,&srcChildNode)))
				{
					if(SUCCEEDED(srcChildNode->QueryInterface(&srcChildMeta)))
					{		
						if( SUCCEEDED(destChildren->AddMeta(name, NULL,  &destUnk))
							&& SUCCEEDED(destUnk->QueryInterface(&destChildMeta)))

							Clone(srcChildMeta, full, destChildMeta, parser);
					}
					else
					{
						if( SUCCEEDED(destChildren->AddNode(name, NULL,  &destUnk))
							&& SUCCEEDED(destUnk->QueryInterface(&destChildNode)))
							Clone(srcChildNode, full, destChildNode, parser);
					}
				}

				if( srcChildNode != NULL) srcChildNode->Release();
				if( srcChildMeta != NULL) srcChildMeta->Release();

				if( destChildNode != NULL) destChildNode->Release();
				if( destChildMeta != NULL) destChildMeta->Release();

				if( destUnk != NULL) destUnk->Release();

				if(name != NULL) SysFreeString(name);
			}
		}
	}

	return S_OK;
}


HRESULT CDataProfileHelper::CopyAttributes(IDataAttributes * srcAttrs, IDataAttributes * destAttrs)
{
	POINTER_NULL(srcAttrs);
	POINTER_NULL(destAttrs);

	::IReadonlyStringList * srcAttrNames = NULL;
	LONG srcAttrCount = 0;

	if(SUCCEEDED( srcAttrs->get_Keys(&srcAttrNames))
		&& SUCCEEDED( srcAttrNames->get_Count(&srcAttrCount))
		&& srcAttrCount !=0)
	{
		for(LONG i=0;i<srcAttrCount;i++)
		{
			BSTR key = NULL;
			BSTR val = NULL;
			if(SUCCEEDED(srcAttrNames->GetAt(i, &key)) 
				&& SUCCEEDED(srcAttrs->Get(key,&val)))
			{
				destAttrs->Set(key,val);
			}
			if(key != NULL) SysFreeString(key);
			if(val != NULL) SysFreeString(val);
		}
	}

	if( srcAttrNames != NULL) srcAttrNames->Release();

	return S_OK;
}

HRESULT CDataProfileHelper::CopyAttributes(IDataNode * srcNode, IDataNode * destNode)
{
	POINTER_NULL(srcNode);
	POINTER_NULL(destNode);

	IDataAttributes * pSrcAttrs = NULL;
	IDataAttributes * pDestAttrs = NULL;

	HRESULT hr = S_OK;
	if( SUCCEEDED( hr = srcNode->get_Attributes(&pSrcAttrs))
		&& SUCCEEDED( hr = destNode->get_Attributes( &pDestAttrs)))
	{
		hr = CopyAttributes(pSrcAttrs, pDestAttrs);
	}

	if(pSrcAttrs != NULL) pSrcAttrs->Release();
	if(pDestAttrs != NULL) pDestAttrs->Release();

	return hr;

}

HRESULT CDataProfileHelper::MergeNode(IDataNode * fullNode, IDataNode * valuesNode)
{
	IDataChildren * fullChildren = NULL;
	IDataChildren * valuesChildren = NULL;

	::IReadonlyStringList * fullChildNames = NULL;

	LONG fullChildCount = 0;

	HRESULT hr = S_OK;

	if(SUCCEEDED(fullNode->get_Children(&fullChildren))
		&& SUCCEEDED(valuesNode->get_Children(&valuesChildren))
		&& SUCCEEDED(fullChildren->get_Names(&fullChildNames))
		&& SUCCEEDED(fullChildNames->get_Count(&fullChildCount))
		&& fullChildCount !=0 )
	{
		for(LONG i =0;i<fullChildCount;i++)
		{
			BSTR name = NULL;
			IDataNode * fullChild = NULL;
			IDataMeta * fullMeta = NULL;

			if(SUCCEEDED(fullChildNames->GetAt(i,&name))
				&& SUCCEEDED(fullChildren->GetByName(name,&fullChild)))
			{
				IDataNode * valuesChild = NULL;
				IDataMeta * valuesMeta = NULL;

				do
				{
					HRESULT hr = valuesChildren->GetByName(name, &valuesChild);
					if(FAILED(hr)) break;

					if( SUCCEEDED(fullChild->QueryInterface(&fullMeta)))
					{
						hr = valuesChild->QueryInterface(&valuesMeta);
						if(FAILED(hr)) break;

						COPY_STRING_NODE2NODE(Text,fullMeta,valuesMeta);
					}
					else
					{
						hr = valuesChild->QueryInterface(&valuesMeta);
						if(SUCCEEDED(hr)) {hr= E_FAIL ; break;}

						hr = MergeNode(fullChild,valuesChild);
						if(FAILED(hr)) break;
					}

				}while(0);

				if(valuesChild!=NULL) valuesChild->Release();
				if(valuesMeta != NULL) valuesMeta->Release();

			}
			if( name != NULL) SysFreeString(name);
			if( fullChild != NULL) fullChild->Release();
			if( fullMeta != NULL) fullMeta->Release();

			if(FAILED(hr)) break;
		}
	}

	if( fullChildren != NULL) fullChildren->Release();
	if( valuesChildren != NULL) valuesChildren->Release();

	if( fullChildNames != NULL) fullChildNames->Release();

	return hr;

}


HRESULT CDataProfileHelper::GetLong(IJsonValue * json, BSTR name, LONG * pVal)
{
	INVALID_NULL(pVal);

	IJsonValue * child= NULL;
	HRESULT hr = json->GetByName(name,&child);
	if(FAILED(hr)) return hr;

	return child->AsInt(pVal);
}

HRESULT CDataProfileHelper::GetBSTR(IJsonValue * json, BSTR name, BSTR * pVal)
{
	INVALID_NULL(pVal);

	IJsonValue * child= NULL;
	HRESULT hr = json->GetByName(name,&child);
	if(FAILED(hr)) return hr;

	hr = child->AsString(pVal);

	child->Release();
	return hr;
}

HRESULT CDataProfileHelper::IsMetaJson(IJsonValue * json)
{
	BSTR type ;
	if(FAILED(GetBSTR(json,SZ_NODETYPE,&type))) return FALSE;
	bool b =  wcscmp(type , SZ_META) == 0;

	SysFreeString(type);
	return b;
}


HRESULT CDataProfileHelper::CopyProperties(IJsonValue * json, IDataNode * node, IDataParser * parser)
{
	COPY_STRING_JSON2NODE(SZ_CAPTION, Caption,json ,node);
	COPY_STRING_JSON2NODE(SZ_DESCRIPTION, Description,json ,node);

	COPY_BASE_JSON2NODE(SZ_VISIBLE, Visible, json, node, VARIANT_BOOL);

	IDataMeta * meta = NULL;

	if(IsMetaJson(json) && SUCCEEDED(node->QueryInterface(&meta)))
	{	
		do
		{
			BSTR str = NULL; 
			if(SUCCEEDED(GetBSTR(json, SZ_DATATYPE, &str)))
			{
				MetaDataType type= CMetaDataTypeMap::Instance.GetType( BSTR2STRING(str));
				meta->put_DataType(type);
			}

		}while(0);


		COPY_STRING_JSON2NODE(SZ_REGEX, Regex ,json ,meta);
		COPY_STRING_JSON2NODE(SZ_TEXT, Text ,json ,meta);
		COPY_STRING_JSON2NODE(SZ_DEFAULT, Default ,json ,meta);	

		// 复制编辑器

		IJsonValue * jsonEditor = NULL;
		BSTR editorType = NULL;		

		if(SUCCEEDED(json->GetByName(SZ_EDITOR, &jsonEditor))
			&& SUCCEEDED(GetBSTR(jsonEditor ,SZ_EDITORTYPE, &editorType)))
		{
			IDataMetaTextEditor * textEditor = NULL;
			IDataMetaSelectEditor * selectEditor = NULL;
			IDataMetaVectorEditor * vectorEditor = NULL;

			if( wcscmp(editorType, SZ_TEXTEDITOR) == 0
				&& SUCCEEDED(parser->CreateTextEditor(&textEditor))				
				&& SUCCEEDED(meta->put_Editor(textEditor)))
			{	
				COPY_BASE_JSON2NODE(SZ_EDITORSTYLE, Style ,jsonEditor ,textEditor, TextEditorStyle);
				COPY_BASE_JSON2NODE(SZ_EDITORDIGITS, Digits ,jsonEditor ,textEditor, LONG);
				COPY_BASE_JSON2NODE(SZ_EDITORSTEP, Step ,jsonEditor ,textEditor, LONG);
				COPY_STRING_JSON2NODE(SZ_EDITORPROFILE, SubProfile ,jsonEditor ,textEditor);
			}

			else if( wcscmp(editorType, SZ_SELECTEDITOR) == 0
				&& SUCCEEDED(parser->CreateSelectEditor(&selectEditor))			
				&& SUCCEEDED(meta->put_Editor(selectEditor)))
			{									
				COPY_BASE_JSON2NODE(SZ_EDITORSTYLE, Style ,jsonEditor ,selectEditor, SelectEditorStyle);
				COPY_STRING_JSON2NODE(SZ_EDITORLIST, SelectList ,jsonEditor ,selectEditor);
				COPY_STRING_JSON2NODE(SZ_EDITORSEPARATOR, Separator ,jsonEditor ,selectEditor);
				COPY_STRING_JSON2NODE(SZ_EDITORPROFILE, SubProfile ,jsonEditor ,selectEditor);
			}	

			else if( wcscmp(editorType, SZ_VECTOREDITOR) == 0
				&& SUCCEEDED(parser->CreateVectorEditor(&vectorEditor))		
				&& SUCCEEDED(meta->put_Editor(vectorEditor)))
			{
				COPY_BASE_JSON2NODE(SZ_EDITORMAX, MaxLength ,jsonEditor ,vectorEditor, LONG);
				COPY_BASE_JSON2NODE(SZ_EDITORMIN, MinLength ,jsonEditor ,vectorEditor, LONG);
				COPY_STRING_JSON2NODE(SZ_EDITORPROFILE, SubProfile ,jsonEditor ,vectorEditor);
			}

			if( textEditor != NULL) textEditor->Release();
			if( selectEditor != NULL) selectEditor->Release();
			if( vectorEditor != NULL) vectorEditor->Release();
		}

		if( jsonEditor != NULL) jsonEditor->Release();
	}

	if(meta != NULL) meta->Release();

	// 复制属性
	IJsonValue * jsonAttrs = NULL;
	IReadonlyStringList * attrNames = NULL;
	LONG attrsCount = 0;
	IDataAttributes * attrs = NULL;

	if(SUCCEEDED(json->GetByName(SZ_ATTRIBUTES, &jsonAttrs))
		&& SUCCEEDED(jsonAttrs->GetNames(&attrNames))
		&& SUCCEEDED(attrNames->get_Count(&attrsCount))
		&& SUCCEEDED(node->get_Attributes(&attrs)))
	{
		for(LONG i=0;i<attrsCount;i++)
		{			
			BSTR name = NULL;
			IJsonValue * attrValue = NULL;
			BSTR strVal = NULL;
			if(SUCCEEDED(attrNames->GetAt(i,&name))
				&&SUCCEEDED(jsonAttrs->GetByName(name, &attrValue))
				&&SUCCEEDED(attrValue->AsString(&strVal)))
			{				
				attrs->Set(name, strVal);
			}

			if(strVal != NULL) SysFreeString(strVal);
			if(attrValue!= NULL) attrValue->Release();
			if(name!=NULL) SysFreeString(name);
			name = NULL;
		}
	}

	if(jsonAttrs != NULL) jsonAttrs->Release();
	if(attrNames != NULL) attrNames->Release();
	if(attrs != NULL) attrs->Release();

	// 复制脚本

	IJsonValue * jsonScripts = NULL;
	IDataScripts * scripts = NULL;
	if(SUCCEEDED(json->GetByName(SZ_SCRIPTS, &jsonScripts))
		&& SUCCEEDED(node->get_Scripts(&scripts))
		&& scripts != NULL)
	{
		COPY_STRING_JSON2NODE(SZ_SCRIPTSTYPE, Type, jsonScripts, scripts);
		COPY_STRING_JSON2NODE(SZ_SCRIPTSRECEIVED, ReceivedAction, jsonScripts, scripts);
		COPY_STRING_JSON2NODE(SZ_SCRIPTSSENDING, SendingAction, jsonScripts, scripts);
		COPY_STRING_JSON2NODE(SZ_SCRIPTSSENDED, SendedAction, jsonScripts, scripts);
		COPY_STRING_JSON2NODE(SZ_SCRIPTSCHANGING, NodeChangingAction, jsonScripts, scripts);
		COPY_STRING_JSON2NODE(SZ_SCRIPTSCHANGED, NodeChangingAction, jsonScripts, scripts);
	}

	if(jsonScripts != NULL) jsonScripts->Release();
	if(scripts != NULL) scripts->Release();

	return S_OK;
}

HRESULT CDataProfileHelper::ParseJson(IJsonValue * json, IDataNode * parent, IDataParser * parser)
{
	if(json== NULL) return S_FALSE;

	CopyProperties(json,parent, parser);

	if(IsMetaJson(json))
		return S_OK;

	HRESULT hr = S_OK;

	IDataChildren * dataChildren = NULL;
	IJsonValue * jsonChildren = NULL;
	IReadonlyStringList *nameList = NULL;

	if(FAILED( hr = parent->get_Children(&dataChildren)))
		goto free_all;

	if(FAILED(hr = json->GetByName(SZ_CHILDREN,&jsonChildren)))
		goto free_all;

	if(FAILED(hr = jsonChildren->GetNames(&nameList)))
		goto free_all;

	LONG count;
	if(FAILED(hr = nameList->get_Count(&count))) 
		goto free_all;

	for(LONG i=0;i<count;i++)
	{
		BSTR name = NULL;
		IJsonValue * child = NULL;

		do
		{
			if(FAILED(nameList->GetAt(i,&name))) break;		
			if(FAILED(jsonChildren->GetByName(name,&child))) break;

			IDataNode * childNode = NULL;

			if(IsMetaJson(child))
			{
				IUnknown * pUnk = NULL;
				if(FAILED(dataChildren->AddMeta(name, NULL,  &pUnk))) break;

				HRESULT hr2 = pUnk->QueryInterface(&childNode);
				pUnk->Release();

				if(FAILED(hr)) break;
			}
			else
			{
				IUnknown * pUnk = NULL;
				if(FAILED(dataChildren->AddNode(name, NULL,  &pUnk))) break;

				HRESULT hr2 = pUnk->QueryInterface(&childNode);
				pUnk->Release();

				if(FAILED(hr)) break;
			}

			if(childNode!= NULL)
			{
				ParseJson(child, childNode, parser);
			}

		}while(0);

		if(name != NULL) { SysFreeString(name); name = NULL;}
		if(child != NULL) { child->Release() ; child = NULL;}
	}

free_all:

	if( dataChildren != NULL) dataChildren->Release();
	if( nameList != NULL) nameList->Release();
	if( jsonChildren != NULL )jsonChildren->Release();

	return hr ;
}

HRESULT CDataProfileHelper::CopyProperties(IDataNode *node, IJsonValue * parent, IJsonParser * jsonParser,  bool full)
{	
	BSTR str = NULL;

	if(full)
	{	
		COPY_STRING_NODE2JSON(Caption, SZ_CAPTION, node,parent);
		COPY_STRING_NODE2JSON(Description, SZ_DESCRIPTION, node,parent);
		COPY_STRING_NODE2JSON(Name, SZ_NAME, node, parent);

		COPY_BASE_NODE2JSON(Visible, SZ_VISIBLE, node, parent, VARIANT_BOOL);

		// 复制属性
		IDataAttributes * attrs = NULL;
		IJsonValue *jsonAttrs = NULL;
		::IReadonlyStringList * attrNames = NULL;
		LONG attrsCount = 0;
		if(SUCCEEDED(node->get_Attributes(&attrs))
			&& SUCCEEDED(jsonParser->CreateJson(VARIANT_DEFAULT,&jsonAttrs))
			&& SUCCEEDED(attrs->get_Keys(&attrNames))
			&& SUCCEEDED(attrNames->get_Count(&attrsCount))
			&& attrsCount !=0)
		{
			for(LONG i=0;i<attrsCount;i++)
			{
				BSTR attrVal = NULL;
				if(SUCCEEDED(attrNames->GetAt(i,&str))
					&& SUCCEEDED(attrs->Get(str,&attrVal)))
				{
					jsonAttrs->SetByName(str, CComVariant(attrVal));
				}

				if(str!=NULL) SysFreeString(str);
				if(attrVal != NULL) SysFreeString(attrVal);
			}		

			parent->SetByName(SZ_ATTRIBUTES, CComVariant(jsonAttrs));
		}

		if( attrs != NULL) attrs->Release();
		if(jsonAttrs != NULL) jsonAttrs->Release();
		if( attrNames != NULL) attrNames->Release();

		// 复制脚本

		IDataScripts * scripts = NULL;
		IJsonValue * jsonScripts = NULL;
		if(SUCCEEDED(node->get_Scripts(&scripts))
			&& scripts != NULL 
			&& SUCCEEDED(jsonParser->CreateJson(VARIANT_DEFAULT,&jsonScripts)))
		{
			COPY_STRING_NODE2JSON(Type, SZ_SCRIPTSTYPE, scripts,  jsonScripts);
			COPY_STRING_NODE2JSON(ReceivedAction, SZ_SCRIPTSRECEIVED, scripts, jsonScripts);
			COPY_STRING_NODE2JSON(SendingAction, SZ_SCRIPTSSENDING, scripts, jsonScripts);
			COPY_STRING_NODE2JSON(SendedAction, SZ_SCRIPTSSENDED, scripts, jsonScripts);
			COPY_STRING_NODE2JSON(NodeChangingAction, SZ_SCRIPTSCHANGING, scripts, jsonScripts);
			COPY_STRING_NODE2JSON(NodeChangedAction, SZ_SCRIPTSCHANGED, scripts, jsonScripts);

			JsonValueType jsonValueType = JVT_NULL;
			if(SUCCEEDED(jsonScripts->GetValueType(&jsonValueType))
				&& jsonValueType != JVT_NULL)
			{
				parent->SetByName(SZ_SCRIPTS, CComVariant(jsonScripts));
			}
		}

		if(scripts != NULL) scripts->Release();
		if(jsonScripts != NULL) jsonScripts->Release();
	}

	IDataMeta * meta;
	if(SUCCEEDED(node->QueryInterface(&meta)))
	{
		parent->SetByName(SZ_NODETYPE, CComVariant(SZ_META));

		COPY_STRING_NODE2JSON(Text, SZ_TEXT, meta,parent);

		if(full)
		{
			do
			{
				MetaDataType dataType = MDT_NULL; 
				if(SUCCEEDED(meta->get_DataType(&dataType)))
				{
					const String & name = CMetaDataTypeMap::Instance.GetName( dataType );
					parent->SetByName(SZ_DATATYPE, CComVariant( name.c_str()) );
				}

			}while(0);

			COPY_STRING_NODE2JSON(Regex, SZ_REGEX, meta,parent);
			COPY_STRING_NODE2JSON(Default, SZ_DEFAULT, meta,parent);

			// 复制编辑器
			IDataMetaEditor * editor = NULL;
			IJsonValue * jsonEditor = NULL;

			if(SUCCEEDED(meta->get_Editor(&editor))
				&& editor != NULL
				&& SUCCEEDED(jsonParser->CreateJson(VARIANT_DEFAULT, &jsonEditor))
				&& SUCCEEDED(parent->SetByName(SZ_EDITOR, CComVariant(jsonEditor))))
			{
				IDataMetaTextEditor * textEditor = NULL;
				IDataMetaSelectEditor * selectEditor = NULL;
				IDataMetaVectorEditor * vectorEditor = NULL;

				if(SUCCEEDED(editor->QueryInterface(&textEditor))
					&& SUCCEEDED(jsonEditor->SetByName(SZ_EDITORTYPE, CComVariant(SZ_TEXTEDITOR))))
				{
					COPY_BASE_NODE2JSON(Style, SZ_EDITORSTYLE, textEditor,jsonEditor, TextEditorStyle);
					COPY_BASE_NODE2JSON(Digits, SZ_EDITORDIGITS, textEditor,jsonEditor, LONG);
					COPY_BASE_NODE2JSON(Step, SZ_EDITORSTEP, textEditor,jsonEditor, LONG);
					COPY_STRING_NODE2JSON(SubProfile, SZ_EDITORPROFILE, textEditor,jsonEditor);
				}

				else if(SUCCEEDED(editor->QueryInterface(&selectEditor))
					&& SUCCEEDED(jsonEditor->SetByName(SZ_EDITORTYPE, CComVariant(SZ_SELECTEDITOR))))
				{

					COPY_BASE_NODE2JSON(Style, SZ_EDITORSTYLE, selectEditor,jsonEditor, SelectEditorStyle);
					COPY_STRING_NODE2JSON(Separator, SZ_EDITORSEPARATOR, selectEditor,jsonEditor);
					COPY_STRING_NODE2JSON(SelectList, SZ_EDITORLIST, selectEditor,jsonEditor);
					COPY_STRING_NODE2JSON(SubProfile, SZ_EDITORPROFILE, selectEditor,jsonEditor);
				}

				else if(SUCCEEDED(editor->QueryInterface(&vectorEditor))
					&& SUCCEEDED(jsonEditor->SetByName(SZ_EDITORTYPE, CComVariant(SZ_VECTOREDITOR))))
				{
					COPY_BASE_NODE2JSON(MaxLength, SZ_EDITORMAX, vectorEditor,jsonEditor, LONG);
					COPY_BASE_NODE2JSON(MinLength, SZ_EDITORMIN, vectorEditor,jsonEditor, LONG);
					COPY_STRING_NODE2JSON(SubProfile, SZ_EDITORPROFILE, vectorEditor,jsonEditor);				
				}

				if(textEditor != NULL) textEditor->Release();
				if(selectEditor != NULL) selectEditor->Release();
				if(vectorEditor != NULL) vectorEditor->Release();
			}

			if(editor != NULL) editor->Release();
			if(jsonEditor != NULL) jsonEditor->Release();
		}			
	}
	else
	{
		parent->SetByName(SZ_NODETYPE , CComVariant(SZ_NODE));

	}
	return S_OK;
}

HRESULT CDataProfileHelper::ToJson(IDataNode * node, IJsonParser * parser, bool full, IJsonValue ** pJsonValue)
{
	INVALID_NULL(pJsonValue);

	if(node == NULL) return S_FALSE;

	IDataChildren * children=NULL;
	IJsonValue * childrenJson= NULL;	

	if(FAILED(parser->CreateJson(VARIANT_DEFAULT, pJsonValue)))
		goto free_all;

	CopyProperties(node, *pJsonValue, parser, full);

	IDataMeta * meta = NULL;
	if(SUCCEEDED(node->QueryInterface(&meta)))
		goto free_all;

	// 获取Children
	if(FAILED(node->get_Children(&children)))
		goto free_all;

	if(FAILED(parser->CreateJson(VARIANT_DEFAULT, &childrenJson)))
		goto free_all ;

	LONG childrenCount = 0;
	if(FAILED(children->get_Count(&childrenCount)))
		goto free_all;

	if(childrenCount > 0)
	{
		// 循环所有子节点
		for(LONG i=0;i<childrenCount;i++)
		{		
			BSTR name = NULL;
			IDataNode * child = NULL;
			if(SUCCEEDED( children->GetByIndex(i, &child)) && SUCCEEDED(child->get_Name(&name)))
			{
				IJsonValue * childJson = NULL;

				ToJson(child,parser, full, &childJson);				
				if(childJson != NULL)
				{
					childrenJson->SetByName(name,CComVariant( childJson));
					childJson->Release();
				}	
			}

			if(child!= NULL) { child->Release(); child = NULL;}
			if(name!=NULL) { SysFreeString(name); name = NULL;}
		}

		if(FAILED( (*pJsonValue) ->SetByName(SZ_CHILDREN, CComVariant(childrenJson))))
			goto free_all ;	
	}

free_all:

	if( children!=NULL) children->Release();
	if( childrenJson!=NULL) childrenJson->Release();

	return S_OK;
}


HRESULT CDataProfileHelper::ParseJson(BSTR json, BSTR rootName, IDataNode ** pVal)
{
	INVALID_NULL(pVal);
	POINTER_NULL(json);
	//INVALID_DATA_NODE_NAME(rootName);

	*pVal = NULL;
	if(json==NULL) return S_OK;

	HRESULT hr = S_OK;

	IJsonValue * pJsonValue = NULL;
	IUnknown * pUnk = NULL;

	CComQIPtr<IDataParser> dataParser = PROFILE_PARSER();

	if(SUCCEEDED( hr = JSON_PARSER()->Parse(json, &pJsonValue)))
	{
		CComBSTR name;
		GetBSTR(pJsonValue,SZ_NAME, &name);
		if(name == NULL) name = CComBSTR(rootName);

		if(SUCCEEDED( hr = dataParser->CreateNode(name, NULL , &pUnk) )
			&& SUCCEEDED( hr = pUnk->QueryInterface(pVal)))
		{
			hr = ParseJson(pJsonValue, *pVal, dataParser);
		}

	}

	if(pUnk != NULL) pUnk->Release();
	if(pJsonValue != NULL) pJsonValue->Release();

	if(FAILED(hr) && (*pVal) != NULL)
	{
		(*pVal)->Release();
		(*pVal) = NULL;
	}
	return hr;
}