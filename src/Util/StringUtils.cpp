/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Util/StringUtils.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"


// C++ Standard Library includes
#include <locale>
#include <codecvt>
#include <algorithm> 
#include <cctype>
#include <iomanip>
#include <sstream>
#include <experimental/filesystem>

// #pragma warning( push )
// #pragma warning( disable : 4996)

#if defined(WIN32) || defined(_WIN32)
#define SEPARATOR '\\'
#else
#define SEPARATOR '/'
#endif

using namespace std::experimental;
using namespace std;

namespace astu {

	// wstring StringUtils::utf8ToUtf16(const string & s)
	// {
	// 	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
	// 	return conversion.from_bytes(s);
	// }

	// string StringUtils::utf16ToUtf8(const wstring & ws)
	// {
	// 	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
	// 	return conversion.to_bytes(ws);
	// }

	// string StringUtils::utf16ToUtf8(const wchar_t & wc)
	// {
	// 	wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
	// 	return conversion.to_bytes(wc);
	// }

	string & StringUtils::RightTrim(string & s)
	{
		s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
			return !(isspace(ch) || iscntrl(ch));
		}).base(), s.end());

		return s;
	}

	string & StringUtils::LeftTrim(string & s)
	{
		s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
			return !isspace(ch);
		}));
		return s;
	}

	string & StringUtils::Trim(string & s) {
        RightTrim(s);
        LeftTrim(s);
		return s;
	}

	string StringUtils::BytesToString(size_t mem, int precision)
	{
		if (mem < 1024) {
			return to_string(mem) + " bytes";
		}
		else if (mem < 1024 * 1024) {
			double s = static_cast<double>(mem) / 1024;
			if (s == static_cast<long>(s)) {
				return to_string(static_cast<long>(s)) + " KiB";
			}
			else {
				stringstream stream;
				stream << fixed << setprecision(precision) << s << " KiB";
				return stream.str();
			}
		}
		else if (mem < 1024 * 1024 * 1024) {
			double s = static_cast<double>(mem) / (1024 * 1024);
			if (s == static_cast<long>(s)) {
				return to_string(static_cast<long>(s)) + " MiB";
			}
			else {
				stringstream stream;
				stream << fixed << setprecision(precision) << s << " MiB";
				return stream.str();
			}
		}
		else {
			double s = static_cast<double>(mem) / (1024 * 1024 * 1024);
			if (s == static_cast<long>(s)) {
				return to_string(static_cast<long>(s)) + " GiB";
			}
			else {
				stringstream stream;
				stream << fixed << setprecision(precision) << s << " GiB";
				return stream.str();
			}
		}
		return string();
	}

	string & StringUtils::ToUpperCase(string & s)
	{
		transform(s.begin(), s.end(), s.begin(), ::toupper);
		return s;
	}

	string & StringUtils::ToLowerCase(string & s)
	{
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		return s;
	}

	vector<string> StringUtils::Split(const string & s, char ch)
	{
		vector<string> result;

		if (s.empty())
			return result;

		size_t pos1 = 0;
		size_t pos2 = s.find_first_of(ch, pos1);
		result.push_back(s.substr(pos1, pos2 == string::npos ? string::npos : pos2 - pos1));
		while (pos2 != string::npos) {
			pos1 = pos2 + 1;
			pos2 = s.find_first_of(ch, pos1);
			result.push_back(s.substr(pos1, pos2 == string::npos ? string::npos : pos2 - pos1));
		}

		return result;
	}

	string StringUtils::ToHexString(unsigned long value, int width)
	{
		stringstream stream;

        stream << "0x";
        if (width > 0) {
            stream << std::setfill('0') << std::setw(width);
        }
		stream << hex << value;
		return stream.str();
	}

	string StringUtils::ToString(void * pointer)
	{
		stringstream stream;
		stream << "0x" << hex << pointer;
		return stream.str();
	}

	string StringUtils::ToString(float value, int precision)
	{
		stringstream stream;
		stream << fixed << setprecision(precision) << value;
		return stream.str();
	}

	string StringUtils::ToString(double value, int precision)
	{
		stringstream stream;
		stream << fixed << setprecision(precision) << value;
		return stream.str();
	}

	bool StringUtils::StartsWith(const string & s, const string & seq)
	{
		return s.rfind(seq, 0) == 0;
	}

	bool StringUtils::EndsWith(const string & s, const string & seq)
	{
		if (seq.size() > s.size())
			return false;

		return equal(seq.rbegin(), seq.rend(), s.rbegin());
	}

	bool StringUtils::StartsWith(const string & s, char ch)
	{
		return s.size() >= 1 && s[0] == ch;
	}

	 string StringUtils::ExtractPath(const string &s)
	 {
        auto pos = s.find_last_of('/');
        if (pos == std::string::npos) {
            pos = s.find_last_of('\\');
            if (pos == std::string::npos) {
                return s;
            }
        }

        return s.substr(0, pos + 1);

        //Using this if file system extension is present.
	 	//filesystem::path path(s);
	 	//return path.parent_path().string();
	 }

    // string StringUtils::extractFilename(const string & s)
	// {
	// 	filesystem::path path(s);
	// 	return path.filename().string();
	// }

	// string StringUtils::extractStem(const string & s)
	// {
	// 	filesystem::path path(s);
	// 	return path.stem().string();
	// }

	// string StringUtils::extractExtension(const string & s)
	// {
	// 	filesystem::path path(s);
	// 	return path.extension().string();
	// }

	char StringUtils::pathSeparator()
	{
		return SEPARATOR;
	}

	string & StringUtils::Replace(string & s, const string & from, const string & to)
	{
		size_t start_pos = 0;
		while ((start_pos = s.find(from, start_pos)) != string::npos) {
			s.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
		}
		return s;
	}

    string astu::StringUtils::ExtractFileExtension(const string &filename, bool keepPeriod) {
        auto idx = filename.find_last_of('.');
        if (idx == string::npos) {
            return "";
        }

        return filename.substr(idx + (keepPeriod ? 0 : 1));
    }

    string StringUtils::StripFileExtension(const string &filename) {
        auto idx = filename.find_last_of('.');
        return filename.substr(0, idx);
    }

    std::string StringUtils::ToPrintable(char ch, bool includeHex) {
        string result;

        switch (ch) {
            case '\t':
                result += "'\t'";
                break;

            case '\n':
                result += "'\n'";
                break;

            case '\r':
                result += "'\r'";
                break;

            default:
                if (isprint(ch)) {
                    result += "'"; result += ch; result += "'";
                } else {
                    result = "(" + ToHexString(ch) + ")";
                    return result;
                }
        }

        if (includeHex) {
            result += " (" + ToHexString(ch) + ")";
        }

        return result;
    }

} // end of namespace

//#pragma warning( pop )