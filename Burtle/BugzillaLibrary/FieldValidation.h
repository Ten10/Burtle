#pragma once
#include "Visitation.h"

namespace Bugzilla
{
	class CFieldValidator
	{
	public:
		struct CValidationInfo
		{
			enum class eFieldType
			{
				typeString,
				typeInt,
				typeBool,
				typeDouble,
				typeTime
			};
			bool bIsArray;
			eFieldType Type;
		};

	protected:
		map<string, CValidationInfo> m_FieldNameToValidationInfo;
	public:
		bool Validate(const string& Value) const;

		friend class CFieldValidatorBuilder;
	};

	class CFieldValidatorBuilder : public CFieldVisitor
	{
		CFieldValidator m_Validator;
	public:
		CFieldValidatorBuilder(CFieldValidator& Validator);

		bool CanVisit(const char* Name, bool bValid = true) const override { return bValid; }

		void Visit(const char* Name, bool& Value) override;
		void Visit(const char* Name, int& Value) override;
		void Visit(const char* Name, double& Value) override;
		void Visit(const char* Name, string& Value) override;
		void Visit(const char* Name, tm& Value) override;

		void Visit(const char* Name, CVisitable& Visitable) override;
		void Visit(const char* Name, CVisitableStruct& Visitable) override;
		void Visit(const char* Name, CVisitableVectorBase& Visitable) override;
	};
}