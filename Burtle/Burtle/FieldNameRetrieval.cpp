#include "stdafx.h"
#include "FieldNameRetrieval.h"

bool CFieldNameRetreiveVisitor::CanVisit(const char* Name, bool bValid) const
{
	return true;
}

void CFieldNameRetreiveVisitor::Visit(const char* Name, bool& Value)
{
	FieldNames.push_back(Name);
}

void CFieldNameRetreiveVisitor::Visit(const char* Name, int& Value)
{
	FieldNames.push_back(Name);
}

void CFieldNameRetreiveVisitor::Visit(const char* Name, double& Value)
{
	FieldNames.push_back(Name);
}

void CFieldNameRetreiveVisitor::Visit(const char* Name, stringA& Value)
{
	FieldNames.push_back(Name);
}

void CFieldNameRetreiveVisitor::Visit(const char* Name, tm& Value)
{
	FieldNames.push_back(Name);
}

void CFieldNameRetreiveVisitor::Visit(const char* Name, CVisitable& Visitable)
{
	FieldNames.push_back(Name);
}

void CFieldNameRetreiveVisitor::Visit(const char* Name, CVisitableStruct& Visitable)
{
	FieldNames.push_back(Name);
}

void CFieldNameRetreiveVisitor::Visit(const char* Name, CVisitableVectorBase& Visitable)
{
	FieldNames.push_back(Name);
}