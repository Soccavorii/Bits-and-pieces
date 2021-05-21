#pragma once

#include <sys/stat.h>
#include <fstream>
#include <string>
#include <climits>
#include <cstdlib>

#ifdef _WINDOWS
#include <Windows.h>
#include <locale>
#include <codecvt>
#include <direct.h>
std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
#define widen(x)	converter.from_bytes(x)
#define narrow(x)	converter.to_bytes(x)
#define MKDIR(dir)	_wmkdir(widen(dir).c_str())
#define RMDIR(dir)	_wrmdir(widen(dir).c_str())
#define realpath(src, dest)	_wfullpath(dest, src, MAX_PATH)
#else
#define _GNU_SOURCE
#define MAX_PATH	PATH_MAX
#define MKDIR(dir)	mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#define RMDIR(dir)	rmdir(dir.c_str())
#endif

namespace FileOperation {
	bool IsExist(const std::string path) {
		struct stat s;
		if (stat(path.c_str(), &s) != 0)
			return false;
		return true;
	}
	
	bool Copy(const std::string src, const std::string dest, bool parents = true) {
		if (src == dest)
			return false;

#ifdef _WINDOWS
		return CopyFile(widen(src).c_str(), widen(dest).c_str(), false);
#else
		std::ifstream f1(src, std::ios::binary);
		if (!f1.is_open())
			return false;
		std::ofstream f2(dest, std::ios::binary);
		if (!f2.is_open())
			return false;
		f2 << f1.rdbuf();
		return true;
#endif
	}

	bool Remove(const std::string path, bool recursive = true) {
		return std::remove(path.c_str()) == 0;
	}

	/*
	* NOTE:
	*	Fails if src & dest are not on the same mounted device
	* try use copy + remove instead
	*	Fails if directory not exist
	*/
	bool Move(const std::string src, const std::string dest, bool parents = true) {
		if (src == dest)
			return false;
		return std::rename(src.c_str(), dest.c_str()) == 0;
	}

	bool MakeDir(const std::string dir, bool parents = true) {
		if (parents) {

		}
		else {
			auto res = MKDIR(dir);
			return (res == 0 || res == EEXIST) ? true : false;
		}
	}
	bool RemoveDir(const std::string dir) {
		return RMDIR(dir) == 0;
	}
	
	/* Split Path
	* Note:
	*	Will not resolve path
	*	Will not validate path
	*/
	std::string DirName(const std::string path) {
		std::size_t pos = path.find_last_of("\\/");
		return pos == std::string::npos ? std::string() : path.substr(0, pos + 1);
	}
	std::string FileName(const std::string path) {
		std::size_t pos = path.find_last_of("\\/");
		return pos < path.size() - 1 ? path.substr(pos + 1, std::string::npos) : std::string();
	}
	std::string ExtName(const std::string path) {
		std::size_t pos = path.find_last_of(".");
		return pos < path.size() - 1 ? path.substr(pos + 1, std::string::npos) : std::string();
	}
	std::string RealPath(const std::string path) {
		wchar_t buffer[MAX_PATH];
		if (realpath(widen(path).c_str(), buffer) == NULL)
			return std::string();
		return narrow(buffer);
	}
}