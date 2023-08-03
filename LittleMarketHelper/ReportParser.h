// ========================================================================
//		Base parsers class
// ========================================================================

#pragma once

#include <filesystem>
#include <set>
#include <memory>
#include <optional>
#include <utility>

#include "Assertions.h"
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

		ReportParser(ReportParser::Type type);
		virtual ~ReportParser() = default;

		// Non-const methods
		
		// Const methods
		std::pair<std::vector<FinProduct>, bool> parse(std::optional<fs::path> file) const;
		std::pair<std::vector<FinProduct>, bool> parseDefault() const;

	protected:

		// Const methods
		virtual std::pair<std::vector<FinProduct>, bool> run(std::vector<FinProduct>& products, bool& successful) const = 0;
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