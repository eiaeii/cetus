#include "url.h"
#include "stringutil/stringutil.h"
using namespace terra;

static bool ValidNetChar(const char* c)
{
	// NOTE: We purposely allow for SPACE characters inside URL strings, since we need to support player aliases
	//   on the URL that potentially have spaces in them.

	// @todo: Support true URL character encode/decode (e.g. %20 for spaces), so that we can be compliant with
	//   URL protocol specifications

	// NOTE: EQUALS characters (=) are not checked here because they're valid within fragments, but incoming
	//   option data should always be filtered of equals signs

	// / Is now allowed because absolute paths are required in various places

	if (::strchr(c, '?') || ::strchr(c, '#'))		// ? and # delimit fragments
	{
		return false;
	}

	return true;
}


URL::URL(const char* local_filename)
	: protocol(url_config.DefaultProtocol)
	, host(url_config.DefaultHost)
	, port(url_config.DefaultPort)
	, op()
	, portal(url_config.DefaultPortal)
	, valid(1)
{
	// strip off any extension from map name
	map_name = "Entry";
}

//
// Helper function.
//
static inline char* HelperStrchr(char* src, char a, char b)
{
	char* aa = ::strchr(src, a);
	char* bb = ::strchr(src, b);
	return (aa && (!bb || aa<bb)) ? aa : bb;
}


/**
* Static: Removes any special URL characters from the specified string
*
* @param Str String to be filtered
*/
void URL::FilterURLString(std::string& str)
{
	std::string newstr;
	for (size_t cur_char_index = 0; cur_char_index < str.length(); ++cur_char_index)
	{
		const char cur_char = str[cur_char_index];

		if (cur_char != ':' && cur_char != '?' && cur_char != '#' && cur_char != '=')
		{
			newstr.push_back(str[cur_char_index]);
		}
	}

	str = newstr;
}




