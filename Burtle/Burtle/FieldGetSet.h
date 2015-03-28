#pragma once
#include <vector>

typedef vector<pair<stringA, stringA>> FieldNameAndValueVector;

struct CGetSetFieldsVisitor : public CFieldVisitor
{
	bool bGet;
	FieldNameAndValueVector& FieldNameAndValue;

	CGetSetFieldsVisitor(FieldNameAndValueVector& FieldNameAndValue, bool bGet);

	bool CanVisit(const char* Name, bool bValid = true) const override;

	void Visit(const char* Name, bool& Value) override;
	void Visit(const char* Name, int& Value) override;
	void Visit(const char* Name, double& Value) override;
	void Visit(const char* Name, stringA& Value) override;
	void Visit(const char* Name, tm& Value) override;

	void Visit(const char* Name, CVisitable& Visitable) override;
	void Visit(const char* Name, CVisitableStruct& Visitable) override;
	void Visit(const char* Name, CVisitableVectorBase& Visitable) override;
};