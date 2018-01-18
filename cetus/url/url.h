#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "url_config.h"

namespace terra
{
	enum ENoInit { NoInit };

	// Traveling from server to server.
	enum ETravelType
	{
		/** Absolute URL. */
		TRAVEL_Absolute,
		/** Partial (carry name, reset server). */
		TRAVEL_Partial,
		/** Relative URL. */
		TRAVEL_Relative,
		TRAVEL_MAX,
	};

	struct URL
	{
	public:
			// Protocol, i.e. "unreal" or "http".
			std::string protocol;

		// Optional hostname, i.e. "204.157.115.40" or "unreal.epicgames.com", blank if local.
			std::string host;

		// Optional host port.
			int32_t port;

		// Map name, i.e. "SkyCity", default is "Entry".
			std::string map_name;

		// Optional place to download Map if client does not possess it
			std::string redirect_url;

		// Options.
			std::vector<std::string> op;

		// Portal to enter through, default is "".
			std::string portal;

			int32_t valid;

		// Statics.
		static FUrlConfig url_config;
		static bool defaults_initialized;

		/**
		* Prevent default from being generated.
		*/
		explicit URL(ENoInit) { }

		/**
		* Construct a purely default, local URL from an optional filename.
		*/
		URL(const char* filename = nullptr);

		/**
		* Construct a URL from text and an optional relative base.
		*/
		URL(URL* base, const char* text_url, ETravelType travel_type);

		static void StaticInit();
		static void StaticExit();

		/**
		* Static: Removes any special URL characters from the specified string
		*
		* @param Str String to be filtered
		*/
		static void FilterURLString(std::string& str);

		/**
		* Returns whether this URL corresponds to an internal object, i.e. an Unreal
		* level which this app can try to connect to locally or on the net. If this
		* is false, the URL refers to an object that a remote application like Internet
		* Explorer can execute.
		*/
		bool IsInternal() const;

		/**
		* Returns whether this URL corresponds to an internal object on this local
		* process. In this case, no Internet use is necessary.
		*/
		bool IsLocalInternal() const;

		/**
		* Tests if the URL contains an option string.
		*/
		bool HasOption(const char* test) const;

		/**
		* Returns the value associated with an option.
		*
		* @param Match The name of the option to get.
		* @param Default The value to return if the option wasn't found.
		*
		* @return The value of the named option, or Default if the option wasn't found.
		*/
		const char* GetOption(const char* match, const char* default_op) const;

		/**
		* Load URL from config.
		*/
		void LoadURLConfig(const char* section, const std::string& filename = "");

		/**
		* Save URL to config.
		*/
		void SaveURLConfig(const char* Section, const char* Item, const std::string& Filename = "") const;

		/**
		* Add a unique option to the URL, replacing any existing one.
		*/
		void AddOption(const char* Str);

		/**
		* Remove an option from the URL
		*/
		void RemoveOption(const char* Key, const char* Section = nullptr, const std::string& Filename = "");

		/**
		* Convert this URL to text.
		*/
		std::string ToString(bool FullyQualified = 0) const;

		/**
		* Serializes a FURL to or from an archive.
		*/
		//friend FArchive& operator<<(FArchive& Ar, FURL& U);

		/**
		* Compare two URLs to see if they refer to the same exact thing.
		*/
		bool operator==(const URL& Other) const;
	};
}