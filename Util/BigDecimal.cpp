#include "BigDecimal.h"
#include "StringHelper.h"


static TCHAR * __ExtendValue(const TCHAR * src, size_t len, size_t decCnt, size_t expectDecCnt)
{
	size_t newLen = len;

	if(decCnt < expectDecCnt)
	{
		newLen += expectDecCnt - decCnt;		
	}	

	TCHAR * value = new TCHAR[newLen+1];
	memset(value, 0, sizeof(TCHAR)*(newLen + 1) );

	TCHAR * tempValue = value;

	for(size_t i=0;i<len;++i)
	{
		if(src[i] !=TDOT) *tempValue++ = src[i];
	}		

	for(size_t i = decCnt ; i< expectDecCnt; ++i)
	{
		*tempValue ++ = TNUMBER0;
	}

	return value;
}

static TCHAR * __FrapValue(const TCHAR * src, size_t len, size_t decCnt, size_t expectDecCnt)
{
	size_t newLen = len;

	if(decCnt > expectDecCnt)
	{
		newLen -= decCnt - expectDecCnt;
	}

	TCHAR * value = new TCHAR[newLen+2];
	memset(value, 0, sizeof(TCHAR)*(newLen + 2) );

	TCHAR * tempValue = value;

	for(size_t i=0;i<newLen;++i)
	{
		if(i == len - decCnt ) *tempValue ++ = TDOT;
		*tempValue++ = src[i];
	}

	return value;
}

static void __FormatValue(TCHAR value[], size_t len)
{
	int dotIndex = CStringHelper::IndexOf(value,TDOT);
	if(dotIndex == -1) return;

	for(size_t i = len -1; i>= (size_t)dotIndex; --i)
	{
		if(value[i] == TZERO || value[i] == TNUMBER0 || value[i] == TDOT)
			value[i] = TZERO;
		else 
			break;
	}
}

TCHAR * DecimalAdd(const TCHAR * src0, const TCHAR * src1, int percison)
{
	int dotIndex0 = CStringHelper::IndexOf(src0, TDOT);
	int dotIndex1 = CStringHelper::IndexOf(src1, TDOT);

	size_t len0 = STRLEN(src0);
	size_t len1 = STRLEN(src1);

	size_t decCnt0 = dotIndex0 == -1? 0 : len0 - dotIndex0 -1;
	size_t decCnt1 = dotIndex1 == -1? 0 : len1 - dotIndex1 -1;

	size_t maxDecCount = max(decCnt0, decCnt1);

	TCHAR * value0 = __ExtendValue(src0, len0, decCnt0, maxDecCount);
	TCHAR * value1 = __ExtendValue(src1, len1, decCnt1, maxDecCount);

	CBigInteger i0 = value0;
	CBigInteger i1 = value1;

	i0 += i1;

	delete [] value0;
	delete [] value1;

	const TCHAR * tempValue = i0.ToString();

	size_t length = STRLEN(tempValue);
	if(maxDecCount !=0) length +=1;

	TCHAR * value = new TCHAR[length+1];
	memset(value,0, sizeof(TCHAR) * (length+1));

	for(size_t i=0;i<length;++i)
	{
		if(i< length - maxDecCount-1) value[i] = tempValue[i];		
		else value[i+1] = tempValue[i];
	}

	value[length -maxDecCount -1] = TDOT;

	__FormatValue(value, length);

	return value;

}

TCHAR * DecimalMulti(const TCHAR * src0, const TCHAR * src1, int percison)
{
	int dotIndex0 = CStringHelper::IndexOf(src0, TDOT);
	int dotIndex1 = CStringHelper::IndexOf(src1, TDOT);

	size_t len0 = STRLEN(src0);
	size_t len1 = STRLEN(src1);

	size_t decCnt0 = dotIndex0 == -1? 0 : len0 - dotIndex0 -1;
	size_t decCnt1 = dotIndex1 == -1? 0 : len1 - dotIndex1 -1;	

	TCHAR * value0 = __ExtendValue(src0, len0, decCnt0, decCnt0);
	TCHAR * value1 = __ExtendValue(src1, len1, decCnt1, decCnt1);


	CBigInteger i0 = value0;
	CBigInteger i1 = value1;

	i0.Multi(i1);

	TCHAR * value = __FrapValue(i0.ToString(),i0.Length(), decCnt0 + decCnt1, percison);

	delete [] value0;
	delete [] value1;

	__FormatValue(value, STRLEN(value));

	return value;
}

TCHAR * DecimalDiv(const TCHAR * src0, const TCHAR * src1, int percison)
{
	int dotIndex0 = CStringHelper::IndexOf(src0, TDOT);
	int dotIndex1 = CStringHelper::IndexOf(src1, TDOT);

	size_t len0 = STRLEN(src0);
	size_t len1 = STRLEN(src1);

	size_t decCnt0 = dotIndex0 == -1? 0 : len0 - dotIndex0 -1;
	size_t decCnt1 = dotIndex1 == -1? 0 : len1 - dotIndex1 -1;	

	TCHAR * value0 = __ExtendValue(src0, len0, decCnt0, decCnt0 + decCnt1 + percison );
	TCHAR * value1 = __ExtendValue(src1, len1, decCnt1, decCnt1);


	CBigInteger i0 = value0;
	CBigInteger i1 = value1;

	i0.Div(i1);

	TCHAR * value = __FrapValue(i0.ToString(),i0.Length(), decCnt0 + percison , percison);

	delete [] value0;
	delete [] value1;

	__FormatValue(value, STRLEN(value));

	return value;
}

int DecimalCompare(const TCHAR * src0, const TCHAR * src1, int percison)
{
	if(src0[0] == TNEGATIVE && src1[0] != TNEGATIVE) return -1;
	if(src0[0] != TNEGATIVE && src1[0] == TNEGATIVE) return 1;

	size_t len0 = STRLEN(src0);
	size_t len1 = STRLEN(src1);

	TCHAR * value0 = new TCHAR[len0 + 1];
	TCHAR * value1 = new TCHAR[len1 + 1];

	size_t dotIndex0 = 0;
	while(*src0 != TZERO)
	{
		*value0 ++ = *src0;
		if(*src0 == TDOT) dotIndex0 = 0;
		src0++;
	}

	size_t dotIndex1 = 0;
	while(*src1 != TZERO)
	{
		*value1 ++ = *src1;
		if(*src1 == TDOT) dotIndex1 = 0;
		src1++;
	}

	int cmp = 0;

	value0[dotIndex0] = TZERO;
	value1[dotIndex1] = TZERO;

	cmp = PositiveIntegerCompare(value0, value1);
	if(cmp == 0)
	{
		cmp =_tccmp(value0 + dotIndex0+1, value1 + dotIndex1 +1);
	}

	delete [] value0;
	delete [] value1;

	if(src0[0] == TNEGATIVE && src1[0] == TNEGATIVE) 
	{
		cmp = -cmp;
	}

	return cmp;

}