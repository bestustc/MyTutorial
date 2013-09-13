#include "SimpleProfile.h"
#include "UtilHelper.h"
#include "ComHelper.h"

namespace SimpleProfile
{
#define COPY_STRING_DATA2CONFIG(propertyName,srcNode, var)\
	do\
	{\
	BSTR __str = NULL;\
	if(SUCCEEDED(srcNode->get_##propertyName(&__str))\
	&& __str != NULL)\
	{\
	var = __str;\
	SysFreeString(__str);\
}\
}while(0);

#define COPY_BASE_DATA2CONFIG(propertyName,srcNode, var, type)\
	do\
	{\
	type __val = type();\
	if(SUCCEEDED(srcNode->get_##propertyName(&__val)))\
	{\
	var = (int)__val;\
}\
}while(0);

#define COPY_STRING_CONFIG2NODE(var, propertyName, destNode)\
	do\
	{\
	BSTR __str = SysAllocString(var.c_str());\
	destNode->put_##propertyName(__str);\
	SysFreeString(__str);\
}while(0);

	template<typename T>
	BOOL CopyBaseToConfigProfileT(IDataNode * srcNode,T & destNode)
	{
		// 复制属性
		IDataAttributes * attrs = NULL;
		IReadonlyStringList * attrNames = NULL;
		LONG attrCount;
		if(SUCCEEDED(srcNode->get_Attributes(&attrs))
			&& SUCCEEDED(attrs->get_Keys(&attrNames))
			&& SUCCEEDED(attrNames->get_Count(&attrCount))
			&& attrCount != 0)
		{
			for(LONG i = 0 ;i<attrCount;i++)
			{
				BSTR name = NULL;
				BSTR val = NULL;
				if(SUCCEEDED(attrNames->GetAt(i, &name))
					&& name != NULL
					&& SUCCEEDED(attrs->Get(name, &val))
					&& val != NULL)
				{
					destNode.attrs[name]=val;
				}

				if(name != NULL) SysFreeString(name);
				if(val != NULL) SysFreeString(val);
			}

		}

		if( attrs != NULL) attrs->Release();
		if( attrNames != NULL) attrNames->Release();

		// 复制脚本

		IDataScripts * scripts = NULL;
		if(SUCCEEDED(srcNode->get_Scripts(&scripts)))
		{
			COPY_STRING_DATA2CONFIG(NodeChangedAction,scripts, destNode.scripts.nodeChangedAction);
			COPY_STRING_DATA2CONFIG(NodeChangingAction,scripts, destNode.scripts.nodeChangingAction);

			COPY_STRING_DATA2CONFIG(ReceivedAction,scripts, destNode.scripts.receivedAction);

			COPY_STRING_DATA2CONFIG(SendedAction,scripts, destNode.scripts.sendedAction);
			COPY_STRING_DATA2CONFIG(SendingAction,scripts, destNode.scripts.sendingAction);
			COPY_STRING_DATA2CONFIG(Type,scripts, destNode.scripts.type);

		}

		// 复制基础量
		COPY_STRING_DATA2CONFIG(Name, srcNode, destNode.name);
		COPY_STRING_DATA2CONFIG(Caption, srcNode, destNode.caption);
		COPY_STRING_DATA2CONFIG(Description, srcNode, destNode.description);

		return TRUE;
	}

	BOOL CopyMeta(IDataMeta * srcMeta, SimpleProfile::DataMeta & destMeta)
	{
		CopyBaseToConfigProfileT(srcMeta, destMeta);

		COPY_STRING_DATA2CONFIG(Text, srcMeta, destMeta.text);
		COPY_STRING_DATA2CONFIG(Regex, srcMeta, destMeta.regex);
		COPY_STRING_DATA2CONFIG(Default, srcMeta, destMeta.defaultText);

		MetaDataType type;
		if(SUCCEEDED(srcMeta->get_DataType(&type)))
		{
			destMeta.dataType = type;
		}

		// 复制编辑器

		IDataMetaEditor * editor = NULL;
		if(SUCCEEDED(srcMeta->get_Editor(&editor)))
		{
			IDataMetaTextEditor * textEditor = NULL;
			IDataMetaSelectEditor * selectEditor = NULL;
			IDataMetaVectorEditor * vectorEditor = NULL;

			if(SUCCEEDED(editor->QueryInterface(&textEditor)))
			{
				COPY_BASE_DATA2CONFIG(Digits,textEditor, destMeta.editor.textDigits, LONG);
				COPY_BASE_DATA2CONFIG(Step,textEditor, destMeta.editor.textStep, LONG);
				COPY_BASE_DATA2CONFIG(Style,textEditor, destMeta.editor.editorStyle, TextEditorStyle);			
				COPY_STRING_DATA2CONFIG(SubProfile,textEditor, destMeta.editor.subProfile);

				destMeta.editor.type = SimpleProfile::MetaEditor::TEXT;
			}
			else if(SUCCEEDED(editor->QueryInterface(&selectEditor)))
			{
				COPY_STRING_DATA2CONFIG(SelectList,selectEditor, destMeta.editor.selectList);
				COPY_STRING_DATA2CONFIG(Separator,selectEditor, destMeta.editor.selectSeparator);
				COPY_STRING_DATA2CONFIG(SubProfile,selectEditor, destMeta.editor.subProfile);
				COPY_BASE_DATA2CONFIG(Style,selectEditor, destMeta.editor.editorStyle, SelectEditorStyle);			

				destMeta.editor.type = SimpleProfile::MetaEditor::SELECT;
			}
			else if(SUCCEEDED(editor->QueryInterface(&vectorEditor)))
			{
				COPY_STRING_DATA2CONFIG(SubProfile, vectorEditor, destMeta.editor.subProfile);
				COPY_BASE_DATA2CONFIG(MaxLength, vectorEditor, destMeta.editor.vecMaxLength, LONG);
				COPY_BASE_DATA2CONFIG(MinLength, vectorEditor, destMeta.editor.vecMinLength, LONG);

				destMeta.editor.type = SimpleProfile::MetaEditor::VECTOR;
			}

			if(textEditor != NULL) textEditor->Release();
			if(selectEditor != NULL) selectEditor->Release();
			if(vectorEditor != NULL) vectorEditor->Release();
		}
		if(editor != NULL) editor->Release();

		return S_OK;

	}

	BOOL Convert(IDataNode * srcNode, SimpleProfile::DataNode & destNode)
	{
		CopyBaseToConfigProfileT(srcNode, destNode);

		// 复制子元素	

		IDataChildren * children = NULL;
		IReadonlyStringList * names = NULL;
		LONG childrenCount = 0;
		if(SUCCEEDED(srcNode->get_Children(&children))
			&& SUCCEEDED(children->get_Names(&names))
			&& SUCCEEDED(names->get_Count(&childrenCount)))
		{
			for(LONG i = 0; i<childrenCount;i++)
			{
				BSTR name = NULL;
				IDataNode * child = NULL;
				IDataMeta * meta = NULL;
				if(SUCCEEDED(names->GetAt(i, &name))
					&& SUCCEEDED(children->GetByName(name, &child))
					&& child != NULL)
				{
					if(SUCCEEDED(child->QueryInterface(&meta)))
					{
						SimpleProfile::DataMeta dataMeta;
						CopyMeta(meta, dataMeta);

						destNode.subMetas.push_back(dataMeta);
					}
					else
					{
						SimpleProfile::DataNode node;
						SimpleProfile::Convert(child, node);
						destNode.subNodes.push_back(node);
					}
				}
			}
		}

		return TRUE;
	}

	template<typename T>
	HRESULT CopyBaseToDataProfileT(T & srcNode, IDataNode * destNode)
	{
		// 复制属性
		IDataAttributes * attrs = NULL;
		if(SUCCEEDED(destNode->get_Attributes(&attrs))
			&& attrs != NULL)
		{
			for(map<String,String>::iterator it = srcNode.attrs.begin() , end = srcNode.attrs.end();
				it != end ;it ++)
			{
				BSTR key = SysAllocString( it->first.c_str());
				BSTR val = SysAllocString( it->second.c_str());

				attrs->Set(key, val);

				SysFreeString(key);
				SysFreeString(val);
			}
		}

		if(attrs != NULL) attrs->Release();

		// 复制脚本
		IDataScripts * scripts = NULL;
		if(SUCCEEDED(destNode->QueryInterface(&scripts))
			&& scripts != NULL)
		{
			COPY_STRING_CONFIG2NODE(srcNode.scripts.type, Type,scripts);

			COPY_STRING_CONFIG2NODE(srcNode.scripts.nodeChangedAction, NodeChangedAction,scripts);
			COPY_STRING_CONFIG2NODE(srcNode.scripts.nodeChangingAction, NodeChangingAction,scripts);

			COPY_STRING_CONFIG2NODE(srcNode.scripts.receivedAction, ReceivedAction,scripts);

			COPY_STRING_CONFIG2NODE(srcNode.scripts.sendedAction, SendedAction,scripts);
			COPY_STRING_CONFIG2NODE(srcNode.scripts.sendingAction, SendingAction,scripts);

		}

		// 复制基本

		//COPY_STRING_CONFIG2NODE(srcNode.name, Name,destNode);
		COPY_STRING_CONFIG2NODE(srcNode.caption, Caption,destNode);
		COPY_STRING_CONFIG2NODE(srcNode.description, Description,destNode);

		return S_OK;
	}

	HRESULT CopyMeta(SimpleProfile::DataMeta & srcMeta, IDataParser * parser, IDataMeta* destMeta)
	{
		CopyBaseToDataProfileT(srcMeta,destMeta);


		COPY_STRING_CONFIG2NODE(srcMeta.text, Text, destMeta);
		COPY_STRING_CONFIG2NODE(srcMeta.regex, Regex ,destMeta);
		COPY_STRING_CONFIG2NODE(srcMeta.defaultText,  Default,destMeta);

		destMeta->put_DataType((MetaDataType)srcMeta.dataType);

		// 复制编辑器
		switch(srcMeta.editor.type)
		{
		case SimpleProfile::MetaEditor::TEXT:
			{
				IDataMetaTextEditor * editor = NULL;
				if(SUCCEEDED(parser->CreateTextEditor(&editor))
					&& SUCCEEDED(destMeta->put_Editor(editor)))
				{
					editor->put_Style((TextEditorStyle) srcMeta.editor.editorStyle);
					editor->put_Digits(srcMeta.editor.textDigits);
					editor->put_Step(srcMeta.editor.textStep);

					COPY_STRING_CONFIG2NODE(srcMeta.editor.subProfile, SubProfile, editor);

				}
				if(editor != NULL) editor->Release();

				break;
			}
		case SimpleProfile::MetaEditor::SELECT:
			{
				IDataMetaSelectEditor * editor = NULL;
				if(SUCCEEDED(parser->CreateSelectEditor(&editor))
					&&SUCCEEDED( destMeta->put_Editor(editor)))
				{
					COPY_STRING_CONFIG2NODE(srcMeta.editor.selectList, SelectList, editor);
					COPY_STRING_CONFIG2NODE(srcMeta.editor.selectSeparator, Separator, editor);
					COPY_STRING_CONFIG2NODE(srcMeta.editor.subProfile, SubProfile, editor);
					editor->put_Style((SelectEditorStyle) srcMeta.editor.editorStyle);

				}
				if(editor != NULL) editor->Release();
				break;
			}
		case SimpleProfile::MetaEditor::VECTOR:
			{
				IDataMetaVectorEditor * editor = NULL;
				if(SUCCEEDED(parser->CreateVectorEditor(&editor))
					&& SUCCEEDED(destMeta->put_Editor(editor)))
				{			
					COPY_STRING_CONFIG2NODE(srcMeta.editor.subProfile, SubProfile, editor);
					editor->put_MaxLength((LONG)srcMeta.editor.editorStyle);
					editor->put_MinLength((LONG) srcMeta.editor.editorStyle);

				}
				if(editor != NULL) editor->Release();
				break;
			}
		}

		return S_OK;
	};


	HRESULT Convert(SimpleProfile::DataNode & srcNode, IDataParser * parser, IDataNode * destNode)
	{
		CopyBaseToDataProfileT(srcNode,destNode);

		IDataChildren * children = NULL;
		if(SUCCEEDED(destNode->get_Children(&children))
			&& children != NULL)
		{
			for(vector<SimpleProfile::DataMeta>::iterator it = srcNode.subMetas.begin() ,end = srcNode.subMetas.end();
				it != end;it++)
			{
				BSTR name = SysAllocString( it->name.c_str());

				IUnknown * pUnk = NULL;				
				IDataMeta * pMeta = NULL;
				if(SUCCEEDED(children->AddMeta(name, NULL, &pUnk))
					&& SUCCEEDED(pUnk->QueryInterface(&pMeta)))
				{
					CopyMeta(*it,parser,pMeta);
				}
				if(pMeta != NULL) pMeta->Release();
				if(pUnk != NULL) pUnk->Release();
				if(name != NULL) SysFreeString(name);
			}


			for(vector<SimpleProfile::DataNode>::iterator it = srcNode.subNodes.begin() ,end = srcNode.subNodes.end();
				it != end;it++)
			{
				BSTR name = SysAllocString(it->name.c_str());

				IUnknown * pUnk = NULL;
				IDataNode * pNode = NULL;
				if(SUCCEEDED(children->AddNode(name, NULL, &pUnk))
					&& SUCCEEDED(pUnk->QueryInterface(&pNode)))
				{
					SimpleProfile::Convert(*it,parser,pNode);
				}
				if(pNode != NULL) pNode->Release();
				if(pUnk != NULL) pUnk->Release();
				if(name != NULL) SysFreeString(name);
			}
		}

		if(children != NULL) children->Release();

		return S_OK;
	}


	IDataNode *  Convert(SimpleProfile::DataNode & srcNode, IDataParser * parser)
	{
		IUnknown * pUnk = NULL;		
		if(FAILED(parser->CreateNode(NULL, NULL, &pUnk))) return NULL;

		IDataNode * pNode = NULL;
		HRESULT hr = pUnk->QueryInterface(&pNode);
		pUnk->Release();

		if(FAILED(hr)) return NULL;

		SimpleProfile::Convert(srcNode,parser,pNode);

		return pNode;
	}

	String ToString(IDataParser * parser, SimpleProfile::DataNode & srcNode, BOOL styled)
	{
		String text;
		IDataNode * pNode = SimpleProfile::Convert(srcNode,parser);
		if(pNode == NULL) return text;

		BSTR json = NULL;
		parser->ToFullJson(pNode, COM_BOOL(styled), &json);

		if(json != NULL)
		{
			text = json;
			SysFreeString(json);
		}

		pNode->Release();

		return text;

	}

	BOOL Parse(IDataParser * parser, LPCWSTR text, SimpleProfile::DataNode & destNode)
	{
		BSTR json = SysAllocString(text);

		if(json==NULL) return FALSE;

		IDataNode * pNode = NULL;
		if(FAILED(parser->ParseJson(json, NULL, &pNode))) return FALSE;

		BOOL b = SimpleProfile::Convert(pNode,destNode);

		pNode->Release();

		return b;
	}


	String ToString(SimpleProfile::DataNode & srcNode,
		BOOL styled)
	{
		return SimpleProfile::ToString(PROFILE_PARSER(), srcNode, styled);
	}

	BOOL Parse(LPCWSTR text, 
		SimpleProfile::DataNode & destNode)
	{
		return SimpleProfile::Parse(PROFILE_PARSER(), text,destNode);
	}

}