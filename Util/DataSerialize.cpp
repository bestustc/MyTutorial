#include "DataSerialize.h"
#include "ComHelper.h"

CDataSerialize::CDataSerialize(BOOL write, IDataNode * pNode)
{
	m_bWrite = write;
	m_pNode = pNode ;

	if(m_pNode == NULL)
	{
		IUnknown * pUnk = NULL;

		HRESULT hr = PROFILE_PARSER()->CreateNode(VARIANT_BSTR("Root"), NULL , &pUnk);
		if(FAILED(hr)) return;

		pUnk->QueryInterface(&m_pNode);
		pUnk->Release();
	}
	else
	{
		pNode->AddRef();
	}
}

CDataSerialize::~CDataSerialize()
{
	if(m_pNode != NULL)
	{
		m_pNode ->Release();
		m_pNode = NULL;
	}
}


HRESULT CDataSerialize::GetNode(IDataNode ** pNode)
{
	INVALID_NULL(pNode);

	*pNode = m_pNode;
	if(m_pNode != NULL)
	{
		m_pNode->AddRef();
	}

	return S_OK;
}


HRESULT CDataSerialize::_Write(LPCTSTR name, const CMetaVariant & value, IDataChildren * pChildren)
{
	BOOL hasChildren = pChildren != NULL;

	if(!hasChildren)
	{	
		HRESULT hr = m_pNode->get_Children(&pChildren);
		if(FAILED(hr)) return hr;
	}

	IUnknown * pUnk = NULL;	
	HRESULT hr = pChildren->AddMeta( VARIANT_BSTR(name), NULL, &pUnk);

	if(!hasChildren)
	{
		pChildren->Release();
	}
	if(FAILED(hr)) return hr;

	IDataMeta * pMeta = NULL;
	hr = pUnk->QueryInterface(&pMeta);
	pUnk->Release();

	if(FAILED(hr)) return hr;

	pMeta->put_Text( VARIANT_BSTR(value.ToString().c_str()) );
	pMeta->put_DataType(value.type);

	pMeta->Release();

	return S_OK;
}


HRESULT CDataSerialize::_Write(LPCTSTR name, IDataSerializable * p, IDataChildren * pChildren )
{
	BOOL hasChildren = pChildren != NULL;

	if(!hasChildren)
	{
		HRESULT hr = m_pNode->get_Children(&pChildren);
		if(FAILED(hr)) return hr;
	}

	IUnknown * pUnk = NULL;	
	HRESULT hr = pChildren->AddNode(VARIANT_BSTR(name), NULL, &pUnk);	

	if(!hasChildren)
	{
		pChildren->Release();
	}

	if(FAILED(hr)) return hr;

	IDataNode * pSubNode = NULL;
	hr = pUnk->QueryInterface(&pSubNode);
	pUnk->Release();

	if(FAILED(hr)) return  hr;

	if(p != NULL)
	{
		CDataSerialize subSerialize = CDataSerialize(TRUE, pSubNode) ;

		hr =  p->Write(subSerialize);
		if(hr == E_NOTIMPL) hr = p->Serialize(subSerialize);
	}

	pSubNode->Release();

	return hr;
}


HRESULT CDataSerialize::_NewSubNode(LPCTSTR name, IDataChildren ** pChildren)
{
	INVALID_NULL(pChildren);

	IDataChildren * children = NULL;
	HRESULT hr = m_pNode->get_Children(&children);
	if(FAILED(hr)) return hr;

	IUnknown * pUnk = NULL;
	hr = children->AddNode( VARIANT_BSTR(name), NULL, &pUnk);
	children->Release();
	if(FAILED(hr)) return hr;

	IDataNode * pNode = NULL;

	hr = pUnk->QueryInterface(&pNode);
	pUnk->Release();

	if(FAILED(hr)) return hr;

	return pNode->get_Children(pChildren);
}


HRESULT CDataSerialize::Write(LPCTSTR name, int count ,const BYTE arr[])
{
	return _Write(name, CMetaVariant(count, arr), NULL);
}

HRESULT CDataSerialize::_Read(LPCTSTR name, MetaDataType dataType, CMetaVariant & value, IDataChildren * pChildren)
{
	BOOL hasChildren = pChildren != NULL;

	if(!hasChildren)
	{
		HRESULT hr = m_pNode->get_Children(&pChildren);
		if(FAILED(hr)) return hr;
	}

	IDataNode * pNode = NULL;
	HRESULT hr = pChildren->GetByName( VARIANT_BSTR(name), &pNode);

	if(!hasChildren)
	{
		pChildren->Release();
	}

	if(FAILED(hr)) return hr;

	IDataMeta * pMeta = NULL;
	hr = pNode->QueryInterface(&pMeta);
	pNode->Release();

	if(FAILED(hr)) return hr;

	BSTR text = NULL;
	hr = pMeta->get_Text(&text);
	pMeta->Release();

	if(FAILED(hr)) return hr;	

	hr = CMetaVariant::Parse(dataType, text, value);
	FREE_BSTR(text);

	return hr;
}


HRESULT CDataSerialize::_Read(LPCTSTR name, IDataSerializable * p, IDataChildren * pChildren)
{
	BOOL hasChildren = pChildren != NULL;

	if(!hasChildren)
	{
		HRESULT hr = m_pNode->get_Children(&pChildren);
		if(FAILED(hr)) return hr;
	}

	IDataNode * pSubNode = NULL;
	HRESULT hr = pChildren->GetByName(VARIANT_BSTR(name), &pSubNode);	

	if(!hasChildren)
	{
		pChildren->Release();
	}

	if(FAILED(hr)) return hr;

	if(p != NULL) 
	{
		CDataSerialize subSerialize = CDataSerialize(FALSE, pSubNode);
		hr = p->Read(subSerialize);
		if(hr == E_NOTIMPL) hr = p->Serialize(subSerialize);
	}
	pSubNode->Release();

	return hr;
}

HRESULT CDataSerialize::_GetSubNode(LPCTSTR name, IDataChildren ** pChildren)
{
	INVALID_NULL(pChildren);

	IDataChildren * children = NULL;
	HRESULT hr = m_pNode->get_Children(&children);

	if(FAILED(hr)) return hr;

	IDataNode * node = NULL;
	hr = children->GetByName( VARIANT_BSTR(name), &node);
	children->Release();

	if(FAILED(hr)) return hr;

	return node->get_Children(pChildren);

}
