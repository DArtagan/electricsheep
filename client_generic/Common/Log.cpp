#include	<stdarg.h>
#include	<inttypes.h>
#include	<string>
#include	<time.h>
#include	<sstream>

#include	"base.h"
#include	"Log.h"

//	Lua.
extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
};

namespace	Base
{

/* logging.lua */
static const unsigned char luaLogging[]={
108,111, 99, 97,108, 32, 80,117, 98,108,105, 99, 44, 32, 80,114,105,118, 97,116,
101, 32, 61, 32,123,125, 44, 32,123,125, 13, 10,108,111,103,103,105,110,103, 32,
 61, 32, 80,117, 98,108,105, 99, 13, 10, 13, 10, 45, 45, 32, 77,101,116, 97, 32,
105,110,102,111,114,109, 97,116,105,111,110, 13, 10, 80,117, 98,108,105, 99, 46,
 95, 67, 79, 80, 89, 82, 73, 71, 72, 84, 32, 61, 32, 34, 67,111,112,121,114,105,
103,104,116, 32, 40, 67, 41, 32, 50, 48, 48, 52, 32, 75,101,112,108,101,114, 32,
 80,114,111,106,101, 99,116, 34, 13, 10, 80,117, 98,108,105, 99, 46, 95, 68, 69,
 83, 67, 82, 73, 80, 84, 73, 79, 78, 32, 61, 32, 34, 65, 32,115,105,109,112,108,
101, 32, 65, 80, 73, 32,116,111, 32,117,115,101, 32,108,111,103,103,105,110,103,
 32,102,101, 97,116,117,114,101,115, 32,105,110, 32, 76,117, 97, 34, 13, 10, 80,
117, 98,108,105, 99, 46, 95, 78, 65, 77, 69, 32, 61, 32, 34, 76,117, 97, 76,111,
103,103,105,110,103, 34, 13, 10, 80,117, 98,108,105, 99, 46, 95, 86, 69, 82, 83,
 73, 79, 78, 32, 61, 32, 34, 49, 46, 49, 46, 48, 34, 13, 10, 13, 10, 45, 45, 32,
 84,104,101, 32, 68, 69, 66, 85, 71, 32, 76,101,118,101,108, 32,100,101,115,105,
103,110, 97,116,101,115, 32,102,105,110,101, 45,103,114, 97,105,110,101,100, 32,
105,110,115,116,114,105,110,103, 46,102,111,114,109, 97,116,105,111,110, 97,108,
 32,101,118,101,110,116,115, 32,116,104, 97,116, 32, 97,114,101, 32,109,111,115,
116, 13, 10, 45, 45, 32,117,115,101,102,117,108, 32,116,111, 32,100,101, 98,117,
103, 32, 97,110, 32, 97,112,112,108,105, 99, 97,116,105,111,110, 13, 10, 80,117,
 98,108,105, 99, 46, 68, 69, 66, 85, 71, 32, 61, 32, 34, 68, 69, 66, 85, 71, 34,
 13, 10, 13, 10, 45, 45, 32, 84,104,101, 32, 73, 78, 70, 79, 32,108,101,118,101,
108, 32,100,101,115,105,103,110, 97,116,101,115, 32,105,110,115,116,114,105,110,
103, 46,102,111,114,109, 97,116,105,111,110, 97,108, 32,109,101,115,115, 97,103,
101,115, 32,116,104, 97,116, 32,104,105,103,104,108,105,103,104,116, 32,116,104,
101, 13, 10, 45, 45, 32,112,114,111,103,114,101,115,115, 32,111,102, 32,116,104,
101, 32, 97,112,112,108,105, 99, 97,116,105,111,110, 32, 97,116, 32, 99,111, 97,
114,115,101, 45,103,114, 97,105,110,101,100, 32,108,101,118,101,108, 13, 10, 80,
117, 98,108,105, 99, 46, 73, 78, 70, 79, 32, 61, 32, 34, 73, 78, 70, 79, 34, 13,
 10, 13, 10, 45, 45, 32, 84,104,101, 32, 87, 65, 82, 78, 32,108,101,118,101,108,
 32,100,101,115,105,103,110, 97,116,101,115, 32,112,111,116,101,110,116,105, 97,
108,108,121, 32,104, 97,114,109,102,117,108, 32,115,105,116,117, 97,116,105,111,
110,115, 13, 10, 80,117, 98,108,105, 99, 46, 87, 65, 82, 78, 32, 61, 32, 34, 87,
 65, 82, 78, 34, 13, 10, 13, 10, 45, 45, 32, 84,104,101, 32, 69, 82, 82, 79, 82,
 32,108,101,118,101,108, 32,100,101,115,105,103,110, 97,116,101,115, 32,101,114,
114,111,114, 32,101,118,101,110,116,115, 32,116,104, 97,116, 32,109,105,103,104,
116, 32,115,116,105,108,108, 32, 97,108,108,111,119, 32,116,104,101, 13, 10, 45,
 45, 32, 97,112,112,108,105, 99, 97,116,105,111,110, 32,116,111, 32, 99,111,110,
116,105,110,117,101, 32,114,117,110,110,105,110,103, 13, 10, 80,117, 98,108,105,
 99, 46, 69, 82, 82, 79, 82, 32, 61, 32, 34, 69, 82, 82, 79, 82, 34, 13, 10, 13,
 10, 45, 45, 32, 84,104,101, 32, 70, 65, 84, 65, 76, 32,108,101,118,101,108, 32,
100,101,115,105,103,110, 97,116,101,115, 32,118,101,114,121, 32,115,101,118,101,
114,101, 32,101,114,114,111,114, 32,101,118,101,110,116,115, 32,116,104, 97,116,
 32,119,105,108,108, 32,112,114,101,115,117,109, 97, 98,108,121, 13, 10, 45, 45,
 32,108,101, 97,100, 32,116,104,101, 32, 97,112,112,108,105, 99, 97,116,105,111,
110, 32,116,111, 32, 97, 98,111,114,116, 13, 10, 80,117, 98,108,105, 99, 46, 70,
 65, 84, 65, 76, 32, 61, 32, 34, 70, 65, 84, 65, 76, 34, 13, 10, 13, 10, 80,114,
105,118, 97,116,101, 46, 76, 69, 86, 69, 76, 32, 61, 32,123, 13, 10,  9, 91, 80,
117, 98,108,105, 99, 46, 68, 69, 66, 85, 71, 93, 32, 61, 32, 49, 44, 13, 10,  9,
 91, 80,117, 98,108,105, 99, 46, 73, 78, 70, 79, 93, 32, 32, 61, 32, 50, 44, 13,
 10,  9, 91, 80,117, 98,108,105, 99, 46, 87, 65, 82, 78, 93, 32, 32, 61, 32, 51,
 44, 13, 10,  9, 91, 80,117, 98,108,105, 99, 46, 69, 82, 82, 79, 82, 93, 32, 61,
 32, 52, 44, 13, 10,  9, 91, 80,117, 98,108,105, 99, 46, 70, 65, 84, 65, 76, 93,
 32, 61, 32, 53, 44, 13, 10,125, 13, 10, 13, 10, 13, 10, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 13, 10, 45, 45, 32, 67,114,
101, 97,116,101,115, 32, 97, 32,110,101,119, 32,108,111,103,103,101,114, 32,111,
 98,106,101, 99,116, 13, 10, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 13, 10,102,117,110, 99,116,105,111,110, 32, 80,117, 98,
108,105, 99, 46,110,101,119, 40, 32, 97,112,112,101,110,100, 44, 32, 95,105,103,
110,111,114,101, 83,116,114,105,110,103, 67,111,110,118,101,114,115,105,111,110,
 32, 41, 13, 10, 13, 10,  9,105,102, 32,116,121,112,101, 40, 32, 97,112,112,101,
110,100, 32, 41, 32,126, 61, 32, 34,102,117,110, 99,116,105,111,110, 34, 32,116,
104,101,110, 13, 10,  9,  9,114,101,116,117,114,110, 32,110,105,108, 44, 32, 34,
 65,112,112,101,110,100,101,114, 32,109,117,115,116, 32, 98,101, 32, 97, 32,102,
117,110, 99,116,105,111,110, 46, 34, 13, 10,  9,101,110,100, 13, 10, 13, 10,  9,
108,111, 99, 97,108, 32,108,111,103,103,101,114, 32, 61, 32,123,125, 13, 10,  9,
108,111,103,103,101,114, 46,108,101,118,101,108, 32, 61, 32, 80,117, 98,108,105,
 99, 46, 68, 69, 66, 85, 71, 13, 10,  9,108,111,103,103,101,114, 46, 97,112,112,
101,110,100, 32, 61, 32, 97,112,112,101,110,100, 13, 10,  9,108,111,103,103,101,
114, 46,105,103,110,111,114,101, 83,116,114,105,110,103, 67,111,110,118,101,114,
115,105,111,110, 32, 61, 32, 95,105,103,110,111,114,101, 83,116,114,105,110,103,
 67,111,110,118,101,114,115,105,111,110, 13, 10, 13, 10,  9,108,111,103,103,101,
114, 46,115,101,116, 76,101,118,101,108, 32, 61, 32,102,117,110, 99,116,105,111,
110, 32, 40,115,101,108,102, 44, 32,108,101,118,101,108, 41, 13, 10,  9,  9, 97,
115,115,101,114,116, 40, 80,114,105,118, 97,116,101, 46, 76, 69, 86, 69, 76, 91,
108,101,118,101,108, 93, 44, 32,115,116,114,105,110,103, 46,102,111,114,109, 97,
116, 40, 34,117,110,100,101,102,105,110,101,100, 32,108,101,118,101,108, 32, 96,
 37,115, 39, 34, 44, 32,116,111,115,116,114,105,110,103, 40,108,101,118,101,108,
 41, 41, 41, 13, 10,  9,  9,115,101,108,102, 46,108,101,118,101,108, 32, 61, 32,
108,101,118,101,108, 13, 10,  9,101,110,100, 13, 10, 13, 10,  9,108,111,103,103,
101,114, 46,108,111,103, 32, 61, 32,102,117,110, 99,116,105,111,110, 40, 32,115,
101,108,102, 44, 32,108,101,118,101,108, 44, 32,109,101,115,115, 97,103,101, 32,
 41, 13, 10,  9,  9, 97,115,115,101,114,116, 40, 80,114,105,118, 97,116,101, 46,
 76, 69, 86, 69, 76, 91,108,101,118,101,108, 93, 44, 32,115,116,114,105,110,103,
 46,102,111,114,109, 97,116, 40, 34,117,110,100,101,102,105,110,101,100, 32,108,
101,118,101,108, 32, 96, 37,115, 39, 34, 44, 32,116,111,115,116,114,105,110,103,
 40,108,101,118,101,108, 41, 41, 41, 13, 10,  9,  9,105,102, 32, 80,114,105,118,
 97,116,101, 46, 76, 69, 86, 69, 76, 91,108,101,118,101,108, 93, 32, 60, 32, 80,
114,105,118, 97,116,101, 46, 76, 69, 86, 69, 76, 91,115,101,108,102, 46,108,101,
118,101,108, 93, 32,116,104,101,110, 13, 10,  9,  9,  9,114,101,116,117,114,110,
 13, 10,  9,  9,101,110,100, 13, 10, 13, 10,  9,  9, 45, 45,  9, 84,104,101, 32,
108,111,103,103,101,114, 32,104, 97,115, 32,114,101,113,117,101,115,116,101,100,
 32,116,111, 32,110,111,116, 32,104, 97,118,101, 32,105,116, 39,115, 32,112, 97,
114, 97,109,115, 32, 99,111,110,118,101,114,116,101,100, 32,116,111, 32,115,116,
114,105,110,103, 44, 32,115,111, 32,108,101,116,115, 32,110,111,116, 46, 13, 10,
  9,  9,105,102, 32,110,111,116, 32,115,101,108,102, 46,105,103,110,111,114,101,
 83,116,114,105,110,103, 67,111,110,118,101,114,115,105,111,110, 32,116,104,101,
110, 13, 10,  9,  9,  9,105,102, 32,116,121,112,101, 40,109,101,115,115, 97,103,
101, 41, 32,126, 61, 32, 34,115,116,114,105,110,103, 34, 32,116,104,101,110, 13,
 10,  9,  9,  9,  9,109,101,115,115, 97,103,101, 32, 61, 32, 80,117, 98,108,105,
 99, 46,116,111,115,116,114,105,110,103, 40,109,101,115,115, 97,103,101, 41, 13,
 10,  9,  9,  9,101,110,100, 13, 10,  9,  9,101,110,100, 13, 10, 13, 10,  9,  9,
114,101,116,117,114,110, 32,108,111,103,103,101,114, 58, 97,112,112,101,110,100,
 40, 32,108,101,118,101,108, 44, 32,109,101,115,115, 97,103,101, 32, 41, 13, 10,
  9,101,110,100, 13, 10, 13, 10,  9,108,111,103,103,101,114, 46,100,101, 98,117,
103, 32, 61, 32,102,117,110, 99,116,105,111,110, 32, 40,108,111,103,103,101,114,
 44, 32,109,101,115,115, 97,103,101, 41, 32,114,101,116,117,114,110, 32,108,111,
103,103,101,114, 58,108,111,103, 40, 80,117, 98,108,105, 99, 46, 68, 69, 66, 85,
 71, 44, 32,109,101,115,115, 97,103,101, 41, 32,101,110,100, 13, 10,  9,108,111,
103,103,101,114, 46,105,110,102,111, 32, 32, 61, 32,102,117,110, 99,116,105,111,
110, 32, 40,108,111,103,103,101,114, 44, 32,109,101,115,115, 97,103,101, 41, 32,
114,101,116,117,114,110, 32,108,111,103,103,101,114, 58,108,111,103, 40, 80,117,
 98,108,105, 99, 46, 73, 78, 70, 79, 44, 32, 32,109,101,115,115, 97,103,101, 41,
 32,101,110,100, 13, 10,  9,108,111,103,103,101,114, 46,119, 97,114,110, 32, 32,
 61, 32,102,117,110, 99,116,105,111,110, 32, 40,108,111,103,103,101,114, 44, 32,
109,101,115,115, 97,103,101, 41, 32,114,101,116,117,114,110, 32,108,111,103,103,
101,114, 58,108,111,103, 40, 80,117, 98,108,105, 99, 46, 87, 65, 82, 78, 44, 32,
 32,109,101,115,115, 97,103,101, 41, 32,101,110,100, 13, 10,  9,108,111,103,103,
101,114, 46,101,114,114,111,114, 32, 61, 32,102,117,110, 99,116,105,111,110, 32,
 40,108,111,103,103,101,114, 44, 32,109,101,115,115, 97,103,101, 41, 32,114,101,
116,117,114,110, 32,108,111,103,103,101,114, 58,108,111,103, 40, 80,117, 98,108,
105, 99, 46, 69, 82, 82, 79, 82, 44, 32,109,101,115,115, 97,103,101, 41, 32,101,
110,100, 13, 10,  9,108,111,103,103,101,114, 46,102, 97,116, 97,108, 32, 61, 32,
102,117,110, 99,116,105,111,110, 32, 40,108,111,103,103,101,114, 44, 32,109,101,
115,115, 97,103,101, 41, 32,114,101,116,117,114,110, 32,108,111,103,103,101,114,
 58,108,111,103, 40, 80,117, 98,108,105, 99, 46, 70, 65, 84, 65, 76, 44, 32,109,
101,115,115, 97,103,101, 41, 32,101,110,100, 13, 10, 13, 10,  9,114,101,116,117,
114,110, 32,108,111,103,103,101,114, 13, 10,101,110,100, 13, 10, 13, 10, 13, 10,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 13,
 10, 45, 45, 32, 80,114,101,112, 97,114,101,115, 32,116,104,101, 32,108,111,103,
 32,109,101,115,115, 97,103,101, 13, 10, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 13, 10,102,117,110, 99,116,105,111,110, 32,
 80,117, 98,108,105, 99, 46,112,114,101,112, 97,114,101, 76,111,103, 77,115,103,
 40,112, 97,116,116,101,114,110, 44, 32,100,116, 44, 32,108,101,118,101,108, 44,
 32,109,101,115,115, 97,103,101, 41, 13, 10, 13, 10,  9,105,102, 32,112, 97,116,
116,101,114,110, 32, 61, 61, 32, 34, 34, 32,116,104,101,110, 13, 10,  9,  9,114,
101,116,117,114,110, 32,109,101,115,115, 97,103,101, 13, 10,  9,101,110,100, 13,
 10, 13, 10,  9,108,111, 99, 97,108, 32,108,111,103, 77,115,103, 32, 61, 32,112,
 97,116,116,101,114,110, 32,111,114, 32, 34, 37,100, 97,116,101, 32, 37,108,101,
118,101,108, 32, 37,109,101,115,115, 97,103,101, 92,110, 34, 13, 10,  9,108,111,
103, 77,115,103, 32, 61, 32,115,116,114,105,110,103, 46,103,115,117, 98, 40,108,
111,103, 77,115,103, 44, 32, 34, 37, 37,100, 97,116,101, 34, 44, 32,100,116, 41,
 13, 10,  9,108,111,103, 77,115,103, 32, 61, 32,115,116,114,105,110,103, 46,103,
115,117, 98, 40,108,111,103, 77,115,103, 44, 32, 34, 37, 37,108,101,118,101,108,
 34, 44, 32,108,101,118,101,108, 41, 13, 10,  9,108,111,103, 77,115,103, 32, 61,
 32,115,116,114,105,110,103, 46,103,115,117, 98, 40,108,111,103, 77,115,103, 44,
 32, 34, 37, 37,109,101,115,115, 97,103,101, 34, 44, 32,109,101,115,115, 97,103,
101, 41, 13, 10,  9,114,101,116,117,114,110, 32,108,111,103, 77,115,103, 13, 10,
101,110,100, 13, 10, 13, 10, 13, 10, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 13, 10, 45, 45, 32, 67,111,110,118,101,114,116,
115, 32, 97, 32, 76,117, 97, 32,118, 97,108,117,101, 32,116,111, 32, 97, 32,115,
116,114,105,110,103, 13, 10, 45, 45, 13, 10, 45, 45, 32, 67,111,110,118,101,114,
116,115, 32, 84, 97, 98,108,101, 32,102,105,101,108,100,115, 32,105,110, 32, 97,
108,112,104, 97, 98,101,116,105, 99, 97,108, 32,111,114,100,101,114, 13, 10, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 13, 10,
102,117,110, 99,116,105,111,110, 32, 80,117, 98,108,105, 99, 46,116,111,115,116,
114,105,110,103, 40, 32,118, 97,108,117,101, 44, 32, 95,105,110,100,101,110,116,
 32, 41, 13, 10, 13, 10,  9,108,111, 99, 97,108, 32,105,110,100,101,110,116, 32,
 61, 32, 95,105,110,100,101,110,116, 32,111,114, 32, 34, 34, 13, 10,  9,108,111,
 99, 97,108, 32,115,116,114, 32, 61, 32,105,110,100,101,110,116, 13, 10, 13, 10,
  9,105,102, 40, 32,116,121,112,101, 40,118, 97,108,117,101, 41, 32,126, 61, 32,
 39,116, 97, 98,108,101, 39, 32, 41, 32,116,104,101,110, 13, 10,  9,  9,105,102,
 40, 32,116,121,112,101, 40,118, 97,108,117,101, 41, 32, 61, 61, 32, 39,115,116,
114,105,110,103, 39, 32, 41, 32,116,104,101,110, 13, 10,  9,  9,  9,105,102, 40,
 32,115,116,114,105,110,103, 46,102,105,110,100, 40, 32,118, 97,108,117,101, 44,
 32, 39, 34, 39, 32, 41, 32, 41, 32,116,104,101,110, 13, 10,  9,  9,  9,  9,115,
116,114, 32, 61, 32, 39, 91, 91, 39, 32, 46, 46, 32,118, 97,108,117,101, 32, 46,
 46, 32, 39, 93, 93, 39, 13, 10,  9,  9,  9,101,108,115,101, 13, 10,  9,  9,  9,
  9,115,116,114, 32, 61, 32, 39, 34, 39, 32, 46, 46, 32,118, 97,108,117,101, 32,
 46, 46, 32, 39, 34, 39, 13, 10,  9,  9,  9,101,110,100, 13, 10,  9,  9,101,108,
115,101, 13, 10,  9,  9,  9,115,116,114, 32, 61, 32,116,111,115,116,114,105,110,
103, 40, 32,118, 97,108,117,101, 32, 41, 13, 10,  9,  9,101,110,100, 13, 10,  9,
101,108,115,101, 13, 10,  9,  9,108,111, 99, 97,108, 32, 97,117,120, 84, 97, 98,
108,101, 32, 61, 32,123,125, 13, 10, 13, 10,  9,  9,116, 97, 98,108,101, 46,102,
111,114,101, 97, 99,104, 40, 32,118, 97,108,117,101, 44, 32,102,117,110, 99,116,
105,111,110, 40,105, 44, 32,118, 41, 13, 10,  9,  9,  9,  9,105,102, 40, 32,116,
111,110,117,109, 98,101,114, 40,105, 41, 32,126, 61, 32,105, 32, 41, 32,116,104,
101,110, 13, 10,  9,  9,  9,  9,  9,116, 97, 98,108,101, 46,105,110,115,101,114,
116, 40, 32, 97,117,120, 84, 97, 98,108,101, 44, 32,105, 32, 41, 13, 10,  9,  9,
  9,  9,101,108,115,101, 13, 10,  9,  9,  9,  9,  9,116, 97, 98,108,101, 46,105,
110,115,101,114,116, 40, 32, 97,117,120, 84, 97, 98,108,101, 44, 32,116,111,115,
116,114,105,110,103, 40,105, 41, 32, 41, 13, 10,  9,  9,  9,  9,101,110,100, 13,
 10,  9,  9,  9,101,110,100, 32, 41, 13, 10, 13, 10,  9,  9,116, 97, 98,108,101,
 46,115,111,114,116, 40, 32, 97,117,120, 84, 97, 98,108,101, 32, 41, 13, 10, 13,
 10,  9,  9,115,116,114, 32, 61, 32,115,116,114, 32, 46, 46, 32, 39, 92,110, 39,
 32, 46, 46, 32,105,110,100,101,110,116, 32, 46, 46, 32, 39,123, 92,110, 39, 13,
 10, 13, 10,  9,  9,108,111, 99, 97,108, 32,115,101,112, 97,114, 97,116,111,114,
 32, 61, 32, 34, 34, 13, 10,  9,  9,108,111, 99, 97,108, 32,101,110,116,114,121,
 32, 61, 32, 34, 34, 13, 10, 13, 10,  9,  9,116, 97, 98,108,101, 46,102,111,114,
101, 97, 99,104,105, 40, 32, 97,117,120, 84, 97, 98,108,101, 44, 32,102,117,110,
 99,116,105,111,110, 40, 32,105, 44, 32,102,105,101,108,100, 78, 97,109,101, 32,
 41, 13, 10,  9,  9,  9,  9,101,110,116,114,121, 32, 61, 32,105,110,100,101,110,
116, 32, 46, 46, 32, 34, 92,116, 34, 32, 46, 46, 32, 39, 34, 39, 46, 46,102,105,
101,108,100, 78, 97,109,101, 46, 46, 39, 34, 39, 46, 46, 34, 32, 61, 32, 34, 13,
 10, 13, 10,  9,  9,  9,  9,105,102, 40, 32, 40,116,111,110,117,109, 98,101,114,
 40,102,105,101,108,100, 78, 97,109,101, 41, 41, 32, 97,110,100, 32, 40,116,111,
110,117,109, 98,101,114, 40,102,105,101,108,100, 78, 97,109,101, 41, 32, 62, 32,
 48, 41, 32, 41, 32,116,104,101,110, 13, 10,  9,  9,  9,  9,  9,101,110,116,114,
121, 32, 61, 32,101,110,116,114,121, 32, 46, 46, 32, 80,117, 98,108,105, 99, 46,
116,111,115,116,114,105,110,103, 40, 32,118, 97,108,117,101, 91, 32,116,111,110,
117,109, 98,101,114, 40, 32,102,105,101,108,100, 78, 97,109,101, 32, 41, 32, 93,
 44, 32,105,110,100,101,110,116, 32, 46, 46, 32, 34, 92,116, 34, 32, 41, 13, 10,
  9,  9,  9,  9,101,108,115,101, 13, 10,  9,  9,  9,  9,  9,101,110,116,114,121,
 32, 61, 32,101,110,116,114,121, 32, 46, 46, 32, 80,117, 98,108,105, 99, 46,116,
111,115,116,114,105,110,103, 40, 32,118, 97,108,117,101, 91, 32,102,105,101,108,
100, 78, 97,109,101, 32, 93, 44, 32,105,110,100,101,110,116, 32, 46, 46, 32, 34,
 92,116, 34, 32, 41, 13, 10,  9,  9,  9,  9,101,110,100, 13, 10, 13, 10,  9,  9,
  9,  9,115,116,114, 32, 61, 32,115,116,114, 32, 46, 46, 32,115,101,112, 97,114,
 97,116,111,114, 32, 46, 46, 32,101,110,116,114,121, 13, 10,  9,  9,  9,  9,115,
101,112, 97,114, 97,116,111,114, 32, 61, 32, 34, 92,110, 34, 13, 10,  9,  9,  9,
101,110,100, 32, 41, 13, 10, 13, 10,  9,  9,  9,115,116,114, 32, 61, 32,115,116,
114, 32, 46, 46, 32, 39, 92,110, 39, 32, 46, 46, 32,105,110,100,101,110,116, 32,
 46, 46, 32, 39,125, 92,110, 39, 13, 10,  9,  9,101,110,100, 13, 10, 13, 10,  9,
114,101,116,117,114,110, 32,115,116,114, 13, 10,101,110,100, 13, 10, 13, 10,102,
117,110, 99,116,105,111,110, 32,108,111,103,103,105,110,103, 46,102,105,108,101,
 40,102,105,108,101,110, 97,109,101, 44, 32,100, 97,116,101, 80, 97,116,116,101,
114,110, 44, 32,108,111,103, 80, 97,116,116,101,114,110, 41, 13, 10, 13, 10, 32,
 32, 32, 32,105,102, 32,116,121,112,101, 40,102,105,108,101,110, 97,109,101, 41,
 32,126, 61, 32, 34,115,116,114,105,110,103, 34, 32,116,104,101,110, 13, 10, 32,
 32, 32, 32, 32, 32, 32, 32,102,105,108,101,110, 97,109,101, 32, 61, 32, 34,108,
117, 97,108,111,103,103,105,110,103, 46,108,111,103, 34, 13, 10, 32, 32, 32, 32,
101,110,100, 13, 10, 32, 32, 32, 32,102,105,108,101,110, 97,109,101, 32, 61, 32,
115,116,114,105,110,103, 46,102,111,114,109, 97,116, 40,102,105,108,101,110, 97,
109,101, 44, 32,111,115, 46,100, 97,116,101, 40,100, 97,116,101, 80, 97,116,116,
101,114,110, 41, 41, 13, 10, 13, 10, 32, 32, 32, 32,114,101,116,117,114,110, 32,
108,111,103,103,105,110,103, 46,110,101,119, 40, 32,102,117,110, 99,116,105,111,
110, 40,115,101,108,102, 44, 32,108,101,118,101,108, 44, 32,109,101,115,115, 97,
103,101, 41, 13, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,108,111, 99, 97,108, 32,102,
 32, 61, 32,105,111, 46,111,112,101,110, 40,102,105,108,101,110, 97,109,101, 44,
 32, 34, 97, 34, 41, 13, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,105,102, 32,110,111,
116, 32,102, 32,116,104,101,110, 13, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32,114,101,116,117,114,110, 32,110,105,108, 44, 32,115,116,114,105,110,103,
 46,102,111,114,109, 97,116, 40, 34,102,105,108,101, 32, 96, 37,115, 39, 32, 99,
111,117,108,100, 32,110,111,116, 32, 98,101, 32,111,112,101,110,101,100, 32,102,
111,114, 32,119,114,105,116,105,110,103, 34, 44, 32,102,105,108,101,110, 97,109,
101, 41, 13, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,101,110,100, 13, 10, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32,108,111, 99, 97,108, 32,115, 32, 61, 32,109,101,115,115, 97,
103,101, 32, 46, 46, 32, 34, 92,110, 34, 45, 45,108,111,103,103,105,110,103, 46,
112,114,101,112, 97,114,101, 76,111,103, 77,115,103, 40,108,111,103, 80, 97,116,
116,101,114,110, 44, 32,111,115, 46,100, 97,116,101, 40, 41, 44, 32,108,101,118,
101,108, 44, 32,109,101,115,115, 97,103,101, 41, 13, 10, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32,102, 58,119,114,105,116,101, 40,115, 41, 13, 10, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32,102, 58, 99,108,111,115,101, 40, 41, 13, 10, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 13, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,114,101,116,117,114,110, 32,116,114,
117,101, 13, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32,101,110,100, 13, 10, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 41, 13, 10,101,110,
100, 13, 10, 13, 10,102,117,110, 99,116,105,111,110, 32,108,111,103,103,105,110,
103, 46, 99,111,110,115,111,108,101, 40,108,111,103, 80, 97,116,116,101,114,110,
 41, 13, 10, 13, 10, 32, 32, 32, 32,114,101,116,117,114,110, 32,108,111,103,103,
105,110,103, 46,110,101,119, 40, 32, 32,102,117,110, 99,116,105,111,110, 40,115,
101,108,102, 44, 32,108,101,118,101,108, 44, 32,109,101,115,115, 97,103,101, 41,
 13, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,105,111, 46,115,116,100,111,117,116, 58,
119,114,105,116,101, 40,108,111,103,103,105,110,103, 46,112,114,101,112, 97,114,
101, 76,111,103, 77,115,103, 40,108,111,103, 80, 97,116,116,101,114,110, 44, 32,
111,115, 46,100, 97,116,101, 40, 41, 44, 32,108,101,118,101,108, 44, 32,109,101,
115,115, 97,103,101, 41, 41, 13, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,114,101,116,
117,114,110, 32,116,114,117,101, 13, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,101,110,100, 13, 10, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 41, 13, 10,101,110,100,
};

static const uint32	maxMessageLength = 4096;

/*
	CLog().

*/
CLog::CLog() : m_bActive( false ), m_pFile(NULL)
{
}


/*
	~CLog().

*/
CLog::~CLog()
{
	//	Mark singleton as shutdown, to track unwanted access after this point.
	SingletonActive( false );
}

/*
	Startup().

*/
const bool	CLog::Startup()
{
	//if( (
		//m_pStdout = freopen( "debug.txt", "a+", stdout ); 
			//) == NULL )
		//exit(-1);
	m_pState = NULL;
	m_pFile = NULL;
	return( true );
}

/*
	Shutdown().

*/
const bool	CLog::Shutdown( void )
{
	Info( "Logger shutting down..." );
	Info( "dummy" );
	Detach();

	//if( m_pStdout )
        //fclose( m_pStdout );

	return( true );
}

/*
*/
void	CLog::Attach( const std::string &_location, const uint32 _level )
{
	time_t	curTime;
	time( &curTime );

	char	timeStamp[32] = { 0 };
	strftime( timeStamp, sizeof(timeStamp), "%Y_%m_%d", localtime( &curTime ) );

    std::stringstream s;
	s << _location << timeStamp << ".log";
	std::string f = s.str();
    if( (m_pFile = freopen( f.c_str(), "a+", stdout ) )== NULL )
	{
#ifdef WIN32
		MessageBoxA(NULL, "Unable to create log file, exiting. Please check file permissions for electricsheep folder.", "Log error", MB_OK);
#endif
        exit( -1 );
	}

/*
	m_pState = new Base::Script::CLuaState();
	m_pState->Init( "" );

	//	Load embedded scripts.
	if( luaL_loadbuffer( m_pState->GetState(),(const char*)luaLogging, sizeof(luaLogging), "logging.lua" ) == 0 )
		lua_pcall( m_pState->GetState(), 0, 0, 0 );

	//
	m_pState->Execute( "require 'os'" );
	m_pState->Execute( "require 'io'" );
	m_pState->Execute( "require 'table'" );
	m_pState->Execute( "require 'string'" );
	m_pState->Execute( "function createLogFile( _location )	logfile = logging.file( _location .. '%s.log', '%y-%m-%d' )	end" );
	m_pState->Execute( "function g_Log( _level, _file, _func, _line, _timestamp, _msg )	logfile:log( _level, _timestamp .. '->['.._level..']:'.._func .. '('..tostring(_line)..'): '.. tostring(_msg) ) end" );
	m_pState->Pop( Script::Call( m_pState->GetState(), "createLogFile", "s", _location.c_str() ) );
*/
	m_bActive = true;

}

/*
*/
void	CLog::Detach( void )
{
	m_bActive = false;
	SAFE_DELETE( m_pState );

	if( m_pFile )
        fclose( m_pFile );
}

/*
	SetFile().
	Store file that did the logging.
*/
void	CLog::SetInfo( const char *_pFileStr, const uint32 _line, const char *_pFunc )
{
	//boost::mutex::scoped_lock locker( m_Lock );

	std::string tmp = _pFileStr;
	size_t offs = tmp.find_last_of( "/\\", tmp.size() );

	m_File = tmp.substr( offs+1, tmp.size() );
	m_Function = std::string( _pFunc );
	m_Line = _line;
}

/*
*/

char CLog::s_MessageSpam[ m_MaxMessageLength ] = {0};
char CLog::s_MessageType[ m_MaxMessageLength ] = {0};
size_t CLog::s_MessageSpamCount = 0;

void	CLog::Log( const char *_pType, /*const char *_file, const uint32 _line, const char *_pFunc,*/ const char *_pStr )
{
	//boost::mutex::scoped_lock locker( m_Lock );

	if (m_pFile == NULL)
		return;

	time_t	curTime;
	time( &curTime );

	char	timeStamp[32] = { 0 };
	strftime( timeStamp, sizeof(timeStamp), "%H:%M:%S", localtime( &curTime ) );

	/* log spam start*/
	if (strcmp(_pStr, s_MessageSpam) == 0)
	{
		++s_MessageSpamCount;
		return;
	} else
	{
		if (s_MessageSpamCount > 0)
		{
			if( m_bActive )
			{
				fprintf( m_pFile, "[%s-%s]: '%s' x%lu\n", s_MessageType, timeStamp, s_MessageSpam, s_MessageSpamCount );
				fflush( m_pFile );
			}
			else
			{
				fprintf( stdout, "[%s-%s]: '%s' x%lu\n", s_MessageType, timeStamp, s_MessageSpam, s_MessageSpamCount );
				fflush( stdout );
			}
		} else
		{
			if( !m_bActive )
			{
				//	Not active/attached, dump to stdout.
				//fprintf( stdout, "[%s]: %s - %s[%s(%d)]: '%s'\n", _pType, timeStamp, _file, _pFunc, m_Line, _pStr );
				fprintf( stdout, "[%s-%s]: '%s'\n", s_MessageType, timeStamp, s_MessageSpam );
				fflush( stdout );
			}
			else
			{
				//fprintf( m_pFile, "[%s]: %s - %s[%s(%d)]: '%s'\n", _pType, timeStamp, _file, _pFunc, m_Line, _pStr );
				fprintf( m_pFile, "[%s-%s]: '%s'\n", s_MessageType, timeStamp, s_MessageSpam );
				fflush( m_pFile );
			}
		}

		s_MessageSpamCount = 0;
		memcpy(s_MessageSpam, _pStr, m_MaxMessageLength);
		strcpy(s_MessageType, _pType);
		return;
	}
	/* log spam end */

	if( !m_bActive )
	{
		//	Not active/attached, dump to stdout.
		//fprintf( stdout, "[%s]: %s - %s[%s(%d)]: '%s'\n", _pType, timeStamp, _file, _pFunc, m_Line, _pStr );
		fprintf( stdout, "[%s-%s]: '%s'\n", _pType, timeStamp, _pStr );
		fflush( stdout );
	}
	else
	{
		//fprintf( m_pFile, "[%s]: %s - %s[%s(%d)]: '%s'\n", _pType, timeStamp, _file, _pFunc, m_Line, _pStr );
		fprintf( m_pFile, "[%s-%s]: '%s'\n", _pType, timeStamp, _pStr );
		fflush( m_pFile );

		/*lua_State *pState = m_pState->GetState();

		uint32	stackSizeIn = lua_gettop( pState );
		lua_getglobal( pState, "g_Log" );

		lua_pushstring( pState, _pType );
		lua_pushstring( pState, _file );
		lua_pushstring( pState, _pFunc );
		lua_pushinteger( pState, _line );
		lua_pushstring( pState, timeStamp );
		lua_pushstring( pState, _pStr );

		luaL_checkstack( pState, 1, "Too many arguments" );

		int status = lua_pcall( pState, 6, 0, 0 );
		if( status != 0 )
		{
			if( !lua_isnil( pState, -1 ) )
			fprintf( stdout, "LUA ERROR: %s\n", lua_tostring( pState, -1 ) );

			fprintf( stdout, "[%s]: %s - %s[%s(%d)]: '%s'\n", _pType, timeStamp, _file, _pFunc, m_Line, _pStr );
			fflush( stdout );

			//	Detach.
			Detach();
		}*/
	}
}

#define	grabvarargs	\
	va_list	ArgPtr;	\
	char	pTempStr[ m_MaxMessageLength ];	\
	va_start( ArgPtr, _pFmt );	\
	vsnprintf( pTempStr, m_MaxMessageLength, _pFmt, ArgPtr );	\
	va_end( ArgPtr );

//	Def our loggers.
void	CLog::Debug( const char *_pFmt, ... )	{	grabvarargs	Log( "DEBUG", /*m_File.c_str(), m_Line, m_Function.c_str(),*/ pTempStr );	}
void	CLog::Info( const char *_pFmt, ... )	{	grabvarargs	Log( "INFO", /*m_File.c_str(), m_Line, m_Function.c_str(),*/ pTempStr );	}
void	CLog::Warning( const char *_pFmt, ... )	{	grabvarargs	Log( "WARN", /*m_File.c_str(), m_Line, m_Function.c_str(),*/ pTempStr );	}
void	CLog::Error( const char *_pFmt, ... )	{	grabvarargs	Log( "ERROR", /*m_File.c_str(), m_Line, m_Function.c_str(),*/ pTempStr );	}
void	CLog::Fatal( const char *_pFmt, ... )	{	grabvarargs	Log( "FATAL", /*m_File.c_str(), m_Line, m_Function.c_str(),*/ pTempStr );	}

};
