#include "ComHelper.h"

CComHelper::CComHelper(void)
{
}

CComHelper::~CComHelper(void)
{
}


HRESULT CComHelper::CreateRemoteInstance(REFCLSID rclsid, 
										 REFIID refIID,
										 void **ppvObject, 
										 LPWSTR hostName ,
										 COAUTHIDENTITY * pAuthEntity )
{
	INVALID_NULL(ppvObject);

	if(hostName == NULL)
	{
		EXPECTED_TRUE( pAuthEntity == NULL);

		return CoCreateInstance(rclsid,
			NULL, 
			CLSCTX_ALL,
			refIID,
			ppvObject);
	}

	COAUTHINFO auth;

	if(pAuthEntity != NULL)
	{
		auth.dwAuthnSvc = RPC_C_AUTHN_WINNT ;
		auth.dwAuthzSvc = RPC_C_AUTHZ_NONE;
		auth.dwAuthnLevel = RPC_C_AUTHN_LEVEL_DEFAULT;
		auth.dwCapabilities =EOAC_NONE;
		auth.dwImpersonationLevel = RPC_C_IMP_LEVEL_IMPERSONATE;
		auth.pwszServerPrincName = hostName;

		auth.pAuthIdentityData=pAuthEntity;		
	}

	COSERVERINFO serverInfo;
	serverInfo.dwReserved1 = 0;
	serverInfo.dwReserved2 = 0;

	serverInfo.pwszName = hostName;
	serverInfo.pAuthInfo = pAuthEntity == NULL ? NULL: &auth;	

	MULTI_QI multiQI;
	multiQI.pIID = &refIID;
	multiQI.pItf = NULL;
	multiQI.hr = S_OK;

	HRESULT hr = CoCreateInstanceEx(rclsid, NULL, CLSCTX_ALL, &serverInfo, 1, &multiQI);

	if(SUCCEEDED(hr) && SUCCEEDED(multiQI.hr))
	{
		*ppvObject = multiQI.pItf;
		return S_OK;
	}
	else if(pAuthEntity != NULL)
	{
		return CreateRemoteInstance(rclsid, refIID, ppvObject, hostName, NULL);
	}
	else if(FAILED(hr))
	{
		return hr;
	}
	else
	{
		return multiQI.hr;
	}
}