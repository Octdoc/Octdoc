#include "CppUnitTest.h"
#include "graphics/directx11/comptr.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace octdoc::gfx::dx11;

namespace UnitTest
{
	class SpecimanBase
	{
	public:
		int _123;
		SpecimanBase() :_123(123) {}
		virtual void Release() {}
		virtual void AddRef() {}
	};

	class Speciman :public SpecimanBase
	{
	public:
		int counter;
		Speciman() :counter(1) {}
		virtual void Release() override
		{
			counter--;
		}
		virtual void AddRef() override
		{
			counter++;
		}
	};

	TEST_CLASS(COMPtr_Test)
	{
	public:

		TEST_METHOD(DefaultCTorTest)
		{
			COM_Ptr<Speciman> p;
			Assert::IsTrue(p.get() == nullptr);
		}
		TEST_METHOD(PtrCTorTest)
		{
			Speciman s;
			COM_Ptr<Speciman> p(&s);
			Assert::IsTrue(p.get() == &s);
			Assert::IsTrue(s.counter == 1);
		}
		TEST_METHOD(CopyCTorTest)
		{
			Speciman s;
			COM_Ptr<Speciman> p1(&s);
			COM_Ptr<Speciman> p2(p1);

			Assert::IsTrue(p1.get() == p2.get());
			Assert::IsTrue(s.counter == 2);
		}
		TEST_METHOD(DTorTest)
		{
			Speciman s;
			COM_Ptr<Speciman>* p = new COM_Ptr<Speciman>(&s);
			delete p;
			p = nullptr;
			Assert::IsTrue(s.counter == 0);
		}
		TEST_METHOD(ReleaseTest)
		{
			Speciman s;
			COM_Ptr<Speciman> p(&s);
			p.Release();
			Assert::IsTrue(s.counter == 0);
			Assert::IsTrue(p.get() == nullptr);
			p.Release();
			Assert::IsTrue(s.counter == 0);
			Assert::IsTrue(p.get() == nullptr);
		}
		TEST_METHOD(UnbindTest)
		{
			Speciman s;
			COM_Ptr<Speciman> p(&s);
			p.Unbind();
			Assert::IsTrue(s.counter == 1);
			Assert::IsTrue(p.get() == nullptr);
			p.Unbind();
			Assert::IsTrue(s.counter == 1);
			Assert::IsTrue(p.get() == nullptr);
		}
		TEST_METHOD(CastTest)
		{
			Speciman s;
			COM_Ptr<Speciman> p(&s);
			Speciman* sp;
			sp = p;
			Assert::IsTrue(sp == &s);

		}
		TEST_METHOD(GetAddressTest)
		{
			Speciman s;
			Speciman* ps = &s;
			COM_Ptr<Speciman> p;
			*(&p) = ps;
			Assert::IsTrue(p.get() == ps);
		}
		TEST_METHOD(GetCastedToDerivedTest)
		{
			Speciman s;
			COM_Ptr<SpecimanBase> p(&s);
			Assert::IsTrue(p.As<Speciman>()->counter == 1);
		}
		TEST_METHOD(GetCastedToBaseTest)
		{
			Speciman s;
			COM_Ptr<Speciman> p(&s);
			Assert::IsTrue(p.As<SpecimanBase>()->_123 == 123);
		}
		TEST_METHOD(GetCastedAddressToDerivedTest)
		{
			Speciman s;
			Speciman* ps = &s;
			COM_Ptr<SpecimanBase> p;
			auto pp = p.AddressAs<Speciman>();
			*pp = ps;
			Assert::IsTrue(p->_123 == 123);
		}
		TEST_METHOD(GetCastedAddressToBaseTest)
		{
			Speciman s;
			SpecimanBase* ps = &s;
			COM_Ptr<Speciman> p;
			auto pp = p.AddressAs<SpecimanBase>();
			*pp = ps;
			Assert::IsTrue(p->counter == 1);
		}
		TEST_METHOD(OperatorArrowTest)
		{
			Speciman s;
			COM_Ptr<Speciman> p(&s);
			p->counter = 123;
			Assert::IsTrue(s.counter == 123);
		}
		TEST_METHOD(EquationOperatorTest)
		{
			Speciman s1, s2;
			COM_Ptr<Speciman> p1(&s1), p2(&s2), p3(p1), p4, p5;

			Assert::IsTrue(p4 == p5);
			Assert::IsFalse(p1 == &s2);
			Assert::IsTrue(p1 == &s1);
			Assert::IsFalse(p1 == p2);
			Assert::IsTrue(p1 == p3);
			Assert::IsFalse(p1 != &s1);
			Assert::IsTrue(p1 != &s2);
			Assert::IsTrue(p1 != p2);
			Assert::IsFalse(p1 != p3);
			Assert::IsFalse(p4);
			Assert::IsTrue(p1);
		}
		TEST_METHOD(ValueAssignTest)
		{
			Speciman s;
			COM_Ptr<Speciman> p1, p2, p3;
			p1 = &s;
			Assert::IsTrue(p1);
			Assert::IsTrue(s.counter == 1);
			Assert::IsTrue(p1->counter == 1);
			p2 = p1;
			Assert::IsTrue(s.counter == 2);
			Assert::IsTrue(p1 == p2);
			p1 = p2;
			Assert::IsTrue(s.counter == 2);
			p1 = &s;
			Assert::IsTrue(s.counter == 2);
			p1 = p3;
			Assert::IsTrue(s.counter == 1);
			Assert::IsFalse(p1);
			Assert::IsTrue(p1 == p3);
			p2 = nullptr;
			Assert::IsTrue(s.counter == 0);
			Assert::IsFalse(p2);
		}
	};
}