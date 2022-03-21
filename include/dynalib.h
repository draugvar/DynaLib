// <copyright>
// Copyright (c) 2022 Salvatore Rivieccio
//
// This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
// To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/.
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// </copyright>
//
// <author>Salvatore Rivieccio</author>
// <date>20/03/2022</date>
//

#pragma once

#include <memory>
#include <iostream>

#if _WIN32
#include <Windows.h>
#else
#include <dlfcn.h> //dlopen
#include <unordered_map>
#include <any>
#include <functional>

#endif

namespace dl
{
	template<typename T>
	class DynaLib
	{
	public:
		DynaLib() = default;
		~DynaLib()
		{
			shutdown();
			if (handle_)
			{
				closeLib(handle_);
			}
		};

	private:
		bool printLibError()
		{
#if _WIN32
			std::cerr << GetLastError() << std::endl;
#else
			std::cerr << dlerror() << std::endl;
#endif
			return false;
		}

		void *openLib(const std::string &aLibName)
		{
#if _WIN32
			return LoadLibrary((aLibName + ".dll").c_str());
#else
			return dlopen((aLibName + ".so").c_str(), RTLD_LAZY);
#endif
		}

		int closeLib(void *aLibHandle)
		{
#if _WIN32
			return FreeLibrary((HMODULE)aLibHandle);
#else
			return dlclose(aLibHandle);
#endif
		}

		void *loadSymbol(void *aLibHandle, const char *aSym)
		{
#if _WIN32
			return (void *)GetProcAddress((HMODULE)aLibHandle, aSym);
#else
			return dlsym(aLibHandle, aSym);
#endif
		}

	public:
		bool open(const std::string &aFilename, const char *aCreateSym = nullptr,
		          const char *aDestroySym = nullptr)
		{
			if (!(handle_ = openLib(aFilename)))
				return printLibError();
			if (aCreateSym != nullptr && !(create_ = (T(*)()) loadSymbol(handle_, aCreateSym)))
				return printLibError();
			if (aDestroySym != nullptr && !(destroy_ = (T(*)()) loadSymbol(handle_, aDestroySym)))
				return printLibError();
			return true;
		}

		T init()
		{
			if (this->create_)
			{
				return this->create_();
			} else return T(-1);
		}

		T shutdown()
		{
			if (this->destroy_)
			{
				return this->destroy_();
			} else return T(-1);
		}

		template<typename ...Args>
		T callFunction(const char *aFunctionName, Args... aArgs)
		{
			typedef T(*FunctionHandler)(Args...);
			if(functionsMap.find(aFunctionName) != functionsMap.end())
			{
				auto function = (FunctionHandler)functionsMap[aFunctionName];
				return function(aArgs...);
			}

			FunctionHandler function;
			if (!(function = (FunctionHandler) loadSymbol(handle_, aFunctionName)))
			{
				printLibError();
				return T(-1);
			}

			functionsMap[aFunctionName] = (void*)function;

			return function(aArgs...);
		}

	private:
		void *handle_{nullptr};
		T(*create_)() = nullptr;
		T(*destroy_)() = nullptr;
		std::unordered_map<std::string, void*> functionsMap{};
	};
}