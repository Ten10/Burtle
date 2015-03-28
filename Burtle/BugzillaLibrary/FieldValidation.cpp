#include "stdafx.h"
#include "FieldValidation.h"

namespace Bugzilla
{
	typedef CFieldValidator::CValidationInfo::eFieldType eFieldType;

	CFieldValidatorBuilder::CFieldValidatorBuilder(CFieldValidator& Validator)
		: m_Validator(Validator)
	{
	}

	void CFieldValidatorBuilder::Visit(const char* Name, bool& Value)
	{
		CFieldValidator::CValidationInfo info;
		info.bIsArray = false;
		info.Type = eFieldType::typeBool;
		m_Validator.m_FieldNameToValidationInfo.insert(make_pair(string(Name), info));
	}

	void CFieldValidatorBuilder::Visit(const char* Name, int& Value)
	{
		CFieldValidator::CValidationInfo info;
		info.bIsArray = false;
		info.Type = eFieldType::typeInt;
		m_Validator.m_FieldNameToValidationInfo.insert(make_pair(string(Name), info));
	}

	void CFieldValidatorBuilder::Visit(const char* Name, double& Value)
	{
		CFieldValidator::CValidationInfo info;
		info.bIsArray = false;
		info.Type = eFieldType::typeDouble;
		m_Validator.m_FieldNameToValidationInfo.insert(make_pair(string(Name), info));
	}

	void CFieldValidatorBuilder::Visit(const char* Name, string& Value)
	{
		CFieldValidator::CValidationInfo info;
		info.bIsArray = false;
		info.Type = eFieldType::typeString;
		m_Validator.m_FieldNameToValidationInfo.insert(make_pair(string(Name), info));
	}

	void CFieldValidatorBuilder::Visit(const char* Name, tm& Value)
	{
		CFieldValidator::CValidationInfo info;
		info.bIsArray = false;
		info.Type = eFieldType::typeTime;
		m_Validator.m_FieldNameToValidationInfo.insert(make_pair(string(Name), info));
	}

	void CFieldValidatorBuilder::Visit(const char* Name, CVisitable& Visitable)
	{
		return;
	}

	void CFieldValidatorBuilder::Visit(const char* Name, CVisitableStruct& Visitable)
	{
		return;
	}

	void CFieldValidatorBuilder::Visit(const char* Name, CVisitableVectorBase& Visitable)
	{
		return;
	}
};