#pragma once
//#ifndef __ENGINE_DEFINES_H__
//#define __ENGINE_DEFINES_H__

#pragma warning(disable: 4251)	//
#pragma warning(disable: 4172)	//

#define MAX_ASCIIART_LEN 4096
#define MAX_BUFFER_LEN 2048
#define MAX_STRING_LEN 128
#define MAX_HEADER_LEN 24
#define MAX_WIDTH_LEN 5

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include<crtdbg.h>
#endif //_DEBUG

using namespace std;
namespace System {}

//STLs
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <time.h>

//#endif//!__ENGINE_DEFINES_H__