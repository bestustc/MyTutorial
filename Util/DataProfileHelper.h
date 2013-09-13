#ifndef __UTIL__DATA_PROFILE_HELPER__H__
#define __UTIL__DATA_PROFILE_HELPER__H__

#pragma once

#include "UtilHelper.h"
#include "DataProfile.h"
#include "Json.h"
#include "MetaDataTypeMap.h"

#define SZ_CAPTION VARIANT_BSTR("caption")
#define SZ_DATATYPE VARIANT_BSTR("datatype")
#define SZ_NODETYPE VARIANT_BSTR("nodetype")
#define SZ_NAME VARIANT_BSTR("name")
#define SZ_REGEX VARIANT_BSTR("regex")
#define SZ_DEFAULT VARIANT_BSTR("default")

#define SZ_DESCRIPTION VARIANT_BSTR("description")
#define SZ_TEXT VARIANT_BSTR("text")
#define SZ_CHILDREN VARIANT_BSTR("children")
#define SZ_ATTRIBUTES VARIANT_BSTR("attributes")

#define SZ_EDITOR VARIANT_BSTR("editor")
#define SZ_EDITORTYPE VARIANT_BSTR("type")

#define SZ_TEXTEDITOR VARIANT_BSTR("text")
#define SZ_SELECTEDITOR VARIANT_BSTR("select")
#define SZ_VECTOREDITOR VARIANT_BSTR("vector")


#define SZ_EDITORSTYLE VARIANT_BSTR("style")
#define SZ_EDITORLIST VARIANT_BSTR("list")
#define SZ_EDITORSEPARATOR VARIANT_BSTR("separator")

#define SZ_EDITORDIGITS VARIANT_BSTR("digits")
#define SZ_EDITORSTEP VARIANT_BSTR("step")


#define SZ_EDITORPROFILE VARIANT_BSTR("profile")
#define SZ_EDITORMAX VARIANT_BSTR("max")
#define SZ_EDITORMIN VARIANT_BSTR("min")

#define SZ_SCRIPTS VARIANT_BSTR("scripts")
#define SZ_SCRIPTSTYPE VARIANT_BSTR("type")

#define SZ_SCRIPTSRECEIVED VARIANT_BSTR("received")

#define SZ_SCRIPTSSENDING VARIANT_BSTR("sending")
#define SZ_SCRIPTSSENDED VARIANT_BSTR("sended")

#define SZ_SCRIPTSCHANGING VARIANT_BSTR("changing")
#define SZ_SCRIPTSCHANGED VARIANT_BSTR("changed")

#define SZ_NODE VARIANT_BSTR("node")
#define SZ_META VARIANT_BSTR("meta")

#define SZ_VISIBLE VARIANT_BSTR("visible")
#define SZ_TRUE VARIANT_BSTR("true")
#define SZ_FALSE VARIANT_BSTR("false")

#define INVALID_DATA_NODE_NAME INVALID_VAR_NAME
#define INVALID_DATA_META_NAME INVALID_DATA_NODE_NAME

class CDataProfileHelper
{
public:

	static HRESULT Clone(IDataNode * srcNode, bool full, IDataNode * destNode, IDataParser * parser);

	static HRESULT CopyAttributes(IDataAttributes * srcAttrs, IDataAttributes * destAttrs);

	static HRESULT CopyAttributes(IDataNode * srcNode, IDataNode * destNode);

	static HRESULT MergeNode(IDataNode * fullNode, IDataNode * valuesNode);

	static HRESULT GetLong(IJsonValue * json, BSTR name, LONG * pVal);

	static HRESULT GetBSTR(IJsonValue * json, BSTR name, BSTR * pVal);

	static HRESULT IsMetaJson(IJsonValue * json);

	static HRESULT CopyProperties(IJsonValue * json, IDataNode * node, IDataParser * parser);

	static HRESULT ParseJson(IJsonValue * json, IDataNode * parent, IDataParser * parser);
	
	static HRESULT ParseJson(BSTR json, BSTR rootName, IDataNode ** pVal);

	static HRESULT CopyProperties(IDataNode *node, IJsonValue * parent, IJsonParser * jsonParser,  bool full);

	static HRESULT ToJson(IDataNode * node, IJsonParser * parser, bool full, IJsonValue ** pJsonValue);


};


#endif