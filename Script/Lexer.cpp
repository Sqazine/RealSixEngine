#include "Lexer.hpp"
#include "Logger.hpp"
#include "String.hpp"
namespace RealSix::Script
{
	struct KeywordBinding
	{
		const char *name;
		TokenKind type;
	};
	constexpr KeywordBinding keywords[] = {
		{"let", TokenKind::LET},
		{"if", TokenKind::IF},
		{"else", TokenKind::ELSE},
		{"true", TokenKind::TRUE},
		{"false", TokenKind::FALSE},
		{"null", TokenKind::NIL},
		{"while", TokenKind::WHILE},
		{"for", TokenKind::FOR},
		{"fn", TokenKind::FUNCTION},
		{"class", TokenKind::CLASS},
		{"this", TokenKind::THIS},
		{"base", TokenKind::BASE},
		{"public", TokenKind::PUBLIC},
		{"protected", TokenKind::PROTECTED},
		{"private", TokenKind::PRIVATE},
		{"return", TokenKind::RETURN},
		{"static", TokenKind::STATIC},
		{"const", TokenKind::CONST},
		{"break", TokenKind::BREAK},
		{"continue", TokenKind::CONTINUE},
		{"import", TokenKind::IMPORT},
		{"module", TokenKind::MODULE},
		{"switch", TokenKind::SWITCH},
		{"default", TokenKind::DEFAULT},
		{"match", TokenKind::MATCH},
		{"enum", TokenKind::ENUM},
		{"u8", TokenKind::U8},
		{"u16", TokenKind::U16},
		{"u32", TokenKind::U32},
		{"u64", TokenKind::U64},
		{"i8", TokenKind::I8},
		{"i16", TokenKind::I16},
		{"i32", TokenKind::I32},
		{"i64", TokenKind::I64},
		{"f32", TokenKind::F32},
		{"f64", TokenKind::F64},
		{"bool", TokenKind::BOOL},
		{"char", TokenKind::CHAR},
		{"void", TokenKind::VOID},
		{"any", TokenKind::ANY},
		{"as", TokenKind::AS},
		{"new", TokenKind::NEW},
		{"struct", TokenKind::STRUCT},
	};

	Lexer::Lexer()
	{
		ResetStatus();
	}