URL::URL(URL* Base, const char* text_url, ETravelType travel_type)
	: protocol(url_config.DefaultProtocol)
	, host(url_config.DefaultHost)
	, port(url_config.DefaultPort)
	, map_name("Entry")
	, op()
	, portal(url_config.DefaultPortal)
	, valid(1)
{
	//assert(text_url);

	//if (!defaults_initialized)
	//{
	//	URL::StaticInit();
	//	protocol = url_config.DefaultProtocol;
	//	host = url_config.DefaultHost;
	//	port = url_config.DefaultPort;
	//	portal = url_config.DefaultPortal;
	//}

	//// Make a copy.
	//const int32_t url_length = ::strlen(text_url);
	//char* temp_url = new char[url_length + 1];
	//char* url = temp_url;
	//strcpy(temp_url, text_url);

	//// Copy Base.
	//if (travel_type == TRAVEL_Relative)
	//{
	//	assert(Base);
	//	protocol = Base->protocol;
	//	host = Base->host;
	//	map_name = Base->map_name;
	//	portal = Base->portal;
	//	port = Base->port;
	//}
	//if (travel_type == TRAVEL_Relative || travel_type == TRAVEL_Partial)
	//{
	//	assert(Base);
	//	for (int32_t i = 0; i<Base->op.size(); i++)
	//	{
	//		op.emplace_back(std::string(Base->op[i]));
	//	}
	//}

	//// Skip leading blanks.
	//while (*url == ' ')
	//{
	//	url++;
	//}

	//// Options.
	//char* s = HelperStrchr(url, '?', '#');
	//if (s)
	//{
	//	char OptionChar = *s, NextOptionChar = 0;
	//	*(s++) = 0;
	//	do
	//	{
	//		char* t = HelperStrchr(s, '?', '#');
	//		if (t)
	//		{
	//			NextOptionChar = *t;
	//			*(t++) = 0;
	//		}
	//		if (!ValidNetChar(s))
	//		{
	//			*this = URL();
	//			valid = 0;
	//			break;
	//		}
	//		if (OptionChar == '?')
	//		{
	//			if (s && s[0] == '-')
	//			{
	//				// Remove an option if it starts with -
	//				s++;
	//				RemoveOption(s);
	//			}
	//			else
	//			{
	//				AddOption(s);
	//			}
	//		}
	//		else
	//		{
	//			portal = s;
	//		}
	//		s = t;
	//		OptionChar = NextOptionChar;
	//	} while (s);
	//}

	//if (valid == 1)
	//{
	//	// Handle pure filenames & Posix paths.
	//	bool FarHost = 0;
	//	bool FarMap = 0;
	//	if (::strlen(url)>2 && ((url[0] != '[' && url[1] == ':') || url[0] == '/'))
	//	{
	//		// Pure filename.
	//		protocol = url_config.DefaultProtocol;
	//		map_name = url;
	//		portal = url_config.DefaultPortal;
	//		url = nullptr;
	//		FarHost = 1;
	//		FarMap = 1;
	//		host = "";
	//	}
	//	else
	//	{
	//		// Determine location of the first opening square bracket.
	//		// Square brackets enclose an IPv6 address.
	//		const char* square_bracket = strchr(url, '[');

	//		// Parse protocol. Don't consider colons that occur after the opening square
	//		// brace, because they are valid characters in an IPv6 address.
	//		if
	//			((strchr(url, ':') != NULL)
	//				&& (strchr(url, ':')>url + 1)
	//				&& (!square_bracket || (strchr(url, ':')<square_bracket))
	//				&& (strchr(url, '.') == NULL || strchr(url, ':')<strchr(url, '.')))
	//		{
	//			char* ss = url;
	//			url = strchr(url, ':');
	//			*(url++) = 0;
	//			protocol = ss;
	//		}

	//		// Parse optional leading double-slashes.
	//		if (*url == '/' && *(url + 1) == '/')
	//		{
	//			url += 2;
	//			FarHost = 1;
	//			host = "";
	//		}

	//		// Parse optional host name and port.
	//		const char* Dot = strchr(url, '.');
	//		const int32_t ExtLen = FPackageName::GetMapPackageExtension().Len();

	//		const bool bIsHostnameWithDot =
	//			(Dot)
	//			&& (Dot - url>0)
	//			&& (FCString::Strnicmp(Dot, *FPackageName::GetMapPackageExtension(), FPackageName::GetMapPackageExtension().Len()) != 0 || FChar::IsAlnum(Dot[ExtLen]))
	//			&& (FCString::Strnicmp(Dot + 1, *url_config.DefaultSaveExt, url_config.DefaultSaveExt.Len()) != 0 || FChar::IsAlnum(Dot[url_config.DefaultSaveExt.Len() + 1]))
	//			&& (FCString::Strnicmp(Dot + 1, "demo"), 4) != 0 || FChar::IsAlnum(Dot[5]));

	//		// Square bracket indicates an IPv6 address, but IPv6 addresses can contain dots also
	//		if (bIsHostnameWithDot || square_bracket)
	//		{
	//			char* ss = url;
	//			url = strchr(url, '/');
	//			if (url)
	//			{
	//				*(url++) = 0;
	//			}

	//			// Skip past all the ':' characters in the IPv6 address to get to the port.
	//			char* ClosingSquareBracket = strchr(ss, ']');

	//			char* PortText = ss;
	//			if (ClosingSquareBracket)
	//			{
	//				PortText = ClosingSquareBracket;
	//			}
	//			char* t = strchr(PortText, ':');
	//			if (t)
	//			{
	//				// Port.
	//				*(t++) = 0;
	//				port = FCString::Atoi(t);
	//			}

	//			if (square_bracket && ClosingSquareBracket)
	//			{
	//				// Trim the brackets from the host address
	//				*ClosingSquareBracket = 0;
	//				host = ss + 1;
	//			}
	//			else
	//			{
	//				// Normal IPv4 address
	//				host = ss;
	//			}

	//			if (FCString::Stricmp(*protocol, *url_config.DefaultProtocol) == 0)
	//			{
	//				map_name = UGameMapsSettings::GetGameDefaultMap();
	//			}
	//			else
	//			{
	//				map_name = "");
	//			}
	//			FarHost = 1;
	//		}
	//	}
	//}

	//// Parse optional map
	//if (valid == 1 && url && *url)
	//{
	//	// Map.
	//	if (*url != '/')
	//	{
	//		// find full pathname from short map name
	//		std::string MapFullName;
	//		FText MapNameError;
	//		bool bFoundMap = false;
	//		if (FPaths::FileExists(url))
	//		{
	//			map_name = FPackageName::FilenameToLongPackageName(url);
	//			bFoundMap = true;
	//		}
	//		else if (!FPackageName::DoesPackageNameContainInvalidCharacters(url, &MapNameError))
	//		{
	//			// First try to use the asset registry if it is available and finished scanning
	//			if (FModuleManager::Get().IsModuleLoaded("AssetRegistry"))
	//			{
	//				IAssetRegistry& AssetRegistry = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();

	//				if (!AssetRegistry.IsLoadingAssets())
	//				{
	//					TArray<FAssetData> MapList;
	//					if (AssetRegistry.GetAssetsByClass(UWorld::StaticClass()->GetFName(), /*out*/ MapList))
	//					{
	//						FName TargetTestName(url);
	//						for (const FAssetData& MapAsset : MapList)
	//						{
	//							if (MapAsset.AssetName == TargetTestName)
	//							{
	//								map_name = MapAsset.PackageName.ToString();
	//								bFoundMap = true;
	//								break;
	//							}
	//						}
	//					}
	//				}
	//			}

	//			if (!bFoundMap)
	//			{
	//				// Fall back to incredibly slow disk scan for the package
	//				//if (FPackageName::SearchForPackageOnDisk(std::string(url) + FPackageName::GetMapPackageExtension(), &MapFullName))
	//				//{
	//				//	map_name = MapFullName;
	//				//	bFoundMap = true;
	//				//}
	//			}
	//		}

	//		if (!bFoundMap)
	//		{
	//			// can't find file, invalidate and bail
	//			//UE_CLOG(MapNameError.ToString().Len() > 0, LogLongPackageNames, Warning, "URL: %s: %s"), url, *MapNameError.ToString());
	//			*this = URL();
	//			valid = 0;
	//		}
	//	}
	//	else
	//	{
	//		// already a full pathname
	//		map_name = url;
	//	}

	//}

	//// Validate everything.
	//// The FarHost check does not serve any purpose I can see, and will just cause valid URLs to fail (URLs with no options, why does a URL
	//// need an option to be valid?)
	//if (valid == 1 && (!ValidNetChar(protocol.c_str()) || !ValidNetChar(host.c_str()) /*|| !ValidNetChar(*Map)*/ || !ValidNetChar(portal.c_str()) /*|| (!FarHost && !FarMap && !Op.size())*/))
	//{
	//	*this = URL();
	//	valid = 0;
	//}

	//// If Valid == 1, success.

	//delete[] temp_url;
	
}

