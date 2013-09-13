#ifndef __UTIL__UTIL__H__
#define __UTIL__UTIL__H__

#pragma once

#include "UtilHelper.h"
#include "ComHelper.h"
#include "Nullable.h"
#include "IndexMap.h"
#include "SysEnvHelper.h"
#include "MetaVariant.h"
#include "FileHelper.h"
#include "Log.h"
#include "Lock/Lock.h"
#include "atlrx.h"

#include "PairEntry.h"

#include "PlugIn.h"
#include "PlugInPool.h"

#include "Collection.h"
#include "ArrayBuffer.h"

#include "SimpleProfile.h"
#include "SafeVector.h"
#include "Ring.h"

#include "MetaDataTypeMap.h"
#include "DataSerialize.h"
#include "LayoutGather.h"

#include "ThreadMessage.h"

#include "StringHelper.h"

#include "DataProfile.h"
#include "Json.h"
#include "ComEssence.h"
#include "MonitorCenter.h"

#include "..\ComEssence\StringList.h"
#include "..\ComEssence\ObjectList.h"
#include "..\ComEssence\UnknownList.h"
#include "..\ComEssence\StringDictionary.h"
#include "..\ComEssence\StringSet.h"

#include "..\ComEssence\SinkList.h"

#include "..\MonitorUtil\OrganBuilder.h"
#include "..\MonitorUtil\OrganBuilderList.h"

#include "JsonHelper.h"
#include "DataProfileHelper.h"

#ifdef DEBUG
#pragma comment(lib, "../Debug/Util.lib")
#else
#pragma comment(lib, "../Release/Util.lib")
#endif


#endif