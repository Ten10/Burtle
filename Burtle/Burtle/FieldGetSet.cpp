#include "stdafx.h"
#include "FieldGetSet.h"

template <typename T>
static void VisitField(CGetSetFieldsVisitor& Visitor, const char* Name, T& Value)
{
	auto it = find_if(Visitor.FieldNameAndValue.begin(), Visitor.FieldNameAndValue.end(), [=](const pair<string, string>& Pair)
	{
		return Pair.first.find(Name) < Pair.first.length();
	});
	if (it != Visitor.FieldNameAndValue.end())
	{
		if (Visitor.bGet)
		{
			ostringstream stream;
			stream << Value;
			it->second = stream.str();
		}
		else
		{
			stringstream stream(it->second);
			stream >> Value;
		}
	}
}

template <>
static void VisitField(CGetSetFieldsVisitor& Visitor, const char* Name, tm& Value)
{
	auto it = find_if(Visitor.FieldNameAndValue.begin(), Visitor.FieldNameAndValue.end(), [=](const pair<string, string>& Pair)
	{
		return Pair.first.find(Name) < Pair.first.length();
	});
	if (it != Visitor.FieldNameAndValue.end())
	{
		if (Visitor.bGet)
			it->second = CT2A(CTime::ToString(Value));
		else
			Value = CTime::FromString(CA2T(it->second));
	}
}

template <>
static void VisitField(CGetSetFieldsVisitor& Visitor, const char* Name, CStringA& Value)
{
	auto it = find_if(Visitor.FieldNameAndValue.begin(), Visitor.FieldNameAndValue.end(), [=](const pair<string, string>& Pair)
	{
		return Pair.first.find(Name) < Pair.first.length();
	});
	if (it != Visitor.FieldNameAndValue.end())
	{
		if (Visitor.bGet)
			it->second = Value;
		else
			Value = it->second;
	}
}

CGetSetFieldsVisitor::CGetSetFieldsVisitor(vector<pair<string, string>>& FieldNameAndValue, bool bGet)
	: FieldNameAndValue(FieldNameAndValue),
	bGet(bGet)
{
}

bool CGetSetFieldsVisitor::CanVisit(const char* Name, bool bValid) const
{
	return !bGet || bValid;
}

void CGetSetFieldsVisitor::Visit(const char* Name, bool& Value)
{
	VisitField(*this, Name, Value);
}

void CGetSetFieldsVisitor::Visit(const char* Name, int& Value)
{
	VisitField(*this, Name, Value);
}

void CGetSetFieldsVisitor::Visit(const char* Name, double& Value)
{
	VisitField(*this, Name, Value);
}

void CGetSetFieldsVisitor::Visit(const char* Name, string& Value)
{
	VisitField(*this, Name, Value);
}

void CGetSetFieldsVisitor::Visit(const char* Name, tm& Value)
{
	VisitField(*this, Name, Value);
}

void CGetSetFieldsVisitor::Visit(const char* Name, CVisitable& Visitable)
{
}

void CGetSetFieldsVisitor::Visit(const char* Name, CVisitableStruct& Visitable)
{
}

void CGetSetFieldsVisitor::Visit(const char* Name, CVisitableVectorBase& Visitable)
{
}