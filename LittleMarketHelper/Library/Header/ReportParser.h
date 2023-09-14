// ========================================================================
//		Base parsers class
// ========================================================================

#pragma once

#include <filesystem>
#include <set>
#include <memory>
#include <utility>

#include "Utils/Assertions.h"
#include "Security.h"


namespace fs = std::filesystem;

namespace lmh {


	class ReportParser
	{
	public:

		// Supported parsers
		enum class Type
		{
			DEGIRO
		};


	public:

		// TODO: add variant, and parse currency in degiro report

		[[nodiscard("Parsed securities will be lost")]]
		static std::pair<std::vector<Security>, bool> parse(ReportParser::Type type, const fs::path&);
		[[nodiscard("Parsed securities will be lost")]]
		static std::pair<std::vector<Security>, bool> parseDefault(ReportParser::Type type);

		virtual ~ReportParser() = default;

	protected:

		// Protected constructor
		ReportParser(ReportParser::Type type);

	protected:

		// Const methods
		virtual void readFile(const fs::path& file, std::vector<Security>& securities, bool& successful) const = 0;
		virtual fs::path defaultFilename() const;
		virtual fs::path defaultExtension() const;

	private:

		// Factory method
		static std::unique_ptr<ReportParser> create(ReportParser::Type type);

	protected:

		const ReportParser::Type type_;
		std::set<fs::path> guesses_;
	};
}