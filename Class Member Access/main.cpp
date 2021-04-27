#include <Windows.h>
#include <iostream>

LARGE_INTEGER iFreq;

class Object
{
public:
	Object *OtherObject;
	int MemberA;
	int MemberB;
	int MemberC;
	int MemberD;

	int GetMemberA()
	{
		return MemberA;
	}

	void SetMemberA(int value)
	{
		MemberA = value;
	}

	double EmptyLoop()
	{
		LARGE_INTEGER start;
		LARGE_INTEGER end;
		LARGE_INTEGER time;
		int i = 0;
		QueryPerformanceCounter(&start);
		for (; i < 1000000000; i++)
		{
		}
		QueryPerformanceCounter(&end);
		time.QuadPart = end.QuadPart - start.QuadPart;
		double result = (double)time.QuadPart / (double)iFreq.QuadPart;
		return result;
	}

	double ModifyOtherObjectDirect()
	{
		LARGE_INTEGER start;
		LARGE_INTEGER end;
		LARGE_INTEGER time;
		int i = 0;
		QueryPerformanceCounter(&start);
		for (; i < 1000000000; i++)
		{
			OtherObject->MemberA = i;
		}
		QueryPerformanceCounter(&end);
		time.QuadPart = end.QuadPart - start.QuadPart;
		double result = (double)time.QuadPart / (double)iFreq.QuadPart;
		return result;
	}

	double AccessOtherObjectDirect()
	{
		LARGE_INTEGER start;
		LARGE_INTEGER end;
		LARGE_INTEGER time;
		int i = 0;
		int test;
		QueryPerformanceCounter(&start);
		for (; i < 1000000000; i++)
		{
			test = OtherObject->MemberA;
		}
		QueryPerformanceCounter(&end);
		time.QuadPart = end.QuadPart - start.QuadPart;
		double result = (double)time.QuadPart / (double)iFreq.QuadPart;
		return result;
	}

	double ModifyOtherObjectIndirect()
	{
		LARGE_INTEGER start;
		LARGE_INTEGER end;
		LARGE_INTEGER time;
		int i = 0;
		QueryPerformanceCounter(&start);
		for (; i < 1000000000; i++)
		{
			OtherObject->SetMemberA(i);
		}
		QueryPerformanceCounter(&end);
		time.QuadPart = end.QuadPart - start.QuadPart;
		double result = (double)time.QuadPart / (double)iFreq.QuadPart;
		return result;
	}

	double AccessOtherObjectIndirect()
	{
		LARGE_INTEGER start;
		LARGE_INTEGER end;
		LARGE_INTEGER time;
		int i = 0;
		int test;
		QueryPerformanceCounter(&start);
		for (; i < 1000000000; i++)
		{
			test = OtherObject->GetMemberA();
		}
		QueryPerformanceCounter(&end);
		time.QuadPart = end.QuadPart - start.QuadPart;
		double result = (double)time.QuadPart / (double)iFreq.QuadPart;
		return result;
	}
};



int main()
{
	QueryPerformanceFrequency(&iFreq);

	Object objectA;
	Object objectB;

	objectA.OtherObject = &objectB;
	
	double directWriteTotal = 0.0;
	double setterWriteTotal = 0.0;
	double directReadTotal = 0.0;
	double getterReadTotal = 0.0;
	
	const int MAX_ITERATIONS = 20;

	for (int i = 0; i < MAX_ITERATIONS; i++)
	{
		std::cout << "Iteration:\t" << i << std::endl;
		std::cout << "---------------------------------------------" << std::endl;

		double directWrite = objectA.ModifyOtherObjectDirect() - objectA.EmptyLoop();
		directWriteTotal += directWrite;
		std::cout << "Direct Write:\t" << directWrite << std::endl;

		double setterWrite = objectA.ModifyOtherObjectIndirect() - objectA.EmptyLoop();
		setterWriteTotal += setterWrite;
		std::cout << "Setter Write:\t" << setterWrite << std::endl;

		std::cout << "Penalty Factor:\t" << (setterWrite / directWrite) << std::endl;

		std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

		double directRead = objectA.AccessOtherObjectDirect() - objectA.EmptyLoop();
		directReadTotal += directRead;
		std::cout << "Direct Read:\t" << directRead << std::endl;

		double getterRead = objectA.AccessOtherObjectIndirect() - objectA.EmptyLoop();
		getterReadTotal += getterRead;
		std::cout << "Getter Read:\t" << getterRead << std::endl;

		std::cout << "Penalty Factor:\t" << (getterRead / directRead) << std::endl;

		std::cout << "=============================================" << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Direct Write Average:\t" << (directWriteTotal / MAX_ITERATIONS) << std::endl;
	std::cout << "Setter Write Average:\t" << (setterWriteTotal / MAX_ITERATIONS) << std::endl;
	std::cout << "Penalty Factor:\t\t" << (setterWriteTotal / directWriteTotal) << std::endl << std::endl;
	std::cout << "Direct Read Average:\t" << (directReadTotal / MAX_ITERATIONS) << std::endl;
	std::cout << "Getter Read Average:\t" << (getterReadTotal / MAX_ITERATIONS) << std::endl;
	std::cout << "Penalty Factor:\t\t" << (getterReadTotal / directReadTotal) << std::endl;


	return 0;
}


