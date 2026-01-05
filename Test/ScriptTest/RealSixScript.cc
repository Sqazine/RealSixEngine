#include "Version.hpp"
#include "Core/String.hpp"
#include "Core/Config.hpp"
#include "Resource/FileSystem.hpp"
#include "Script/Token.hpp"
#include "Script/Ast.hpp"
#include "Script/Lexer.hpp"
#include "Script/Parser.hpp"
#include "Script/AstPass.hpp"
#include "Script/ConstantFoldPass.hpp"
#include "Script/TypeCheckAndResolvePass.hpp"
#include "Script/SyntaxCheckPass.hpp"
#include "Script/Compiler.hpp"
#include "Script/VM.hpp"
#include "Script/Context.hpp"

using namespace RealSix;

#if defined(_WIN32) || defined(_WIN64)
#pragma warning(disable : 4996)
#endif

#define REPL_CLEAR "clear"
#define REPL_EXIT "exit"

Script::Lexer *gLexer{nullptr};
Script::Parser *gParser{nullptr};

Script::AstPassManager *gAstPassManager{nullptr};

Script::Compiler *gCompiler{nullptr};
Script::VM *gVm{nullptr};

int32_t PrintVersion()
{
	REALSIX_LOG_INFO(REALSIX_VERSION);
	return EXIT_FAILURE;
}

int32_t PrintUsage()
{
	REALSIX_LOG_INFO("Usage: RealSix [option]:");
	REALSIX_LOG_INFO("-h or --help:show usage info.");
	REALSIX_LOG_INFO("-v or --version:show current RealSix version");
	REALSIX_LOG_INFO("-s or --serialize: serialize source file as bytecode binary file");
	REALSIX_LOG_INFO("-f or --file:run source file with a valid file path,like : RealSix -f examples/array.cd.");
	REALSIX_LOG_INFO("--function-cache:use function cache optimize.");
#ifndef NDEBUG
	REALSIX_LOG_INFO("--gc-debug:debug gc.");
	REALSIX_LOG_INFO("--gc-stress:stressing gc.");
#endif
	REALSIX_LOG_INFO("In REPL mode, you can input '{}' to clear the REPL history, and '{}' to exit the REPL.", REPL_CLEAR, REPL_EXIT);
	return EXIT_FAILURE;
}

void Run( StringView content)
{
	auto tokens = gLexer->ScanTokens(content);
#ifndef NDEBUG
	for (const auto &token : tokens)
		Logger::Println("{}", *token);
#endif
	auto stmt = gParser->Parse(tokens);

#ifndef NDEBUG
	Logger::Println("{}", stmt->ToString());
#endif

	stmt = gAstPassManager->Execute(stmt);

#ifndef NDEBUG
	Logger::Println("{}", stmt->ToString());
#endif

	auto mainFunc = gCompiler->Compile(stmt);

#ifndef NDEBUG
	auto str = mainFunc->ToStringWithChunk();
	Logger::Println("{}", str);
#endif

	if (ScriptConfig::GetInstance().IsSerializeBinaryChunk())
	{
		auto data = mainFunc->chunk.Serialize();
		FileSystem::WriteBinaryFile(ScriptConfig::GetInstance().GetSerializeBinaryFilePath(), data);
	}
	else
	{
		gVm->Run(mainFunc);
	}
}

void Repl()
{
	std::string line;
	std::string allLines;

	PrintVersion();

	Logger::Print(">> ");
	while (getline(std::cin, line))
	{
		allLines += line;
		if (line == REPL_CLEAR)
		{
			allLines.clear();
		}
		else if (line == REPL_EXIT)
		{
			Logger::Println("Bye!");
			return;
		}
		else
		{
			Run(allLines);
		}

		Logger::Print(">> ");
	}
}

void RunFile(StringView path)
{
	String content = FileSystem::ReadUnicodeTextFile(path);
	Run(content);
}

int32_t ParseArgs(int32_t argc, const char *argv[])
{
	for (size_t i = 0; i < argc; ++i)
	{
		StringView arg = argv[i];
		if (arg == "-f" || arg == "--file")
		{
			if (i + 1 < argc)
				ScriptConfig::GetInstance().SetExecuteFilePath(argv[++i]);
			else
				return PrintUsage();
		}

		if (arg == "-s" || arg == "--serialize")
		{
			if (i + 1 < argc)
			{
				ScriptConfig::GetInstance().SetSerializeBinaryChunk(true);
				ScriptConfig::GetInstance().SetSerializeBinaryFilePath(argv[++i]);
			}
			else
				return PrintUsage();
		}
		if (arg == "--function-cache")
			ScriptConfig::GetInstance().SetUseFunctionCache(true);

		if (arg == "-h" || arg == "--help")
			return PrintUsage();

		if (arg == "-v"|| arg == "--version")
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

	Script::Context::GetInstance().Init();

	gLexer = new Script::Lexer();
	gParser = new Script::Parser();
	gAstPassManager = new Script::AstPassManager();
	gCompiler = new Script::Compiler();
	gVm = new Script::VM();

	gAstPassManager
		->Add<Script::ConstantFoldPass>()
		->Add<Script::SyntaxCheckPass>()
		->Add<Script::TypeCheckAndResolvePass>();

	if (!ScriptConfig::GetInstance().GetExecuteFilePath().Empty())
		RunFile(ScriptConfig::GetInstance().GetExecuteFilePath());
	else
		Repl();

	SAFE_DELETE(gLexer);
	SAFE_DELETE(gParser);
	SAFE_DELETE(gAstPassManager);
	SAFE_DELETE(gCompiler);
	SAFE_DELETE(gVm);

	Script::Context::GetInstance().CleanUp();

	return EXIT_SUCCESS;
}