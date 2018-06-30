#pragma once

typedef ::std::string stringA;
class XmlRpcValue;
namespace Bugzilla
{
	struct CVisitor;
	struct CFieldVisitor;
	struct CElementVisitor;
	struct CVisitable
	{
		virtual void Accept(CVisitor& Visitor) = 0;
	};

	struct CVisitableStruct
	{
		virtual void Accept(CFieldVisitor& Visitor) = 0;
	};

	struct CVisitableVectorBase
	{
		virtual int GetSize() const = 0;
		virtual void SetSize(int Size) = 0;

		virtual void Accept(CElementVisitor& Visitor) = 0;
	};

	struct CVisitor
	{
		virtual void Visit(bool& Value) = 0;
		virtual void Visit(int& Value) = 0;
		virtual void Visit(double& Value) = 0;
		virtual void Visit(stringA& Value) = 0;
		virtual void Visit(tm& Value) = 0;
	};

	struct CElementVisitor
	{
		virtual void Visit(int nElement, bool& Value) = 0;
		virtual void Visit(int nElement, int& Value) = 0;
		virtual void Visit(int nElement, double& Value) = 0;
		virtual void Visit(int nElement, stringA& Value) = 0;
		virtual void Visit(int nElement, tm& Value) = 0;

		virtual void Visit(int nElement, CVisitable& Visitable) = 0;
		virtual void Visit(int nElement, CVisitableStruct& Visitable) = 0;
		virtual void Visit(int nElement, CVisitableVectorBase& Visitable) = 0;
	};

	struct CFieldVisitor
	{
		virtual bool CanVisit(const char* Name, bool bValidField) const = 0;

		virtual void Visit(const char* Name, bool& Value) = 0;
		virtual void Visit(const char* Name, int& Value) = 0;
		virtual void Visit(const char* Name, double& Value) = 0;
		virtual void Visit(const char* Name, stringA& Value) = 0;
		virtual void Visit(const char* Name, tm& Value) = 0;

		virtual void Visit(const char* Name, CVisitable& Visitable) = 0;
		virtual void Visit(const char* Name, CVisitableStruct& Visitable) = 0;
		virtual void Visit(const char* Name, CVisitableVectorBase& Visitable) = 0;
	};

	struct CVisitException : exception
	{
		CVisitException(string&& Message)
			: exception("Visitation Exception")
			, m_Message(std::move(Message))
		{

		}

		const char* what() const override
		{
			return m_Message.c_str();
		}

		string m_Message;
	};

#define THROW_VISIT_EXCEPTION(msg) throw CVisitException(__FUNCTION__ ": " msg)
};