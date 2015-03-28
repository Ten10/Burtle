#pragma once
#include <list>

struct CFieldNameRetreiveVisitor : public CFieldVisitor
{
	list<CStringA> FieldNames;

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