	const std::vector<Token *> &Lexer::ScanTokens(StringView src, StringView filePath)
	{
		ResetStatus();
		{
		mSource = src;
		mFilePath = filePath;
		}
		while (!IsAtEnd())
		{
			mStartPos = mCurPos;
			ScanToken();
		}

		AddToken(TokenKind::END, "END");

		return mTokens;
	}
	void Lexer::ScanToken()
	{
		String c;

		char ch;
		if (!isascii(GetCurChar()))
		{
			ch = GetCurChar();
			while (!isascii(GetCurChar())) // not a ASCII char
			{
				c.Append(1, ch);
				ch = GetCurCharAndStepOnce();
			}
		}
		else
		{
			ch = GetCurCharAndStepOnce();
			c.Append(1, ch);
		}

		if (c == "(")
		{
			if (IsMatchCurCharAndStepOnce('{'))
				AddToken(TokenKind::LPAREN_LBRACE);
			else
				AddToken(TokenKind::LPAREN);
		}
		else if (c == ")")
			AddToken(TokenKind::RPAREN);
		else if (c == "[")
			AddToken(TokenKind::LBRACKET);
		else if (c == "]")
			AddToken(TokenKind::RBRACKET);
		else if (c == "{")
			AddToken(TokenKind::LBRACE);
		else if (c == "}")
		{
			if (IsMatchCurCharAndStepOnce(')'))
				AddToken(TokenKind::RBRACE_RPAREN);
			else
				AddToken(TokenKind::RBRACE);
		}
		else if (c == ".")
		{
			if (IsMatchCurCharAndStepOnce('.'))
			{
				if (IsMatchCurCharAndStepOnce('.'))
					AddToken(TokenKind::ELLIPSIS);
				else
					REALSIX_SCRIPT_LOG_ERROR(GetSourceLocation(), "Unknown literal:'..',did you want '.' or '...'?");
			}
			else
				AddToken(TokenKind::DOT);
		}
		else if (c == ",")
			AddToken(TokenKind::COMMA);
		else if (c == ":")
			AddToken(TokenKind::COLON);
		else if (c == ";")
			AddToken(TokenKind::SEMICOLON);
		else if (c == "~")
			AddToken(TokenKind::TILDE);
		else if (c == "?")
			AddToken(TokenKind::QUESTION);
		else if (c == "\"")
			ScanString();
		else if (c == "\'")
			ScanCharacter();
		else if (c == " " || c == "\t" || c == "\r")
		{
		}
		else if (c == "\n")
		{
			mLine++;
			mColumn = 1;
		}
		else if (c == "+")
		{
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TokenKind::PLUS_EQUAL);
			else if (IsMatchCurCharAndStepOnce('+'))
				AddToken(TokenKind::PLUS_PLUS);
			else
				AddToken(TokenKind::PLUS);
		}
		else if (c == "-")
		{
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TokenKind::MINUS_EQUAL);
			else if (IsMatchCurCharAndStepOnce('-'))
				AddToken(TokenKind::MINUS_MINUS);
			else
				AddToken(TokenKind::MINUS);
		}
		else if (c == "*")
		{
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TokenKind::ASTERISK_EQUAL);
			else
				AddToken(TokenKind::ASTERISK);
		}
		else if (c == "/")
		{
			if (IsMatchCurCharAndStepOnce('/'))
			{
				while (!IsMatchCurChar('\n') && !IsAtEnd())
					GetCurCharAndStepOnce();
			}
			else if (IsMatchCurCharAndStepOnce('*'))
			{
				while (!IsAtEnd())
				{
					if (IsMatchCurChar('\n'))
					{
						mLine++;
						mColumn = 1;
					}
					Logger::Println("{}", GetCurChar());
					GetCurCharAndStepOnce();
					if (IsMatchCurChar('*'))
					{
						GetCurCharAndStepOnce();
						if (IsMatchCurChar('/'))
						{
							GetCurCharAndStepOnce();
							break;
						}
					}
				}
			}
			else if (IsMatchCurCharAndStepOnce('='))
				AddToken(TokenKind::SLASH_EQUAL);
			else
				AddToken(TokenKind::SLASH);
		}
		else if (c == "%")
		{
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TokenKind::PERCENT_EQUAL);
			AddToken(TokenKind::PERCENT);
		}
		else if (c == "!")
		{
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TokenKind::BANG_EQUAL);
			else
				AddToken(TokenKind::BANG);
		}
		else if (c == "&")
		{
			if (IsMatchCurCharAndStepOnce('&'))
				AddToken(TokenKind::AMPERSAND_AMPERSAND);
			else if (IsMatchCurCharAndStepOnce('='))
				AddToken(TokenKind::AMPERSAND_EQUAL);
			else
				AddToken(TokenKind::AMPERSAND);
		}
		else if (c == "|")
		{
			if (IsMatchCurCharAndStepOnce('|'))
				AddToken(TokenKind::VBAR_VBAR);
			else if (IsMatchCurCharAndStepOnce('='))
				AddToken(TokenKind::VBAR_EQUAL);
			else
				AddToken(TokenKind::VBAR);
		}
		else if (c == "^")
		{
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TokenKind::CARET_EQUAL);
			else
				AddToken(TokenKind::CARET);
		}
		else if (c == "<")
		{
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TokenKind::LESS_EQUAL);
			else if (IsMatchCurCharAndStepOnce('<'))
			{
				if (IsMatchCurCharAndStepOnce('='))
					AddToken(TokenKind::LESS_LESS_EQUAL);
				else
					AddToken(TokenKind::LESS_LESS);
			}
			else
				AddToken(TokenKind::LESS);
		}
		else if (c == ">")
		{
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TokenKind::GREATER_EQUAL);
			else if (IsMatchCurCharAndStepOnce('>'))
			{
				if (IsMatchCurCharAndStepOnce('='))
					AddToken(TokenKind::GREATER_GREATER_EQUAL);
				else
					AddToken(TokenKind::GREATER_GREATER);
			}
			else
				AddToken(TokenKind::GREATER);
		}
		else if (c == "=")
		{
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TokenKind::EQUAL_EQUAL);
			else
				AddToken(TokenKind::EQUAL);
		}
		else
		{
			if (IsNumber(ch))
				ScanNumber();
			else if (IsLetter(ch))
				ScanIdentifier();
			else
			{
				auto literal = mSource.SubStr(mStartPos, mCurPos - mStartPos);
				REALSIX_SCRIPT_LOG_ERROR(GetSourceLocation(), "Unknown literal:" + literal);
			}
		}
	}

	void Lexer::ResetStatus()
	{
		mStartPos = mCurPos = 0;
		mLine = 1;
		mColumn = 1;
		std::vector<Token *>().swap(mTokens);
	}

	bool Lexer::IsMatchCurChar(char c)
	{
		return GetCurChar() == c;
	}
	bool Lexer::IsMatchCurCharAndStepOnce(char c)
	{
		bool result = GetCurChar() == c;
		if (result)
		{
			mCurPos++;
			mColumn++;
		}
		return result;
	}

	char Lexer::GetCurCharAndStepOnce()
	{
		if (!IsAtEnd())
		{
			mColumn++;
			return mSource[mCurPos++];
		}
		return '\0';
	}

	char Lexer::GetCurChar()
	{
		if (!IsAtEnd())
			return mSource[mCurPos];
		return '\0';
	}

	void Lexer::AddToken(TokenKind type)
	{
		auto literal = mSource.SubStr(mStartPos, mCurPos - mStartPos);

		AddToken(type, literal);
	}
	void Lexer::AddToken(TokenKind type, StringView literal)
	{
		SourceLocation location;
		location.line = mLine;
		location.column = mColumn - literal.Size();
		location.pos = mCurPos - literal.Size();
		location.sourceCode = mSource;
		location.filePath = mFilePath;

		mTokens.push_back(new Token(type, literal, location));
	}

	bool Lexer::IsAtEnd()
	{
		return mCurPos >= mSource.Size();
	}

	bool Lexer::IsNumber(char c)
	{
		return c >= '0' && c <= '9';
	}
	bool Lexer::IsLetter(char c)
	{
		if (!isascii(c))
			return true;
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
	}
	bool Lexer::IsLetterOrNumber(char c)
	{
		return IsLetter(c) || IsNumber(c);
	}

	void Lexer::ScanNumber()
	{
		while (IsNumber(GetCurChar()))
			GetCurCharAndStepOnce();

		if (IsMatchCurCharAndStepOnce('.'))
		{
			if (IsNumber(GetCurChar()))
				while (IsNumber(GetCurChar()))
					GetCurCharAndStepOnce();
			else if (GetCurChar() == 'f')
				GetCurCharAndStepOnce();
			else
				REALSIX_SCRIPT_LOG_ERROR(GetSourceLocation(), "The character next to '.' in a floating number must be in [0-9] range or a single 'f' character.");
		}

		AddToken(TokenKind::NUMBER);
	}

	void Lexer::ScanIdentifier()
	{
		char c = GetCurChar();
		while (IsLetterOrNumber(c))
		{
			GetCurCharAndStepOnce();
			c = GetCurChar();
		}

		String literal = mSource.SubStr(mStartPos, mCurPos - mStartPos);

		bool isKeyWord = false;
		for (const auto &keyword : keywords)
			if (keyword.name == literal)
			{
				AddToken(keyword.type, literal);
				isKeyWord = true;
				break;
			}

		if (!isKeyWord)
			AddToken(TokenKind::IDENTIFIER, literal);
	}

	void Lexer::ScanString()
	{
		while (!IsMatchCurChar('\"') && !IsAtEnd())
		{
			if (IsMatchCurChar('\n'))
			{
				mLine++;
				mColumn = 1;
			}
			GetCurCharAndStepOnce();
		}

		if (IsAtEnd())
			Logger::Println("[line {}]:Uniterminated string.", mLine);

		GetCurCharAndStepOnce(); // eat the second '\"'

		AddToken(TokenKind::STR, mSource.SubStr(mStartPos + 1, mCurPos - mStartPos - 2));
	}

	void Lexer::ScanCharacter()
	{
		GetCurCharAndStepOnce(); // eat the first '\''

		AddToken(TokenKind::CHARACTER, mSource.SubStr(mStartPos + 1, 1));

		GetCurCharAndStepOnce(); // eat the second '\''
	}
	SourceLocation Lexer::GetSourceLocation()
	{
		SourceLocation location;
		location.pos = mCurPos;
		location.column = mColumn;
		location.sourceCode = mSource;
		location.filePath = mFilePath;
		return location;
	}
}