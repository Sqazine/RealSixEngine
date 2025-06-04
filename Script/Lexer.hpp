#pragma once
#include <cstdint>
#include <vector>
#include "String.hpp"
#include <iostream>
#include <unordered_map>
#include "Token.hpp"
#include "Common.hpp"

namespace RealSix::Script
{
	class REALSIX_API Lexer
	{
	public:
		Lexer();
		~Lexer() = default;

		const std::vector<Token *> &ScanTokens(StringView src,StringView filePath="interpreter");

	private:
		void ResetStatus();

		void ScanToken();

		bool IsMatchCurChar(char c);
		bool IsMatchCurCharAndStepOnce(char c);

		char GetCurCharAndStepOnce();
		char GetCurChar();

		void AddToken(TokenKind type);
		void AddToken(TokenKind type, StringView literal);

		bool IsAtEnd();

		bool IsNumber(char c);
		bool IsLetter(char c);
		bool IsLetterOrNumber(char c);

		void ScanNumber();
		void ScanIdentifier();
		void ScanString();
		void ScanCharacter();

		SourceLocation GetSourceLocation();

		uint64_t mStartPos;
		uint64_t mCurPos;
		uint64_t mLine;
		uint64_t mColumn;
		String mSource;
		StringView mFilePath;
		std::vector<Token *> mTokens;
	};
}