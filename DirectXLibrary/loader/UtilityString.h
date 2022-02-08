#pragma once
#include <string>

namespace gamelib
{
namespace utility_string
{
/// <summary>
/// string型をwchar型にして返す
/// </summary>
/// <param name="name">参照する文字列</param>
/// <returns>ワイド文字列</returns>
wchar_t* StrToWchar(const std::string& name);

/// <summary>
/// 特定の文字列を調べる
/// </summary>
/// <param name="fullPath">フルパス</param>
/// <param name="name">調べる文字列</param>
/// <returns>見つかったらtrueを返す</returns>
bool CheckStr(const std::string& fullPath, const std::string& name);

/// <summary>
/// パスが無くなるまで読み取る
/// </summary>
/// <param name="name">参照する文字列</param>
/// <returns>読み終えたファイル名</returns>
std::string ExtractFileName(const std::string& path);

void SaveText(const std::string& textFileName, const std::string& data);

std::string LoadTextFile(const std::string& textFileName);

} // namespace utility_string
} // namespace gamelib