/*-----------------------------------------------------------------------------
Conversion to text.
-----------------------------------------------------------------------------*/

//
// Convert this URL to text.
//
std::string URL::ToString(bool fully_qualified) const
{
	std::string ret;

	// Emit protocol.
	if ((protocol != url_config.DefaultProtocol) || fully_qualified)
	{
		ret += protocol;
		ret += ":";

		if (host != url_config.DefaultHost)
		{
			ret += "//";
		}
	}

	// Emit host.
	if ((host != url_config.DefaultHost) || (port != url_config.DefaultPort))
	{
		ret += host;

		if (port != url_config.DefaultPort)
		{
			ret += ":";
			ret += StringUtils::Format("%i", port);
		}

		ret += "/";
	}

	// Emit map.
	if (map_name.length() > 0)
	{
		ret += map_name;
	}

	// Emit options.
	for (int32_t i = 0; i < op.size(); i++)
	{
		ret += "?";
		ret += op[i];
	}

	// Emit portal.
	if (portal.length() > 0)
	{
		ret += "#";
		ret += portal;
	}

	return ret;
}

/*-----------------------------------------------------------------------------
Informational.
-----------------------------------------------------------------------------*/

bool URL::IsInternal() const
{
	return protocol == url_config.DefaultProtocol;
}

bool URL::IsLocalInternal() const
{
	return IsInternal() && host.length() == 0;
}

void URL::AddOption(const char* str)
{
	int32_t match = strchr(str, '=') ? (strchr(str, '=') - str) : strlen(str);
	int32_t i;

	for (i = 0; i < op.size(); i++)
	{
		if (strnicmp(op[i].c_str(), str, match) == 0 && ((op[i])[match] == '=' || (op[i])[match] == '\0'))
		{
			break;
		}
	}

	if (i == op.size())
	{
		op.emplace_back(std::string(str));
	}
	else
	{
		op[i] = str;
	}
}


void URL::RemoveOption(const char* Key, const char* Section, const std::string& Filename)
{
	//if (!Key)
	//	return;

	//for (int32_t i = op.size() - 1; i >= 0; i--)
	//{
	//	if (op[i].Left(strlen(Key)) == Key)
	//	{
	//		FConfigSection* Sec = GConfig->GetSectionPrivate(Section ? Section : "DefaultPlayer"), 0, 0, Filename);
	//		if (Sec)
	//		{
	//			if (Sec->Remove(Key) > 0)
	//				GConfig->Flush(0, Filename);
	//		}

	//		op.RemoveAt(i);
	//	}
	//}
}

void URL::LoadURLConfig(const char* Section, const std::string& Filename)
{
	//TArray<std::string> Options;
	//GConfig->GetSection(Section, Options, Filename);
	//for (int32_t i = 0; i<Options.size(); i++)
	//{
		//AddOption(*Options[i]);
	//}
}

void URL::SaveURLConfig(const char* section, const char* item, const std::string& filename) const
{
	for (int32_t i = 0; i<op.size(); i++)
	{
		char temp[1024];
		strcpy(temp, op[i].c_str());
		char* val = strchr(temp, '=');
		if (val)
		{
			*val++ = 0;
			if (stricmp(temp, item) == 0)
			{
				//GConfig->SetString(Section, Temp, Value, Filename);
			}
		}
	}
}

bool URL::HasOption(const char* Test) const
{
	return GetOption(Test, NULL) != NULL;
}

const char* URL::GetOption(const char* match, const char* Default) const
{
	const int32_t Len = strlen(match);

	if (Len > 0)
	{
		for (int32_t i = 0; i < op.size(); i++)
		{
			const char* s = op[i].c_str();
			if (strnicmp(s, match, Len) == 0)
			{
				if (s[Len - 1] == '=' || s[Len] == '=' || s[Len] == '\0')
				{
					return s + Len;
				}
			}
		}
	}

	return Default;
}

/*-----------------------------------------------------------------------------
Comparing.
-----------------------------------------------------------------------------*/

bool URL::operator==(const URL& other) const
{
	if
		(protocol != other.protocol
			|| host != other.host
			|| map_name != other.map_name
			|| port != other.port
			|| op.size() != other.op.size())
		return 0;

	for (int32_t i = 0; i<op.size(); i++)
		if (op[i] != other.op[i])
			return 0;

	return 1;
}

