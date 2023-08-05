// ========================================================================
//		Base parsers class
// ========================================================================

#pragma once

#include <filesystem>
#include <set>
#include <memory>
#include <optional>
#include <utility>

#include "Utils/Assertions.h"
#include "FinProduct.h"


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

		[[nodiscard("Parsed products will not be stored anywhere")]]
		static std::pair<std::vector<FinProduct>, bool> parse(ReportParser::Type type, std::optional<fs::path> file = std::nullopt);
		[[nodiscard("Parsed products will not be stored anywhere")]]
		static std::pair<std::vector<FinProduct>, bool> parseDefault(ReportParser::Type type);

		virtual ~ReportParser() = default;

	protected:

		// Protected constructor
		ReportParser(ReportParser::Type type);

	protected:

		// Const methods
		virtual void readFile(const fs::path& file, std::vector<FinProduct>& products, bool& successful) const = 0;
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