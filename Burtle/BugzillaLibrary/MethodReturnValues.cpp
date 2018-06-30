#include "stdafx.h"
#include "MethodReturnValues.h"

namespace Bugzilla
{
	void CActiveLogin::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::TryVisit(Visitor, "id", UserID);
		Bugzilla::TryVisit(Visitor, "token", Token);
	}

	void CGetVersionInfo::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::TryVisit(Visitor, "version", Version);
	}

	void CGetTimeInfo::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::TryVisit(Visitor, "web_time", WebTime);
		Bugzilla::TryVisit(Visitor, "db_time", DatabaseTime);
	}

	void CBugUpdateResult::Accept(CFieldVisitor& Visitor)
	{
		__super::Accept(Visitor);
		Bugzilla::TryVisit(Visitor, "alias", Alias);
		Bugzilla::TryVisit(Visitor, "last_change_time", LastChangeTime);
	}

	void CBugField::CFieldValue::CStatusOption::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::TryVisit(Visitor, "name", Name);
		Bugzilla::TryVisit(Visitor, "comment_required", bCommentRequired);
	}

	void CBugField::CFieldValue::Accept(CFieldVisitor& Visitor)
	{
		Bugzilla::TryVisit(Visitor, "is_open", bOpen);
		Bugzilla::TryVisit(Visitor, "is_active", bIsActive);
		Bugzilla::TryVisit(Visitor, "name", Name);
		Bugzilla::TryVisit(Visitor, "sort_key", SortKey);
		Bugzilla::TryVisit(Visitor, "description", Description);
		Bugzilla::TryVisit(Visitor, "visibility_values", VisibillityValues);
				  
		Bugzilla::TryVisit(Visitor, "can_change_to", CanChangeTo);
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
		Bugzilla::TryVisit(Visitor, "type", nType);
		FieldType = (eType)nType;
		Bugzilla::TryVisit(Visitor, "is_custom", bCustom);
		Bugzilla::TryVisit(Visitor, "is_mandatory", bMandatory);
		Bugzilla::TryVisit(Visitor, "is_on_bug_entry", bInBugEntry);
		Bugzilla::TryVisit(Visitor, "name", Name);
		Bugzilla::TryVisit(Visitor, "display_name", DisplayName);
		Bugzilla::TryVisit(Visitor, "visibility_field", VisibillityField);
		Bugzilla::TryVisit(Visitor, "visibility_values", VisibillityValues);
		Bugzilla::TryVisit(Visitor, "value_field", ValueField);
		Bugzilla::TryVisit(Visitor, "values", Values);
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
		Bugzilla::TryVisit(Visitor, "real_name", Name);
		Bugzilla::TryVisit(Visitor, "email", Email);
		Bugzilla::TryVisit(Visitor, "name", LoginName);
		Bugzilla::TryVisit(Visitor, "can_login", bCanLogin);
		Bugzilla::TryVisit(Visitor, "email_enabled", bEmailEnabled);
		Bugzilla::TryVisit(Visitor, "login_denied_text", LoginDeniedText);
	}

	void CProductInfo::Accept(CFieldVisitor& Visitor)
	{
		__super::Accept(Visitor);
		Bugzilla::TryVisit(Visitor, "name", Name);
		Bugzilla::TryVisit(Visitor, "description", Description);
	}
}