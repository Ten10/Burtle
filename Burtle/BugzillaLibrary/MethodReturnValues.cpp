#include "stdafx.h"
#include "MethodReturnValues.h"

namespace Bugzilla
{
	void CActiveLogin::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::Visit(Visitor, "id", UserID);
		Bugzilla::Visit(Visitor, "token", Token);
	}

	void CGetVersionInfo::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::Visit(Visitor, "version", Version);
	}

	void CGetTimeInfo::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::Visit(Visitor, "web_time", WebTime);
		Bugzilla::Visit(Visitor, "db_time", DatabaseTime);
	}

	void CBugUpdateResult::Accept(CFieldVisitor& Visitor)
	{
		__super::Accept(Visitor);
		Bugzilla::Visit(Visitor, "alias", Alias);
		Bugzilla::Visit(Visitor, "last_change_time", LastChangeTime);
	}

	void CBugField::CFieldValue::CStatusOption::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::Visit(Visitor, "name", Name);
		Bugzilla::Visit(Visitor, "comment_required", bCommentRequired);
	}

	void CBugField::CFieldValue::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::Visit(Visitor, "is_open", bOpen, false);
		Bugzilla::Visit(Visitor, "is_active", bIsActive, false);
		Bugzilla::Visit(Visitor, "name", Name, false);
		Bugzilla::Visit(Visitor, "sort_key", SortKey);
		Bugzilla::Visit(Visitor, "description", Description, false);
		Bugzilla::Visit(Visitor, "visibility_values", VisibillityValues, false);

		Bugzilla::Visit(Visitor, "can_change_to", CanChangeTo, false);
	}

	bool CBugField::ShouldDisplay() const
	{
		if (FieldType == CBugField::typeUnknown)
			return false;

		if (FieldType == CBugField::typeDropDown && Values.Container.empty())
			return false;

		if (!VisibillityField.empty())
			return false;

		return true;
	}

	void CBugField::Accept(CFieldVisitor& Visitor)
	{
		__super::Accept(Visitor);
		int nType = FieldType;
		Bugzilla::Visit(Visitor, "type", nType);
		FieldType = (eType)nType;
		Bugzilla::Visit(Visitor, "is_custom", bCustom);
		Bugzilla::Visit(Visitor, "is_mandatory", bMandatory);
		Bugzilla::Visit(Visitor, "is_on_bug_entry", bInBugEntry);
		Bugzilla::Visit(Visitor, "name", Name);
		Bugzilla::Visit(Visitor, "display_name", DisplayName);
		Bugzilla::Visit(Visitor, "visibility_field", VisibillityField, false);
		Bugzilla::Visit(Visitor, "visibility_values", VisibillityValues);
		Bugzilla::Visit(Visitor, "value_field", ValueField, false);
		Bugzilla::Visit(Visitor, "values", Values, false);
	}

	CBugInfo::CBugInfo(XmlRpcValue&& Value)
		: BugInfo(Value)
	{

	}

	int CBugInfo::GetBugID() const
	{
		int bugID = 0;
		if (BugInfo.hasMember("id"))
			bugID = const_cast<XmlRpcValue&>(BugInfo)["id"];
		return bugID;
	}

	void CBugInfo::Accept(CFieldVisitor& Visitor)
	{
		CXmlRpcStructWrapper wrapper(BugInfo);
		wrapper.Accept(Visitor);
	}

	void CUserInfo::Accept(CFieldVisitor& Visitor)
	{
		__super::Accept(Visitor);
		Bugzilla::Visit(Visitor, "real_name", Name);
		Bugzilla::Visit(Visitor, "email", Email);
		Bugzilla::Visit(Visitor, "name", LoginName);
		Bugzilla::Visit(Visitor, "can_login", bCanLogin);
		Bugzilla::Visit(Visitor, "email_enabled", bEmailEnabled);
		Bugzilla::Visit(Visitor, "login_denied_text", LoginDeniedText);
	}

	void CProductInfo::Accept(CFieldVisitor& Visitor)
	{
		__super::Accept(Visitor);
		Bugzilla::Visit(Visitor, "name", Name);
		Bugzilla::Visit(Visitor, "description", Description);
	}
}