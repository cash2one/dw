#pragma  once
#include "plugin.h"
#include "pluginDepot.h"
#include "reflection.h"
#include "test_generated.h"
#include "test2_generated.h"

class PluginDemo
	:public Plugin
{
public:
	BEGIN_INPUT_MSG_MAP()
		//INPUT_HANDLE_MSG(1, &PluginDemo::on_scene_xs2xs_ntf_scene_logout, new MobaGo::FlatBuffer::MsgOperateFriendReq)
	END_INPUT_MSG_MAP()


	int on_scene_xs2xs_ntf_scene_logout(const PackInfo & pack_info) { return 0; }
};


#include <random>

#define TEST_OUTPUT_LINE(...) \
    { printf(__VA_ARGS__); printf("\n"); }

int testing_fails = 0;

void TestFail(const char *expval, const char *val, const char *exp,
	const char *file, int line) {
	TEST_OUTPUT_LINE("TEST FAILED: %s:%d, %s (%s) != %s", file, line,
		exp, expval, val);
	assert(0);
	testing_fails++;
}

void TestEqStr(const char *expval, const char *val, const char *exp,
	const char *file, int line) {
	if (strcmp(expval, val) != 0) {
		TestFail(expval, val, exp, file, line);
	}
}

template<typename T, typename U>
void TestEq(T expval, U val, const char *exp, const char *file, int line) {
	if (U(expval) != val) {
		TestFail(flatbuffers::NumToString(expval).c_str(),
			flatbuffers::NumToString(val).c_str(),
			exp, file, line);
	}
}

#define TEST_EQ(exp, val) TestEq(exp,         val,   #exp, __FILE__, __LINE__)
#define TEST_NOTNULL(exp) TestEq(exp == NULL, false, #exp, __FILE__, __LINE__)
#define TEST_EQ_STR(exp, val) TestEqStr(exp,  val,   #exp, __FILE__, __LINE__)

// Include simple random number generator to ensure results will be the
// same cross platform.
// http://en.wikipedia.org/wiki/Park%E2%80%93Miller_random_number_generator
uint32_t lcg_seed = 48271;
uint32_t lcg_rand() {
	return lcg_seed = ((uint64_t)lcg_seed * 279470273UL) % 4294967291UL;
}
void lcg_reset() { lcg_seed = 48271; }

#include <iostream>
#include "util.h"
#include "idl.h"
#include "fbparse.h"
class TestPlugin
{
public:
	static void run() 
	{
		//PluginDemo demo;
		//PluginDepot depot;

		std::string schemafile;
		TEST_EQ(flatbuffers::LoadFile(
			"test.fbs", false, &schemafile), true);

		std::string schemafile2;
		TEST_EQ(flatbuffers::LoadFile(
			"test2.fbs", false, &schemafile2), true);
		using namespace flatbuffers;

		FBParser parser;
		TEST_EQ(parser.init("."), 0);

		std::vector<Fieldst> st;
		TEST_EQ(parser.getFields("Monster2", st), 0);
		TEST_EQ(st.size(), 9);

		for (auto& i : st)
		{
			if (i.name == "x1")
				TEST_EQ_STR("long", i.type.c_str());
		}


		parser.setRootType("Monster2");
		parser.setField("x1", "999");

		std::string output;
		TEST_EQ(parser.finish(output), 0);

		
		std::string json_output;
		TEST_EQ(parser.parse(output, json_output), 0);
		
		std::cout << "json : \n" << json_output << std::endl;
	}
};