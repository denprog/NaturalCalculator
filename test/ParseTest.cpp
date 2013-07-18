#include "ParseTest.h"
#include <auto_ptr.h>

void ParseTest::Test1()
{
	QString s;
	std::auto_ptr<RootFormulaNode> n;
	
	std::string tests[] = 
		{
			"(2)", 
			"(23)",
			"(23+)",
			"(-34.4)",
			"(+456.45+3-5)",
			"(*45)",
			"(-3*4)",
			"(2+4)",
			"(23-45)",
			"([(2+3)])",
			"([([(2+3)])])",
			"([([(2+3)]+34)])",
			"([(2-3+34))",
			"((2+3+34)])",
			"((3)/(2))",
			"((3+4)/(2))",
			"((3+4)/(2*5))",
			"((3)/(2-234))",
			"(pow((12),()))",
			"(pow((1.2),(4)))",
			"(pow)",
			"(po)",
			"(sqrt((12)))",
			"(sqrt)",
			"(sqrt((12*45)))",
			"(2+3*pow((sqrt((3))-4.5),(2.3)))"
		};
	
	for (std::string s : tests)
	{
		n = std::auto_ptr<RootFormulaNode>(RootFormulaNode::FromString(s, wnd));
		Check(n->ToString(), s, 0);
	}
}
