#include <string>
#include <string_view>
#include "Version.h"
#include "Script/Logger.h"
#include "Script/Token.h"
#include "Script/Ast.h"
#include "Script/Lexer.h"
#include "Script/Parser.h"
#include "Script/AstPass.h"
#include "Script/ConstantFoldPass.h"
#include "Script/TypeCheckAndResolvePass.h"
#include "Script/SyntaxCheckPass.h"
#include "Script/Compiler.h"
#include "Script/VM.h"
#include "Script/Context.h"
#include "Config/Config.h"
#include "Resource/IO.h"

#if defined(_WIN32) || defined(_WIN64)
#pragma warning(disable : 4996)
#endif

#define REPL_CLEAR TEXT("clear")
#define REPL_EXIT TEXT("exit")

RealSix::Script::Lexer *gLexer{nullptr};
RealSix::Script::Parser *gParser{nullptr};

RealSix::Script::AstPassManager *gAstPassManager{nullptr};

RealSix::Script::Compiler *gCompiler{nullptr};
RealSix::Script::VM *gVm{nullptr};

int32_t PrintVersion()
{
	REALSIX_LOG_INFO(REALSIX_VERSION);
	return EXIT_FAILURE;
}

int32_t PrintUsage()
{
	REALSIX_LOG_INFO(TEXT("Usage: RealSix [option]:"));
	REALSIX_LOG_INFO(TEXT("-h or --help:show usage info."));
	REALSIX_LOG_INFO(TEXT("-v or --version:show current RealSix version"));
	REALSIX_LOG_INFO(TEXT("-s or --serialize: serialize source file as bytecode binary file"));
	REALSIX_LOG_INFO(TEXT("-f or --file:run source file with a valid file path,like : RealSix -f examples/array.cd."));
	REALSIX_LOG_INFO(TEXT("--function-cache:use function cache optimize."));
#ifndef NDEBUG
	REALSIX_LOG_INFO(TEXT("--gc-debug:debug gc."));
	REALSIX_LOG_INFO(TEXT("--gc-stress:stressing gc."));
#endif
	REALSIX_LOG_INFO(TEXT("In REPL mode, you can input '{}' to clear the REPL history, and '{}' to exit the REPL."), REPL_CLEAR, REPL_EXIT);
	return EXIT_FAILURE;
}

void Run(STRING_VIEW content)
{
	auto tokens = gLexer->ScanTokens(content);
#ifndef NDEBUG
	for (const auto &token : tokens)
		RealSix::Logger::Println(TEXT("{}"), *token);
#endif
	auto stmt = gParser->Parse(tokens);

#ifndef NDEBUG
	RealSix::Logger::Println(TEXT("{}"), stmt->ToString());
#endif

	stmt = gAstPassManager->Execute(stmt);

#ifndef NDEBUG
	RealSix::Logger::Println(TEXT("{}"), stmt->ToString());
#endif

	auto mainFunc = gCompiler->Compile(stmt);

#ifndef NDEBUG
	auto str = mainFunc->ToStringWithChunk();
	RealSix::Logger::Println(TEXT("{}"), str);
#endif

	if (RealSix::ScriptConfig::GetInstance().IsSerializeBinaryChunk())
	{
		auto data = mainFunc->chunk.Serialize();
		RealSix::WriteBinaryFile(RealSix::ScriptConfig::GetInstance().GetSerializeBinaryFilePath(), data);
	}
	else
	{
		gVm->Run(mainFunc);
	}
}

void Repl()
{
	STRING line;
	STRING allLines;

	PrintVersion();

	RealSix::Logger::Print(TEXT(">> "));
	while (getline(CIN, line))
	{
		allLines += line;
		if (line == REPL_CLEAR)
		{
			allLines.clear();
		}
		else if (line == REPL_EXIT)
		{
			RealSix::Logger::Println(TEXT("Bye!"));
			return;
		}
		else
		{
			Run(allLines);
		}

		RealSix::Logger::Print(TEXT(">> "));
	}
}

void RunFile(std::string_view path)
{
	STRING content = RealSix::ReadUnicodeTextFile(path);
#ifdef REALSIX_UTF8_ENCODE
	auto utf8Path = RealSix::Utf8::Decode(path.data());
	RealSix::Script::Record::mCurFilePath = utf8Path;
#else
	RealSix::Script::Record::mCurFilePath = path;
#endif
	Run(content);
}

int32_t ParseArgs(int32_t argc, const char *argv[])
{
	for (size_t i = 0; i < argc; ++i)
	{
		if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0)
		{
			if (i + 1 < argc)
				RealSix::ScriptConfig::GetInstance().SetExecuteFilePath(argv[++i]);
			else
				return PrintUsage();
		}

		if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--serialize") == 0)
		{
			if (i + 1 < argc)
			{
				RealSix::ScriptConfig::GetInstance().SetSerializeBinaryChunk(true);
				RealSix::ScriptConfig::GetInstance().SetSerializeBinaryFilePath(argv[++i]);
			}
			else
				return PrintUsage();
		}
		if (strcmp(argv[i], "--function-cache") == 0)
			RealSix::ScriptConfig::GetInstance().SetUseFunctionCache(true);

		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
			return PrintUsage();

		if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
			return PrintVersion();
	}

	return EXIT_SUCCESS;
}

int32_t main(int32_t argc, const char *argv[])
{
#if defined(_WIN32) || defined(_WIN64)
	system("chcp 65001");
#endif
	if (ParseArgs(argc, argv) == EXIT_FAILURE)
		return EXIT_FAILURE;

	RealSix::Script::Context::GetInstance().Init();

	gLexer = new RealSix::Script::Lexer();
	gParser = new RealSix::Script::Parser();
	gAstPassManager = new RealSix::Script::AstPassManager();
	gCompiler = new RealSix::Script::Compiler();
	gVm = new RealSix::Script::VM();

	gAstPassManager
		->Add<RealSix::Script::ConstantFoldPass>()
		->Add<RealSix::Script::SyntaxCheckPass>()
		->Add<RealSix::Script::TypeCheckAndResolvePass>();

	if (!RealSix::ScriptConfig::GetInstance().GetExecuteFilePath().empty())
		RunFile(RealSix::ScriptConfig::GetInstance().GetExecuteFilePath());
	else
		Repl();

	SAFE_DELETE(gLexer);
	SAFE_DELETE(gParser);
	SAFE_DELETE(gAstPassManager);
	SAFE_DELETE(gCompiler);
	SAFE_DELETE(gVm);

	RealSix::Script::Context::GetInstance().Destroy();

	return EXIT_SUCCESS;
}