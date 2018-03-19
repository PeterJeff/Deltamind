#include "stdafx.h"

#include "Omiracron.h"
#include "PythonInterface.h"
#include <string>

namespace Omiracron {
	PythonInterface python;

	//python.ExecuteFile("zhou", "A3C_RNN");



	void Functions::Print(const char* text) {
		std::string code("print(\"");
		code.append(text);
		code.append("\")");
		python.RunCode(code.c_str());
	}

	double Functions::Multiply(double a, double b) {
		python.OpenFile("TestMult");
		return python.ExecuteFunction<double>("mult", a, b);
	}


	void Functions::Beep() {
		python.ExecuteFile("Beep");
	}

}