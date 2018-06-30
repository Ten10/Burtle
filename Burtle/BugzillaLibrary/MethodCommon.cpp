#include "stdafx.h"
#include "MethodCommon.h"

namespace Bugzilla
{
	CXmlRpcWrapper::CXmlRpcWrapper(XmlRpcValue& XmlRpcValue)
		: Value(XmlRpcValue)
	{
	}

	void CXmlRpcWrapper::Accept(CVisitor& Visitor)
	{
		auto type = Value.getType();
		switch (type)
		{
		case XmlRpcValue::TypeBoolean:
			Visit(Visitor, Value.operator bool &());
			break;
		case XmlRpcValue::TypeInt:
			Visit(Visitor, Value.operator int &());
			break;
		case XmlRpcValue::TypeDouble:
			Visit(Visitor, Value.operator double &());
			break;
		case XmlRpcValue::TypeDateTime:
			Visit(Visitor, Value.operator tm &());
			break;
		case XmlRpcValue::TypeString:
		{
			string sText = Value.operator string();
			Visit(Visitor, sText);
			Value = sText;
		}
		break;
		default:
			THROW_VISIT_EXCEPTION("Unsupported xml rpc type");
			break;
		}
	}

	CXmlRpcArrayWrapper::CXmlRpcArrayWrapper(XmlRpcValue& XmlRpcValue)
		: Value(XmlRpcValue)
	{
	}

	int CXmlRpcArrayWrapper::GetSize() const
	{
		auto type = Value.getType();
		if (type != XmlRpcValue::TypeArray)
			THROW_VISIT_EXCEPTION("expected an xml rpc array");

		return Value.size();
	}

	void CXmlRpcArrayWrapper::SetSize(int Size)
	{
		auto type = Value.getType();
		if (type != XmlRpcValue::TypeArray)
			THROW_VISIT_EXCEPTION("expected an xml rpc array");

		Value.setSize(Size);
	}

	void CXmlRpcArrayWrapper::Accept(CElementVisitor& Visitor)
	{
		auto type = Value.getType();
		if (type != XmlRpcValue::TypeArray)
			THROW_VISIT_EXCEPTION("expected an xml rpc array");

		XmlRpcValue::ValueArray& valueArray = Value;
		for (int index = 0; index < valueArray.size(); ++index)
		{
			auto& element = valueArray[index];
			auto type = element.getType();
			switch (type)
			{
			case XmlRpcValue::TypeBoolean:
				Visit(Visitor, index, element.operator bool &());
				break;
			case XmlRpcValue::TypeInt:
				Visit(Visitor, index, element.operator int &());
				break;
			case XmlRpcValue::TypeDouble:
				Visit(Visitor, index, element.operator double &());
				break;
			case XmlRpcValue::TypeDateTime:
				Visit(Visitor, index, element.operator tm &());
				break;
			case XmlRpcValue::TypeString:
			{
				string sText = element.operator string();
				Visit(Visitor, index, sText);
				element = sText;
			}
			break;
			case XmlRpcValue::TypeArray:
			{
				CXmlRpcArrayWrapper wrapper(element);
				Visit(Visitor, index, wrapper);
			}
			break;
			case XmlRpcValue::TypeStruct:
			{
				CXmlRpcStructWrapper wrapper(element);
				Visit(Visitor, index, wrapper);
			}
			break;
			default:
				THROW_VISIT_EXCEPTION("Unsupported xml rpc type");
				break;
			}
		}
	}

	CXmlRpcStructWrapper::CXmlRpcStructWrapper(XmlRpcValue& XmlRpcValue)
		: Value(XmlRpcValue)
	{
	}

	void CXmlRpcStructWrapper::Accept(CFieldVisitor& Visitor)
	{
		auto type = Value.getType();
		if (type != XmlRpcValue::TypeStruct)
			THROW_VISIT_EXCEPTION("expected an xml rpc struct");

		XmlRpcValue::ValueStruct& valueStruct = Value;
		for (auto it = valueStruct.begin(); it != valueStruct.end(); ++it)
		{
			auto& member = it->second;
			auto type = member.getType();
			switch (type)
			{
			case XmlRpcValue::TypeBoolean:
				Visit(Visitor, it->first.c_str(), member.operator bool &());
				break;
			case XmlRpcValue::TypeInt:
				Visit(Visitor, it->first.c_str(), member.operator int &());
				break;
			case XmlRpcValue::TypeDouble:
				Visit(Visitor, it->first.c_str(), member.operator double &());
				break;
			case XmlRpcValue::TypeDateTime:
				Visit(Visitor, it->first.c_str(), member.operator tm &());
				break;
			case XmlRpcValue::TypeString:
			{
				string sText = member.operator string();
				Visit(Visitor, it->first.c_str(), sText);
				member = sText;
			}
			break;
			case XmlRpcValue::TypeArray:
			{
				CXmlRpcArrayWrapper wrapper(member);
				Visit(Visitor, it->first.c_str(), wrapper);
			}
			break;
			case XmlRpcValue::TypeStruct:
			{
				CXmlRpcStructWrapper wrapper(member);
				Visit(Visitor, it->first.c_str(), wrapper);
			}
			break;
			default:
				THROW_VISIT_EXCEPTION("Unsupported xml rpc type");
				break;
			}
		}
	}

	void CBugzillaException::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::TryVisit(Visitor, "faultCode", ErrorCode);
		Bugzilla::TryVisit(Visitor, "faultString", ErrorMessage);
	}

	CBugzillaID::CBugzillaID() : ID(0)
	{
	}

	void CBugzillaID::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::TryVisit(Visitor, "id", ID);
	}

	void CBugzillaIDs::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::TryVisit(Visitor, "ids", IDs);
	}

	void CBugzillaObject::Accept(CFieldVisitor& Visitor)
	{
		ObjectID.Accept(Visitor);
	}

	void CCommonBugParamaters::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::TryVisit(Visitor, "product", Product);
		Bugzilla::TryVisit(Visitor, "component", Component);
		Bugzilla::TryVisit(Visitor, "summary", Summary);
		Bugzilla::TryVisit(Visitor, "version", Version);
		Bugzilla::TryVisit(Visitor, "description", Description);
		Bugzilla::TryVisit(Visitor, "op_sys", OperatingSystem);
		Bugzilla::TryVisit(Visitor, "rep_platform", Platform);
		Bugzilla::TryVisit(Visitor, "priority", Priority);
		Bugzilla::TryVisit(Visitor, "severity", Severity);
		Bugzilla::TryVisit(Visitor, "alias", Alias);
		Bugzilla::TryVisit(Visitor, "assigned_to", AssignedTo);
		Bugzilla::TryVisit(Visitor, "qa_contact", QAContact);
		Bugzilla::TryVisit(Visitor, "status", Status);
		Bugzilla::TryVisit(Visitor, "target_milestone", TargetMilestone);
	}
}